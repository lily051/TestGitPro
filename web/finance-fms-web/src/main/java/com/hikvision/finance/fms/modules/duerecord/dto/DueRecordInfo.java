/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月7日 下午7:53:25
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.duerecord.dto;

import java.math.BigInteger;
import java.util.Date;
import java.util.List;

import org.apache.struts2.json.annotations.JSON;


/**
 * <p>录像到期模块</p>
 * @author jinchenwu 2016年4月7日 下午7:53:25
 * @version V1.0 
 */
public class DueRecordInfo {
	
	// prosaverecord
	private Integer regionId;// 区域ID
	
	// clientinput
	private Integer busiId;// 业务单ID
	private String nproId;// 业务单号
	private String inputRegionName;// 交易区域名称
	private Date businessTime; // 业务办理时间
	private String proName;// 业务员姓名
	private String name;// 客户姓名
	private String productName;// 产品名称
	private Integer nisPay;// 是否已到期(已兑付) 0：未兑付，1：已兑付
	private Integer nisRead;// 是否已读，0：未读，1：已读
	private Date strVideoEndDate;// 录像到期日
	private BigInteger videoNum;// 录像数目
	
	// 其他
	private Integer daysAway;// 距离删除天数
	private Integer isFlag;// 是否显示子孙区域节点的理财录像信息，0：不显示，1：显示
	private Integer delayDays;// 录像保存期限延长天数
	private List<Integer> busiIds;// 若干业务单ID的字符串
	private String strIsPay;
	private String strIsRead;
	private String strBusinessTime;
	private String strRegionIds;// 过滤的区域ID
	private Date thresholdDate;// 到期日临界值
	
	public Integer getRegionId() {
		return regionId;
	}
	
	public void setRegionId(Integer regionId) {
		this.regionId = regionId;
	}
	
	public Integer getBusiId() {
		return busiId;
	}
	
	public void setBusiId(Integer busiId) {
		this.busiId = busiId;
	}
	
	public String getProductName() {
		return productName;
	}
	
	public void setProductName(String productName) {
		this.productName = productName;
	}
	
	public String getNproId() {
		return nproId;
	}
	
	public void setNproId(String nproId) {
		this.nproId = nproId;
	}
	
	public String getProName() {
		return proName;
	}
	
	public void setProName(String proName) {
		this.proName = proName;
	}
	
	public Integer getDaysAway() {
		return daysAway;
	}
	
	public void setDaysAway(Integer daysAway) {
		this.daysAway = daysAway;
	}
	
	public Integer getNisPay() {
		return nisPay;
	}
	
	public Integer getIsFlag() {
		return isFlag;
	}
	
	public void setIsFlag(Integer isFlag) {
		this.isFlag = isFlag;
	}
	
	@JSON(format="yyyy-MM-dd")
	public Date getBusinessTime() {
		return businessTime;
	}
	
	public void setBusinessTime(Date businessTime) {
		this.businessTime = businessTime;
	}
	
	public Integer getNisRead() {
		return nisRead;
	}
	
	public Integer getDelayDays() {
		return delayDays;
	}
	
	public void setDelayDays(Integer delayDays) {
		this.delayDays = delayDays;
	}
	
	public String getStrIsPay() {
		return strIsPay;
	}
	
	public void setStrIsPay(String strIsPay) {
		this.strIsPay = strIsPay;
	}
	
	public String getStrIsRead() {
		return strIsRead;
	}
	
	public void setStrIsRead(String strIsRead) {
		this.strIsRead = strIsRead;
	}
	
	public void setNisPay(Integer nisPay) {
		this.nisPay = nisPay;
	}
	
	public void setNisRead(Integer nisRead) {
		this.nisRead = nisRead;
	}
	
	public String getStrBusinessTime() {
		return strBusinessTime;
	}
	
	public void setStrBusinessTime(String strBusinessTime) {
		this.strBusinessTime = strBusinessTime;
	}
	
	public String getStrRegionIds() {
		return strRegionIds;
	}
	
	public void setStrRegionIds(String strRegionIds) {
		this.strRegionIds = strRegionIds;
	}
	
	public Date getThresholdDate() {
		return thresholdDate;
	}
	
	public void setThresholdDate(Date thresholdDate) {
		this.thresholdDate = thresholdDate;
	}
	
	public List<Integer> getBusiIds() {
		return busiIds;
	}
	
	public void setBusiIds(List<Integer> busiIds) {
		this.busiIds = busiIds;
	}
	
	public String getName() {
		return name;
	}
	
	public void setName(String name) {
		this.name = name;
	}
	
	public String getInputRegionName() {
		return inputRegionName;
	}
	
	public void setInputRegionName(String inputRegionName) {
		this.inputRegionName = inputRegionName;
	}
	
	public BigInteger getVideoNum() {
		return videoNum;
	}
	
	public void setVideoNum(BigInteger videoNum) {
		this.videoNum = videoNum;
	}
	
	@JSON(format = "yyyy-MM-dd")
	public Date getStrVideoEndDate() {
		return strVideoEndDate;
	}
	
	public void setStrVideoEndDate(Date strVideoEndDate) {
		this.strVideoEndDate = strVideoEndDate;
	}
	
}
