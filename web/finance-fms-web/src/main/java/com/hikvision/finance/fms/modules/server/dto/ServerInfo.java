/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月11日 下午5:51:24
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.server.dto;

import java.util.Date;



/**
 * <p></p>
 * @author jinxindong 2016年3月11日 下午5:51:24
 * @version V1.0 
 */
public class ServerInfo {
	private Integer id;
	private Integer ntype;
	private String strIp;
	private Integer nctrlPort;
	private Integer ndataPort;
	private String strUser;
	private String strComment;
	private Integer nmultId;
	private Integer nmultVer;
	private Integer nmultSysId;
	private String strConfig;
	private Integer nonline;
	private Date heartbeatTime;
	private String onlineSatus;//在线或者离线状态
	private String strSysIp;//系统巡检IP
	private Integer nctrlSysPort;//系统巡检端口
	private Integer timeOut;//上传时间间隔或者任务时间间隔 （复用）
	private Integer model;//校时模式（每天固定时间校时/固定时间间隔校时）
	private Integer condition;//自动校时误差上限（分钟）
	private String time;//每天定时校时配置（每日校时时间（24小时制）：时 分 秒）
	private Integer fixedTime;//固定时间间隔校时配置（校时时间间隔（小时））
	private Integer alarm;//报警配置（同一报警发送报警中心间隔（分钟））
	private String keyWord;//搜索参数
	private Integer collectId;//采集器的id
	private String collectName;//采集器的名字
	private String workTimeBegin;//上传时间开始
	private String workTimeEnd;//上传时间结束
	private String taskCount;//上传服务并发数量
	private String uploadType;//usb还是ipc "ipc"  "usb"
	private String speed;//usb模式下，最大上传速度(Mbps)
	
    public Integer getId() {
    	return id;
    }
	
    public void setId(Integer id) {
    	this.id = id;
    }
	
    public Integer getNtype() {
    	return ntype;
    }
	
    public void setNtype(Integer ntype) {
    	this.ntype = ntype;
    }
	
    public String getStrIp() {
    	return strIp;
    }
	
    public void setStrIp(String strIp) {
    	this.strIp = strIp;
    }
	
    public Integer getNctrlPort() {
    	return nctrlPort;
    }
	
    public void setNctrlPort(Integer nctrlPort) {
    	this.nctrlPort = nctrlPort;
    }
	
    public Integer getNdataPort() {
    	return ndataPort;
    }
	
    public void setNdataPort(Integer ndataPort) {
    	this.ndataPort = ndataPort;
    }
	
    public String getStrUser() {
    	return strUser;
    }
	
    public void setStrUser(String strUser) {
    	this.strUser = strUser;
    }
	
    public String getStrComment() {
    	return strComment;
    }
	
    public void setStrComment(String strComment) {
    	this.strComment = strComment;
    }
	
    public Integer getNmultId() {
    	return nmultId;
    }
	
    public void setNmultId(Integer nmultId) {
    	this.nmultId = nmultId;
    }
	
    public Integer getNmultVer() {
    	return nmultVer;
    }
	
    public void setNmultVer(Integer nmultVer) {
    	this.nmultVer = nmultVer;
    }
	
    public Integer getNmultSysId() {
    	return nmultSysId;
    }
	
    public void setNmultSysId(Integer nmultSysId) {
    	this.nmultSysId = nmultSysId;
    }
	
    public String getStrConfig() {
    	return strConfig;
    }
	
    public void setStrConfig(String strConfig) {
    	this.strConfig = strConfig;
    }
	
    public Integer getNonline() {
    	return nonline;
    }
	
    public void setNonline(Integer nonline) {
    	this.nonline = nonline;
    }
	
    public String getStrSysIp() {
    	return strSysIp;
    }
	
    public void setStrSysIp(String strSysIp) {
    	this.strSysIp = strSysIp;
    }
	
    public Integer getNctrlSysPort() {
    	return nctrlSysPort;
    }
	
    public void setNctrlSysPort(Integer nctrlSysPort) {
    	this.nctrlSysPort = nctrlSysPort;
    }
	
    public Integer getTimeOut() {
    	return timeOut;
    }
	
    public void setTimeOut(Integer timeOut) {
    	this.timeOut = timeOut;
    }
	
    public Integer getModel() {
    	return model;
    }
	
    public void setModel(Integer model) {
    	this.model = model;
    }
	
    public Integer getCondition() {
    	return condition;
    }
	
    public void setCondition(Integer condition) {
    	this.condition = condition;
    }
	

	
    
    public String getTime() {
    	return time;
    }

	
    public void setTime(String time) {
    	this.time = time;
    }

	public Integer getFixedTime() {
    	return fixedTime;
    }
	
    public void setFixedTime(Integer fixedTime) {
    	this.fixedTime = fixedTime;
    }
	
    public Integer getAlarm() {
    	return alarm;
    }
	
    public void setAlarm(Integer alarm) {
    	this.alarm = alarm;
    }

	
    public String getKeyWord() {
    	return keyWord;
    }

	
    public void setKeyWord(String keyWord) {
    	this.keyWord = keyWord;
    }

	
    public Integer getCollectId() {
    	return collectId;
    }

	
    public void setCollectId(Integer collectId) {
    	this.collectId = collectId;
    }

	
    public String getCollectName() {
    	return collectName;
    }

	
    public void setCollectName(String collectName) {
    	this.collectName = collectName;
    }

	
    public String getOnlineSatus() {
    	return onlineSatus;
    }

	
    public void setOnlineSatus(String onlineSatus) {
    	this.onlineSatus = onlineSatus;
    }

	
    public String getWorkTimeBegin() {
    	return workTimeBegin;
    }

	
    public void setWorkTimeBegin(String workTimeBegin) {
    	this.workTimeBegin = workTimeBegin;
    }

	
    public String getWorkTimeEnd() {
    	return workTimeEnd;
    }

	
    public void setWorkTimeEnd(String workTimeEnd) {
    	this.workTimeEnd = workTimeEnd;
    }

	
    public String getTaskCount() {
    	return taskCount;
    }

	
    public void setTaskCount(String taskCount) {
    	this.taskCount = taskCount;
    }

	
    public Date getHeartbeatTime() {
    	return heartbeatTime;
    }

	
    public void setHeartbeatTime(Date heartbeatTime) {
    	this.heartbeatTime = heartbeatTime;
    }

	
    public String getUploadType() {
    	return uploadType;
    }

	
    public void setUploadType(String uploadType) {
    	this.uploadType = uploadType;
    }

    public String getSpeed() {
		return speed;
	}
    
    public void setSpeed(String speed) {
		this.speed = speed;
	}
}
