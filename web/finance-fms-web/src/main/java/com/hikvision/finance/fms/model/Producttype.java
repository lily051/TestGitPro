package com.hikvision.finance.fms.model;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import static javax.persistence.GenerationType.IDENTITY;
import javax.persistence.Id;
import javax.persistence.Table;

/**
 * Producttype entity. @author MyEclipse Persistence Tools
 */
@Entity
@Table(name = "producttype")
public class Producttype implements java.io.Serializable {

	// Fields

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = -1319915452456171051L;
	private Integer id;
	private Integer pid;
	private String strPath;
	private String strName;
	private Integer nsortId;
	private String strInterAreaCode;

	// Constructors

	/** default constructor */
	public Producttype() {
	}

	/** full constructor */
	public Producttype(Integer pid, String strPath, String strName,
			Integer nsortId, String strInterAreaCode) {
		this.pid = pid;
		this.strPath = strPath;
		this.strName = strName;
		this.nsortId = nsortId;
		this.strInterAreaCode = strInterAreaCode;
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

	@Column(name = "pid")
	public Integer getPid() {
		return this.pid;
	}

	public void setPid(Integer pid) {
		this.pid = pid;
	}

	@Column(name = "strPath", length = 200)
	public String getStrPath() {
		return this.strPath;
	}

	public void setStrPath(String strPath) {
		this.strPath = strPath;
	}

	@Column(name = "strName")
	public String getStrName() {
		return this.strName;
	}

	public void setStrName(String strName) {
		this.strName = strName;
	}

	@Column(name = "nsortId")
	public Integer getNsortId() {
		return this.nsortId;
	}

	public void setNsortId(Integer nsortId) {
		this.nsortId = nsortId;
	}

	@Column(name = "strInterAreaCode", length = 200)
	public String getStrInterAreaCode() {
		return this.strInterAreaCode;
	}

	public void setStrInterAreaCode(String strInterAreaCode) {
		this.strInterAreaCode = strInterAreaCode;
	}

}