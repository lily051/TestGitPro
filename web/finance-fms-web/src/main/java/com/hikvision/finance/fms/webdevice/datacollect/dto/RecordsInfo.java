/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-4-20 下午6:11:11
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.webdevice.datacollect.dto;

import java.util.Date;

/**
 * <p></p> 后台查出来的数据（以records表为主表，和其他表的一些数据）
 * @author xujiangfei 2016-4-20 下午6:11:11
 * @version V1.0 
 */
public class RecordsInfo {
	//records表
	private Integer id;
	private Integer nuserId;
	private String streamNo;
	private String strRecordName;
	private Integer nrecordType;
	private Integer nrecordFileType;
	private Integer nrecordStatus;
	private Date dtBusinessTime;
	private Integer nrecordLocalTime;
	private String startTime;
	private String endTime;
	private Integer nfileHeadLen;
	private String strUrl;
	private Integer ncallerType;
	private String strCallerId;
	private Integer nrepairType;
	private Integer nrepairDevId;
	private Integer nrepairTime;
	private Integer nlinkOrderId;
	private Integer nlinkType;
	private String strCollectIp;
	private Integer ncollectChan;
	private Integer ncollectRegionId;
	private Integer nupLoadPercent;
	private String dvrUserName;
	private String dvrPwd;
	private Integer dvrPort;
	private Integer nmanufacturerType;
	private Integer nfileSize;
	private String screenshotSrc;
	
	//Device表
	private Integer streamType;// 码流类型

	//DeviceChannel表
	private Integer nchannelNo;
	private String nchannelName;
	
	
	public Integer getId() {
		return id;
	}
	public void setId(Integer id) {
		this.id = id;
	}
	public Integer getNuserId() {
		return nuserId;
	}
	public void setNuserId(Integer nuserId) {
		this.nuserId = nuserId;
	}
	public String getStrRecordName() {
		return strRecordName;
	}
	public void setStrRecordName(String strRecordName) {
		this.strRecordName = strRecordName;
	}
	public Integer getNrecordType() {
		return nrecordType;
	}
	public void setNrecordType(Integer nrecordType) {
		this.nrecordType = nrecordType;
	}
	public Integer getNrecordFileType() {
		return nrecordFileType;
	}
	public void setNrecordFileType(Integer nrecordFileType) {
		this.nrecordFileType = nrecordFileType;
	}
	public Integer getNrecordStatus() {
		return nrecordStatus;
	}
	public void setNrecordStatus(Integer nrecordStatus) {
		this.nrecordStatus = nrecordStatus;
	}
	public Date getDtBusinessTime() {
		return dtBusinessTime;
	}
	public void setDtBusinessTime(Date dtBusinessTime) {
		this.dtBusinessTime = dtBusinessTime;
	}
	public Integer getNrecordLocalTime() {
		return nrecordLocalTime;
	}
	public void setNrecordLocalTime(Integer nrecordLocalTime) {
		this.nrecordLocalTime = nrecordLocalTime;
	}
	public Integer getNfileHeadLen() {
		return nfileHeadLen;
	}
	public void setNfileHeadLen(Integer nfileHeadLen) {
		this.nfileHeadLen = nfileHeadLen;
	}
	public String getStrUrl() {
		return strUrl;
	}
	public void setStrUrl(String strUrl) {
		this.strUrl = strUrl;
	}
	public Integer getNcallerType() {
		return ncallerType;
	}
	public void setNcallerType(Integer ncallerType) {
		this.ncallerType = ncallerType;
	}
	public String getStrCallerId() {
		return strCallerId;
	}
	public void setStrCallerId(String strCallerId) {
		this.strCallerId = strCallerId;
	}
	public Integer getNrepairType() {
		return nrepairType;
	}
	public void setNrepairType(Integer nrepairType) {
		this.nrepairType = nrepairType;
	}
	public Integer getNrepairDevId() {
		return nrepairDevId;
	}
	public void setNrepairDevId(Integer nrepairDevId) {
		this.nrepairDevId = nrepairDevId;
	}
	public Integer getNrepairTime() {
		return nrepairTime;
	}
	public void setNrepairTime(Integer nrepairTime) {
		this.nrepairTime = nrepairTime;
	}
	public Integer getNlinkOrderId() {
		return nlinkOrderId;
	}
	public void setNlinkOrderId(Integer nlinkOrderId) {
		this.nlinkOrderId = nlinkOrderId;
	}
	public Integer getNlinkType() {
		return nlinkType;
	}
	public void setNlinkType(Integer nlinkType) {
		this.nlinkType = nlinkType;
	}
	public String getStrCollectIp() {
		return strCollectIp;
	}
	public void setStrCollectIp(String strCollectIp) {
		this.strCollectIp = strCollectIp;
	}
	public Integer getNcollectChan() {
		return ncollectChan;
	}
	public void setNcollectChan(Integer ncollectChan) {
		this.ncollectChan = ncollectChan;
	}
	public Integer getNcollectRegionId() {
		return ncollectRegionId;
	}
	public void setNcollectRegionId(Integer ncollectRegionId) {
		this.ncollectRegionId = ncollectRegionId;
	}
	public Integer getNupLoadPercent() {
		return nupLoadPercent;
	}
	public void setNupLoadPercent(Integer nupLoadPercent) {
		this.nupLoadPercent = nupLoadPercent;
	}
	public String getDvrUserName() {
		return dvrUserName;
	}
	public void setDvrUserName(String dvrUserName) {
		this.dvrUserName = dvrUserName;
	}
	public String getDvrPwd() {
		return dvrPwd;
	}
	public void setDvrPwd(String dvrPwd) {
		this.dvrPwd = dvrPwd;
	}
	public Integer getDvrPort() {
		return dvrPort;
	}
	public void setDvrPort(Integer dvrPort) {
		this.dvrPort = dvrPort;
	}
	public Integer getNmanufacturerType() {
		return nmanufacturerType;
	}
	public void setNmanufacturerType(Integer nmanufacturerType) {
		this.nmanufacturerType = nmanufacturerType;
	}
	public Integer getNfileSize() {
		return nfileSize;
	}
	public void setNfileSize(Integer nfileSize) {
		this.nfileSize = nfileSize;
	}
	public String getScreenshotSrc() {
		return screenshotSrc;
	}
	public void setScreenshotSrc(String screenshotSrc) {
		this.screenshotSrc = screenshotSrc;
	}
	public Integer getNchannelNo() {
		return nchannelNo;
	}
	public void setNchannelNo(Integer nchannelNo) {
		this.nchannelNo = nchannelNo;
	}
	public String getNchannelName() {
		return nchannelName;
	}
	public void setNchannelName(String nchannelName) {
		this.nchannelName = nchannelName;
	}

    public Integer getStreamType() {
    	return streamType;
    }
	
    public void setStreamType(Integer streamType) {
    	this.streamType = streamType;
    }
	public String getStreamNo() {
		return streamNo;
	}
	public void setStreamNo(String streamNo) {
		this.streamNo = streamNo;
	}
	/*public Date getStartTime() {
		return startTime;
	}
	public void setStartTime(Date startTime) {
		this.startTime = startTime;
	}
	public Date getEndTime() {
		return endTime;
	}
	public void setEndTime(Date endTime) {
		this.endTime = endTime;
	}*/
	public String getStartTime() {
		return startTime;
	}
	public void setStartTime(String startTime) {
		this.startTime = startTime;
	}
	public String getEndTime() {
		return endTime;
	}
	public void setEndTime(String endTime) {
		this.endTime = endTime;
	}
	
	
	
}
