package com.hikvision.finance.fms.model;

import java.util.Date;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;

import static javax.persistence.GenerationType.IDENTITY;

import javax.persistence.Id;
import javax.persistence.Table;
import javax.persistence.Temporal;
import javax.persistence.TemporalType;

import org.apache.struts2.json.annotations.JSON;

/**
 * Product entity. @author MyEclipse Persistence Tools
 */
@Entity
@Table(name = "product")
public class Product implements java.io.Serializable {

	// Fields

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = -4407768812666581138L;
	private Integer id;
	private String strCode;
	private String strName;
	private Integer pid;
	private Integer saveYears;
	private Integer saveType; //保存期限类型 ，年月日
	private Integer guaranteeYears;
	private Integer guaranteeType;//保障期限类型，年月日
	private Integer nenabled;
	private Date strEndDate;
	private Integer importUser;
	private Integer checkUser;
	private Date importTime;
	private Date checkTime;
	private String strGrade;
	private String strRisklevel;
	private String remark;
	private String wordTemplate;

	// Constructors

	/** default constructor */
	public Product() {
	}

	/** full constructor */
	public Product(String strCode, String strName, Integer pid, Integer saveYears, Integer guaranteeYears, Integer nenabled,
	        Date strEndDate, Integer importUser, Integer checkUser, Date importTime, Date checkTime, String strGrade,
	        String strRisklevel, String remark, String wordTemplate) {
		this.strCode = strCode;
		this.strName = strName;
		this.pid = pid;
		this.saveYears = saveYears;
		this.guaranteeYears = guaranteeYears;
		this.nenabled = nenabled;
		this.strEndDate = strEndDate;
		this.importUser = importUser;
		this.checkUser = checkUser;
		this.importTime = importTime;
		this.checkTime = checkTime;
		this.strGrade = strGrade;
		this.strRisklevel = strRisklevel;
		this.remark = remark;
		this.wordTemplate = wordTemplate;
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

	@Column(name = "strCode", length = 40)
	public String getStrCode() {
		return this.strCode;
	}

	public void setStrCode(String strCode) {
		this.strCode = strCode;
	}

	@Column(name = "strName", length = 40)
	public String getStrName() {
		return this.strName;
	}

	public void setStrName(String strName) {
		this.strName = strName;
	}

	@Column(name = "pId")
	public Integer getPid() {
		return this.pid;
	}

	public void setPid(Integer pid) {
		this.pid = pid;
	}

	@Column(name = "saveYears")
	public Integer getSaveYears() {
		return this.saveYears;
	}

	public void setSaveYears(Integer saveYears) {
		this.saveYears = saveYears;
	}

	@Column(name = "guaranteeYears")
	public Integer getGuaranteeYears() {
		return this.guaranteeYears;
	}

	public void setGuaranteeYears(Integer guaranteeYears) {
		this.guaranteeYears = guaranteeYears;
	}

	@Column(name = "nEnabled")
	public Integer getNenabled() {
		return this.nenabled;
	}

	public void setNenabled(Integer nenabled) {
		this.nenabled = nenabled;
	}

	@JSON(format="yyyy-MM-dd")
	@Temporal(TemporalType.DATE)
	@Column(name = "strEndDate", length = 0)
	public Date getStrEndDate() {
		return this.strEndDate;
	}

	public void setStrEndDate(Date strEndDate) {
		this.strEndDate = strEndDate;
	}

	@Column(name = "importUser")
	public Integer getImportUser() {
		return this.importUser;
	}

	public void setImportUser(Integer importUser) {
		this.importUser = importUser;
	}

	@Column(name = "checkUser")
	public Integer getCheckUser() {
		return this.checkUser;
	}

	public void setCheckUser(Integer checkUser) {
		this.checkUser = checkUser;
	}

	@Column(name = "importTime", length = 0)
	public Date getImportTime() {
		return this.importTime;
	}

	public void setImportTime(Date importTime) {
		this.importTime = importTime;
	}

	@Column(name = "checkTime", length = 0)
	public Date getCheckTime() {
		return this.checkTime;
	}

	public void setCheckTime(Date checkTime) {
		this.checkTime = checkTime;
	}


	@Column(name = "strGrade", length = 40)
	public String getStrGrade() {
		return this.strGrade;
	}

	public void setStrGrade(String strGrade) {
		this.strGrade = strGrade;
	}

	@Column(name = "strRisklevel", length = 40)
	public String getStrRisklevel() {
		return this.strRisklevel;
	}

	public void setStrRisklevel(String strRisklevel) {
		this.strRisklevel = strRisklevel;
	}

	@Column(name = "remark", length = 200)
	public String getRemark() {
		return this.remark;
	}

	public void setRemark(String remark) {
		this.remark = remark;
	}

	@Column(name = "wordTemplate", length = 2500)
	public String getWordTemplate() {
		return this.wordTemplate;
	}

	public void setWordTemplate(String wordTemplate) {
		this.wordTemplate = wordTemplate;
	}

	@Column(name = "saveType")
	public Integer getSaveType() {
	    return saveType;
    }

	public void setSaveType(Integer saveType) {
	    this.saveType = saveType;
    }

	@Column(name = "guaranteeType")
	public Integer getGuaranteeType() {
	    return guaranteeType;
    }

	public void setGuaranteeType(Integer guaranteeType) {
	    this.guaranteeType = guaranteeType;
    }

}