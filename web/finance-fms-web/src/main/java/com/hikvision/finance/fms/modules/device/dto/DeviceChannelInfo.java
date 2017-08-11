/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月5日 下午2:44:57
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.device.dto;


/**
 * <p></p>
 * @author jinxindong 2016年4月5日 下午2:44:57
 * @version V1.0 
 */
public class DeviceChannelInfo {
	private Integer id;
	private Integer ndeviceId;
	private Integer nchannelNo;
	private String nchannelName;
	private Integer nflag;
	private Integer nregionId;
	private Integer recordPlane;
	private String regionName;
	private String ndeviceName;
	private Integer isFlag;//是否显示子机构
	private String userIp;//通道ip
	
    public Integer getId() {
    	return id;
    }
	
    public void setId(Integer id) {
    	this.id = id;
    }
	
    public Integer getNdeviceId() {
    	return ndeviceId;
    }
	
    public void setNdeviceId(Integer ndeviceId) {
    	this.ndeviceId = ndeviceId;
    }
	
    public Integer getNchannelNo() {
    	return nchannelNo;
    }
	
    public void setNchannelNo(Integer nchannelNo) {
    	this.nchannelNo = nchannelNo;
    }
	
    public String getNchannelName() {
    	return nchannelName;
    }
	
    public void setNchannelName(String nchannelName) {
    	this.nchannelName = nchannelName;
    }
	
    public Integer getNflag() {
    	return nflag;
    }
	
    public void setNflag(Integer nflag) {
    	this.nflag = nflag;
    }
	
    public Integer getNregionId() {
    	return nregionId;
    }
	
    public void setNregionId(Integer nregionId) {
    	this.nregionId = nregionId;
    }
	
    public Integer getRecordPlane() {
    	return recordPlane;
    }
	
    public void setRecordPlane(Integer recordPlane) {
    	this.recordPlane = recordPlane;
    }

	public Integer getIsFlag() {
	    return isFlag;
    }

	public void setIsFlag(Integer isFlag) {
	    this.isFlag = isFlag;
    }

	public String getNdeviceName() {
	    return ndeviceName;
    }

	public void setNdeviceName(String ndeviceName) {
	    this.ndeviceName = ndeviceName;
    }

	public String getRegionName() {
	    return regionName;
    }

	public void setRegionName(String regionName) {
	    this.regionName = regionName;
    }
	
	public String getUserIp() {
		return userIp;
	}
	
	public void setUserIp(String userIp) {
		this.userIp = userIp;
	}
	
}
