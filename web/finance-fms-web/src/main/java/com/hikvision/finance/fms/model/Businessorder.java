package com.hikvision.finance.fms.model;

import static javax.persistence.GenerationType.IDENTITY;

import java.util.Date;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.Id;
import javax.persistence.Table;
import javax.persistence.Temporal;
import javax.persistence.TemporalType;

import org.apache.struts2.json.annotations.JSON;

/**
 * Businessorder entity. @author MyEclipse Persistence Tools
 */
@Entity
@Table(name = "businessorder")
public class Businessorder implements java.io.Serializable {

	// Fields

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = -7056167282439949512L;
	private Integer id;
	private Integer inputRegionId;
	private Integer nuserId;
	private Integer clientId;
	private Integer status;
	private String nproId;
	private Date businessTime;
	private String customerName;
	private String creditCode;
	private Integer creditId;
	private Integer productTypeId;
	private String productTypeName;
	private Integer productId;
	private String productName;
	private String productCode;
	private Date strEndDate;
	private Integer guaranteeYears;
	private Integer guaranteeType; //保障周期类型
	private Integer productSaveMonth;
	private Integer productSaveType;//保存时间类型
	private String strGrade;
	private String strRisklevel;
	private String strBankNo;
	private String strBuyMoney;
	private String strCreditPhone;
	private String strRemarks;
	private String compReason;
	private Date complainTime;
	
	// Constructors
	
	/** default constructor */
	public Businessorder() {
	}
	
	/** full constructor */
	public Businessorder(Integer inputRegionId, Integer nuserId, Integer clientId, Integer status, String nproId,
			Date businessTime, String customerName, String creditCode, Integer creditId,
	        Integer productTypeId, String productTypeName, Integer productId, String productName, String productCode,
	        Date strEndDate, Integer guaranteeYears, Integer productSaveMonth, String strGrade, String strRisklevel,
	        String strBankNo, String strBuyMoney, String strCreditPhone, String strRemarks, String compReason,
	        Date complainTime) {
		this.inputRegionId = inputRegionId;
		this.nuserId = nuserId;
		this.clientId = clientId;
		this.status = status;
		this.nproId = nproId;
		this.businessTime = businessTime;
		this.customerName = customerName;
		this.creditCode = creditCode;
		this.creditId = creditId;
		this.productTypeId = productTypeId;
		this.productTypeName = productTypeName;
		this.productId = productId;
		this.productName = productName;
		this.productCode = productCode;
		this.strEndDate = strEndDate;
		this.guaranteeYears = guaranteeYears;
		this.productSaveMonth = productSaveMonth;
		this.strGrade = strGrade;
		this.strRisklevel = strRisklevel;
		this.strBankNo = strBankNo;
		this.strBuyMoney = strBuyMoney;
		this.strCreditPhone = strCreditPhone;
		this.strRemarks = strRemarks;
		this.compReason = compReason;
		this.complainTime = complainTime;
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
	
	@Column(name = "inputRegionId")
	public Integer getInputRegionId() {
		return this.inputRegionId;
	}
	
	public void setInputRegionId(Integer inputRegionId) {
		this.inputRegionId = inputRegionId;
	}
	
	@Column(name = "nUserId")
	public Integer getNuserId() {
		return this.nuserId;
	}
	
	public void setNuserId(Integer nuserId) {
		this.nuserId = nuserId;
	}
	
	@Column(name = "clientId")
	public Integer getClientId() {
		return this.clientId;
	}
	
	public void setClientId(Integer clientId) {
		this.clientId = clientId;
	}
	
	@Column(name = "status")
	public Integer getStatus() {
		return this.status;
	}
	
	public void setStatus(Integer status) {
		this.status = status;
	}
	
	@Column(name = "nProId", length = 50)
	public String getNproId() {
		return this.nproId;
	}
	
	public void setNproId(String nproId) {
		this.nproId = nproId;
	}
	
	@JSON(format="yyyy-MM-dd HH:mm:ss")
	@Column(name = "businessTime", length = 0)
	public Date getBusinessTime() {
		return this.businessTime;
	}
	
	public void setBusinessTime(Date businessTime) {
		this.businessTime = businessTime;
	}
	
	@Column(name = "customerName", length = 50)
	public String getCustomerName() {
		return this.customerName;
	}
	
	public void setCustomerName(String customerName) {
		this.customerName = customerName;
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
	
	@Column(name = "productTypeId")
	public Integer getProductTypeId() {
		return this.productTypeId;
	}
	
	public void setProductTypeId(Integer productTypeId) {
		this.productTypeId = productTypeId;
	}
	
	@Column(name = "productTypeName")
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
	@Column(name = "strEndDate", length = 0)
	public Date getStrEndDate() {
		return this.strEndDate;
	}
	
	public void setStrEndDate(Date strEndDate) {
		this.strEndDate = strEndDate;
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
	
	@Column(name = "strGrade", length = 50)
	public String getStrGrade() {
		return this.strGrade;
	}
	
	public void setStrGrade(String strGrade) {
		this.strGrade = strGrade;
	}
	
	@Column(name = "strRisklevel", length = 50)
	public String getStrRisklevel() {
		return this.strRisklevel;
	}
	
	public void setStrRisklevel(String strRisklevel) {
		this.strRisklevel = strRisklevel;
	}
	
	@Column(name = "strBankNo", length = 50)
	public String getStrBankNo() {
		return this.strBankNo;
	}
	
	public void setStrBankNo(String strBankNo) {
		this.strBankNo = strBankNo;
	}
	
	@Column(name = "strBuyMoney", length = 50)
	public String getStrBuyMoney() {
		return this.strBuyMoney;
	}
	
	public void setStrBuyMoney(String strBuyMoney) {
		this.strBuyMoney = strBuyMoney;
	}
	
	@Column(name = "strCreditPhone", length = 50)
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
	
	@Column(name = "compReason", length = 1000)
	public String getCompReason() {
		return this.compReason;
	}
	
	public void setCompReason(String compReason) {
		this.compReason = compReason;
	}
	
	@Column(name = "complainTime", length = 0)
	public Date getComplainTime() {
		return this.complainTime;
	}
	
	public void setComplainTime(Date complainTime) {
		this.complainTime = complainTime;
	}

	public Integer getGuaranteeType() {
	    return guaranteeType;
    }

	public void setGuaranteeType(Integer guaranteeType) {
	    this.guaranteeType = guaranteeType;
    }

	public Integer getProductSaveType() {
	    return productSaveType;
    }

	public void setProductSaveType(Integer productSaveType) {
	    this.productSaveType = productSaveType;
    }
	
}
