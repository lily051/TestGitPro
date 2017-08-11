/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月17日 上午10:10:29
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.product.qo;

import java.util.Date;

import com.hikvision.finance.extend.hibernate.qo.BaseQo;


/**
 * <p></p>
 * @author jinxindong 2016年3月17日 上午10:10:29
 * @version V1.0 
 */
public class ProductQo  extends BaseQo{
	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 3038270647114738990L;
	private Integer id;
	private String strCode;
	private String strName;
	private Integer pid;
	private Integer saveYears;
	private Integer guaranteeYears;
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

	

	
    public String getRemark() {
    	return remark;
    }

	
    public void setRemark(String remark) {
    	this.remark = remark;
    }

	

	
	
}
