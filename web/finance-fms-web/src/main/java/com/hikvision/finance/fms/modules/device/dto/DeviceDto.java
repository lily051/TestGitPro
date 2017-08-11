/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月5日 下午4:32:21
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.device.dto;

import java.util.List;


/**
 * <p></p>
 * @author jinxindong 2016年4月5日 下午4:32:21
 * @version V1.0 
 */
public class DeviceDto {
	private List<CloudPoolInfo> cloudPoolInfoList;
	private List<DeviceChannelInfo> deviceChannelList;

	
    public List<CloudPoolInfo> getCloudPoolInfoList() {
    	return cloudPoolInfoList;
    }

	
    public void setCloudPoolInfoList(List<CloudPoolInfo> cloudPoolInfoList) {
    	this.cloudPoolInfoList = cloudPoolInfoList;
    }


	
    public List<DeviceChannelInfo> getDeviceChannelList() {
    	return deviceChannelList;
    }


	
    public void setDeviceChannelList(List<DeviceChannelInfo> deviceChannelList) {
    	this.deviceChannelList = deviceChannelList;
    }
	
	
}
