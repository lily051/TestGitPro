/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月8日 下午4:18:57
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.log.qo;

import java.util.Date;

import com.hikvision.finance.extend.hibernate.qo.BaseQo;


/**
 * <p></p>
 * @author jinxindong 2016年4月8日 下午4:18:57
 * @version V1.0 
 */
public class ClientdeletelogQo extends BaseQo{
	/**
     * 序列化ID
     */
    private static final long serialVersionUID = -3556762540273121031L;
	private Integer id;
	private String nproId;
	private String name;
	private String creditId;
	private Integer productType;
	private String productName;
	private Date inputTime;
	private String inputUser;
	private Integer inputRegionId;
	private Date deleteTime;
	private Integer soundNum;
	private Integer vedioNum;
	
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
	
    public Integer getProductType() {
    	return productType;
    }
	
    public void setProductType(Integer productType) {
    	this.productType = productType;
    }
	
    public String getProductName() {
    	return productName;
    }
	
    public void setProductName(String productName) {
    	this.productName = productName;
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
	
    public Date getDeleteTime() {
    	return deleteTime;
    }
	
    public void setDeleteTime(Date deleteTime) {
    	this.deleteTime = deleteTime;
    }
	
    public Integer getSoundNum() {
    	return soundNum;
    }
	
    public void setSoundNum(Integer soundNum) {
    	this.soundNum = soundNum;
    }
	
    public Integer getVedioNum() {
    	return vedioNum;
    }
	
    public void setVedioNum(Integer vedioNum) {
    	this.vedioNum = vedioNum;
    }
	
}
