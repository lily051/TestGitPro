/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月6日 下午7:20:05
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.alarm.dto;

import java.util.Date;

import org.apache.struts2.json.annotations.JSON;


/**
 * <p></p>
 * @author jinxindong 2016年4月6日 下午7:20:05
 * @version V1.0 
 */
public class AlarmlogInfo {
	private Integer id;
	private Integer checkType;
	private Integer checkObjId;
	private Integer checkChan;
	private Integer contactObjId;
	private Integer alarmType;
	private Date alarmTime;
	private Date alarmbeginTime;
	private Integer alarmRegionId;
	private String comments;
	private String strHost;
	private Integer status;
	private String checker;
	private String userNo;
	private Date dealTime;
	private String dealcomment;
	private Integer alarmlevel;
	private String strGuid;
	private String alarmbeginTimeStart ;//报警发生开始 起始时间
	private String alarmbeginTimeEnd ;//报警发生开始  结束时间
	private String alarmTypeName;//报警类型名称
	private String alarmRegionName;//报警所在区域
	private Integer isFlag;//是否显示子区域
    public Integer getId() {
    	return id;
    }
	
    public void setId(Integer id) {
    	this.id = id;
    }
	
    public Integer getCheckType() {
    	return checkType;
    }
	
    public void setCheckType(Integer checkType) {
    	this.checkType = checkType;
    }
	
    public Integer getCheckObjId() {
    	return checkObjId;
    }
	
    public void setCheckObjId(Integer checkObjId) {
    	this.checkObjId = checkObjId;
    }
	
    public Integer getCheckChan() {
    	return checkChan;
    }
	
    public void setCheckChan(Integer checkChan) {
    	this.checkChan = checkChan;
    }
	
    public Integer getContactObjId() {
    	return contactObjId;
    }
	
    public void setContactObjId(Integer contactObjId) {
    	this.contactObjId = contactObjId;
    }
	
    public Integer getAlarmType() {
    	return alarmType;
    }
	
    public void setAlarmType(Integer alarmType) {
    	this.alarmType = alarmType;
    }
    @JSON(format="yyyy-MM-dd HH:mm:ss")
    public Date getAlarmTime() {
    	return alarmTime;
    }
	
    public void setAlarmTime(Date alarmTime) {
    	this.alarmTime = alarmTime;
    }
    @JSON(format="yyyy-MM-dd HH:mm:ss")
    public Date getAlarmbeginTime() {
    	return alarmbeginTime;
    }
	
    public void setAlarmbeginTime(Date alarmbeginTime) {
    	this.alarmbeginTime = alarmbeginTime;
    }
	
    public Integer getAlarmRegionId() {
    	return alarmRegionId;
    }
	
    public void setAlarmRegionId(Integer alarmRegionId) {
    	this.alarmRegionId = alarmRegionId;
    }
	
    public String getComments() {
    	return comments;
    }
	
    public void setComments(String comments) {
    	this.comments = comments;
    }
	
    public String getStrHost() {
    	return strHost;
    }
	
    public void setStrHost(String strHost) {
    	this.strHost = strHost;
    }
	
    public Integer getStatus() {
    	return status;
    }
	
    public void setStatus(Integer status) {
    	this.status = status;
    }
	
    public String getChecker() {
    	return checker;
    }
	
    public void setChecker(String checker) {
    	this.checker = checker;
    }
	
    public String getUserNo() {
    	return userNo;
    }
	
    public void setUserNo(String userNo) {
    	this.userNo = userNo;
    }
    @JSON(format="yyyy-MM-dd HH:mm:ss")
    public Date getDealTime() {
    	return dealTime;
    }
	
    public void setDealTime(Date dealTime) {
    	this.dealTime = dealTime;
    }
	
    public String getDealcomment() {
    	return dealcomment;
    }
	
    public void setDealcomment(String dealcomment) {
    	this.dealcomment = dealcomment;
    }
	
    public Integer getAlarmlevel() {
    	return alarmlevel;
    }
	
    public void setAlarmlevel(Integer alarmlevel) {
    	this.alarmlevel = alarmlevel;
    }
	
    public String getStrGuid() {
    	return strGuid;
    }
	
    public void setStrGuid(String strGuid) {
    	this.strGuid = strGuid;
    }

	
    public String getAlarmbeginTimeStart() {
    	return alarmbeginTimeStart;
    }

	
    public void setAlarmbeginTimeStart(String alarmbeginTimeStart) {
    	this.alarmbeginTimeStart = alarmbeginTimeStart;
    }

	
    public String getAlarmbeginTimeEnd() {
    	return alarmbeginTimeEnd;
    }

	
    public void setAlarmbeginTimeEnd(String alarmbeginTimeEnd) {
    	this.alarmbeginTimeEnd = alarmbeginTimeEnd;
    }

	
    public String getAlarmTypeName() {
    	return alarmTypeName;
    }

	
    public void setAlarmTypeName(String alarmTypeName) {
    	this.alarmTypeName = alarmTypeName;
    }

	
    public String getAlarmRegionName() {
    	return alarmRegionName;
    }

	
    public void setAlarmRegionName(String alarmRegionName) {
    	this.alarmRegionName = alarmRegionName;
    }

	
    public Integer getIsFlag() {
    	return isFlag;
    }

	
    public void setIsFlag(Integer isFlag) {
    	this.isFlag = isFlag;
    }
	
	
}
