/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月30日 下午1:43:01
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.sysconfig.action;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.net.URLDecoder;
import java.util.List;

import org.apache.struts2.ServletActionContext;

import com.hikvision.finance.core.util.IOUtils;
import com.hikvision.finance.fms.common.model.UserSession;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.common.util.Constants.SysConfigType;
import com.hikvision.finance.fms.common.util.SessionUtil;
import com.hikvision.finance.fms.model.Sysconfig;
import com.hikvision.finance.fms.modules.sysconfig.dto.LogParamInfo;
import com.hikvision.finance.fms.modules.sysconfig.dto.ModeParamInfo;
import com.hikvision.finance.fms.modules.sysconfig.dto.NtpParamInfo;
import com.hikvision.finance.fms.modules.sysconfig.dto.PasswordParamInfo;
import com.hikvision.finance.fms.modules.sysconfig.dto.PlatformParamInfo;
import com.hikvision.finance.fms.modules.sysconfig.dto.RecordParamInfo;
import com.hikvision.finance.fms.modules.sysconfig.dto.SysconfigDto;
import com.hikvision.finance.fms.modules.sysconfig.service.ISysconfigService;
import com.hikvision.finance.fwork.exception.ExpectedException;
import com.hikvision.finance.fwork.www.action.BaseAction;

/**
 * <p>系统参数-系统配置管理Action</p>
 * @author jinxindong 2016年3月30日 下午1:43:01
 * @version V1.0
 */
public class SysconfigAction extends BaseAction<Sysconfig> {
	
	/**
	 * 序列化ID
	 */
	private static final long serialVersionUID = -4691278351843621441L;
	private ISysconfigService sysconfigService;
	private SysconfigDto sysconfigDto;
	private Sysconfig sysconfig;
	private File logoFile;//登录页logo
	private File loginFile;//登录页背景图片
	private File navbarFile;//导航栏logo
	private String logoFileFileName;//登录页logo
	private String loginFileFileName;//登录页背景图片
	private String navbarFileFileName;//导航栏logo
	private String navbarUrl;//导航栏logo
	private String logoUrl;//登录页logo
	private String loginUrl;//登录页背景图片
	private RecordParamInfo recordParamInfo;
	private PasswordParamInfo passwordParamInfo;
	private ModeParamInfo modeParamInfo;
	private LogParamInfo logParamInfo;
	private PlatformParamInfo platformParamInfo;
	private NtpParamInfo ntpParamInfo;
	private List<Integer> keys;
	private List<Sysconfig> sysconfigListIn;
	private List<Sysconfig> sysconfigListOut;
	
	
	private static final String rootPath = ServletActionContext.getServletContext().getRealPath("/");
	private static final String sysconfigTmpFolder = Constants.FILEUPLOAD + "sysconfig/tmp/";
	private static final String sysconfigFolder = Constants.FILEUPLOAD + "sysconfig/images/";
	
	/**
	 * 跳到参数配置界面
	 * @author jinxindong 2016年4月11日 下午6:46:28
	 * @return
	 */
	public String toConfigPage() {
		operPage = "/modules/sysconfig/sysconfig.jsp";
		return DISPATCHER;
	}
	
	
	/**
	 * 提交录像参数
	 * @author jinxindong 2016年3月31日 上午9:20:30
	 * @return
	 */
	public String updateRecordParam() {
		ajaxData = sysconfigService.updateRecordParam(recordParamInfo);
		return AJAX;
	}
	
