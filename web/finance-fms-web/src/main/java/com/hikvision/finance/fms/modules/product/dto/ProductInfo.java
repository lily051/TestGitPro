/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月21日 下午6:12:45
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.product.dto;

import java.util.Date;

import org.apache.struts2.json.annotations.JSON;

/**
 * <p>
 * </p>
 * @author jinxindong 2016年3月21日 下午6:12:45
 * @version V1.0
 */
public class ProductInfo {
	
	private Integer id;
	private String strCode;
	private String strName;
	private Integer pid;
	private Integer saveYears;
	private Integer saveType; // 保存期限类型 ，年月日
	private Integer guaranteeYears;
	private Integer guaranteeType;// 保障期限类型，年月日
	private Integer nenabled;
	private Date strEndDate;
	private Date strApplyDate;
	private Date strVideoEndDate;
	private Integer importUser;
	private Integer checkUser;
	private Date importTime;
	private Date checkTime;
	private String strGrade;
	private String strRisklevel;
	private String wordTemplate;
	private String remark;
	private String checkUserName;// 复合员名字
	private String productTypeName;// 该产品的产品类型名字
	private Integer relateStatus;// 关联状态0未关联 1已关联
	private String strRisklevelName;// 风险等级名称
	private String relateStatusName;// 关联状态0未关联 1已关联
	private String nenabledName;// 禁用启用
	
	private String strSaveType;
	private String strGuaranteeType;
	
	public String getNenabledName() {
		return nenabledName;
	}
	
	public void setNenabledName(String nenabledName) {
		this.nenabledName = nenabledName;
	}
	
	public String getRemark() {
		return remark;
	}
	
	public void setRemark(String remark) {
		this.remark = remark;
	}
	
	public Integer getId() {
		return id;
	}
	
	public void setId(Integer id) {
		this.id = id;
	}
	
	public String getStrCode() {
		return strCode;
	}
	
	public void setStrCode(String strCode) {
		this.strCode = strCode;
	}
	
	public Integer getPid() {
		return pid;
	}
	
	public void setPid(Integer pid) {
		this.pid = pid;
	}
	
	public Integer getSaveYears() {
		return saveYears;
	}
	
	public void setSaveYears(Integer saveYears) {
		this.saveYears = saveYears;
	}
	
	public Integer getGuaranteeYears() {
		return guaranteeYears;
	}
	
	public void setGuaranteeYears(Integer guaranteeYears) {
		this.guaranteeYears = guaranteeYears;
	}
	
	public Integer getNenabled() {
		return nenabled;
	}
	
	public void setNenabled(Integer nenabled) {
		this.nenabled = nenabled;
	}
	
	@JSON(format = "yyyy-MM-dd")
	public Date getStrEndDate() {
		return strEndDate;
	}
	
	public void setStrEndDate(Date strEndDate) {
		this.strEndDate = strEndDate;
	}
	
	public Date getStrApplyDate() {
		return strApplyDate;
	}
	
	public void setStrApplyDate(Date strApplyDate) {
		this.strApplyDate = strApplyDate;
	}
	
	public Date getStrVideoEndDate() {
		return strVideoEndDate;
	}
	
	public void setStrVideoEndDate(Date strVideoEndDate) {
		this.strVideoEndDate = strVideoEndDate;
	}
	
	public Integer getImportUser() {
		return importUser;
	}
	
	public void setImportUser(Integer importUser) {
		this.importUser = importUser;
	}
	
	public Integer getCheckUser() {
		return checkUser;
	}
	
	public void setCheckUser(Integer checkUser) {
		this.checkUser = checkUser;
	}
	
	public Date getImportTime() {
		return importTime;
	}
	
	public void setImportTime(Date importTime) {
		this.importTime = importTime;
	}
	
	public Date getCheckTime() {
		return checkTime;
	}
	
	public void setCheckTime(Date checkTime) {
		this.checkTime = checkTime;
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
	
	public String getWordTemplate() {
		return wordTemplate;
	}
	
	public void setWordTemplate(String wordTemplate) {
		this.wordTemplate = wordTemplate;
	}
	
	public String getStrName() {
		return strName;
	}
	
	public void setStrName(String strName) {
		this.strName = strName;
	}
	
	public String getCheckUserName() {
		return checkUserName;
	}
	
	public void setCheckUserName(String checkUserName) {
		this.checkUserName = checkUserName;
	}
	
	public Integer getRelateStatus() {
		return relateStatus;
	}
	
	public void setRelateStatus(Integer relateStatus) {
		this.relateStatus = relateStatus;
	}
	
	public String getProductTypeName() {
		return productTypeName;
	}
	
	public void setProductTypeName(String productTypeName) {
		this.productTypeName = productTypeName;
	}
	
	public String getStrRisklevelName() {
		return strRisklevelName;
	}
	
	public void setStrRisklevelName(String strRisklevelName) {
		this.strRisklevelName = strRisklevelName;
	}
	
	public String getRelateStatusName() {
		return relateStatusName;
	}
	
	public void setRelateStatusName(String relateStatusName) {
		this.relateStatusName = relateStatusName;
	}
	
	public Integer getSaveType() {
		return saveType;
	}
	
	public void setSaveType(Integer saveType) {
		this.saveType = saveType;
	}
	
	public Integer getGuaranteeType() {
		return guaranteeType;
	}
	
	public void setGuaranteeType(Integer guaranteeType) {
		this.guaranteeType = guaranteeType;
	}
	
	public String getStrGuaranteeType() {
		return strGuaranteeType;
	}
	
	public void setStrGuaranteeType(String strGuaranteeType) {
		this.strGuaranteeType = strGuaranteeType;
	}
	
	public String getStrSaveType() {
		return strSaveType;
	}
	
	public void setStrSaveType(String strSaveType) {
		this.strSaveType = strSaveType;
	}
	
}
