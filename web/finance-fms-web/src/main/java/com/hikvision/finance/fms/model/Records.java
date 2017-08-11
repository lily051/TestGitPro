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
 * Records entity. @author MyEclipse Persistence Tools
 */
@Entity
@Table(name = "records")
public class Records implements java.io.Serializable {
	
	// Fields
	
	/**
     * 序列化ID
     */
    private static final long serialVersionUID = -2019262801797500683L;
	/**
     * 序列化ID
     */
	private Integer id;
	private Integer nHistoryRecordId;
	private Integer nuserId;
	private String streamNo;
	private String strRecordName;
	private Integer nrecordType;
	private Integer nrecordFileType;
	private Integer nrecordStatus;
	private Date startTime;
	private Date endTime;
	private String strUrl;
	private Integer nrepairDevId;
	private String strCollectIp;
	private Integer ncollectChan;
	private Integer ncollectRegionId;
	private Integer nupLoadPercent;
	private String dvrUserName;
	private String dvrPwd;
	private Integer dvrPort;
	private Integer nfileSize;
	private String screenshotSrc;
	private Integer nTaskId;
	private String strIp;
	private String errorMsg;
	
	// Constructors
	
	/** default constructor */
	public Records() {
	}
	
	/** full constructor */
	public Records(Integer nuserId, Integer nHistoryRecordId, String streamNo, String strRecordName, Integer nrecordType, Integer nrecordFileType,
	        Integer nrecordStatus, Date startTime, Date endTime, String strUrl, Integer nrepairDevId,
	        String strCollectIp, Integer ncollectChan, Integer ncollectRegionId, Integer nupLoadPercent, String dvrUserName,
	        String dvrPwd, Integer dvrPort, Integer nfileSize, String screenshotSrc, Integer nTaskId, String strIp,
	        String errorMsg) {
		this.nuserId = nuserId;
		this.nHistoryRecordId = nHistoryRecordId;
		this.streamNo = streamNo;
		this.strRecordName = strRecordName;
		this.nrecordType = nrecordType;
		this.nrecordFileType = nrecordFileType;
		this.nrecordStatus = nrecordStatus;
		this.startTime = startTime;
		this.endTime = endTime;
		this.strUrl = strUrl;
		this.nrepairDevId = nrepairDevId;
		this.strCollectIp = strCollectIp;
		this.ncollectChan = ncollectChan;
		this.ncollectRegionId = ncollectRegionId;
		this.nupLoadPercent = nupLoadPercent;
		this.dvrUserName = dvrUserName;
		this.dvrPwd = dvrPwd;
		this.dvrPort = dvrPort;
		this.nfileSize = nfileSize;
		this.screenshotSrc = screenshotSrc;
		this.nTaskId = nTaskId;
		this.strIp = strIp;
		this.errorMsg = errorMsg;
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
	
	@Column(name = "nHistoryRecordId")
	public Integer getnHistoryRecordId() {
		return this.nHistoryRecordId;
	}
	
	public void setnHistoryRecordId(Integer nHistoryRecordId) {
		this.nHistoryRecordId = nHistoryRecordId;
	}
	
	@Column(name = "nUserId")
	public Integer getNuserId() {
		return this.nuserId;
	}
	
	public void setNuserId(Integer nuserId) {
		this.nuserId = nuserId;
	}
	
	@Column(name = "streamNo", length = 64)
	public String getStreamNo() {
		return this.streamNo;
	}
	
	public void setStreamNo(String streamNo) {
		this.streamNo = streamNo;
	}
	
	@Column(name = "strRecordName", length = 256)
	public String getStrRecordName() {
		return this.strRecordName;
	}
	
	public void setStrRecordName(String strRecordName) {
		this.strRecordName = strRecordName;
	}
	
	@Column(name = "nRecordType")
	public Integer getNrecordType() {
		return this.nrecordType;
	}
	
	public void setNrecordType(Integer nrecordType) {
		this.nrecordType = nrecordType;
	}
	
	@Column(name = "nRecordFileType")
	public Integer getNrecordFileType() {
		return this.nrecordFileType;
	}
	
	public void setNrecordFileType(Integer nrecordFileType) {
		this.nrecordFileType = nrecordFileType;
	}
	
	@Column(name = "nRecordStatus")
	public Integer getNrecordStatus() {
		return this.nrecordStatus;
	}
	
	public void setNrecordStatus(Integer nrecordStatus) {
		this.nrecordStatus = nrecordStatus;
	}
	
	@JSON(format="yyyy-MM-dd HH:mm:ss")
	@Column(name = "startTime", length = 0)
	public Date getStartTime() {
		return this.startTime;
	}
	
	public void setStartTime(Date startTime) {
		this.startTime = startTime;
	}
	@JSON(format="yyyy-MM-dd HH:mm:ss")
	@Column(name = "endTime", length = 0)
	public Date getEndTime() {
		return this.endTime;
	}
	
	public void setEndTime(Date endTime) {
		this.endTime = endTime;
	}
	
	@Column(name = "strUrl", length = 256)
	public String getStrUrl() {
		return this.strUrl;
	}
	
	public void setStrUrl(String strUrl) {
		this.strUrl = strUrl;
	}
	
	@Column(name = "nRepairDevId")
	public Integer getNrepairDevId() {
		return this.nrepairDevId;
	}
	
	public void setNrepairDevId(Integer nrepairDevId) {
		this.nrepairDevId = nrepairDevId;
	}
	
	@Column(name = "strCollectIp", length = 20)
	public String getStrCollectIp() {
		return this.strCollectIp;
	}
	
	public void setStrCollectIp(String strCollectIp) {
		this.strCollectIp = strCollectIp;
	}
	
	@Column(name = "nCollectChan")
	public Integer getNcollectChan() {
		return this.ncollectChan;
	}
	
	public void setNcollectChan(Integer ncollectChan) {
		this.ncollectChan = ncollectChan;
	}
	
	@Column(name = "nCollectRegionId")
	public Integer getNcollectRegionId() {
		return this.ncollectRegionId;
	}
	
	public void setNcollectRegionId(Integer ncollectRegionId) {
		this.ncollectRegionId = ncollectRegionId;
	}
	
	@Column(name = "nUpLoadPercent")
	public Integer getNupLoadPercent() {
		return this.nupLoadPercent;
	}
	
	public void setNupLoadPercent(Integer nupLoadPercent) {
		this.nupLoadPercent = nupLoadPercent;
	}
	
	@Column(name = "dvrUserName", length = 50)
	public String getDvrUserName() {
		return this.dvrUserName;
	}
	
	public void setDvrUserName(String dvrUserName) {
		this.dvrUserName = dvrUserName;
	}
	
	@Column(name = "dvrPwd", length = 50)
	public String getDvrPwd() {
		return this.dvrPwd;
	}
	
	public void setDvrPwd(String dvrPwd) {
		this.dvrPwd = dvrPwd;
	}
	
	@Column(name = "dvrPort")
	public Integer getDvrPort() {
		return this.dvrPort;
	}
	
	public void setDvrPort(Integer dvrPort) {
		this.dvrPort = dvrPort;
	}
	
	@Column(name = "nFileSize")
	public Integer getNfileSize() {
		return this.nfileSize;
	}
	
	public void setNfileSize(Integer nfileSize) {
		this.nfileSize = nfileSize;
	}
	
	@Column(name = "screenshotSrc", length = 500)
	public String getScreenshotSrc() {
		return this.screenshotSrc;
	}
	
	public void setScreenshotSrc(String screenshotSrc) {
		this.screenshotSrc = screenshotSrc;
	}
	
	@Column(name = "nTaskId")
	public Integer getnTaskId() {
		return nTaskId;
	}

	public void setnTaskId(Integer nTaskId) {
		this.nTaskId = nTaskId;
	}
	
	@Column(name = "strIp", length = 50)
	public String getStrIp() {
		return strIp;
	}
	
	public void setStrIp(String strIp) {
		this.strIp = strIp;
	}
	
	@Column(name = "errorMsg", length = 1000)
	public String getErrorMsg() {
		return this.errorMsg;
	}
	
	public void setErrorMsg(String errorMsg) {
		this.errorMsg = errorMsg;
	}
	
}