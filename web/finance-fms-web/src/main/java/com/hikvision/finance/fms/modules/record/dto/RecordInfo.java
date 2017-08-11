/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月6日 下午4:06:06
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.record.dto;

import java.math.BigInteger;
import java.util.Date;
import java.util.List;

import org.apache.struts2.json.annotations.JSON;

/**
 * <p>
 * 用于web交互
 * </p>
 * @author jinchenwu 2016年4月6日 下午4:06:06
 * @version V1.0
 */
public class RecordInfo {
	
	// prosaverecord
	private Integer videoId;// 录像ID
	private Integer regionId;// 区域ID
	private String streamNo;// 通道在CVR中的唯一标识
	private Integer stateType;// 录像类型，1：签约录像，2：投诉录像
	private String fileName;// 录像名称
	private Date startTime;// 录像开始时间
	private String startTimeString;// 录像开始时间
	private Date endTime;// 录像结束时间
	private Integer recordCheck;// 是否合规，0：未检查，1：不合规，2：合规，3：存在投诉，4：投诉关闭
	private Integer upload;// 是否已上传，0：未上传，1：已上传
	private String checkRemark;// 备注
	private Integer devId;// 存储设备的Id
	private Integer cdevId;// 采集设备的Id
	private Integer channel;// 通道号
	private String rtspPath;// KMS路径
	private Integer nFileSize;// 录像大小
	private Integer isUpdateKmsTime;// 是否从KMS上更新录像的开始结束时间
	private String strBucket;//云存储-对象存储Bucket
	private String strUrl; //录像本地路径
	private String screenshotSrc; //截图路径 
	
	// clientinput
	private Integer busiId;// 业务单ID
	private Integer inputRegionId;// 交易网点Id
	private String inputRegionCode;// 交易网点 
	private String inputRegionName;// 交易网点名称
	private Integer status;// 业务单状态，0：没有纠纷，1：有纠纷
	private Integer productId; //产品ID
	private String productCode;// 产品代码
	private String productName;// 产品名称
	private Integer productTypeId; // 产品类型
	private String productTypeName; // 产品类型名称
	private String nproId;// 业务单号
	private String proUserNo;// 业务员工号
	private String proName;// 业务员姓名
	private Integer creditId; // 客户证件类型id
	private String creditCode;// 客户证件号
	private String name;// 客户姓名
	private String strCreditPhone;// 客户电话
	private Date businessTime;// 办理时间
	private Date strVideoEndDate;// 录像到期时间
	private String strBuyMoney;// 购买金额
	private String strBankNo;// 客户银行卡号
	private Integer lockClient;//业务单是否锁定
	private Integer inputUser; //用户id
	private String strGrade; // 产品发行机构
	
	// 其他
	private List<Integer> busiIds;// 若干业务单ID的字符串
	private Integer delayDays;// 录像保存期限延长天数
	private Integer isFlag;// 是否显示子孙区域节点的理财录像信息，0：不显示，1：显示
	private String strRegionIds;// 过滤的区域ID
	private String recordLength;// 录像时长
	private List<Integer> lockClients;// 是否锁定  1通过  2退回  0未审核
	private BigInteger videoCount;// 视频数量
	private Integer isCanApproval;// 业务单是否能够审批  0不能审批    1能够审批
	// 对应的str类型
	private String strRecordCheck;
	private String strUpload;
	private String strStatus;
	private Integer pageLenth;
	
	public Integer getVideoId() {
		return videoId;
	}
	
	public void setVideoId(Integer videoId) {
		this.videoId = videoId;
	}
	
	public Integer getRegionId() {
		return regionId;
	}
	
	public void setRegionId(Integer regionId) {
		this.regionId = regionId;
	}
	
	public String getFileName() {
		return fileName;
	}
	
