/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月7日 下午6:57:46
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.sale.dto;

import java.util.Date;
import java.util.List;

import org.apache.struts2.json.annotations.JSON;

/**
 * <p>
 * </p>
 * 
 * @author jinxindong 2016年4月7日 下午6:57:46
 * @version V1.0
 */
public class SaleInfo {
	private Integer id;
	private String nproId;// 业务单号 *
	private String name;// 客户姓名*
	private Integer creditId;// 证件类型Id*
	private Integer status;// 业务单状态 签约，投诉等 0签约 1 投诉
	private Integer productType;// 这个字段暂时没有用
	private Integer productTypeId;// 产品类型id
	private Date inputTime;// 理财系统建单时间
	private Integer inputUser;// 理财系统建单用户
	private Integer inputRegionId;// 理财系统建单区域号
	private String inputRegionName;// 理财系统建单区域名称*
	private String inputRegionCode;// 理财系统建单区域代码
	private String proName;// 业务员姓名*
	private String proUserNo;// 业务员工号*
	private Date businessTime;// 业务时间 入单时间 重复*
	private String businessTimeString;// 业务时间 入单时间 重复*
	private Integer productId;// 产品id
	private String productName;// 产品名称*
	private String productCode;// 产品代码*
	private Integer productSaveType; // 保存期限类型 ，年月日
	private Integer productSaveMonth;// 产品到期后保存期限（视频保存时间）
	private String productSave;
	private Date strEndDate;// 产品到期日
	private Integer guaranteeType;// 保障期限类型，年月日
	private Integer guaranteeYears;// 保障期限
	private String guarantee;
	private Date strVideoEndDate;// 视频到期日
	private Integer delaydays;// 延期天数
	private Integer saveflag;// 保存标志 1 永久保存 0正常
	private Integer tipflag;// 删除提示标志0 正常 >1删除提示天数（最大提示天数7）
	private Date markDate;// 更新tipflag标志时间
	private String creditCode;// 证件号
	private String strGrade;// 产品发行机构
	private String strRisklevel;// 产品风险等级
	private String strBankNo;// 客户银行卡号
	private String strBuyMoney;// 购买金额*
	private String strRemarks;// 业务单备注
	private Integer nisUpload;// 是否上传完成
	private Integer nisPay;// 是否兑付 1是 0否 是否过期
	private String strCreditPhone;// 客户电话
	private String creditCodeName;// 证件类型名称(单个查询用)
	private String statusName;// 业务单状态名称
	private String productTypeName;// 产品类别名称
	private Date compTime;// 投诉时间
	private String compReason;// 投诉原因
	private String videoType;// signvideo, complainvideo前台约定,
								// 从那个页面跳转过来(获取投诉信息有用)
	private Integer videoId; // 录像的id(prosaverecord的id)

	private String businessTimeStart;// 业务时间查 开始查询
	private String businessTimeEnd;// 业务时间查 结束查询
	private Integer isFlag;// 是否显示子孙节点 1显示 0不显示 默认0

	private Integer lockClient;// 是否锁定 1通过 2退回 0未审核
	private String strLockClient;// 是否锁定，String类型
	private String screenshotSrc; // 截图路径

	private List<Integer> lockClients;// 是否锁定 1通过 2退回 0未审核

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

	public String getStatusName() {
		return statusName;
	}

	public void setStatusName(String statusName) {
		this.statusName = statusName;
	}

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

	public Integer getCreditId() {
		return creditId;
	}

	public void setCreditId(Integer creditId) {
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

	@JSON(format = "yyyy-MM-dd")
	public Date getInputTime() {
		return inputTime;
	}

	public void setInputTime(Date inputTime) {
		this.inputTime = inputTime;
	}

	public Integer getInputUser() {
		return inputUser;
	}

	public void setInputUser(Integer inputUser) {
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

	@JSON(format = "yyyy-MM-dd HH:mm:ss")
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

	public String getBusinessTimeStart() {
		return businessTimeStart;
	}

	public void setBusinessTimeStart(String businessTimeStart) {
		this.businessTimeStart = businessTimeStart;
	}

	public String getBusinessTimeEnd() {
		return businessTimeEnd;
	}

	public void setBusinessTimeEnd(String businessTimeEnd) {
		this.businessTimeEnd = businessTimeEnd;
	}

	public Integer getIsFlag() {
		return isFlag;
	}

	public void setIsFlag(Integer isFlag) {
		this.isFlag = isFlag;
	}

	public String getCreditCodeName() {
		return creditCodeName;
	}

	public void setCreditCodeName(String creditCodeName) {
		this.creditCodeName = creditCodeName;
	}

	public Integer getProductTypeId() {
		return productTypeId;
	}

	public void setProductTypeId(Integer productTypeId) {
		this.productTypeId = productTypeId;
	}

	public String getInputRegionCode() {
		return inputRegionCode;
	}

	public void setInputRegionCode(String inputRegionCode) {
		this.inputRegionCode = inputRegionCode;
	}

	public String getBusinessTimeString() {
		return businessTimeString;
	}

	public void setBusinessTimeString(String businessTimeString) {
		this.businessTimeString = businessTimeString;
	}

	public Integer getProductSaveType() {
		return productSaveType;
	}

	public void setProductSaveType(Integer productSaveType) {
		this.productSaveType = productSaveType;
	}

	public Integer getGuaranteeType() {
		return guaranteeType;
	}

	public void setGuaranteeType(Integer guaranteeType) {
		this.guaranteeType = guaranteeType;
	}

	public String getProductSave() {
		return productSave;
	}

	public void setProductSave(String productSave) {
		this.productSave = productSave;
	}

	public String getGuarantee() {
		return guarantee;
	}

	public void setGuarantee(String guarantee) {
		this.guarantee = guarantee;
	}

	public Integer getLockClient() {
		return lockClient;
	}

	public void setLockClient(Integer lockClient) {
		this.lockClient = lockClient;
	}

	public String getStrLockClient() {
		return strLockClient;
	}

	public void setStrLockClient(String strLockClient) {
		this.strLockClient = strLockClient;
	}

	public List<Integer> getLockClients() {
		return lockClients;
	}

	public void setLockClients(List<Integer> lockClients) {
		this.lockClients = lockClients;
	}

	public String getScreenshotSrc() {
		return screenshotSrc;
	}

	public void setScreenshotSrc(String screenshotSrc) {
		this.screenshotSrc = screenshotSrc;
	}

	public Date getCompTime() {
		return compTime;
	}

	public void setCompTime(Date compTime) {
		this.compTime = compTime;
	}

	public String getCompReason() {
		return compReason;
	}

	public void setCompReason(String compReason) {
		this.compReason = compReason;
	}

	public String getVideoType() {
		return videoType;
	}

	public void setVideoType(String videoType) {
		this.videoType = videoType;
	}

	public Integer getVideoId() {
		return videoId;
	}

	public void setVideoId(Integer videoId) {
		this.videoId = videoId;
	}

}
