/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月8日 下午4:13:43
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.log.dto;

import java.util.Date;

import org.apache.struts2.json.annotations.JSON;


/**
 * <p></p>
 * @author jinxindong 2016年4月8日 下午4:13:43
 * @version V1.0 
 */
public class RecordlogInfo {
	private Integer id;
	private String recordName;//recordName
	private Integer recordType;//recordType
	private String nproId;//业务单号
	private Integer checkUserNo;//checkUserNo
	private Integer regionId;//区域id
	private String remark;//操作备注
	private Date operDate;//操作时间
	private String operDateString;//操作时间
	private String checkUserName;//检查员工名字
	private Integer isFlag;//是否显示子区域
	private String operDateStart;//开始
	private String operDateEnd;//结束
	private String recordTypeName;//录像类型名称
	
    public Integer getId() {
    	return id;
    }
	
    public void setId(Integer id) {
    	this.id = id;
    }
	
    public String getRecordName() {
    	return recordName;
    }
	
    public void setRecordName(String recordName) {
    	this.recordName = recordName;
    }
	
    public Integer getRecordType() {
    	return recordType;
    }
	
    public void setRecordType(Integer recordType) {
    	this.recordType = recordType;
    }
	
    public String getNproId() {
    	return nproId;
    }
	
    public void setNproId(String nproId) {
    	this.nproId = nproId;
    }
	
    public Integer getCheckUserNo() {
    	return checkUserNo;
    }
	
    public void setCheckUserNo(Integer checkUserNo) {
    	this.checkUserNo = checkUserNo;
    }
	

	
    
    public Integer getRegionId() {
    	return regionId;
    }

	
    public void setRegionId(Integer regionId) {
    	this.regionId = regionId;
    }

	public String getRemark() {
    	return remark;
    }
	
    public void setRemark(String remark) {
    	this.remark = remark;
    }
	
    @JSON(format="yyyy-MM-dd HH:mm:ss")
    public Date getOperDate() {
    	return operDate;
    }
	
    public void setOperDate(Date operDate) {
    	this.operDate = operDate;
    }
	
    public String getCheckUserName() {
    	return checkUserName;
    }
	
    public void setCheckUserName(String checkUserName) {
    	this.checkUserName = checkUserName;
    }

	


	
    
    public Integer getIsFlag() {
    	return isFlag;
    }

	
    public void setIsFlag(Integer isFlag) {
    	this.isFlag = isFlag;
    }

	public String getOperDateStart() {
    	return operDateStart;
    }

	
    public void setOperDateStart(String operDateStart) {
    	this.operDateStart = operDateStart;
    }

	
    public String getOperDateEnd() {
    	return operDateEnd;
    }

	
    public void setOperDateEnd(String operDateEnd) {
    	this.operDateEnd = operDateEnd;
    }

	
    public String getRecordTypeName() {
    	return recordTypeName;
    }

	
    public void setRecordTypeName(String recordTypeName) {
    	this.recordTypeName = recordTypeName;
    }

	
    public String getOperDateString() {
    	return operDateString;
    }

	
    public void setOperDateString(String operDateString) {
    	this.operDateString = operDateString;
    }
	
	
}
