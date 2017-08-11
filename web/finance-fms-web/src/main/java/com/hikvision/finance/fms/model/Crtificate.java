package com.hikvision.finance.fms.model;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import static javax.persistence.GenerationType.IDENTITY;
import javax.persistence.Id;
import javax.persistence.Table;

/**
 * Crtificate entity. @author MyEclipse Persistence Tools
 */
@Entity
@Table(name = "crtificate")
public class Crtificate implements java.io.Serializable {

	// Fields

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 8831541512347463177L;
	private Integer id;
	private String strCode;
	private String strName;
	private Integer nsortId;

	// Constructors

	/** default constructor */
	public Crtificate() {
	}

	/** full constructor */
	public Crtificate(String strCode, String strName, Integer nsortId) {
		this.strCode = strCode;
		this.strName = strName;
		this.nsortId = nsortId;
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

	@Column(name = "strCode")
	public String getStrCode() {
		return this.strCode;
	}

	public void setStrCode(String strCode) {
		this.strCode = strCode;
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

}