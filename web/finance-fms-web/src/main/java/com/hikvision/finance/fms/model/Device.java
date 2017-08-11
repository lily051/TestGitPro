package com.hikvision.finance.fms.model;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import static javax.persistence.GenerationType.IDENTITY;
import javax.persistence.Id;
import javax.persistence.Table;

/**
 * Device entity. @author MyEclipse Persistence Tools
 */
@Entity
@Table(name = "device")
public class Device implements java.io.Serializable {

	// Fields

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 8635647146275880068L;
	private Integer id;
	private Integer ntype;
	private String strName;
	private String strIp;
	private Integer nport;
	private String strUser;
	private String strPassword;
	private String strPasswordLevel;
	private Integer nchanNum;
	private Integer telChanNum;
	private Integer nregionId;
	private Integer nstreamType;
	private Integer online;
	private Integer uiTotalSpace;
	private Integer uiFreeSpace;
	private String strIndexCode;
	private String strManufacture;
	private Integer ntpEnable;//是否启用NTP校时
	private String strAccessKey;
	private String strSecretKey;

	// Constructors

	/** default constructor */
	public Device() {
	}

	/** full constructor */
	public Device(Integer ntype, String strName, String strIp, Integer nport, String strUser, String strPassword,
	        String strPasswordLevel, Integer nchanNum, Integer telChanNum, Integer nregionId, Integer nstreamType,
	        Integer online, Integer uiTotalSpace, Integer uiFreeSpace, String strIndexCode, String strManufacture,
	        String strAccessKey, String strSecretKey) {
		this.ntype = ntype;
		this.strName = strName;
		this.strIp = strIp;
		this.nport = nport;
		this.strUser = strUser;
		this.strPassword = strPassword;
		this.strPasswordLevel = strPasswordLevel;
		this.nchanNum = nchanNum;
		this.telChanNum = telChanNum;
		this.nregionId = nregionId;
		this.nstreamType = nstreamType;
		this.online = online;
		this.uiTotalSpace = uiTotalSpace;
		this.uiFreeSpace = uiFreeSpace;
		this.strIndexCode = strIndexCode;
		this.strManufacture = strManufacture;
		this.strAccessKey = strAccessKey;
		this.strSecretKey = strSecretKey;
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

	@Column(name = "strName", length = 50)
	public String getStrName() {
		return this.strName;
	}

	public void setStrName(String strName) {
		this.strName = strName;
	}

	@Column(name = "strIP", length = 16)
	public String getStrIp() {
		return this.strIp;
	}

	public void setStrIp(String strIp) {
		this.strIp = strIp;
	}

	@Column(name = "nPort")
	public Integer getNport() {
		return this.nport;
	}

	public void setNport(Integer nport) {
		this.nport = nport;
	}

	@Column(name = "strUser", length = 50)
	public String getStrUser() {
		return this.strUser;
	}

	public void setStrUser(String strUser) {
		this.strUser = strUser;
	}

	@Column(name = "strPassword", length = 50)
	public String getStrPassword() {
		return this.strPassword;
	}

	public void setStrPassword(String strPassword) {
		this.strPassword = strPassword;
	}

	@Column(name = "strPasswordLevel", length = 10)
	public String getStrPasswordLevel() {
		return this.strPasswordLevel;
	}

	public void setStrPasswordLevel(String strPasswordLevel) {
		this.strPasswordLevel = strPasswordLevel;
	}

	@Column(name = "nChanNum")
	public Integer getNchanNum() {
		return this.nchanNum;
	}

	public void setNchanNum(Integer nchanNum) {
		this.nchanNum = nchanNum;
	}

	@Column(name = "telChanNum")
	public Integer getTelChanNum() {
		return this.telChanNum;
	}

	public void setTelChanNum(Integer telChanNum) {
		this.telChanNum = telChanNum;
	}

	@Column(name = "nRegionId")
	public Integer getNregionId() {
		return this.nregionId;
	}

	public void setNregionId(Integer nregionId) {
		this.nregionId = nregionId;
	}

	@Column(name = "nStreamType")
	public Integer getNstreamType() {
		return this.nstreamType;
	}

	public void setNstreamType(Integer nstreamType) {
		this.nstreamType = nstreamType;
	}

	@Column(name = "online")
	public Integer getOnline() {
		return this.online;
	}

	public void setOnline(Integer online) {
		this.online = online;
	}

	@Column(name = "uiTotalSpace")
	public Integer getUiTotalSpace() {
		return this.uiTotalSpace;
	}

	public void setUiTotalSpace(Integer uiTotalSpace) {
		this.uiTotalSpace = uiTotalSpace;
	}

	@Column(name = "uiFreeSpace")
	public Integer getUiFreeSpace() {
		return this.uiFreeSpace;
	}

	public void setUiFreeSpace(Integer uiFreeSpace) {
		this.uiFreeSpace = uiFreeSpace;
	}

	@Column(name = "strIndexCode")
	public String getStrIndexCode() {
		return this.strIndexCode;
	}

	public void setStrIndexCode(String strIndexCode) {
		this.strIndexCode = strIndexCode;
	}

	@Column(name = "strManufacture")
	public String getStrManufacture() {
		return this.strManufacture;
	}

	public void setStrManufacture(String strManufacture) {
		this.strManufacture = strManufacture;
	}

	@Column(name = "ntpEnable")
	public Integer getNtpEnable() {
	    return ntpEnable;
    }

	public void setNtpEnable(Integer ntpEnable) {
	    this.ntpEnable = ntpEnable;
    }
	
	@Column(name = "strAccessKey")
	public String getStrAccessKey() {
		return strAccessKey;
	}
	public void setStrAccessKey(String strAccessKey) {
		this.strAccessKey = strAccessKey;
	}

	@Column(name = "strSecretKey")
	public String getStrSecretKey() {
		return strSecretKey;
	}
	public void setStrSecretKey(String strSecretKey) {
		this.strSecretKey = strSecretKey;
	}
}