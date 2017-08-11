/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月9日 上午10:21:12
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.device.qo;

import java.util.List;

import com.hikvision.finance.extend.hibernate.qo.BaseQo;


/**
 * <p></p>
 * @author jinxindong 2016年3月9日 上午10:21:12
 * @version V1.0 
 */
public class DeviceQo extends BaseQo{
	/**
     * 序列化ID
     */
    private static final long serialVersionUID = -7865566392318459014L;
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
	private List<Integer> ntypes;
	
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

	public List<Integer> getNtypes() {
		return ntypes;
	}

	public void setNtypes(List<Integer> ntypes) {
		this.ntypes = ntypes;
	}
	
}
