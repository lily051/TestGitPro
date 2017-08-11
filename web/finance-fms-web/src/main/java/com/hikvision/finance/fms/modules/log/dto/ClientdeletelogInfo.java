/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月8日 下午4:18:57
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.log.dto;

import java.util.Date;

import org.apache.struts2.json.annotations.JSON;


/**
 * <p></p>
 * @author jinxindong 2016年4月8日 下午4:18:57
 * @version V1.0 
 */
public class ClientdeletelogInfo {
	private Integer id;
	private String nproId;//业务单号*
	private String name;//客户姓名*
	private String creditId;//客户证件号*
	private Integer productType;//产品类型*
	private String productName;//产品名称*
	private Date inputTime;//录入时间
	private String inputUser;//录入用户*
	private Integer inputRegionId;//录入区域
	private Date deleteTime;//删除时间*
	private String deleteTimeString;//删除时间*
	private Integer soundNum;//音频文件数量
	private Integer vedioNum;//视频文件数量
	private String deleteTimeStart;
	private String deleteTimeEnd;
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
    @JSON(format="yyyy-MM-dd HH:mm:ss")
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
    @JSON(format="yyyy-MM-dd HH:mm:ss")
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

	
    public String getDeleteTimeStart() {
    	return deleteTimeStart;
    }

	
    public void setDeleteTimeStart(String deleteTimeStart) {
    	this.deleteTimeStart = deleteTimeStart;
    }

	
    public String getDeleteTimeEnd() {
    	return deleteTimeEnd;
    }

	
    public void setDeleteTimeEnd(String deleteTimeEnd) {
    	this.deleteTimeEnd = deleteTimeEnd;
    }

	
    public String getDeleteTimeString() {
    	return deleteTimeString;
    }

	
    public void setDeleteTimeString(String deleteTimeString) {
    	this.deleteTimeString = deleteTimeString;
    }

	
}
