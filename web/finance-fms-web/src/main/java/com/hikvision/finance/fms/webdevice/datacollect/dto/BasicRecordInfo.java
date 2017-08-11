/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年5月12日 下午3:09:45
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.webdevice.datacollect.dto;


/**
 * <p>存储基础录像信息，与Records对应，视频文件列表专用</p>
 * @author jinchenwu 2016年5月12日 下午3:09:45
 * @version V1.0 
 */
public class BasicRecordInfo {
	
	private Integer id;// 录像Id
	private Integer nuserId;// 用户Id
	private String strRecordName;// 录像名称
	private Integer nrecordStatus;// 录像状态
	private String errorMsg;// 上传错误信息
	
	public Integer getId() {
		return id;
	}
	
	public void setId(Integer id) {
		this.id = id;
	}
	
	public String getStrRecordName() {
		return strRecordName;
	}
	
	public void setStrRecordName(String strRecordName) {
		this.strRecordName = strRecordName;
	}
	
	public Integer getNrecordStatus() {
		return nrecordStatus;
	}
	
	public void setNrecordStatus(Integer nrecordStatus) {
		this.nrecordStatus = nrecordStatus;
	}
	
	public Integer getNuserId() {
		return nuserId;
	}
	
	public void setNuserId(Integer nuserId) {
		this.nuserId = nuserId;
	}
	
	public String getErrorMsg() {
		return errorMsg;
	}
	
	public void setErrorMsg(String errorMsg) {
		this.errorMsg = errorMsg;
	}
	
}
