/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月9日 上午10:42:28
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.device.dto;

import java.util.List;

/**
 * <p>
 * </p>
 * @author jinxindong 2016年3月9日 上午10:42:28
 * @version V1.0
 */
public class DeviceInfo {
	
	private Integer id;
	private Integer ntype;
	private String strName;
	private String strIp;
	private Integer nport;
	private String strUser;
	private String strPassword;
	private String strPasswordLevel;
	private Integer ndevType;
	private Integer nproductType;
	private Integer nchanNum;
	private String strDeviceId;
	private Integer telChanNum;
	private Integer nregionId;
	private Integer nstreamType;
	private Integer dvrType;
	private Integer nconfigPort;
	private String strConfigUser;
	private String strConfigPassword;
	private Integer online;
	private Integer uiTotalSpace;
	private Integer uiFreeSpace;
	private String strIndexCode;
	private String strManufacture;
	private String regionName;// 所属区域名称
	private Integer ndevStoId;// 关联存储设备id
	private String ndevStoName;// 关联存储设备名称
	private String groupIndex;// 关联设备的分卷信息
	private Integer groupIndexId;// 存储池的poolId
	private List<DeviceChannelInfo> deviceChannelList;
	private Integer isFlag;// 是否查询子孙节点
	private String onlineName;// 在线状态名字
	private String strPasswordLevelName;// 密码等级名字
	private String ntypeName;// 设备类型名字
	private Integer storageRelate;// 导入时 是否获取信道 0未 1有
	private Integer collectRelate;// 导入时 是否关联存储设备 0未 1有
	private Integer flag;// 标记存储池传空时 是否更改原有存储池信息 0不需要(没有点重新获取) 1需要
	private Integer rflag;// 标记信道传空时 是否更改原有信道信息 0不需要(没有点重新获取) 1需要
	private Integer ntpEnable;//是否启用NTP
	private String accessKey;//云存储-对象存储需要的key
	private String secretKey;//云存储-对象存储需要的key
	
	public Integer getId() {
		return id;
	}
	
	public void setId(Integer id) {
		this.id = id;
	}
	
	public Integer getNtype() {
		return ntype;
	}
	
	public void setNtype(Integer ntype) {
		this.ntype = ntype;
	}
	
	public String getStrName() {
		return strName;
	}
	
	public void setStrName(String strName) {
		this.strName = strName;
	}
	
	public String getStrIp() {
		return strIp;
	}
	
	public void setStrIp(String strIp) {
		this.strIp = strIp;
	}
	
	public Integer getNport() {
		return nport;
	}
	
	public void setNport(Integer nport) {
		this.nport = nport;
	}
	
	public String getStrUser() {
		return strUser;
	}
	
	public void setStrUser(String strUser) {
		this.strUser = strUser;
	}
	
	public String getStrPassword() {
		return strPassword;
	}
	
	public void setStrPassword(String strPassword) {
		this.strPassword = strPassword;
	}
	
	public String getStrPasswordLevel() {
		return strPasswordLevel;
	}
	
	public void setStrPasswordLevel(String strPasswordLevel) {
		this.strPasswordLevel = strPasswordLevel;
	}
	
	public Integer getNdevType() {
		return ndevType;
	}
	
	public void setNdevType(Integer ndevType) {
		this.ndevType = ndevType;
	}
	
	public Integer getNproductType() {
		return nproductType;
	}
	
	public void setNproductType(Integer nproductType) {
		this.nproductType = nproductType;
	}
	
	public Integer getNchanNum() {
		return nchanNum;
	}
	
	public void setNchanNum(Integer nchanNum) {
		this.nchanNum = nchanNum;
	}
	
	public String getStrDeviceId() {
		return strDeviceId;
	}
	
	public void setStrDeviceId(String strDeviceId) {
		this.strDeviceId = strDeviceId;
	}
	
	public Integer getTelChanNum() {
		return telChanNum;
	}
	
	public void setTelChanNum(Integer telChanNum) {
		this.telChanNum = telChanNum;
	}
	
	public Integer getNregionId() {
		return nregionId;
	}
	
	public void setNregionId(Integer nregionId) {
		this.nregionId = nregionId;
	}
	
	public Integer getNstreamType() {
		return nstreamType;
	}
	
	public void setNstreamType(Integer nstreamType) {
		this.nstreamType = nstreamType;
	}
	
	public Integer getDvrType() {
		return dvrType;
	}
	
	public void setDvrType(Integer dvrType) {
		this.dvrType = dvrType;
	}
	
