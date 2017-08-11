/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-4-26 下午1:49:01
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.webdevice.contact.dto;

import java.util.Date;

import org.apache.struts2.json.annotations.JSON;

/**
 * <p>
 * 业务关联 模块的返回的页面数据对象；以records表为主表
 * </p>
 * 
 * @author xujiangfei 2016-4-26 下午1:49:01
 * @version V1.0
 */
public class RealOrdersInfo {

	// ProSaveRecord 表
	private Integer proid;// ProSaveRecord 表的id
	private Integer recordId;// Records表的Id
	private String strRecordName;
	private Date startTime;
	private Date endTime;
	private Integer stateType; // 1-签约录像，2-投诉录像

	// ClientInput
	private Integer cliid;// ClientInput 表的id
	private String nProId;// 业务单号
	private Integer status;// 签约状态
	private String customername;// 客户姓名
	private String productName;// 产品名称
	private String strBuyMoney;// 购买金额
	private Date dtBusinessTime;// 业务时间
	private Integer lockClient; // 业务单锁定状态

	public Integer getProid() {
		return proid;
	}

	public void setProid(Integer proid) {
		this.proid = proid;
	}

	public String getStrRecordName() {
		return strRecordName;
	}

	public void setStrRecordName(String strRecordName) {
		this.strRecordName = strRecordName;
	}

	public Integer getCliid() {
		return cliid;
	}

	public void setCliid(Integer cliid) {
		this.cliid = cliid;
	}

	public String getnProId() {
		return nProId;
	}

	public void setnProId(String nProId) {
		this.nProId = nProId;
	}

	@JSON(format = "yyyy-MM-dd HH:mm:ss")
	public Date getDtBusinessTime() {
		return dtBusinessTime;
	}

	public void setDtBusinessTime(Date dtBusinessTime) {
		this.dtBusinessTime = dtBusinessTime;
	}

	public String getCustomername() {
		return customername;
	}

	public void setCustomername(String customername) {
		this.customername = customername;
	}

	public Integer getStatus() {
		return status;
	}

	public void setStatus(Integer status) {
		this.status = status;
	}

	public String getProductName() {
		return productName;
	}

	public void setProductName(String productName) {
		this.productName = productName;
	}

	public String getStrBuyMoney() {
		return strBuyMoney;
	}

	public void setStrBuyMoney(String strBuyMoney) {
		this.strBuyMoney = strBuyMoney;
	}

	@JSON(format = "yyyy-MM-dd HH:mm:ss")
	public Date getStartTime() {
		return startTime;
	}

	public void setStartTime(Date startTime) {
		this.startTime = startTime;
	}

	@JSON(format = "yyyy-MM-dd HH:mm:ss")
	public Date getEndTime() {
		return endTime;
	}

	public void setEndTime(Date endTime) {
		this.endTime = endTime;
	}

	public Integer getRecordId() {
		return recordId;
	}

	public void setRecordId(Integer recordId) {
		this.recordId = recordId;
	}

	public Integer getLockClient() {
		return lockClient;
	}

	public void setLockClient(Integer lockClient) {
		this.lockClient = lockClient;
	}

	public Integer getStateType() {
		return stateType;
	}

	public void setStateType(Integer stateType) {
		this.stateType = stateType;
	}
}