	/**
	 * 跳到录像参数页面
	 * @author jinxindong 2016年3月31日 上午9:21:15
	 * @return
	 */
	public String toRecordParamPage() {
		recordParamInfo = sysconfigService.getRecordParam();
		operPage = "/modules/sysconfig/dialogs/videoParam.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 跳到模式参数页面
	 * @author jinxindong 2016年3月31日 上午9:22:29
	 * @return
	 */
	public String toModeParamPage() {
		modeParamInfo = sysconfigService.getModeParam();
		UserSession session  = SessionUtil.getUserSession();
		modeParamInfo.setOperName(null==session.getUserName()?"":session.getUserName());
		operPage = "/modules/sysconfig/dialogs/modelParam.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 提交模式参数
	 * @author jinxindong 2016年3月31日 上午9:23:09
	 * @return
	 */
	public String updateModeParam() {
		ajaxData = sysconfigService.updateModeParam(modeParamInfo);
		return AJAX;
	}
	
	/**
	 * 跳到安全策略页面
	 * @author jinxindong 2016年3月31日 上午9:27:24
	 * @return
	 */
	public String toPasswordParamPage() {
		passwordParamInfo = sysconfigService.getPasswordParam();
		operPage = "/modules/sysconfig/dialogs/safety.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 提交安全策略
	 * @author jinxindong 2016年3月31日 上午9:27:34
	 * @return
	 */
	public String updatePasswordParam() {
		ajaxData = sysconfigService.updatePasswordParam(passwordParamInfo);
		return AJAX;
	}
	
	/**
	 * 跳到日志保存天数页面
	 * @author jinxindong 2016年3月31日 上午9:56:14
	 * @return
	 */
	public String toLogParamPage() {
		logParamInfo = sysconfigService.getLogParam();
		operPage = "/modules/sysconfig/dialogs/logSave.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 提交日志保存天数配置
	 * @author jinxindong 2016年3月31日 上午9:56:20
	 * @return
	 */
	public String updateLogParam() {
		ajaxData = sysconfigService.updateLogParam(logParamInfo);
		return AJAX;
	}
	
	
	/**
	 * NTP校时配置
	 * @author fuqunqing 2016年9月21日 下午3:23:37
	 * @return
	 */
	public String toNtpTimePage(){
		ntpParamInfo = sysconfigService.getNtpParam();
		operPage = "/modules/sysconfig/dialogs/ntpParam.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 获取校时信息
	 * @author fuqunqing 2016年9月23日 下午7:18:07
	 * @return
	 */
	public String getNtpParamInfoJson(){
		ntpParamInfo = sysconfigService.getNtpParam();
		ajaxData.put("ntpParamInfo", ntpParamInfo);
		return AJAX;
	}
	
	/**
	 * 更新NTP校时服务信息
	 * @author fuqunqing 2016年9月21日 下午6:18:50
	 * @return
	 */
	public String updateNtpParam(){
		ajaxData = sysconfigService.updateNtpParam(ntpParamInfo);
		return AJAX;
	}
	
//======================================平台信息方法====================================================	
	/**
	 * 跳到平台信息页面
	 * @author jinxindong 2016年3月31日 下午3:10:00
	 * @return
	 */
	public String toPlatformParamPage() {
		platformParamInfo = sysconfigService.getPlatformParam();
		operPage = "/modules/sysconfig/dialogs/platformInfo.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 设置平台信息参数
	 * @author jinxindong 2016年3月31日 下午2:11:09
	 * @return
	 */
	public String updatePlatformParam() {
		String path = rootPath + sysconfigFolder;
		File saveFile = new File(path);
		if(!saveFile.exists()) {
			saveFile.mkdirs();
		}
		// logo图片
		if(logoFile != null) {
			String fileLast = logoFileFileName.substring(logoFileFileName.lastIndexOf("."));
			savePic(path + "logoUserDefine" + fileLast, logoFile);
			platformParamInfo.setImgLogo("/" + sysconfigFolder + "logoUserDefine" + fileLast);
		}
		// 登录图片
		if(loginFile != null) {
			String fileLast = loginFileFileName.substring(loginFileFileName.lastIndexOf("."));
			savePic(path + "loginLogoUserDefine" + fileLast, loginFile);
			platformParamInfo.setImgLogin("/" + sysconfigFolder + "loginLogoUserDefine" + fileLast);
		}
		// 主导航栏图片
		if(navbarFile != null) {
			String fileLast = navbarFileFileName.substring(navbarFileFileName.lastIndexOf("."));
			savePic(path + "navbarUserDefine" + fileLast, navbarFile);
			platformParamInfo.setImgBar("/" + sysconfigFolder + "navbarUserDefine" + fileLast);
		}
		ajaxData = sysconfigService.updatePlatformParam(platformParamInfo);
		return "upload";
	}
	
	/**
	 * 平台信息参数恢复默认
	 * @author jinxindong 2016年3月31日 下午2:11:09
	 * @return
	 */
	public String resetPlatformParam() {
		platformParamInfo = new PlatformParamInfo();
		platformParamInfo.setPlatformDiscribe(getText("sysconfig.default.title"));// 平台描述文字
		platformParamInfo.setPlatformCopyright(getText("sysconfig.default.copyRight"));// 版权描述文字
		platformParamInfo.setImgLogo(SysConfigType.Default_LOGO_URL);
		platformParamInfo.setImgBar(SysConfigType.Default_NAVBAR_URL);
		platformParamInfo.setImgLogin(SysConfigType.Default_LOGIN_URL);
		ajaxData = sysconfigService.resetPlatformParam(platformParamInfo);
		return AJAX;
	}
	
	/**
	 * 预览
	 * @author jinxindong 2016年3月31日 下午2:11:41
	 * @return
	 * @throws UnsupportedEncodingException
	 */
	public String preview() throws UnsupportedEncodingException {
		platformParamInfo.setPlatformDiscribe(URLDecoder.decode(platformParamInfo.getPlatformDiscribe(), "UTF-8"));// 平台描述文字
		platformParamInfo.setPlatformCopyright(URLDecoder.decode(platformParamInfo.getPlatformCopyright(), "UTF-8"));// 版权描述文字
		operPage = "/modules/sysconfig/dialogs/preview.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 导航图片上传回显
	 * @author jinxindong 2016年3月31日 下午4:05:51
	 * @return
	 */
	public String loginLogoFilePreview() {
		String path = rootPath + sysconfigTmpFolder;
		File saveFile = new File(path);
		if(!saveFile.exists()) {
			saveFile.mkdirs();
		}
		// 导航图片处理
		if(navbarFileFileName != null) {
			savePic(path + navbarFileFileName, navbarFile);
			msg = "/" + sysconfigTmpFolder + navbarFileFileName;
		} else {
			success = false;
		}
		return "upload";
	}
	
	/**
	 * logo图片上传回显
	 * @author jinxindong 2016年3月31日 下午4:05:32
	 * @return
	 */
	public String logoFilePreview() {
		String path = rootPath + sysconfigTmpFolder;
		File saveFile = new File(path);
		if(!saveFile.exists()) {
			saveFile.mkdirs();
		}
		// 平台logo图片处理
		if(logoFileFileName != null) {
			savePic(path + logoFileFileName, logoFile);
			msg = "/" + sysconfigTmpFolder + logoFileFileName;
		} else {
			success = false;
		}
		return "upload";
	}
	
	/**
	 * 背景登录图片上传回显
	 * @author jinxindong 2016年3月31日 下午4:05:40
	 * @return
	 */
	public String loginFilePreview() {
		String path = rootPath + sysconfigTmpFolder;
		File saveFile = new File(path);
		if(!saveFile.exists()) {
			saveFile.mkdirs();
		}
		// 登录图片
		if(loginFileFileName != null) {
			savePic(path + loginFileFileName, loginFile);
			msg = "/" + sysconfigTmpFolder + loginFileFileName;
		} else {
			success = false;
		}
		return "upload";
	}
	
	/**
	 * 保存文件方法
	 * @author jinxindong 2016年3月31日 下午4:09:24
	 * @param path
	 * @param file
	 */
	private void savePic(String path , File file) {
		FileInputStream inputStream = null;
		FileOutputStream outputStream = null;
		try {
			inputStream = new FileInputStream(file);
			outputStream = new FileOutputStream(path);
            IOUtils.copy(inputStream, outputStream);
            outputStream.flush();
        } catch(IOException e) {
	        throw new ExpectedException("", "图片文件保存失败");
        } finally {
        	IOUtils.closeQuietly(inputStream);
        	IOUtils.closeQuietly(outputStream);
        }
	}
	
	
//======================================平台信息方法====================================================	
	
	
	/**
	 * 跳到业务参数页面
	 * @author jinxindong 2016年3月31日 下午4:29:11
	 * @return
	 */
	public String toBussinessParamPage() {
		sysconfigListIn = sysconfigService.getBussinessParamIn();
		sysconfigListOut = sysconfigService.getBussinessParamOut();
		operPage = "/modules/sysconfig/dialogs/bussinessParam.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 将禁用的置成可用
	 * @author jinxindong 2016年3月31日 下午4:55:57
	 * @return
	 */
	public String enableBussinessParam() {
		ajaxData = sysconfigService.enableBussinessParam(keys);
		return AJAX;
	}
	
	/**
	 * 将可用的置成禁用的
	 * @author jinxindong 2016年3月31日 下午4:56:02
	 * @return
	 */
	public String disableBussinessParam() {
		ajaxData = sysconfigService.disableBussinessParam(keys);
		return AJAX;
	}
	
	/**
	 * 修改业务参数名称
	 * @author jinxindong 2016年3月31日 下午6:19:49
	 * @return
	 */
	public String editBussinessName() {
		if (sysconfig == null || sysconfig.getNkey() == null) {
			throw new ExpectedException("", "未获取到该业务参数的key值");
		}
		ajaxData = sysconfigService.editBussinessName(sysconfig);
		return AJAX;
	}
	// ==========================================================getter/setter=================================================================
	public ISysconfigService getSysconfigService() {
		return sysconfigService;
	}
	
	public void setSysconfigService(ISysconfigService sysconfigService) {
		this.sysconfigService = sysconfigService;
	}
	
	
	
    public SysconfigDto getSysconfigDto() {
    	return sysconfigDto;
    }

	
    public void setSysconfigDto(SysconfigDto sysconfigDto) {
    	this.sysconfigDto = sysconfigDto;
    }

	
    public PlatformParamInfo getPlatformParamInfo() {
    	return platformParamInfo;
    }

	
    public void setPlatformParamInfo(PlatformParamInfo platformParamInfo) {
    	this.platformParamInfo = platformParamInfo;
    }

	


	
    public List<Integer> getKeys() {
    	return keys;
    }

	
    public void setKeys(List<Integer> keys) {
    	this.keys = keys;
    }

	public File getLogoFile() {
		return logoFile;
	}
	
	public void setLogoFile(File logoFile) {
		this.logoFile = logoFile;
	}
	
	public File getLoginFile() {
		return loginFile;
	}
	
	public void setLoginFile(File loginFile) {
		this.loginFile = loginFile;
	}
	
	public File getNavbarFile() {
		return navbarFile;
	}
	
	public void setNavbarFile(File navbarFile) {
		this.navbarFile = navbarFile;
	}
	
	public String getLogoFileFileName() {
		return logoFileFileName;
	}
	
	public void setLogoFileFileName(String logoFileFileName) {
		this.logoFileFileName = logoFileFileName;
	}
	
	public String getLoginFileFileName() {
		return loginFileFileName;
	}
	
	public void setLoginFileFileName(String loginFileFileName) {
		this.loginFileFileName = loginFileFileName;
	}
	
	public String getNavbarFileFileName() {
		return navbarFileFileName;
	}
	
	public void setNavbarFileFileName(String navbarFileFileName) {
		this.navbarFileFileName = navbarFileFileName;
	}
	
	public String getLogoUrl() {
		return logoUrl;
	}
	
	public void setLogoUrl(String logoUrl) {
		this.logoUrl = logoUrl;
	}
	
	public String getLoginUrl() {
		return loginUrl;
	}
	
	public void setLoginUrl(String loginUrl) {
		this.loginUrl = loginUrl;
	}
	
	
    public String getNavbarUrl() {
    	return navbarUrl;
    }

	
    public void setNavbarUrl(String navbarUrl) {
    	this.navbarUrl = navbarUrl;
    }

	public RecordParamInfo getRecordParamInfo() {
		return recordParamInfo;
	}
	
	public void setRecordParamInfo(RecordParamInfo recordParamInfo) {
		this.recordParamInfo = recordParamInfo;
	}
	
	public PasswordParamInfo getPasswordParamInfo() {
		return passwordParamInfo;
	}
	
	public void setPasswordParamInfo(PasswordParamInfo passwordParamInfo) {
		this.passwordParamInfo = passwordParamInfo;
	}
	
	public ModeParamInfo getModeParamInfo() {
		return modeParamInfo;
	}
	
	public void setModeParamInfo(ModeParamInfo modeParamInfo) {
		this.modeParamInfo = modeParamInfo;
	}
	
	public LogParamInfo getLogParamInfo() {
		return logParamInfo;
	}
	
	public void setLogParamInfo(LogParamInfo logParamInfo) {
		this.logParamInfo = logParamInfo;
	}

	
    public Sysconfig getSysconfig() {
    	return sysconfig;
    }

	
    public void setSysconfig(Sysconfig sysconfig) {
    	this.sysconfig = sysconfig;
    }

	
    public List<Sysconfig> getSysconfigListIn() {
    	return sysconfigListIn;
    }

	
    public void setSysconfigListIn(List<Sysconfig> sysconfigListIn) {
    	this.sysconfigListIn = sysconfigListIn;
    }

	
    public List<Sysconfig> getSysconfigListOut() {
    	return sysconfigListOut;
    }

	
    public void setSysconfigListOut(List<Sysconfig> sysconfigListOut) {
    	this.sysconfigListOut = sysconfigListOut;
    }


	public NtpParamInfo getNtpParamInfo() {
	    return ntpParamInfo;
    }


	public void setNtpParamInfo(NtpParamInfo ntpParamInfo) {
	    this.ntpParamInfo = ntpParamInfo;
    }

	

	
	
}
