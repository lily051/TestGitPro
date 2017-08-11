/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年5月20日 下午2:47:20
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.webdevice.contact.dto;


/**
 * <p>业务单字段使能状态</p>
 * @author jinchenwu 2016年5月20日 下午2:47:20
 * @version V1.0 
 */
public class OrderEnableInfo {
	private int inputRegionCodeEnable;// 交易区域号
	private int strCreditPhoneEnable;// 客户电话
	private int strBankNoEnable;// 客户银行卡号
	private int strGradeEnable;// 产品发行机构
	private int guaranteeYearsEnable;// 产品保障期限
	private int strVideoEndDateEnable;// 产品到期日
	private int productSaveMonthEnable;// 视频保存时间
	private int strRisklevelEnable;// 产品风险等级
	private int strBuyMoneyEnable;// 购买金额
	private int proUserNoEnable;// 业务员工号
	private int strRemarksEnable;// 备注
	
    public int getInputRegionCodeEnable() {
    	return inputRegionCodeEnable;
    }
	
    public void setInputRegionCodeEnable(int inputRegionCodeEnable) {
    	this.inputRegionCodeEnable = inputRegionCodeEnable;
    }
	
    public int getStrCreditPhoneEnable() {
    	return strCreditPhoneEnable;
    }
	
    public void setStrCreditPhoneEnable(int strCreditPhoneEnable) {
    	this.strCreditPhoneEnable = strCreditPhoneEnable;
    }
	
    public int getStrBankNoEnable() {
    	return strBankNoEnable;
    }
	
    public void setStrBankNoEnable(int strBankNoEnable) {
    	this.strBankNoEnable = strBankNoEnable;
    }
	
    public int getStrGradeEnable() {
    	return strGradeEnable;
    }
	
    public void setStrGradeEnable(int strGradeEnable) {
    	this.strGradeEnable = strGradeEnable;
    }
	
    public int getGuaranteeYearsEnable() {
    	return guaranteeYearsEnable;
    }
	
    public void setGuaranteeYearsEnable(int guaranteeYearsEnable) {
    	this.guaranteeYearsEnable = guaranteeYearsEnable;
    }
	
    public int getStrVideoEndDateEnable() {
    	return strVideoEndDateEnable;
    }
	
    public void setStrVideoEndDateEnable(int strVideoEndDateEnable) {
    	this.strVideoEndDateEnable = strVideoEndDateEnable;
    }
	
    public int getProductSaveMonthEnable() {
    	return productSaveMonthEnable;
    }
	
    public void setProductSaveMonthEnable(int productSaveMonthEnable) {
    	this.productSaveMonthEnable = productSaveMonthEnable;
    }
	
    public int getStrRisklevelEnable() {
    	return strRisklevelEnable;
    }
	
    public void setStrRisklevelEnable(int strRisklevelEnable) {
    	this.strRisklevelEnable = strRisklevelEnable;
    }
	
    public int getStrBuyMoneyEnable() {
    	return strBuyMoneyEnable;
    }
	
    public void setStrBuyMoneyEnable(int strBuyMoneyEnable) {
    	this.strBuyMoneyEnable = strBuyMoneyEnable;
    }
	
    public int getProUserNoEnable() {
    	return proUserNoEnable;
    }
	
    public void setProUserNoEnable(int proUserNoEnable) {
    	this.proUserNoEnable = proUserNoEnable;
    }
	
    public int getStrRemarksEnable() {
    	return strRemarksEnable;
    }
	
    public void setStrRemarksEnable(int strRemarksEnable) {
    	this.strRemarksEnable = strRemarksEnable;
    }
}
