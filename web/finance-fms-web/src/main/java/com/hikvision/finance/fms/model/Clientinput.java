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
import javax.persistence.Transient;

import org.apache.struts2.json.annotations.JSON;

/**
 * Clientinput entity. @author MyEclipse Persistence Tools
 */
@Entity
@Table(name = "clientinput")
public class Clientinput implements java.io.Serializable {

	// Fields

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 5457717072019520257L;
	private Integer id;
	private String nproId;
	private Date inputTime;
	private Date businessTime;
	private Integer inputRegionId;
	private String inputRegionCode;
	private String inputRegionName;
	private Integer status;
	private Integer nisRead;
	private Integer nisPay;
	private Integer delaydays;
	private Date strEndDate;
	private String name;
	private String creditCode;
	private Integer creditId;
	private Integer inputUser;
	private String proName;
	private String proUserNo;
	private Integer productTypeId;
	private String productTypeName;
	private Integer productId;
	private String productName;
	private String productCode;
	private Date strVideoEndDate;
	private Integer guaranteeType;//保障期限类型，年月日
	private Integer guaranteeYears;
	private Integer productSaveType; //保存期限类型 ，年月日
	private Integer productSaveMonth;
	private String strGrade;
	private String strRisklevel;
	private String strBankNo;
	private String strBuyMoney;
	private String strCreditPhone;
	private String strRemarks;
	
	private String creditName;
	private Integer lockClient;
	
	// Constructors
	
	/** default constructor */
	public Clientinput() {
	}
	
