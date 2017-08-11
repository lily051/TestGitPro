/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-4-26 下午1:49:01
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.webdevice.datacollect.dto;


/**
 * 存储usb的一些信息
 * <p></p>
 * @author xujiangfei 2016-5-4 下午4:45:47
 * @version V1.0
 */
public class Usbrinfo {

	private String usbCamIndex;
	private String usbCamName;
	private String usbMicIndex;
	private String usbMicName;
	
	
	
	public String getUsbCamIndex() {
		return usbCamIndex;
	}
	public void setUsbCamIndex(String usbCamIndex) {
		this.usbCamIndex = usbCamIndex;
	}
	public String getUsbCamName() {
		return usbCamName;
	}
	public void setUsbCamName(String usbCamName) {
		this.usbCamName = usbCamName;
	}
	public String getUsbMicIndex() {
		return usbMicIndex;
	}
	public void setUsbMicIndex(String usbMicIndex) {
		this.usbMicIndex = usbMicIndex;
	}
	public String getUsbMicName() {
		return usbMicName;
	}
	public void setUsbMicName(String usbMicName) {
		this.usbMicName = usbMicName;
	}
	
	
	
}
