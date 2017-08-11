/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年10月25日 下午6:49:48
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.cms.hppservice.model;

import java.util.List;

/**
 * <p>
 * </p>
 * 业务单详细信息(DC业务单需求)
 * 
 * @author zhoujiajun 2016年10月25日 下午6:49:48
 * @version V1.0
 */
public class RecordsInfoVO {

	private Integer id; // 业务单ID
	private String nProId; // 业务单号
	private Integer inputRegionId; // 区域id
	private String inputRegionCode; // 区域编码
	private String inputRegionName; // 区域名称
	private String custName; // 客户姓名
	private String strCreditPhone; // 客户电话
	private Integer creditId; // 客户证件类型id
	private String creditCode; // 客户证件号
	private String creditName; // 客户证类型名称
	private Integer productTypeId; // 产品类型
	private String productTypeName; // 产品类型名称
	private Integer productId; // 产品id
	private String productCode; // 产品编码
	private String productName; // 产品名称
	private String strBuyMoney; // 购买金额
	private String strGrade; // 产品发行机构
	private Integer videoNum; // 录像数量
	private String businessTime; // 业务办理时间
	private Integer lockClient; // 业务审批状态Id
	private String strlockClient; // 业务审批状态
	private Integer recordStatus; // 录像状态Id
	private String strRecordStatus; // 录像状态
	private String pictureStrart;      // 左侧图片(没有图片传入[nopic])
	private String pictureEnd;      // 右侧图片(没有图片传入[nopic])
	
	private List<VideoDetailInfoVO> recordsDetailInfos; // 录像的详细信息

	public Integer getId() {
		return id;
	}

	public void setId(Integer id) {
		this.id = id;
	}

	public String getnProId() {
		return nProId;
	}

	public void setnProId(String nProId) {
		this.nProId = nProId;
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

	public String getCustName() {
		return custName;
	}

	public void setCustName(String custName) {
		this.custName = custName;
	}

	public String getStrCreditPhone() {
		return strCreditPhone;
	}

	public void setStrCreditPhone(String strCreditPhone) {
		this.strCreditPhone = strCreditPhone;
	}

	public Integer getCreditId() {
		return creditId;
	}

	public void setCreditId(Integer creditId) {
		this.creditId = creditId;
	}

	public String getCreditCode() {
		return creditCode;
	}

	public void setCreditCode(String creditCode) {
		this.creditCode = creditCode;
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

	public String getProductCode() {
		return productCode;
	}

	public void setProductCode(String productCode) {
		this.productCode = productCode;
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

	public String getStrGrade() {
		return strGrade;
	}

	public void setStrGrade(String strGrade) {
		this.strGrade = strGrade;
	}

	public Integer getVideoNum() {
		return videoNum;
	}

	public void setVideoNum(Integer videoNum) {
		this.videoNum = videoNum;
	}

	public String getBusinessTime() {
		return businessTime;
	}

	public void setBusinessTime(String businessTime) {
		this.businessTime = businessTime;
	}

	public String getStrlockClient() {
		return strlockClient;
	}

	public void setStrlockClient(String strlockClient) {
		this.strlockClient = strlockClient;
	}

	public String getStrRecordStatus() {
		return strRecordStatus;
	}

	public void setStrRecordStatus(String strRecordStatus) {
		this.strRecordStatus = strRecordStatus;
	}

	public List<VideoDetailInfoVO> getRecordsDetailInfos() {
		return recordsDetailInfos;
	}

	public void setRecordsDetailInfos(List<VideoDetailInfoVO> recordsDetailInfos) {
		this.recordsDetailInfos = recordsDetailInfos;
	}

	public String getInputRegionCode() {
		return inputRegionCode;
	}

	public void setInputRegionCode(String inputRegionCode) {
		this.inputRegionCode = inputRegionCode;
	}

	public Integer getRecordStatus() {
		return recordStatus;
	}

	public void setRecordStatus(Integer recordStatus) {
		this.recordStatus = recordStatus;
	}

	public Integer getLockClient() {
		return lockClient;
	}

	public void setLockClient(Integer lockClient) {
		this.lockClient = lockClient;
	}

	public String getPictureStrart() {
		return pictureStrart;
	}

	public void setPictureStrart(String pictureStrart) {
		this.pictureStrart = pictureStrart;
	}

	public String getPictureEnd() {
		return pictureEnd;
	}

	public void setPictureEnd(String pictureEnd) {
		this.pictureEnd = pictureEnd;
	}

	public String getCreditName() {
		return creditName;
	}

	public void setCreditName(String creditName) {
		this.creditName = creditName;
	}
}