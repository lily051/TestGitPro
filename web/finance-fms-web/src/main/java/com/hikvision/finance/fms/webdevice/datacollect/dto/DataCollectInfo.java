/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月19日 下午4:52:55
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.webdevice.datacollect.dto;


/**
 * <p>存放Records中没有的字段</p>
 * @author jinchenwu 2016年4月19日 下午4:52:55
 * @version V1.0 
 */
public class DataCollectInfo {
	
	private Integer deviceChannelId;// 设备通道Id
	
	public Integer getDeviceChannelId() {
		return deviceChannelId;
	}
	
	public void setDeviceChannelId(Integer deviceChannelId) {
		this.deviceChannelId = deviceChannelId;
	}
}
