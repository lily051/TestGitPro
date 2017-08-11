package com.hikvision.finance.fms.model;

import java.util.Date;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import static javax.persistence.GenerationType.IDENTITY;
import javax.persistence.Id;
import javax.persistence.Table;

/**
 * Users entity. @author MyEclipse Persistence Tools
 */
@Entity
@Table(name = "users")
public class Users implements java.io.Serializable {

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = -3680323229397769325L;
	// Fields

	private Integer id;
	private Integer netId;
	private Integer sysRole;
	private String strName;
	private String strCode;
	private Integer nsex;
	private String userNo;
	private String net;
	private Integer nstate;
	private Date strDate;
	private String strPassword;
	private Integer npasswordLevel;
	private String strCredentialNo;
	private String strDuty;
	private String strEmail;
	private String strTel;
	private String strComment;
	private Date strUpdatePwdTime;
	private Integer nowner;
	private Integer nonline;
	private Date lastTimeOnline;
	private String userSessionId;

	// Constructors

	/** default constructor */
	public Users() {
	}

	/** full constructor */
	public Users(Integer netId, Integer sysRole, String strName,
			String strCode, Integer nsex, String userNo, String net,
			Integer nstate, Date strDate, String strPassword,
			Integer npasswordLevel, String strCredentialNo, String strDuty,
			String strEmail, String strTel, String strComment,
			Date strUpdatePwdTime, Integer nowner, Integer nonline,
			Date lastTimeOnline, String userSessionId) {
		this.netId = netId;
		this.sysRole = sysRole;
		this.strName = strName;
		this.strCode = strCode;
		this.nsex = nsex;
		this.userNo = userNo;
		this.net = net;
		this.nstate = nstate;
		this.strDate = strDate;
		this.strPassword = strPassword;
		this.npasswordLevel = npasswordLevel;
		this.strCredentialNo = strCredentialNo;
		this.strDuty = strDuty;
		this.strEmail = strEmail;
		this.strTel = strTel;
		this.strComment = strComment;
		this.strUpdatePwdTime = strUpdatePwdTime;
		this.nowner = nowner;
		this.nonline = nonline;
		this.lastTimeOnline = lastTimeOnline;
		this.userSessionId = userSessionId;
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

	@Column(name = "netId")
	public Integer getNetId() {
		return this.netId;
	}

	public void setNetId(Integer netId) {
		this.netId = netId;
	}

	@Column(name = "sysRole")
	public Integer getSysRole() {
		return this.sysRole;
	}

	public void setSysRole(Integer sysRole) {
		this.sysRole = sysRole;
	}

	@Column(name = "strName", length = 50)
	public String getStrName() {
		return this.strName;
	}

	public void setStrName(String strName) {
		this.strName = strName;
	}

	@Column(name = "strCode", length = 50)
	public String getStrCode() {
		return this.strCode;
	}

	public void setStrCode(String strCode) {
		this.strCode = strCode;
	}

	@Column(name = "nSex")
	public Integer getNsex() {
		return this.nsex;
	}

	public void setNsex(Integer nsex) {
		this.nsex = nsex;
	}

	@Column(name = "userNo", length = 50)
	public String getUserNo() {
		return this.userNo;
	}

	public void setUserNo(String userNo) {
		this.userNo = userNo;
	}

	@Column(name = "net", length = 50)
	public String getNet() {
		return this.net;
	}

	public void setNet(String net) {
		this.net = net;
	}

	@Column(name = "nState")
	public Integer getNstate() {
		return this.nstate;
	}

	public void setNstate(Integer nstate) {
		this.nstate = nstate;
	}

	@Column(name = "strDate", length = 0)
	public Date getStrDate() {
		return this.strDate;
	}

	public void setStrDate(Date strDate) {
		this.strDate = strDate;
	}

	@Column(name = "strPassword", length = 100)
	public String getStrPassword() {
		return this.strPassword;
	}

	public void setStrPassword(String strPassword) {
		this.strPassword = strPassword;
	}

	@Column(name = "nPasswordLevel")
	public Integer getNpasswordLevel() {
		return this.npasswordLevel;
	}

	public void setNpasswordLevel(Integer npasswordLevel) {
		this.npasswordLevel = npasswordLevel;
	}

	@Column(name = "strCredentialNo", length = 40)
	public String getStrCredentialNo() {
		return this.strCredentialNo;
	}

	public void setStrCredentialNo(String strCredentialNo) {
		this.strCredentialNo = strCredentialNo;
	}

	@Column(name = "strDuty", length = 50)
	public String getStrDuty() {
		return this.strDuty;
	}

	public void setStrDuty(String strDuty) {
		this.strDuty = strDuty;
	}

	@Column(name = "strEmail", length = 50)
	public String getStrEmail() {
		return this.strEmail;
	}

	public void setStrEmail(String strEmail) {
		this.strEmail = strEmail;
	}

	@Column(name = "strTel", length = 50)
	public String getStrTel() {
		return this.strTel;
	}

	public void setStrTel(String strTel) {
		this.strTel = strTel;
	}

	@Column(name = "strComment", length = 50)
	public String getStrComment() {
		return this.strComment;
	}

	public void setStrComment(String strComment) {
		this.strComment = strComment;
	}

	@Column(name = "strUpdatePwdTime", length = 0)
	public Date getStrUpdatePwdTime() {
		return this.strUpdatePwdTime;
	}

	public void setStrUpdatePwdTime(Date strUpdatePwdTime) {
		this.strUpdatePwdTime = strUpdatePwdTime;
	}

	@Column(name = "nOwner")
	public Integer getNowner() {
		return this.nowner;
	}

	public void setNowner(Integer nowner) {
		this.nowner = nowner;
	}

	@Column(name = "nonline")
	public Integer getNonline() {
		return this.nonline;
	}

	public void setNonline(Integer nonline) {
		this.nonline = nonline;
	}

	@Column(name = "lastTimeOnline", length = 0)
	public Date getLastTimeOnline() {
		return this.lastTimeOnline;
	}

	public void setLastTimeOnline(Date lastTimeOnline) {
		this.lastTimeOnline = lastTimeOnline;
	}

	@Column(name = "userSessionId", length = 100)
	public String getUserSessionId() {
		return this.userSessionId;
	}

	public void setUserSessionId(String userSessionId) {
		this.userSessionId = userSessionId;
	}

}