	public void setFileName(String fileName) {
		this.fileName = fileName;
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
	
	public Integer getRecordCheck() {
		return recordCheck;
	}
	
	public void setRecordCheck(Integer recordCheck) {
		this.recordCheck = recordCheck;
	}
	
	public Integer getStateType() {
		return stateType;
	}
	
	public void setStateType(Integer stateType) {
		this.stateType = stateType;
	}
	
	public Integer getUpload() {
		return upload;
	}
	
	public void setUpload(Integer upload) {
		this.upload = upload;
	}
	
	public String getCheckRemark() {
		return checkRemark;
	}
	
	public void setCheckRemark(String checkRemark) {
		this.checkRemark = checkRemark;
	}
	
	public Integer getBusiId() {
		return busiId;
	}
	
	public void setBusiId(Integer busiId) {
		this.busiId = busiId;
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
	
	public String getNproId() {
		return nproId;
	}
	
	public void setNproId(String nproId) {
		this.nproId = nproId;
	}
	
	public String getProUserNo() {
		return proUserNo;
	}
	
	public void setProUserNo(String proUserNo) {
		this.proUserNo = proUserNo;
	}
	
	public String getProName() {
		return proName;
	}
	
	public void setProName(String proName) {
		this.proName = proName;
	}
	
	public String getName() {
		return name;
	}
	
	public void setName(String name) {
		this.name = name;
	}
	
	@JSON(format = "yyyy-MM-dd")
	public Date getBusinessTime() {
		return businessTime;
	}
	
	public void setBusinessTime(Date businessTime) {
		this.businessTime = businessTime;
	}
	
	public Integer getStatus() {
		return status;
	}
	
	public void setStatus(Integer status) {
		this.status = status;
	}
	
	public Integer getDelayDays() {
		return delayDays;
	}
	
	public void setDelayDays(Integer delayDays) {
		this.delayDays = delayDays;
	}
	
	public Integer getIsFlag() {
		return isFlag;
	}
	
	public void setIsFlag(Integer isFlag) {
		this.isFlag = isFlag;
	}
	
	public String getStrRecordCheck() {
		return strRecordCheck;
	}
	
	public void setStrRecordCheck(String strRecordCheck) {
		this.strRecordCheck = strRecordCheck;
	}
	
	public String getStrUpload() {
		return strUpload;
	}
	
	public void setStrUpload(String strUpload) {
		this.strUpload = strUpload;
	}
	
	public String getStrStatus() {
		return strStatus;
	}
	
	public void setStrStatus(String strStatus) {
		this.strStatus = strStatus;
	}
	
	public String getStrBuyMoney() {
		return strBuyMoney;
	}
	
	public void setStrBuyMoney(String strBuyMoney) {
		this.strBuyMoney = strBuyMoney;
	}
	
	public String getStrRegionIds() {
		return strRegionIds;
	}
	
	public void setStrRegionIds(String strRegionIds) {
		this.strRegionIds = strRegionIds;
	}
	
	public Integer getChannel() {
		return channel;
	}
	
	public void setChannel(Integer channel) {
		this.channel = channel;
	}
	
	public Integer getDevId() {
		return devId;
	}
	
	public void setDevId(Integer devId) {
		this.devId = devId;
	}
	
	public String getRecordLength() {
		return recordLength;
	}
	
	public void setRecordLength(String recordLength) {
		this.recordLength = recordLength;
	}
	
	public String getStreamNo() {
		return streamNo;
	}
	
	public void setStreamNo(String streamNo) {
		this.streamNo = streamNo;
	}
	
	public String getRtspPath() {
		return rtspPath;
	}
	
	public void setRtspPath(String rtspPath) {
		this.rtspPath = rtspPath;
	}
	
	public String getInputRegionName() {
		return inputRegionName;
	}
	
	public void setInputRegionName(String inputRegionName) {
		this.inputRegionName = inputRegionName;
	}
	
	public String getStrBankNo() {
		return strBankNo;
	}
	
	public void setStrBankNo(String strBankNo) {
		this.strBankNo = strBankNo;
	}
	
	public Integer getInputRegionId() {
		return inputRegionId;
	}
	
	public void setInputRegionId(Integer inputRegionId) {
		this.inputRegionId = inputRegionId;
	}
	
	public List<Integer> getBusiIds() {
		return busiIds;
	}
	
	public void setBusiIds(List<Integer> busiIds) {
		this.busiIds = busiIds;
	}
	
	public Integer getnFileSize() {
		return nFileSize;
	}
	
	public void setnFileSize(Integer nFileSize) {
		this.nFileSize = nFileSize;
	}
	
	public String getStartTimeString() {
		return startTimeString;
	}
	
	public void setStartTimeString(String startTimeString) {
		this.startTimeString = startTimeString;
	}
	
	public Integer getCdevId() {
		return cdevId;
	}
	
	public void setCdevId(Integer cdevId) {
		this.cdevId = cdevId;
	}
	
	public Integer getPageLenth() {
		return pageLenth;
	}
	
	public void setPageLenth(Integer pageLenth) {
		this.pageLenth = pageLenth;
	}
	
	public Integer getIsUpdateKmsTime() {
		return isUpdateKmsTime;
	}
	
	public void setIsUpdateKmsTime(Integer isUpdateKmsTime) {
		this.isUpdateKmsTime = isUpdateKmsTime;
	}
	
	public String getStrBucket() {
		return strBucket;
	}
	
	public void setStrBucket(String strBucket) {
		this.strBucket = strBucket;
	}
	
	public String getCreditCode() {
		return creditCode;
	}
	
	public void setCreditCode(String creditCode) {
		this.creditCode = creditCode;
	}
	
	@JSON(format = "yyyy-MM-dd")
	public Date getStrVideoEndDate() {
		return strVideoEndDate;
	}
	
	public void setStrVideoEndDate(Date strVideoEndDate) {
		this.strVideoEndDate = strVideoEndDate;
	}
	
	public Integer getLockClient() {
		return lockClient;
	}
	public void setLockClient(Integer lockClient) {
		this.lockClient = lockClient;
	}

	public Integer getProductId() {
		return productId;
	}

	public void setProductId(Integer productId) {
		this.productId = productId;
	}

	public List<Integer> getLockClients() {
		return lockClients;
	}

	public void setLockClients(List<Integer> lockClients) {
		this.lockClients = lockClients;
	}

	public Integer getInputUser() {
		return inputUser;
	}

	public void setInputUser(Integer inputUser) {
		this.inputUser = inputUser;
	}

	public BigInteger getVideoCount() {
		return videoCount;
	}

	public void setVideoCount(BigInteger videoCount) {
		this.videoCount = videoCount;
	}

	public String getStrUrl() {
		return strUrl;
	}

	public void setStrUrl(String strUrl) {
		this.strUrl = strUrl;
	}
	
	public Integer getIsCanApproval() {
		return isCanApproval;
	}
	
	public void setIsCanApproval(Integer isCanApproval) {
		this.isCanApproval = isCanApproval;
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

	public String getStrGrade() {
		return strGrade;
	}

	public void setStrGrade(String strGrade) {
		this.strGrade = strGrade;
	}

	public String getInputRegionCode() {
		return inputRegionCode;
	}

	public void setInputRegionCode(String inputRegionCode) {
		this.inputRegionCode = inputRegionCode;
	}

	public String getScreenshotSrc() {
		return screenshotSrc;
	}

	public void setScreenshotSrc(String screenshotSrc) {
		this.screenshotSrc = screenshotSrc;
	}
	
}
