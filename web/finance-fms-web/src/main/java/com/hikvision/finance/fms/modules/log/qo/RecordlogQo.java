/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月8日 下午4:13:43
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.log.qo;

import java.util.Date;

import com.hikvision.finance.extend.hibernate.qo.BaseQo;


/**
 * <p></p>
 * @author jinxindong 2016年4月8日 下午4:13:43
 * @version V1.0 
 */
public class RecordlogQo extends BaseQo{
	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 1350428294091456786L;
	private Integer id;
	private String recordName;
	private Integer recordType;
	private String nproId;
	private Integer checkUserNo;
	private String regionId;
	private String remark;
	private Date operDate;
	private String checkUserName;
	private Integer isflag;
	
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
	
    public String getRegionId() {
    	return regionId;
    }
	
    public void setRegionId(String regionId) {
    	this.regionId = regionId;
    }
	
    public String getRemark() {
    	return remark;
    }
	
    public void setRemark(String remark) {
    	this.remark = remark;
    }
	
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

	
    public Integer getIsflag() {
    	return isflag;
    }

	
    public void setIsflag(Integer isflag) {
    	this.isflag = isflag;
    }
	
	
}
