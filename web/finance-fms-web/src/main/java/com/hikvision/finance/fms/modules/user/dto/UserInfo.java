/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月3日 上午11:10:28
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.user.dto;

import java.util.Date;
import java.util.List;

import org.apache.struts2.json.annotations.JSON;

import com.hikvision.finance.fms.model.Crtificate;
import com.hikvision.finance.fms.model.Userdevicemapping;


/**
 * <p></p>
 * @author jinxindong 2016年3月3日 上午11:10:28
 * @version V1.0 
 */
public class UserInfo {
	
	private Integer id;
	private String strName;
	private String strCode;
	private Integer sysRole;
	private String strPassword;
	private Integer npasswordLevel;
	private String strTel;
	private String strEmail;
	private String strComment;
	private Integer nstate;
	private Date strDate;
	private Integer nsex;
	private Integer netId;
	private String net;
	private String strDuty;
	private String userNo;
	private Date strUpdatePwdTime;
	private Integer nowner;
	private Integer nupdated;
	private Integer nonline;
	private String strCredentialNo;
	private Date lastTimeOnline;
	private Integer ncrtificateId;
	private String sysTypeName;// 用户类型名
	private String roleName;// 分配角色名
	private String roleId;// 角色Id
	private Integer regionId;// 编辑用户前台显示已选根节点的id
	private String regionName;// 编辑用户前台显示已选根节点的名称
	private List<Integer> certIds;// 编辑用户前台显示已选证书的id
	private List<Crtificate> certName;// 编辑用户前台显示已选证书的名称
	private List<Userdevicemapping> channelNos;// 编辑用户前台显示已选信道的信道号
	private Boolean isOneself;
	private Integer userChannel;// 用户是否关联信道 1关联 0未关联
	private Integer authRelate;// 用户是否关联权限 1关联 0未关联
	private String nstateName;// 禁用启用
	
	public String getNstateName() {
		return nstateName;
	}
	
	public void setNstateName(String nstateName) {
		this.nstateName = nstateName;
	}
	
	public Integer getAuthRelate() {
		return authRelate;
	}
	
	public void setAuthRelate(Integer authRelate) {
		this.authRelate = authRelate;
	}
	
	public Integer getId() {
		return id;
	}
	
	public void setId(Integer id) {
		this.id = id;
	}
	
	public String getStrName() {
		return strName;
	}
	
	public void setStrName(String strName) {
		this.strName = strName;
	}
	
	public String getStrCode() {
		return strCode;
	}
	
	public void setStrCode(String strCode) {
		this.strCode = strCode;
	}
	
	public Integer getSysRole() {
		return sysRole;
	}
	
	public void setSysRole(Integer sysRole) {
		this.sysRole = sysRole;
	}
	
	public String getStrPassword() {
		return strPassword;
	}
	
	public void setStrPassword(String strPassword) {
		this.strPassword = strPassword;
	}
	
	public Integer getNpasswordLevel() {
		return npasswordLevel;
	}
	
	public void setNpasswordLevel(Integer npasswordLevel) {
		this.npasswordLevel = npasswordLevel;
	}
	
	public String getStrTel() {
		return strTel;
	}
	
	public void setStrTel(String strTel) {
		this.strTel = strTel;
	}
	
	public String getStrEmail() {
		return strEmail;
	}
	
	public void setStrEmail(String strEmail) {
		this.strEmail = strEmail;
	}
	
	public String getStrComment() {
		return strComment;
	}
	
	public void setStrComment(String strComment) {
		this.strComment = strComment;
	}
	
	public Integer getNstate() {
		return nstate;
	}
	
	public void setNstate(Integer nstate) {
		this.nstate = nstate;
	}
	
	@JSON(format = "yyyy-MM-dd")
	public Date getStrDate() {
		return strDate;
	}
	
	public void setStrDate(Date strDate) {
		this.strDate = strDate;
	}
	
	public Integer getNsex() {
		return nsex;
	}
	
	public void setNsex(Integer nsex) {
		this.nsex = nsex;
	}
	
	public String getNet() {
		return net;
	}
	
	public void setNet(String net) {
		this.net = net;
	}
	
	public String getStrDuty() {
		return strDuty;
	}
	
	public void setStrDuty(String strDuty) {
		this.strDuty = strDuty;
	}
	
	public String getUserNo() {
		return userNo;
	}
	
	public void setUserNo(String userNo) {
		this.userNo = userNo;
	}
	
	public Date getStrUpdatePwdTime() {
		return strUpdatePwdTime;
	}
	
	public void setStrUpdatePwdTime(Date strUpdatePwdTime) {
		this.strUpdatePwdTime = strUpdatePwdTime;
	}
	
	public Integer getNowner() {
		return nowner;
	}
	
	public void setNowner(Integer nowner) {
		this.nowner = nowner;
	}
	
	public Integer getNupdated() {
		return nupdated;
	}
	
	public void setNupdated(Integer nupdated) {
		this.nupdated = nupdated;
	}
	
	public Integer getNonline() {
		return nonline;
	}
	
	public void setNonline(Integer nonline) {
		this.nonline = nonline;
	}
	
	public String getStrCredentialNo() {
		return strCredentialNo;
	}
	
	public void setStrCredentialNo(String strCredentialNo) {
		this.strCredentialNo = strCredentialNo;
	}
	
	public Date getLastTimeOnline() {
		return lastTimeOnline;
	}
	
	public void setLastTimeOnline(Date lastTimeOnline) {
		this.lastTimeOnline = lastTimeOnline;
	}
	
	public Integer getNcrtificateId() {
		return ncrtificateId;
	}
	
	public void setNcrtificateId(Integer ncrtificateId) {
		this.ncrtificateId = ncrtificateId;
	}
	
	public String getSysTypeName() {
		return sysTypeName;
	}
	
	public void setSysTypeName(String sysTypeName) {
		this.sysTypeName = sysTypeName;
	}
	
	public String getRoleName() {
		return roleName;
	}
	
	public void setRoleName(String roleName) {
		this.roleName = roleName;
	}
	
	public String getRoleId() {
		return roleId;
	}
	
	public void setRoleId(String roleId) {
		this.roleId = roleId;
	}
	
	public Integer getRegionId() {
		return regionId;
	}
	
	public void setRegionId(Integer regionId) {
		this.regionId = regionId;
	}
	
	public String getRegionName() {
		return regionName;
	}
	
	public void setRegionName(String regionName) {
		this.regionName = regionName;
	}
	
	public List<Integer> getCertIds() {
		return certIds;
	}
	
	public void setCertIds(List<Integer> certIds) {
		this.certIds = certIds;
	}
	
	public List<Crtificate> getCertName() {
		return certName;
	}
	
	public void setCertName(List<Crtificate> certName) {
		this.certName = certName;
	}
	
	public List<Userdevicemapping> getChannelNos() {
		return channelNos;
	}
	
	public void setChannelNos(List<Userdevicemapping> channelNos) {
		this.channelNos = channelNos;
	}
	
	public Boolean getIsOneself() {
		return isOneself;
	}
	
	public void setIsOneself(Boolean isOneself) {
		this.isOneself = isOneself;
	}
	
	public Integer getUserChannel() {
		return userChannel;
	}
	
	public void setUserChannel(Integer userChannel) {
		this.userChannel = userChannel;
	}
	
	public Integer getNetId() {
		return netId;
	}
	
	public void setNetId(Integer netId) {
		this.netId = netId;
	}
	
}
