/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年5月7日 上午10:20:04
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.license.task;

import java.io.IOException;
import java.util.Date;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.hikvision.finance.core.util.DateUtils;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.license.model.AmountLimit;
import com.hikvision.finance.fms.license.service.ISoftdogService;
import com.hikvision.finance.fms.license.util.LicenseConstant.DOGTYPE;
import com.hikvision.finance.fms.model.Softdongle;
import com.hikvision.finance.fwork.core.task.AbstractTaskEntity;
import com.hikvision.license.constant.LicenseConstant.DOGVALUE;
import com.hikvision.license.entity.License;
import com.hikvision.license.entity.baseinfo.ProjectInfo;
import com.hikvision.license.entity.common.ToConvert;
import com.hikvision.license.entity.subsystem.Controller;
import com.hikvision.license.entity.subsystem.SubSystem;
import com.hikvision.license.ws.ILicenseService;
import com.hikvision.rpc.factory.CmsHppFactory;
import com.ivms6.core.hpp.exception.HppInitException;


/**
 * <p>授权定时任务</p>
 * @author jinxindong 2016年5月7日 上午10:20:04
 * @version V1.0 
 */
public class LicenseTask extends AbstractTaskEntity {
	
	private Logger logger = LoggerFactory.getLogger(LicenseTask.class);
	
	/**
	 * 看门狗Service
	 */
	private ISoftdogService softdogService;

	/**
	 * 读狗/文件 Service
	 */
	private ILicenseService licenseService;
	
	/* (non-Javadoc)
     * @see com.hikvision.finance.fwork.core.task.AbstractTaskEntity#run()
     */
    @Override
	public synchronized void run() {
		
		logger.info("开始授权更新");
		
		// 检查数据库中的授权信息
		Boolean checkResult = softdogService.checkDBAuthorization();
		
		if (!checkResult) {// 需要更新授权
			
			// 捕获CMSException更新授权
			writeAuthInfo2DB();
			
			// 判断是否授权失效, 启动/关闭 Mina 服务
			Boolean shouldStart = isAllowedToServe();
			
			try {
				if (shouldStart) {
					startMINAserver();
				} else {
					stopMINAserver();
				}
			} catch (IOException ioe) {
				logger.error("启动MINA服务失败！【" + ioe.getMessage() + "】", ioe);
			} catch (HppInitException hppe) {
				logger.error("启动MINA服务失败！【" + hppe.getMessage() + "】", hppe);
			}
		}
		
		logger.info("结束授权更新");
	}
	
    
    /**
     * 写/更新授权信息到数据库
     * @author jinxindong 2016年5月7日 上午10:36:07
     */
	private void writeAuthInfo2DB() {
		
		// 1. 写入狗的授权信息
		License license = licenseService.getLicense();
		Integer status = licenseService.getLicenseStatus();
		String[] strs = licenseService.getLicenseExpireDateDetail();
		
		if (license == null) {
			return;
		}
		
		// 获取授权版本信息
		ProjectInfo projectInfo = license.getProjectInfo();
		
		Softdongle softdog = new Softdongle();
		softdog.setNtype(DOGTYPE.DOG_VERSION);
		if (projectInfo != null && projectInfo.getApplyCatalog() == 1) {
			// 购销版
			softdog.setNvalue(status);
		} else {
			// 演示版
			softdog.setNvalue(DOGVALUE.AUTH_FILE);
		}
		if (softdog.getNvalue() == DOGVALUE.AUTH_FILE && strs != null && strs.length > 1) {
			softdog.setStrDescribe(strs[1]);// 试用版到期时间
		}
		
		// 写获取授权版本信息
		softdogService.saveOrUpdateSoftDog(softdog);
		
		// 写监控单号信息
		if (projectInfo != null) {
			String notesId = projectInfo.getNotesId();// 监控单号
			softdogService.updateSysConfig(Constants.SysConfigType.DOG_NOTES_ID, notesId);
		}
		
		// 写licenseNo信息
		String licenseNo = license.getLicenseInfo().getLicenseNo();
		softdogService.updateSysConfig(Constants.SysConfigType.DOG_LICENSE_NO, licenseNo);
				
		List<SubSystem> subSystemList = license.getSubSystemList();
		
		if (null != subSystemList && subSystemList.size() > 0) {
			SubSystem subSystem = subSystemList.get(0);
			Controller controller = subSystem.getController();
			// 写入数量限制
			writeAmountLimit(controller.getAmountLimit());
		}
	}
	
