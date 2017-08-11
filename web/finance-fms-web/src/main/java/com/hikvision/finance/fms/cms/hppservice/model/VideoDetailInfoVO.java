/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年10月25日 下午6:49:48
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.cms.hppservice.model;

/**
 * <p>
 * </p>
 * 录像的详细信息(DC业务单需求)
 * 
 * @author zhoujiajun 2016年10月25日 下午6:49:48
 * @version V1.0
 */
public class VideoDetailInfoVO {

	private Integer videoId; // 关联表中的主键
	private String fileName; // 录像名
	private Integer recordStatus; // 录像状态Id
	private String strRecordStatus; // 录像上传状态
	private String errorMsg; // 上传失败原因
	private String strUrl; // 录像本地路径

	public String getFileName() {
		return fileName;
	}

	public void setFileName(String fileName) {
		this.fileName = fileName;
	}

	public String getStrRecordStatus() {
		return strRecordStatus;
	}

	public void setStrRecordStatus(String strRecordStatus) {
		this.strRecordStatus = strRecordStatus;
	}

	public String getErrorMsg() {
		return errorMsg;
	}

	public void setErrorMsg(String errorMsg) {
		this.errorMsg = errorMsg;
	}

	public String getStrUrl() {
		return strUrl;
	}

	public void setStrUrl(String strUrl) {
		this.strUrl = strUrl;
	}

	public Integer getRecordStatus() {
		return recordStatus;
	}

	public void setRecordStatus(Integer recordStatus) {
		this.recordStatus = recordStatus;
	}

	public Integer getVideoId() {
		return videoId;
	}

	public void setVideoId(Integer videoId) {
		this.videoId = videoId;
	}
}
