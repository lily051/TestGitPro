package com.hikvision.finance.fms.model;

import static javax.persistence.GenerationType.IDENTITY;

import java.util.Date;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.Id;
import javax.persistence.Table;

import org.apache.struts2.json.annotations.JSON;


/**
 * Prosaverecord entity. @author MyEclipse Persistence Tools
 */
@Entity
@Table(name = "prosaverecord")
public class Prosaverecord implements java.io.Serializable {
	
	// Fields
	
	/**
     * 序列化ID
     */
    private static final long serialVersionUID = -884251757032728535L;
	private Integer id;
	private Integer clientId;
	private Integer regionId;
	private Integer userId;
	private Integer cvr;
	private String streamNo;
	private Integer stateType;
	private String fileName;
	private Date startTime;
	private Date endTime;
	private Integer upload;
	private Integer recordCheck;
	private String checkRemark;
	private String strUrl;
	private Integer devId;
	private Integer devChannel;
	private String screenshotSrc;
	private String rtspPath;
	private Integer nfileSize;
	private Integer ntype;
	private Integer mediaType;
	private Integer calltype;
	private String callNo;
	private String recordUserNo;
	private String devIp;
	private Integer isUpdateKmsTime;//是否从KMS上更新录像的开始结束时间
	// Constructors
	
	/** default constructor */
	public Prosaverecord() {
	}
	
	/** full constructor */
	public Prosaverecord(Integer clientId, Integer regionId, Integer userId, Integer cvr, String streamNo,
	        Integer stateType, String fileName, Date startTime, Date endTime, Integer upload, Integer recordCheck,
	        String checkRemark, String strUrl, Integer devId, Integer devChannel, String screenshotSrc, String rtspPath,
	        Integer nfileSize, Integer ntype, Integer mediaType, Integer calltype, String callNo, String recordUserNo,
	        String devIp) {
		this.clientId = clientId;
		this.regionId = regionId;
		this.userId = userId;
		this.cvr = cvr;
		this.streamNo = streamNo;
		this.stateType = stateType;
		this.fileName = fileName;
		this.startTime = startTime;
		this.endTime = endTime;
		this.upload = upload;
		this.recordCheck = recordCheck;
		this.checkRemark = checkRemark;
		this.strUrl = strUrl;
		this.devId = devId;
		this.devChannel = devChannel;
		this.screenshotSrc = screenshotSrc;
		this.rtspPath = rtspPath;
		this.nfileSize = nfileSize;
		this.ntype = ntype;
		this.mediaType = mediaType;
		this.calltype = calltype;
		this.callNo = callNo;
		this.recordUserNo = recordUserNo;
		this.devIp = devIp;
	}
	
	// Property accessors
	@Id
	@GeneratedValue(strategy = IDENTITY)
	@Column(name = "id", unique = true, nullable = false)
	public Integer getId() {
		return this.id;
	}
	
	public void setId(Integer id) {
		this.id = id;
	}
	
	@Column(name = "clientId")
	public Integer getClientId() {
		return this.clientId;
	}
	
	public void setClientId(Integer clientId) {
		this.clientId = clientId;
	}
	
	@Column(name = "regionId")
	public Integer getRegionId() {
		return this.regionId;
	}
	
	public void setRegionId(Integer regionId) {
		this.regionId = regionId;
	}
	
	@Column(name = "userId")
	public Integer getUserId() {
		return this.userId;
	}
	
	public void setUserId(Integer userId) {
		this.userId = userId;
	}
	
	@Column(name = "cvr")
	public Integer getCvr() {
		return this.cvr;
	}
	
	public void setCvr(Integer cvr) {
		this.cvr = cvr;
	}
	
	@Column(name = "streamNo", length = 64)
	public String getStreamNo() {
		return this.streamNo;
	}
	
	public void setStreamNo(String streamNo) {
		this.streamNo = streamNo;
	}
	
	@Column(name = "stateType")
	public Integer getStateType() {
		return this.stateType;
	}
	
	public void setStateType(Integer stateType) {
		this.stateType = stateType;
	}
	
