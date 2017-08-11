package com.hikvision.finance.fms.model;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import static javax.persistence.GenerationType.IDENTITY;
import javax.persistence.Id;
import javax.persistence.Table;

/**
 * Sysdictionary entity. @author MyEclipse Persistence Tools
 */
@Entity
@Table(name = "sysdictionary")
public class Sysdictionary implements java.io.Serializable {

	// Fields

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 8674754548234954175L;
	private Integer id;
	private Integer nbigType;
	private Integer ntype;
	private Integer nkey;
	private String strValue;
	private Integer nenabled;
	private Integer nprotected;
	private Integer nsortId;
	private String strText;

	// Constructors

	/** default constructor */
	public Sysdictionary() {
	}

	/** full constructor */
	public Sysdictionary(Integer nbigType, Integer ntype, Integer nkey,
			String strValue, Integer nenabled, Integer nprotected,
			Integer nsortId, String strText) {
		this.nbigType = nbigType;
		this.ntype = ntype;
		this.nkey = nkey;
		this.strValue = strValue;
		this.nenabled = nenabled;
		this.nprotected = nprotected;
		this.nsortId = nsortId;
		this.strText = strText;
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

	@Column(name = "nBigType")
	public Integer getNbigType() {
		return this.nbigType;
	}

	public void setNbigType(Integer nbigType) {
		this.nbigType = nbigType;
	}

	@Column(name = "nType")
	public Integer getNtype() {
		return this.ntype;
	}

	public void setNtype(Integer ntype) {
		this.ntype = ntype;
	}

	@Column(name = "nKey")
	public Integer getNkey() {
		return this.nkey;
	}

	public void setNkey(Integer nkey) {
		this.nkey = nkey;
	}

	@Column(name = "strValue", length = 40)
	public String getStrValue() {
		return this.strValue;
	}

	public void setStrValue(String strValue) {
		this.strValue = strValue;
	}

	@Column(name = "nEnabled")
	public Integer getNenabled() {
		return this.nenabled;
	}

	public void setNenabled(Integer nenabled) {
		this.nenabled = nenabled;
	}

	@Column(name = "nProtected")
	public Integer getNprotected() {
		return this.nprotected;
	}

	public void setNprotected(Integer nprotected) {
		this.nprotected = nprotected;
	}

	@Column(name = "nSortId")
	public Integer getNsortId() {
		return this.nsortId;
	}

	public void setNsortId(Integer nsortId) {
		this.nsortId = nsortId;
	}

	@Column(name = "strText", length = 500)
	public String getStrText() {
		return this.strText;
	}

	public void setStrText(String strText) {
		this.strText = strText;
	}

}