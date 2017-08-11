package com.hikvision.finance.fms.model;

import java.util.Date;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;

import static javax.persistence.GenerationType.IDENTITY;

import javax.persistence.Id;
import javax.persistence.Table;

/**
 * Softdongle entity. @author MyEclipse Persistence Tools
 */
@Entity
@Table(name = "softdongle")
public class Softdongle implements java.io.Serializable {

	// Fields

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 7336068071317537504L;
	private Integer id;
	private Integer ntype;
	private Integer nvalue;
	private String strDescribe;
	private Date dtUpdateTime;

	// Constructors

	/** default constructor */
	public Softdongle() {
	}

	/** full constructor */
	public Softdongle(Integer ntype, Integer nvalue, String strDescribe,
			Date dtUpdateTime) {
		this.ntype = ntype;
		this.nvalue = nvalue;
		this.strDescribe = strDescribe;
		this.dtUpdateTime = dtUpdateTime;
	}

	/**
     * 创建一个新的实例Softdongle.
     * @param ntype
     * @param nvalue
     */
    public Softdongle(Integer ntype, Integer nvalue) {
	    this.ntype = ntype;
	    this.nvalue = nvalue;
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

	@Column(name = "nValue")
	public Integer getNvalue() {
		return this.nvalue;
	}

	public void setNvalue(Integer nvalue) {
		this.nvalue = nvalue;
	}

	@Column(name = "strDescribe", length = 50)
	public String getStrDescribe() {
		return this.strDescribe;
	}

	public void setStrDescribe(String strDescribe) {
		this.strDescribe = strDescribe;
	}

	@Column(name = "dtUpdateTime", length = 0)
	public Date getDtUpdateTime() {
		return this.dtUpdateTime;
	}

	public void setDtUpdateTime(Date dtUpdateTime) {
		this.dtUpdateTime = dtUpdateTime;
	}

}