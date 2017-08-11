package com.hikvision.finance.fms.model;

import java.util.Date;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import static javax.persistence.GenerationType.IDENTITY;
import javax.persistence.Id;
import javax.persistence.Table;

/**
 * Alarmlog entity. @author MyEclipse Persistence Tools
 */
@Entity
@Table(name = "alarmlog")
public class Alarmlog implements java.io.Serializable {

	// Fields

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = -8539027058426587566L;
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

	// Constructors

	/** default constructor */
	public Alarmlog() {
	}

	/** full constructor */
	public Alarmlog(Integer checkType, Integer checkObjId, Integer checkChan,
			Integer contactObjId, Integer alarmType, Date alarmTime,
			Date alarmbeginTime, Integer alarmRegionId, String comments,
			String strHost, Integer status, String checker, String userNo,
			Date dealTime, String dealcomment, Integer alarmlevel,
			String strGuid) {
		this.checkType = checkType;
		this.checkObjId = checkObjId;
		this.checkChan = checkChan;
		this.contactObjId = contactObjId;
		this.alarmType = alarmType;
		this.alarmTime = alarmTime;
		this.alarmbeginTime = alarmbeginTime;
		this.alarmRegionId = alarmRegionId;
		this.comments = comments;
		this.strHost = strHost;
		this.status = status;
		this.checker = checker;
		this.userNo = userNo;
		this.dealTime = dealTime;
		this.dealcomment = dealcomment;
		this.alarmlevel = alarmlevel;
		this.strGuid = strGuid;
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

	@Column(name = "checkType")
	public Integer getCheckType() {
		return this.checkType;
	}

	public void setCheckType(Integer checkType) {
		this.checkType = checkType;
	}

	@Column(name = "checkObjId")
	public Integer getCheckObjId() {
		return this.checkObjId;
	}

	public void setCheckObjId(Integer checkObjId) {
		this.checkObjId = checkObjId;
	}

	@Column(name = "checkChan")
	public Integer getCheckChan() {
		return this.checkChan;
	}

	public void setCheckChan(Integer checkChan) {
		this.checkChan = checkChan;
	}

	@Column(name = "contactObjId")
	public Integer getContactObjId() {
		return this.contactObjId;
	}

	public void setContactObjId(Integer contactObjId) {
		this.contactObjId = contactObjId;
	}

	@Column(name = "alarmType")
	public Integer getAlarmType() {
		return this.alarmType;
	}

	public void setAlarmType(Integer alarmType) {
		this.alarmType = alarmType;
	}

	@Column(name = "alarmTime", length = 0)
	public Date getAlarmTime() {
		return this.alarmTime;
	}

	public void setAlarmTime(Date alarmTime) {
		this.alarmTime = alarmTime;
	}

	@Column(name = "alarmbeginTime", length = 0)
	public Date getAlarmbeginTime() {
		return this.alarmbeginTime;
	}

	public void setAlarmbeginTime(Date alarmbeginTime) {
		this.alarmbeginTime = alarmbeginTime;
	}

	@Column(name = "alarmRegionId")
	public Integer getAlarmRegionId() {
		return this.alarmRegionId;
	}

	public void setAlarmRegionId(Integer alarmRegionId) {
		this.alarmRegionId = alarmRegionId;
	}

	@Column(name = "comments", length = 500)
	public String getComments() {
		return this.comments;
	}

	public void setComments(String comments) {
		this.comments = comments;
	}

	@Column(name = "strHost", length = 20)
	public String getStrHost() {
		return this.strHost;
	}

	public void setStrHost(String strHost) {
		this.strHost = strHost;
	}

	@Column(name = "status")
	public Integer getStatus() {
		return this.status;
	}

	public void setStatus(Integer status) {
		this.status = status;
	}

	@Column(name = "checker", length = 50)
	public String getChecker() {
		return this.checker;
	}

	public void setChecker(String checker) {
		this.checker = checker;
	}

	@Column(name = "userNo", length = 50)
	public String getUserNo() {
		return this.userNo;
	}

	public void setUserNo(String userNo) {
		this.userNo = userNo;
	}

	@Column(name = "dealTime", length = 0)
	public Date getDealTime() {
		return this.dealTime;
	}

	public void setDealTime(Date dealTime) {
		this.dealTime = dealTime;
	}

	@Column(name = "dealcomment", length = 50)
	public String getDealcomment() {
		return this.dealcomment;
	}

	public void setDealcomment(String dealcomment) {
		this.dealcomment = dealcomment;
	}

	@Column(name = "alarmlevel")
	public Integer getAlarmlevel() {
		return this.alarmlevel;
	}

	public void setAlarmlevel(Integer alarmlevel) {
		this.alarmlevel = alarmlevel;
	}

	@Column(name = "strGuid", length = 128)
	public String getStrGuid() {
		return this.strGuid;
	}

	public void setStrGuid(String strGuid) {
		this.strGuid = strGuid;
	}

}