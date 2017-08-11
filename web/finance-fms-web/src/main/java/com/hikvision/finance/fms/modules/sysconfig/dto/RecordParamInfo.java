/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月30日 下午7:15:24
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.sysconfig.dto;

/**
 * <p>录像参数包装类</p>
 * @author jinxindong 2016年3月30日 下午7:15:24
 * @version V1.0
 */
public class RecordParamInfo {
	
	private String maximumVideoLength; // 最大录像时长
	private String playBackVideoNum; // 同时回放录像数
	private String videoPlayBackBandWidth; // 录像回放带宽
	private String videoUploadBandWidth; // 上传录像带宽
	private String uploadTimeStart; // 上传时间段开始时间
	private String uploadTimeEnd; // 上传时间段结束时间
	private String videoDeleteDays; // 录像删除提示天数
	
    public String getMaximumVideoLength() {
    	return maximumVideoLength;
    }
	
    public void setMaximumVideoLength(String maximumVideoLength) {
    	this.maximumVideoLength = maximumVideoLength;
    }
	
    public String getPlayBackVideoNum() {
    	return playBackVideoNum;
    }
	
    public void setPlayBackVideoNum(String playBackVideoNum) {
    	this.playBackVideoNum = playBackVideoNum;
    }
	
    public String getVideoPlayBackBandWidth() {
    	return videoPlayBackBandWidth;
    }
	
    public void setVideoPlayBackBandWidth(String videoPlayBackBandWidth) {
    	this.videoPlayBackBandWidth = videoPlayBackBandWidth;
    }
	
    public String getVideoUploadBandWidth() {
    	return videoUploadBandWidth;
    }
	
    public void setVideoUploadBandWidth(String videoUploadBandWidth) {
    	this.videoUploadBandWidth = videoUploadBandWidth;
    }
	
    public String getUploadTimeStart() {
    	return uploadTimeStart;
    }
	
    public void setUploadTimeStart(String uploadTimeStart) {
    	this.uploadTimeStart = uploadTimeStart;
    }
	
    public String getUploadTimeEnd() {
    	return uploadTimeEnd;
    }
	
    public void setUploadTimeEnd(String uploadTimeEnd) {
    	this.uploadTimeEnd = uploadTimeEnd;
    }
	
    public String getVideoDeleteDays() {
    	return videoDeleteDays;
    }
	
    public void setVideoDeleteDays(String videoDeleteDays) {
    	this.videoDeleteDays = videoDeleteDays;
    }
	

	
	
	
}
