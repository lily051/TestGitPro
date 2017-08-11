/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年9月21日 下午3:44:16
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.sysconfig.dto;

/**
 * <p>
 * ntp校时配置参数对象
 * </p>
 * @author fuqunqing 2016年9月21日 下午3:44:16
 * @version V1.0
 */
public class NtpParamInfo {
	
	private int nenable;
	private String ntpAddress;// NTP服务器地址
	private String ntpPortNo;// NTP服务端口
	private String ntpInterval;// 校时间隔
	
	public String getNtpAddress() {
		return this.ntpAddress;
	}
	
	public void setNtpAddress(String ntpAddress) {
		this.ntpAddress = ntpAddress;
	}
	
	public String getNtpPortNo() {
		return this.ntpPortNo;
	}
	
	public void setNtpPortNo(String ntpPortNo) {
		this.ntpPortNo = ntpPortNo;
	}
	
	public String getNtpInterval() {
		return this.ntpInterval;
	}
	
	public void setNtpInterval(String ntpInterval) {
		this.ntpInterval = ntpInterval;
	}
	
	public int getNenable() {
		return nenable;
	}
	
	public void setNenable(int nenable) {
		this.nenable = nenable;
	}
	
}
