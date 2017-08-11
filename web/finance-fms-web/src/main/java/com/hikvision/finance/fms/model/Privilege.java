package com.hikvision.finance.fms.model;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import static javax.persistence.GenerationType.IDENTITY;
import javax.persistence.Id;
import javax.persistence.Table;

/**
 * Privilege entity. @author MyEclipse Persistence Tools
 */
@Entity
@Table(name = "privilege")
public class Privilege implements java.io.Serializable {

	// Fields

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 8043991697124774989L;
	private Integer id;
	private String strCode;
	private String strName;
	private String strHref;
	private String strSuper;
	private Integer nsysNo;
	private Integer ntype;
	private Integer nsortId;
	private String strDescription;
	private Integer nenabled;
	private Integer nbtype;
	private String nameSpace;

	// Constructors

	/** default constructor */
	public Privilege() {
	}

	/** full constructor */
	public Privilege(String strCode, String strName, String strHref,
			String strSuper, Integer nsysNo, Integer ntype, Integer nsortId,
			String strDescription, Integer nenabled, Integer nbtype,
			String nameSpace) {
		this.strCode = strCode;
		this.strName = strName;
		this.strHref = strHref;
		this.strSuper = strSuper;
		this.nsysNo = nsysNo;
		this.ntype = ntype;
		this.nsortId = nsortId;
		this.strDescription = strDescription;
		this.nenabled = nenabled;
		this.nbtype = nbtype;
		this.nameSpace = nameSpace;
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

	@Column(name = "strCode", length = 50)
	public String getStrCode() {
		return this.strCode;
	}

	public void setStrCode(String strCode) {
		this.strCode = strCode;
	}

	@Column(name = "strName", length = 60)
	public String getStrName() {
		return this.strName;
	}

	public void setStrName(String strName) {
		this.strName = strName;
	}

	@Column(name = "strHref", length = 200)
	public String getStrHref() {
		return this.strHref;
	}

	public void setStrHref(String strHref) {
		this.strHref = strHref;
	}

	@Column(name = "strSuper", length = 64)
	public String getStrSuper() {
		return this.strSuper;
	}

	public void setStrSuper(String strSuper) {
		this.strSuper = strSuper;
	}

	@Column(name = "nSysNo")
	public Integer getNsysNo() {
		return this.nsysNo;
	}

	public void setNsysNo(Integer nsysNo) {
		this.nsysNo = nsysNo;
	}

	@Column(name = "nType")
	public Integer getNtype() {
		return this.ntype;
	}

	public void setNtype(Integer ntype) {
		this.ntype = ntype;
	}

	@Column(name = "nSortId")
	public Integer getNsortId() {
		return this.nsortId;
	}

	public void setNsortId(Integer nsortId) {
		this.nsortId = nsortId;
	}

	@Column(name = "strDescription", length = 200)
	public String getStrDescription() {
		return this.strDescription;
	}

	public void setStrDescription(String strDescription) {
		this.strDescription = strDescription;
	}

	@Column(name = "nEnabled")
	public Integer getNenabled() {
		return this.nenabled;
	}

	public void setNenabled(Integer nenabled) {
		this.nenabled = nenabled;
	}

	@Column(name = "nBtype")
	public Integer getNbtype() {
		return this.nbtype;
	}

	public void setNbtype(Integer nbtype) {
		this.nbtype = nbtype;
	}

	@Column(name = "nameSpace", length = 200)
	public String getNameSpace() {
		return this.nameSpace;
	}

	public void setNameSpace(String nameSpace) {
		this.nameSpace = nameSpace;
	}

}