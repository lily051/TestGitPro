/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-4-17 下午5:12:39
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.webdevice.contact.qo;

import java.util.Date;

import com.hikvision.finance.extend.hibernate.qo.BaseQo;

/**
 * <p></p>
 * @author xujiangfei 2016-4-17 下午5:12:39
 * @version V1.0 
 */
public class BusinessOrderQo extends BaseQo{

	
	/**
	 * 序列化ID
	 */
	private static final long serialVersionUID = -6489789222528563857L;
	
	private Integer id;
	private String nproId;
	private Integer nuserId;
	private String customerName;
	private String creditId;
	private Integer status;
	private Date inputTime;
	private String inputUser;
	private Integer inputRegionId;
	private String proName;
	private String proUserNo;
	private Date businessTime;
	private String productName;
	private String productCode;
	private Integer productSaveMonth;
	private Date strEndDate;
	private String creditCode;
	private Integer guaranteeYears;
	private String strGrade;
	private String strRisklevel;
	private String strBankNo;
	private String strBuyMoney;
	private String strRemarks;
	private String strCreditPhone;
	
	
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
	public Integer getNuserId() {
		return nuserId;
	}
	public void setNuserId(Integer nuserId) {
		this.nuserId = nuserId;
	}
	public String getCustomerName() {
		return customerName;
	}
	public void setCustomerName(String customerName) {
		this.customerName = customerName;
	}
	public String getCreditId() {
		return creditId;
	}
	public void setCreditId(String creditId) {
		this.creditId = creditId;
	}
	public Integer getStatus() {
		return status;
	}
	public void setStatus(Integer status) {
		this.status = status;
	}
	public Date getInputTime() {
		return inputTime;
	}
	public void setInputTime(Date inputTime) {
		this.inputTime = inputTime;
	}
	public String getInputUser() {
		return inputUser;
	}
	public void setInputUser(String inputUser) {
		this.inputUser = inputUser;
	}
	public Integer getInputRegionId() {
		return inputRegionId;
	}
	public void setInputRegionId(Integer inputRegionId) {
		this.inputRegionId = inputRegionId;
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
	public Date getBusinessTime() {
		return businessTime;
	}
	public void setBusinessTime(Date businessTime) {
		this.businessTime = businessTime;
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
	public Integer getProductSaveMonth() {
		return productSaveMonth;
	}
	public void setProductSaveMonth(Integer productSaveMonth) {
		this.productSaveMonth = productSaveMonth;
	}
	public Date getStrEndDate() {
		return strEndDate;
	}
	public void setStrEndDate(Date strEndDate) {
		this.strEndDate = strEndDate;
	}
	public String getCreditCode() {
		return creditCode;
	}
	public void setCreditCode(String creditCode) {
		this.creditCode = creditCode;
	}
	public Integer getGuaranteeYears() {
		return guaranteeYears;
	}
	public void setGuaranteeYears(Integer guaranteeYears) {
		this.guaranteeYears = guaranteeYears;
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
	public String getStrRemarks() {
		return strRemarks;
	}
	public void setStrRemarks(String strRemarks) {
		this.strRemarks = strRemarks;
	}
	public String getStrCreditPhone() {
		return strCreditPhone;
	}
	public void setStrCreditPhone(String strCreditPhone) {
		this.strCreditPhone = strCreditPhone;
	}
	
}
