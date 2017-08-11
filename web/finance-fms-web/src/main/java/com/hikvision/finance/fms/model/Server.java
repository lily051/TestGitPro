package com.hikvision.finance.fms.model;

import java.util.Date;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import static javax.persistence.GenerationType.IDENTITY;
import javax.persistence.Id;
import javax.persistence.Table;

/**
 * Server entity. @author MyEclipse Persistence Tools
 */
@Entity
@Table(name = "server")
public class Server implements java.io.Serializable {

	// Fields

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 2298022960561099647L;
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

	// Constructors

	/** default constructor */
	public Server() {
	}

	/** full constructor */
	public Server(Integer ntype, String strIp, Integer nctrlPort,
			Integer ndataPort, String strUser, String strComment,
			Integer nmultId, Integer nmultVer, Integer nmultSysId,
			String strConfig, Integer nonline, Date heartbeatTime) {
		this.ntype = ntype;
		this.strIp = strIp;
		this.nctrlPort = nctrlPort;
		this.ndataPort = ndataPort;
		this.strUser = strUser;
		this.strComment = strComment;
		this.nmultId = nmultId;
		this.nmultVer = nmultVer;
		this.nmultSysId = nmultSysId;
		this.strConfig = strConfig;
		this.nonline = nonline;
		this.heartbeatTime = heartbeatTime;
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

	@Column(name = "nType")
	public Integer getNtype() {
		return this.ntype;
	}

	public void setNtype(Integer ntype) {
		this.ntype = ntype;
	}

	@Column(name = "strIP", length = 16)
	public String getStrIp() {
		return this.strIp;
	}

	public void setStrIp(String strIp) {
		this.strIp = strIp;
	}

	@Column(name = "nCtrlPort")
	public Integer getNctrlPort() {
		return this.nctrlPort;
	}

	public void setNctrlPort(Integer nctrlPort) {
		this.nctrlPort = nctrlPort;
	}

	@Column(name = "nDataPort")
	public Integer getNdataPort() {
		return this.ndataPort;
	}

	public void setNdataPort(Integer ndataPort) {
		this.ndataPort = ndataPort;
	}

	@Column(name = "strUser", length = 50)
	public String getStrUser() {
		return this.strUser;
	}

	public void setStrUser(String strUser) {
		this.strUser = strUser;
	}

	@Column(name = "strComment", length = 2000)
	public String getStrComment() {
		return this.strComment;
	}

	public void setStrComment(String strComment) {
		this.strComment = strComment;
	}

	@Column(name = "nMultId")
	public Integer getNmultId() {
		return this.nmultId;
	}

	public void setNmultId(Integer nmultId) {
		this.nmultId = nmultId;
	}

	@Column(name = "nMultVer")
	public Integer getNmultVer() {
		return this.nmultVer;
	}

	public void setNmultVer(Integer nmultVer) {
		this.nmultVer = nmultVer;
	}

	@Column(name = "nMultSysId")
	public Integer getNmultSysId() {
		return this.nmultSysId;
	}

	public void setNmultSysId(Integer nmultSysId) {
		this.nmultSysId = nmultSysId;
	}

	@Column(name = "strConfig", length = 2000)
	public String getStrConfig() {
		return this.strConfig;
	}

	public void setStrConfig(String strConfig) {
		this.strConfig = strConfig;
	}

	@Column(name = "nonline")
	public Integer getNonline() {
		return this.nonline;
	}

	public void setNonline(Integer nonline) {
		this.nonline = nonline;
	}

	@Column(name = "heartbeatTime", length = 0)
	public Date getHeartbeatTime() {
		return this.heartbeatTime;
	}

	public void setHeartbeatTime(Date heartbeatTime) {
		this.heartbeatTime = heartbeatTime;
	}

}