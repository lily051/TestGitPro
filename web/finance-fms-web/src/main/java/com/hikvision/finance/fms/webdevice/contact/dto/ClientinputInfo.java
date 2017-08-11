package com.hikvision.finance.fms.webdevice.contact.dto;

import java.util.Date;

import org.apache.struts2.json.annotations.JSON;

public class ClientinputInfo {

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
	
	private String complainTime;//投诉业务单的时候, 要在前端返回后端服务器的时间
	
	public Integer getId() {
		return id;
	}
	public void setId(Integer id) {
		this.id = id;
	}
	public String getNproId() {
		return nproId;
	}
	public void setNproId(String nproId) {
		this.nproId = nproId;
	}
	public Date getInputTime() {
		return inputTime;
	}
	public void setInputTime(Date inputTime) {
		this.inputTime = inputTime;
	}
	
	@JSON(format = "yyyy-MM-dd HH:mm:ss")
	public Date getBusinessTime() {
		return businessTime;
	}
	public void setBusinessTime(Date businessTime) {
		this.businessTime = businessTime;
	}
	public Integer getInputRegionId() {
		return inputRegionId;
	}
	public void setInputRegionId(Integer inputRegionId) {
		this.inputRegionId = inputRegionId;
	}
	public String getInputRegionCode() {
		return inputRegionCode;
	}
	public void setInputRegionCode(String inputRegionCode) {
		this.inputRegionCode = inputRegionCode;
	}
	public String getInputRegionName() {
		return inputRegionName;
	}
	public void setInputRegionName(String inputRegionName) {
		this.inputRegionName = inputRegionName;
	}
	public Integer getStatus() {
		return status;
	}
	public void setStatus(Integer status) {
		this.status = status;
	}
	public Integer getNisRead() {
		return nisRead;
	}
	public void setNisRead(Integer nisRead) {
		this.nisRead = nisRead;
	}
	public Integer getNisPay() {
		return nisPay;
	}
	public void setNisPay(Integer nisPay) {
		this.nisPay = nisPay;
	}
	public Integer getDelaydays() {
		return delaydays;
	}
	public void setDelaydays(Integer delaydays) {
		this.delaydays = delaydays;
	}
	@JSON(format = "yyyy-MM-dd")
	public Date getStrEndDate() {
		return strEndDate;
	}
	public void setStrEndDate(Date strEndDate) {
		this.strEndDate = strEndDate;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getCreditCode() {
		return creditCode;
	}
	public void setCreditCode(String creditCode) {
		this.creditCode = creditCode;
	}
	public Integer getCreditId() {
		return creditId;
	}
	public void setCreditId(Integer creditId) {
		this.creditId = creditId;
	}
	public Integer getInputUser() {
		return inputUser;
	}
	public void setInputUser(Integer inputUser) {
		this.inputUser = inputUser;
	}
	public String getProName() {
		return proName;
	}
	public void setProName(String proName) {
		this.proName = proName;
	}
	public String getProUserNo() {
		return proUserNo;
	}
	public void setProUserNo(String proUserNo) {
		this.proUserNo = proUserNo;
	}
	public Integer getProductTypeId() {
		return productTypeId;
	}
	public void setProductTypeId(Integer productTypeId) {
		this.productTypeId = productTypeId;
	}
	public String getProductTypeName() {
		return productTypeName;
	}
	public void setProductTypeName(String productTypeName) {
		this.productTypeName = productTypeName;
	}
	public Integer getProductId() {
		return productId;
	}
	public void setProductId(Integer productId) {
		this.productId = productId;
	}
	public String getProductName() {
		return productName;
	}
	public void setProductName(String productName) {
		this.productName = productName;
	}
	public String getProductCode() {
		return productCode;
	}
	public void setProductCode(String productCode) {
		this.productCode = productCode;
	}
	public Date getStrVideoEndDate() {
		return strVideoEndDate;
	}
	public void setStrVideoEndDate(Date strVideoEndDate) {
		this.strVideoEndDate = strVideoEndDate;
	}
	public Integer getGuaranteeType() {
		return guaranteeType;
	}
	public void setGuaranteeType(Integer guaranteeType) {
		this.guaranteeType = guaranteeType;
	}
	public Integer getGuaranteeYears() {
		return guaranteeYears;
	}
	public void setGuaranteeYears(Integer guaranteeYears) {
		this.guaranteeYears = guaranteeYears;
	}
	public Integer getProductSaveType() {
		return productSaveType;
	}
	public void setProductSaveType(Integer productSaveType) {
		this.productSaveType = productSaveType;
	}
	public Integer getProductSaveMonth() {
		return productSaveMonth;
	}
	public void setProductSaveMonth(Integer productSaveMonth) {
		this.productSaveMonth = productSaveMonth;
	}
	public String getStrGrade() {
		return strGrade;
	}
	public void setStrGrade(String strGrade) {
		this.strGrade = strGrade;
	}
	public String getStrRisklevel() {
		return strRisklevel;
	}
	public void setStrRisklevel(String strRisklevel) {
		this.strRisklevel = strRisklevel;
	}
	public String getStrBankNo() {
		return strBankNo;
	}
	public void setStrBankNo(String strBankNo) {
		this.strBankNo = strBankNo;
	}
	public String getStrBuyMoney() {
		return strBuyMoney;
	}
	public void setStrBuyMoney(String strBuyMoney) {
		this.strBuyMoney = strBuyMoney;
	}
	public String getStrCreditPhone() {
		return strCreditPhone;
	}
	public void setStrCreditPhone(String strCreditPhone) {
		this.strCreditPhone = strCreditPhone;
	}
	public String getStrRemarks() {
		return strRemarks;
	}
	public void setStrRemarks(String strRemarks) {
		this.strRemarks = strRemarks;
	}
	public String getCreditName() {
		return creditName;
	}
	public void setCreditName(String creditName) {
		this.creditName = creditName;
	}
	public Integer getLockClient() {
		return lockClient;
	}
	public void setLockClient(Integer lockClient) {
		this.lockClient = lockClient;
	}
	public String getComplainTime() {
		return complainTime;
	}
	public void setComplainTime(String complainTime) {
		this.complainTime = complainTime;
	}
	
}