	public Integer getNconfigPort() {
		return nconfigPort;
	}
	
	public void setNconfigPort(Integer nconfigPort) {
		this.nconfigPort = nconfigPort;
	}
	
	public String getStrConfigUser() {
		return strConfigUser;
	}
	
	public void setStrConfigUser(String strConfigUser) {
		this.strConfigUser = strConfigUser;
	}
	
	public String getStrConfigPassword() {
		return strConfigPassword;
	}
	
	public void setStrConfigPassword(String strConfigPassword) {
		this.strConfigPassword = strConfigPassword;
	}
	
	public Integer getOnline() {
		return online;
	}
	
	public void setOnline(Integer online) {
		this.online = online;
	}
	
	public Integer getUiTotalSpace() {
		return uiTotalSpace;
	}
	
	public void setUiTotalSpace(Integer uiTotalSpace) {
		this.uiTotalSpace = uiTotalSpace;
	}
	
	public Integer getUiFreeSpace() {
		return uiFreeSpace;
	}
	
	public void setUiFreeSpace(Integer uiFreeSpace) {
		this.uiFreeSpace = uiFreeSpace;
	}
	
	public String getStrIndexCode() {
		return strIndexCode;
	}
	
	public void setStrIndexCode(String strIndexCode) {
		this.strIndexCode = strIndexCode;
	}
	
	public String getStrManufacture() {
		return strManufacture;
	}
	
	public void setStrManufacture(String strManufacture) {
		this.strManufacture = strManufacture;
	}
	
	public String getRegionName() {
		return regionName;
	}
	
	public void setRegionName(String regionName) {
		this.regionName = regionName;
	}
	
	public Integer getNdevStoId() {
		return ndevStoId;
	}
	
	public void setNdevStoId(Integer ndevStoId) {
		this.ndevStoId = ndevStoId;
	}
	
	public String getGroupIndex() {
		return groupIndex;
	}
	
	public void setGroupIndex(String groupIndex) {
		this.groupIndex = groupIndex;
	}
	
	public List<DeviceChannelInfo> getDeviceChannelList() {
		return deviceChannelList;
	}
	
	public void setDeviceChannelList(List<DeviceChannelInfo> deviceChannelList) {
		this.deviceChannelList = deviceChannelList;
	}
	
	public String getNdevStoName() {
		return ndevStoName;
	}
	
	public void setNdevStoName(String ndevStoName) {
		this.ndevStoName = ndevStoName;
	}
	
	public Integer getIsFlag() {
		return isFlag;
	}
	
	public void setIsFlag(Integer isFlag) {
		this.isFlag = isFlag;
	}
	
	public String getOnlineName() {
		return onlineName;
	}
	
	public void setOnlineName(String onlineName) {
		this.onlineName = onlineName;
	}
	
	public String getStrPasswordLevelName() {
		return strPasswordLevelName;
	}
	
	public void setStrPasswordLevelName(String strPasswordLevelName) {
		this.strPasswordLevelName = strPasswordLevelName;
	}
	
	public String getNtypeName() {
		return ntypeName;
	}
	
	public void setNtypeName(String ntypeName) {
		this.ntypeName = ntypeName;
	}
	
	public Integer getGroupIndexId() {
		return groupIndexId;
	}
	
	public void setGroupIndexId(Integer groupIndexId) {
		this.groupIndexId = groupIndexId;
	}
	
	public Integer getStorageRelate() {
		return storageRelate;
	}
	
	public void setStorageRelate(Integer storageRelate) {
		this.storageRelate = storageRelate;
	}
	
	public Integer getCollectRelate() {
		return collectRelate;
	}
	
	public void setCollectRelate(Integer collectRelate) {
		this.collectRelate = collectRelate;
	}
	
	public Integer getFlag() {
		return flag;
	}
	
	public void setFlag(Integer flag) {
		this.flag = flag;
	}
	
	public Integer getRflag() {
		return rflag;
	}
	
	public void setRflag(Integer rflag) {
		this.rflag = rflag;
	}
	
	public Integer getNtpEnable() {
		return ntpEnable;
	}
	
	public void setNtpEnable(Integer ntpEnable) {
		this.ntpEnable = ntpEnable;
	}
	
	public String getAccessKey() {
		return accessKey;
	}
	public void setAccessKey(String accessKey) {
		this.accessKey = accessKey;
	}
	
	public String getSecretKey() {
		return secretKey;
	}
	public void setSecretKey(String secretKey) {
		this.secretKey = secretKey;
	}
	
}
