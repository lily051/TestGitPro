/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-4-24 下午8:17:15
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.webdevice.log.action;

import com.hikvision.finance.fms.model.Operlog;
import com.hikvision.finance.fms.webdevice.log.dto.OperlogInfo;
import com.hikvision.finance.fms.webdevice.log.service.ICollLogService;
import com.hikvision.finance.fwork.www.action.BaseAction;

/**
 * <p></p>
 * @author xujiangfei 2016-4-24 下午8:17:15
 * @version V1.0 
 */
public class LogcollAction extends BaseAction<Operlog> {
	
	/**
	 * 序列化ID
	 */
	private static final long serialVersionUID = -6636455833695409015L;
	private ICollLogService collLogService;
	private OperlogInfo operlogInfo;
	
	/**
	 * 获得操作日志页面数据
	 * @author xujiangfei 2016-4-28 下午8:32:28
	 * @return
	 */
	public String getDatePage() {
		page = collLogService.getDataPage(limit, start, operlogInfo);
		return PAGE;
	}
	
	// ************************setter--getter******************************//
	
	public ICollLogService getCollLogService() {
		return collLogService;
	}
	
	public void setCollLogService(ICollLogService collLogService) {
		this.collLogService = collLogService;
	}
	
	public OperlogInfo getOperlogInfo() {
		return operlogInfo;
	}
	
	public void setOperlogInfo(OperlogInfo operlogInfo) {
		this.operlogInfo = operlogInfo;
	}

}
