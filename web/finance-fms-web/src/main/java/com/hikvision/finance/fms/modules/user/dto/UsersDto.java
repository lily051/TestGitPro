/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月3日 上午10:57:09
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.user.dto;

import java.util.List;

import com.hikvision.finance.fms.model.Usercrtificatemapping;
import com.hikvision.finance.fms.model.Userregionmapping;
import com.hikvision.finance.fms.model.Userrolemapping;
import com.hikvision.finance.fms.modules.region.dto.RegionInfo;

/**
 * <p>
 * </p>
 * @author jinxindong 2016年3月3日 上午10:57:09
 * @version V1.0
 */
public class UsersDto {
	
	private UserInfo userInfos;
	private List<Userrolemapping> roleList;
	private List<UserdevicemappingInfo> deviceList;
	private List<Usercrtificatemapping> crtificateList;
	private List<Userregionmapping> regionList;
	private List<RegionInfo> regionIdList;
	
    public UserInfo getUserInfos() {
    	return userInfos;
    }
	
    public void setUserInfos(UserInfo userInfos) {
    	this.userInfos = userInfos;
    }
	
    public List<Userrolemapping> getRoleList() {
    	return roleList;
    }
	
    public void setRoleList(List<Userrolemapping> roleList) {
    	this.roleList = roleList;
    }
	
    public List<UserdevicemappingInfo> getDeviceList() {
    	return deviceList;
    }
	
    public void setDeviceList(List<UserdevicemappingInfo> deviceList) {
    	this.deviceList = deviceList;
    }
	
    public List<Usercrtificatemapping> getCrtificateList() {
    	return crtificateList;
    }
	
    public void setCrtificateList(List<Usercrtificatemapping> crtificateList) {
    	this.crtificateList = crtificateList;
    }
	
    public List<Userregionmapping> getRegionList() {
    	return regionList;
    }
	
    public void setRegionList(List<Userregionmapping> regionList) {
    	this.regionList = regionList;
    }
	
    public List<RegionInfo> getRegionIdList() {
    	return regionIdList;
    }
	
    public void setRegionIdList(List<RegionInfo> regionIdList) {
    	this.regionIdList = regionIdList;
    }
	

	
}
