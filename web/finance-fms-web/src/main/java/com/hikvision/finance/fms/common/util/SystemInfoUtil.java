/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年10月31日 下午2:09:24
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.common.util;

/**
 * <p>
 * 配置一些系统参数，这些系统参数写在config.properties文件中
 * </p>
 * @author zhengquanwu 2016年10月31日 下午2:09:24
 * @version V1.0 
 */
public class SystemInfoUtil {
	private String nonCenterStorage;//无中心存储
	private String channelIpBind;//通道ip绑定
	private String logReport;//日志上报
	private String choiceUsbIpc;//通道方式
	private String logReportMethod;//日志上报方式

	public String getNonCenterStorage() {
		return nonCenterStorage;
	}

	public void setNonCenterStorage(String nonCenterStorage) {
		this.nonCenterStorage = nonCenterStorage;
	}

	public String getChannelIpBind() {
		return channelIpBind;
	}

	public void setChannelIpBind(String channelIpBind) {
		this.channelIpBind = channelIpBind;
	}

	public String getLogReport() {
		return logReport;
	}

	public void setLogReport(String logReport) {
		this.logReport = logReport;
	}
	
	public String getChoiceUsbIpc() {
		return choiceUsbIpc;
	}
	
	public void setChoiceUsbIpc(String choiceUsbIpc) {
		this.choiceUsbIpc = choiceUsbIpc;
	}
	
	public String getLogReportMethod() {
		return logReportMethod;
	}
	
	public void setLogReportMethod(String logReportMethod) {
		this.logReportMethod = logReportMethod;
	}
}
