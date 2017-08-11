/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月7日 下午6:58:23
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.sale.qo;

import java.util.Date;

import com.hikvision.finance.extend.hibernate.qo.BaseQo;


/**
 * <p></p>
 * @author jinxindong 2016年4月7日 下午6:58:23
 * @version V1.0 
 */
public class SaleQo extends BaseQo{
	/**
     * 序列化ID
     */
    private static final long serialVersionUID = -7083946162939856875L;
	private Integer id;
	private String nproId;//业务单号
	private String name;//客户姓名
	private String creditId;//证件类型Id
	private Integer status;//业务单状态 签约，投诉等 1签约 2 投诉
	private Integer productType;//产品类别
	private Date inputTime;//理财系统建单时间
	private String inputUser;//理财系统建单用户
	private Integer inputRegionId;//理财系统建单区域号
	private String inputRegionName;//理财系统建单区域名称
	private String proName;//业务员姓名
	private String proUserNo;//业务员工号
	private Date businessTime;//业务时间  入单时间  重复
	private String productName;//产品名称
	private String productCode;//产品代码
	private Integer productSaveMonth;//产品到期后保存期限
	private Date strEndDate;//产品到期日
	private Integer guaranteeYears;//保障期限
	private Date strVideoEndDate;//视频到期日
	private Integer delaydays;//延期天数
	private Integer saveflag;//保存标志 1 永久保存  0正常
	private Integer tipflag;//删除提示标志0 正常 >1删除提示天数（最大提示天数7）
	private Date markDate;//更新tipflag标志时间
	private String creditCode;//证件号
	private String strGrade;//产品发行机构
	private String strRisklevel;//产品风险等级
	private String strBankNo;//客户银行卡号
	private String strBuyMoney;//购买金额
	private String strRemarks;//业务单备注
	private Integer nisUpload;//是否上传完成
	private Integer nisPay;//是否兑付 1是 0否   是否过期
	private String strCreditPhone;//客户电话
	
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
	
    public String getName() {
    	return name;
    }
	
    public void setName(String name) {
    	this.name = name;
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
	
    public Integer getProductType() {
    	return productType;
    }
	
    public void setProductType(Integer productType) {
    	this.productType = productType;
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
	
    public String getInputRegionName() {
    	return inputRegionName;
    }
	
    public void setInputRegionName(String inputRegionName) {
    	this.inputRegionName = inputRegionName;
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
	
    public Integer getGuaranteeYears() {
    	return guaranteeYears;
    }
	
    public void setGuaranteeYears(Integer guaranteeYears) {
    	this.guaranteeYears = guaranteeYears;
    }
	
    public Date getStrVideoEndDate() {
    	return strVideoEndDate;
    }
	
    public void setStrVideoEndDate(Date strVideoEndDate) {
    	this.strVideoEndDate = strVideoEndDate;
    }
	
    public Integer getDelaydays() {
    	return delaydays;
    }
	
    public void setDelaydays(Integer delaydays) {
    	this.delaydays = delaydays;
    }
	
    public Integer getSaveflag() {
    	return saveflag;
    }
	
    public void setSaveflag(Integer saveflag) {
    	this.saveflag = saveflag;
    }
	
    public Integer getTipflag() {
    	return tipflag;
    }
	
    public void setTipflag(Integer tipflag) {
    	this.tipflag = tipflag;
    }
	
    public Date getMarkDate() {
    	return markDate;
    }
	
    public void setMarkDate(Date markDate) {
    	this.markDate = markDate;
    }
	
    public String getCreditCode() {
    	return creditCode;
    }
	
    public void setCreditCode(String creditCode) {
    	this.creditCode = creditCode;
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
	
    public Integer getNisUpload() {
    	return nisUpload;
    }
	
    public void setNisUpload(Integer nisUpload) {
    	this.nisUpload = nisUpload;
    }
	
    public Integer getNisPay() {
    	return nisPay;
    }
	
    public void setNisPay(Integer nisPay) {
    	this.nisPay = nisPay;
    }
	
    public String getStrCreditPhone() {
    	return strCreditPhone;
    }
	
    public void setStrCreditPhone(String strCreditPhone) {
    	this.strCreditPhone = strCreditPhone;
    }
	
}