	/**
	 * 写入设备通道授权限制
	 * @author fuqunqing 2015年8月26日 下午7:27:56
	 * @param amountToConvert
	 */
	private void writeAmountLimit(ToConvert amountToConvert) {
		
		AmountLimit amountLimit = amountToConvert.toEntity(AmountLimit.class);
		softdogService.saveOrUpdateSoftDog(new Softdongle(DOGTYPE.DOG_CHANNEL, amountLimit.getAmtChannelNumLimit()));
		
	}
	
	/**
	 * 是否需要启动服务
	 * @author jinxindong 2016年5月7日 上午10:31:31
	 * @return
	 */
	private Boolean isAllowedToServe() {
		
		Integer status = licenseService.getLicenseStatus();
		License license = licenseService.getLicense();
		
		if (license == null)
			return false;
		
		ProjectInfo projectInfo = license.getProjectInfo();
		
		Boolean shouldStart = false;
		
		if (status == DOGVALUE.AUTH_VAIL) {
			
			if (projectInfo != null && projectInfo.getApplyCatalog() == 1) {
				// 购销版
				// 2.1 授权信息正常, 启动服务
				shouldStart = true;
			} else {
				// 演示版
				// 2.2 授权信息试用, 判断是否试用过期
				Date nowDate = softdogService.getSysDate();
				
				String[] strs = licenseService.getLicenseExpireDateDetail();
				String expireDateStr = strs[1];
				Date expireDate = DateUtils.getDateTime(DateUtils.yyyy_MM_dd_HH_mm_ss, expireDateStr);
				
				if (expireDate.before(nowDate)) {
					// 2.2.1 已过期
					shouldStart = false;
				} else {
					// 2.2.2 未过期
					shouldStart = true;
				}
			}
		} else if (status == DOGVALUE.AUTH_FILE) {
			
			// 2.2 授权信息试用, 判断是否试用过期
			Date nowDate = softdogService.getSysDate();
			
			String[] strs = licenseService.getLicenseExpireDateDetail();
			String tryDateStr = strs[1];
			Date tryDate = DateUtils.getDateTime(DateUtils.yyyy_MM_dd_HH_mm_ss, tryDateStr);
			
			if (tryDate.before(nowDate)) {
				// 2.2.1 已过期
				shouldStart = false;
			} else {
				// 2.2.2 未过期
				shouldStart = true;
			}
		} else {
			// 2.3 授权信息失效, 关闭服务
			shouldStart = false;
		}
		return shouldStart;
	}
	
	/**
	 * 启动MINA服务
	 * @author jinxindong 2016年5月7日 上午10:31:47
	 * @throws IOException
	 * @throws HppInitException
	 */
	private void startMINAserver() throws IOException, HppInitException {
		CmsHppFactory.getInstance().startServer();
	}
	
	/**
	 * 关闭MINA服务
	 * @author jinxindong 2016年5月7日 上午10:31:53
	 * @throws HppInitException
	 */
	private void stopMINAserver() throws HppInitException {
		CmsHppFactory.getInstance().stopServer();
	}
    
    public ISoftdogService getSoftdogService() {
    	return softdogService;
    }
	
    public void setSoftdogService(ISoftdogService softdogService) {
    	this.softdogService = softdogService;
	}
	
	public ILicenseService getLicenseService() {
		return licenseService;
	}
	
	public void setLicenseService(ILicenseService licenseService) {
		this.licenseService = licenseService;
	}
	
}
