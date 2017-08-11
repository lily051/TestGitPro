package com.hikvision.finance.fms.model;

import java.util.Date;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import static javax.persistence.GenerationType.IDENTITY;
import javax.persistence.Id;
import javax.persistence.Table;

/**
 * Logsys entity. @author MyEclipse Persistence Tools
 */
@Entity
@Table(name = "logsys")
public class Logsys implements java.io.Serializable {

	// Fields

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = -203723702354640438L;
	private Integer id;
	private Integer nmoduleType;
	private Integer ntype;
	private String strUser;
	private Date strDate;
	private String strIp;
	private String strVersionInfo;

	// Constructors

	/** default constructor */
	public Logsys() {
	}

	/** full constructor */
	public Logsys(Integer nmoduleType, Integer ntype, String strUser,
			Date strDate, String strIp, String strVersionInfo) {
		this.nmoduleType = nmoduleType;
		this.ntype = ntype;
		this.strUser = strUser;
		this.strDate = strDate;
		this.strIp = strIp;
		this.strVersionInfo = strVersionInfo;
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

	@Column(name = "nModuleType")
	public Integer getNmoduleType() {
		return this.nmoduleType;
	}

	public void setNmoduleType(Integer nmoduleType) {
		this.nmoduleType = nmoduleType;
	}

	@Column(name = "nType")
	public Integer getNtype() {
		return this.ntype;
	}

	public void setNtype(Integer ntype) {
		this.ntype = ntype;
	}

	@Column(name = "strUser", length = 50)
	public String getStrUser() {
		return this.strUser;
	}

	public void setStrUser(String strUser) {
		this.strUser = strUser;
	}

	@Column(name = "strDate", length = 0)
	public Date getStrDate() {
		return this.strDate;
	}

	public void setStrDate(Date strDate) {
		this.strDate = strDate;
	}

	@Column(name = "strIP", length = 20)
	public String getStrIp() {
		return this.strIp;
	}

	public void setStrIp(String strIp) {
		this.strIp = strIp;
	}

	@Column(name = "strVersionInfo", length = 1000)
	public String getStrVersionInfo() {
		return this.strVersionInfo;
	}

	public void setStrVersionInfo(String strVersionInfo) {
		this.strVersionInfo = strVersionInfo;
	}

}