	@Column(name = "fileName", length = 50)
	public String getFileName() {
		return this.fileName;
	}
	
	public void setFileName(String fileName) {
		this.fileName = fileName;
	}
	
	@Column(name = "startTime", length = 0)
	@JSON(format="yyyy-MM-dd HH:mm:ss")
	public Date getStartTime() {
		return this.startTime;
	}
	
	public void setStartTime(Date startTime) {
		this.startTime = startTime;
	}
	
	@Column(name = "endTime", length = 0)
	@JSON(format="yyyy-MM-dd HH:mm:ss")
	public Date getEndTime() {
		return this.endTime;
	}
	
	public void setEndTime(Date endTime) {
		this.endTime = endTime;
	}
	
	@Column(name = "upload")
	public Integer getUpload() {
		return this.upload;
	}
	
	public void setUpload(Integer upload) {
		this.upload = upload;
	}
	
	@Column(name = "recordCheck")
	public Integer getRecordCheck() {
		return this.recordCheck;
	}
	
	public void setRecordCheck(Integer recordCheck) {
		this.recordCheck = recordCheck;
	}
	
	@Column(name = "checkRemark", length = 200)
	public String getCheckRemark() {
		return this.checkRemark;
	}
	
	public void setCheckRemark(String checkRemark) {
		this.checkRemark = checkRemark;
	}
	
	@Column(name = "strUrl", length = 200)
	public String getStrUrl() {
		return this.strUrl;
	}
	
	public void setStrUrl(String strUrl) {
		this.strUrl = strUrl;
	}
	
	@Column(name = "devId")
	public Integer getDevId() {
		return this.devId;
	}
	
	public void setDevId(Integer devId) {
		this.devId = devId;
	}
	
	@Column(name = "devChannel")
	public Integer getDevChannel() {
		return this.devChannel;
	}
	
	public void setDevChannel(Integer devChannel) {
		this.devChannel = devChannel;
	}
	
	@Column(name = "screenshotSrc", length = 500)
	public String getScreenshotSrc() {
		return this.screenshotSrc;
	}
	
	public void setScreenshotSrc(String screenshotSrc) {
		this.screenshotSrc = screenshotSrc;
	}
	
	@Column(name = "rtspPath")
	public String getRtspPath() {
		return this.rtspPath;
	}
	
	public void setRtspPath(String rtspPath) {
		this.rtspPath = rtspPath;
	}
	
	@Column(name = "nFileSize")
	public Integer getNfileSize() {
		return this.nfileSize;
	}
	
	public void setNfileSize(Integer nfileSize) {
		this.nfileSize = nfileSize;
	}
	
	@Column(name = "nType")
	public Integer getNtype() {
		return this.ntype;
	}
	
	public void setNtype(Integer ntype) {
		this.ntype = ntype;
	}
	
	@Column(name = "mediaType")
	public Integer getMediaType() {
		return this.mediaType;
	}
	
	public void setMediaType(Integer mediaType) {
		this.mediaType = mediaType;
	}
	
	@Column(name = "calltype")
	public Integer getCalltype() {
		return this.calltype;
	}
	
	public void setCalltype(Integer calltype) {
		this.calltype = calltype;
	}
	
	@Column(name = "callNo", length = 20)
	public String getCallNo() {
		return this.callNo;
	}
	
	public void setCallNo(String callNo) {
		this.callNo = callNo;
	}
	
	@Column(name = "recordUserNo", length = 50)
	public String getRecordUserNo() {
		return this.recordUserNo;
	}
	
	public void setRecordUserNo(String recordUserNo) {
		this.recordUserNo = recordUserNo;
	}
	
	@Column(name = "devIp", length = 16)
	public String getDevIp() {
		return this.devIp;
	}
	
	public void setDevIp(String devIp) {
		this.devIp = devIp;
	}
	
	@Column(name = "isUpdateKmsTime")
	public Integer getIsUpdateKmsTime() {
		return isUpdateKmsTime;
	}

	public void setIsUpdateKmsTime(Integer isUpdateKmsTime) {
		this.isUpdateKmsTime = isUpdateKmsTime;
	}

}