	/** full constructor */
	public Clientinput(String nproId, Date inputTime, Date businessTime, Integer inputRegionId, String inputRegionCode,
	        String inputRegionName, Integer status, Integer nisRead, Integer nisPay, Integer delaydays, Date strEndDate,
	        String name, String creditCode, Integer creditId, Integer inputUser, String proName, String proUserNo,
	        Integer productTypeId, String productTypeName, Integer productId, String productName, String productCode,
	        Date strVideoEndDate, Integer guaranteeYears, Integer productSaveMonth, String strGrade, String strRisklevel,
	        String strBankNo, String strBuyMoney, String strCreditPhone, String strRemarks, Integer lockClient) {
		this.nproId = nproId;
		this.inputTime = inputTime;
		this.businessTime = businessTime;
		this.inputRegionId = inputRegionId;
		this.inputRegionCode = inputRegionCode;
		this.inputRegionName = inputRegionName;
		this.status = status;
		this.nisRead = nisRead;
		this.nisPay = nisPay;
		this.delaydays = delaydays;
		this.strEndDate = strEndDate;
		this.name = name;
		this.creditCode = creditCode;
		this.creditId = creditId;
		this.inputUser = inputUser;
		this.proName = proName;
		this.proUserNo = proUserNo;
		this.productTypeId = productTypeId;
		this.productTypeName = productTypeName;
		this.productId = productId;
		this.productName = productName;
		this.productCode = productCode;
		this.strVideoEndDate = strVideoEndDate;
		this.guaranteeYears = guaranteeYears;
		this.productSaveMonth = productSaveMonth;
		this.strGrade = strGrade;
		this.strRisklevel = strRisklevel;
		this.strBankNo = strBankNo;
		this.strBuyMoney = strBuyMoney;
		this.strCreditPhone = strCreditPhone;
		this.strRemarks = strRemarks;
		this.lockClient = lockClient;
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
	
	@Column(name = "nProId", length = 50)
	public String getNproId() {
		return this.nproId;
	}
	
	public void setNproId(String nproId) {
		this.nproId = nproId;
	}
	
	@Column(name = "inputTime", length = 0)
	public Date getInputTime() {
		return this.inputTime;
	}
	
	public void setInputTime(Date inputTime) {
		this.inputTime = inputTime;
	}
	
	@Column(name = "businessTime", length = 0)
	@JSON(format="yyyy-MM-dd HH:mm:ss")
	public Date getBusinessTime() {
		return this.businessTime;
	}
	
	public void setBusinessTime(Date businessTime) {
		this.businessTime = businessTime;
	}
	
	@Column(name = "inputRegionId")
	public Integer getInputRegionId() {
		return this.inputRegionId;
	}
	
	public void setInputRegionId(Integer inputRegionId) {
		this.inputRegionId = inputRegionId;
	}
	
	@Column(name = "inputRegionCode", length = 50)
	public String getInputRegionCode() {
		return this.inputRegionCode;
	}
	
	public void setInputRegionCode(String inputRegionCode) {
		this.inputRegionCode = inputRegionCode;
	}
	
	@Column(name = "inputRegionName", length = 50)
	public String getInputRegionName() {
		return this.inputRegionName;
	}
	
	public void setInputRegionName(String inputRegionName) {
		this.inputRegionName = inputRegionName;
	}
	
	@Column(name = "status")
	public Integer getStatus() {
		return this.status;
	}
	
	public void setStatus(Integer status) {
		this.status = status;
	}
	
	@Column(name = "nisRead")
	public Integer getNisRead() {
		return this.nisRead;
	}
	
	public void setNisRead(Integer nisRead) {
		this.nisRead = nisRead;
	}
	
	@Column(name = "nisPay")
	public Integer getNisPay() {
		return this.nisPay;
	}
	
	public void setNisPay(Integer nisPay) {
		this.nisPay = nisPay;
	}
	
	@Column(name = "delaydays")
	public Integer getDelaydays() {
		return this.delaydays;
	}
	
	public void setDelaydays(Integer delaydays) {
		this.delaydays = delaydays;
	}
	
	@Temporal(TemporalType.DATE)
	@JSON(format="yyyy-MM-dd")
	@Column(name = "strEndDate", length = 0)
	public Date getStrEndDate() {
		return this.strEndDate;
	}
	
	public void setStrEndDate(Date strEndDate) {
		this.strEndDate = strEndDate;
	}
	
	@Column(name = "name", length = 50)
	public String getName() {
		return this.name;
	}
	
	public void setName(String name) {
		this.name = name;
	}
	
	@Column(name = "creditCode", length = 50)
	public String getCreditCode() {
		return this.creditCode;
	}
	
	public void setCreditCode(String creditCode) {
		this.creditCode = creditCode;
	}
	
	@Column(name = "creditId")
	public Integer getCreditId() {
		return this.creditId;
	}
	
	public void setCreditId(Integer creditId) {
		this.creditId = creditId;
	}
	
	@Column(name = "inputUser")
	public Integer getInputUser() {
		return this.inputUser;
	}
	
	public void setInputUser(Integer inputUser) {
		this.inputUser = inputUser;
	}
	
	@Column(name = "proName", length = 50)
	public String getProName() {
		return this.proName;
	}
	
	public void setProName(String proName) {
		this.proName = proName;
	}
	
	@Column(name = "proUserNo", length = 50)
	public String getProUserNo() {
		return this.proUserNo;
	}
	
	public void setProUserNo(String proUserNo) {
		this.proUserNo = proUserNo;
	}
	
	@Column(name = "productTypeId")
	public Integer getProductTypeId() {
		return this.productTypeId;
	}
	
	public void setProductTypeId(Integer productTypeId) {
		this.productTypeId = productTypeId;
	}
	
	@Column(name = "productTypeName", length = 50)
	public String getProductTypeName() {
		return this.productTypeName;
	}
	
	public void setProductTypeName(String productTypeName) {
		this.productTypeName = productTypeName;
	}
	
	@Column(name = "productId")
	public Integer getProductId() {
		return this.productId;
	}
	
	public void setProductId(Integer productId) {
		this.productId = productId;
	}
	
	@Column(name = "productName", length = 40)
	public String getProductName() {
		return this.productName;
	}
	
	public void setProductName(String productName) {
		this.productName = productName;
	}
	
	@Column(name = "productCode", length = 40)
	public String getProductCode() {
		return this.productCode;
	}
	
	public void setProductCode(String productCode) {
		this.productCode = productCode;
	}
	
	@Temporal(TemporalType.DATE)
	@JSON(format="yyyy-MM-dd")
	@Column(name = "strVideoEndDate", length = 0)
	public Date getStrVideoEndDate() {
		return this.strVideoEndDate;
	}
	
	public void setStrVideoEndDate(Date strVideoEndDate) {
		this.strVideoEndDate = strVideoEndDate;
	}
	
	@Column(name = "guaranteeYears")
	public Integer getGuaranteeYears() {
		return this.guaranteeYears;
	}
	
	public void setGuaranteeYears(Integer guaranteeYears) {
		this.guaranteeYears = guaranteeYears;
	}
	
	@Column(name = "productSaveMonth")
	public Integer getProductSaveMonth() {
		return this.productSaveMonth;
	}
	
	public void setProductSaveMonth(Integer productSaveMonth) {
		this.productSaveMonth = productSaveMonth;
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
	
	@Column(name = "strBankNo", length = 40)
	public String getStrBankNo() {
		return this.strBankNo;
	}
	
	public void setStrBankNo(String strBankNo) {
		this.strBankNo = strBankNo;
	}
	
	@Column(name = "strBuyMoney", length = 40)
	public String getStrBuyMoney() {
		return this.strBuyMoney;
	}
	
	public void setStrBuyMoney(String strBuyMoney) {
		this.strBuyMoney = strBuyMoney;
	}
	
	@Column(name = "strCreditPhone", length = 40)
	public String getStrCreditPhone() {
		return this.strCreditPhone;
	}
	
	public void setStrCreditPhone(String strCreditPhone) {
		this.strCreditPhone = strCreditPhone;
	}
	
	@Column(name = "strRemarks", length = 500)
	public String getStrRemarks() {
		return this.strRemarks;
	}
	
	public void setStrRemarks(String strRemarks) {
		this.strRemarks = strRemarks;
	}
	
	@Column(name = "guaranteeType")
	public Integer getGuaranteeType() {
		return guaranteeType;
	}
	
	public void setGuaranteeType(Integer guaranteeType) {
		this.guaranteeType = guaranteeType;
	}
	
	@Column(name = "productSaveType")
	public Integer getProductSaveType() {
		return productSaveType;
	}
	
	public void setProductSaveType(Integer productSaveType) {
		this.productSaveType = productSaveType;
	}
	
	@Transient
	public String getCreditName() {
		return creditName;
	}
	
	public void setCreditName(String creditName) {
		this.creditName = creditName;
	}
	
	@Column(name = "lockClient")
	public Integer getLockClient() {
		return lockClient;
	}
	
	public void setLockClient(Integer lockClient) {
		this.lockClient = lockClient;
	}

}
