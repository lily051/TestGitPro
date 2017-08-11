/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月22日 下午2:16:33
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.common.model;

import java.util.List;

import com.hikvision.finance.fms.common.util.Constants.RoleType;
import com.hikvision.finance.fms.model.Role;
import com.hikvision.finance.fms.model.Users;

/**
 * 用户会话信息
 * @author jinxindong 2016年2月22日 下午2:16:33
 * @version V1.0
 */
public class UserSession {
	
	private Integer userId;
	private Integer roleId;
	private String userName;
	private String roleName;
	private Integer netLine;
	private String loginIp;
	private String loginTime;
	
	private boolean isAdmin; // 是否admin用户
	private boolean isSystemRole;// 是否系统管理员
	private Integer sysRole;// 存放理财经理还是中心管理员0中心1理财
	private Integer rootOrgId; // 权限组织根节点的ID
	private String rootOrgPath; // 权限组织根节点的编码
	private List<String> privileges; // 权限集合

	private Integer userRegionId;// 用户的区域ID
	private Integer deviceRegionId;// dvr设备区域ID
	private Integer deviceId;// dvr设备Id
	private String deviceIp;// dvr设备IP
	private Integer devicePort;// dvr设备端口
	private String deviceUserName;// dvr设备用户名
	private String devicePassword;// dvr设备密码
	private Integer streamType;// 码流类型
	private Integer deviceChannelId;// 设备通道ID
	private Integer deviceChannelNo;// 设备通道号
	private String devicechannelName;//通道名
	private String usbCamIndex;//USB摄像头index
	private String usbCamName;//USB摄像头name
	private String usbMicIndex;//USB麦克风index
	private String usbMicName;//USB麦克风name
	
	/**
	 * 创建一个新的实例UserSession
	 */
	public UserSession() {
		super();
	}
	
	/**
	 * 创建一个新的实例UserSession
	 * @param user 用户信息
	 * @param role 角色信息
	 */
	public UserSession(Users user, Role role) {
		this.userId = user.getId();
		this.roleId = role.getId();
		this.userName = user.getStrName();
		this.roleName = role.getStrName();
		this.isAdmin = user.getStrName().equals("admin");
		this.isSystemRole = role.getNtype() == RoleType.SYSTEM_MANAGER;
		this.sysRole = user.getSysRole();
	}
	public UserSession(Users user) {
		this.userId = user.getId();
		this.userName = user.getStrName();
		this.sysRole = user.getSysRole();
	}

	public Integer getUserId() {
		return userId;
	}
	
	public void setUserId(Integer userId) {
		this.userId = userId;
	}
	
	public Integer getRoleId() {
		return roleId;
	}
	
	public void setRoleId(Integer roleId) {
		this.roleId = roleId;
	}
	
	public String getUserName() {
		return userName;
	}
	
	public void setUserName(String userName) {
		this.userName = userName;
	}
	
	public String getRoleName() {
		return roleName;
	}
	
	public void setRoleName(String roleName) {
		this.roleName = roleName;
	}
	
	public Integer getNetLine() {
		return netLine;
	}
	
	public void setNetLine(Integer netLine) {
		this.netLine = netLine;
	}
	
	public String getLoginIp() {
		return loginIp;
	}
	
	public void setLoginIp(String loginIp) {
		this.loginIp = loginIp;
	}
	
	public String getLoginTime() {
		return loginTime;
	}
	
	public void setLoginTime(String loginTime) {
		this.loginTime = loginTime;
	}
	
	public boolean isAdmin() {
		return isAdmin;
	}
	
	public void setAdmin(boolean isAdmin) {
		this.isAdmin = isAdmin;
	}
	
	public boolean isSystemRole() {
		return isSystemRole;
	}
	
	public void setSystemRole(boolean isSystemRole) {
		this.isSystemRole = isSystemRole;
	}
	
	public Integer getRootOrgId() {
		return rootOrgId;
	}
	
	public void setRootOrgId(Integer rootOrgId) {
		this.rootOrgId = rootOrgId;
	}
	
	public String getRootOrgPath() {
		return rootOrgPath;
	}
	
	public void setRootOrgPath(String rootOrgPath) {
		this.rootOrgPath = rootOrgPath;
	}
	
	public List<String> getPrivileges() {
		return privileges;
	}
	
	public void setPrivileges(List<String> privileges) {
		this.privileges = privileges;
	}
	
	public String getDeviceIp() {
		return deviceIp;
	}
	
	public void setDeviceIp(String deviceIp) {
		this.deviceIp = deviceIp;
	}
	
	public Integer getDevicePort() {
		return devicePort;
	}
	
	public void setDevicePort(Integer devicePort) {
		this.devicePort = devicePort;
	}
	
	public String getDeviceUserName() {
		return deviceUserName;
	}
	
	public void setDeviceUserName(String deviceUserName) {
		this.deviceUserName = deviceUserName;
	}
	
	public String getDevicePassword() {
		return devicePassword;
	}
	
	public void setDevicePassword(String devicePassword) {
		this.devicePassword = devicePassword;
	}
	
	public Integer getDeviceChannelNo() {
		return deviceChannelNo;
	}
	
	public void setDeviceChannelNo(Integer deviceChannelNo) {
		this.deviceChannelNo = deviceChannelNo;
	}
	
	public String getDevicechannelName() {
		return devicechannelName;
	}
	
	public void setDevicechannelName(String devicechannelName) {
		this.devicechannelName = devicechannelName;
	}
	
	public Integer getStreamType() {
		return streamType;
	}
	
	public void setStreamType(Integer streamType) {
		this.streamType = streamType;
	}

	public String getUsbCamIndex() {
		return usbCamIndex;
	}

	public void setUsbCamIndex(String usbCamIndex) {
		this.usbCamIndex = usbCamIndex;
	}

	public String getUsbCamName() {
		return usbCamName;
	}

	public void setUsbCamName(String usbCamName) {
		this.usbCamName = usbCamName;
	}

	public String getUsbMicIndex() {
		return usbMicIndex;
	}

	public void setUsbMicIndex(String usbMicIndex) {
		this.usbMicIndex = usbMicIndex;
	}

	public String getUsbMicName() {
		return usbMicName;
	}

	public void setUsbMicName(String usbMicName) {
		this.usbMicName = usbMicName;
	}
	
	public Integer getDeviceRegionId() {
		return deviceRegionId;
	}
	
	public void setDeviceRegionId(Integer deviceRegionId) {
		this.deviceRegionId = deviceRegionId;
	}
	
	public Integer getDeviceId() {
		return deviceId;
	}
	
	public void setDeviceId(Integer deviceId) {
		this.deviceId = deviceId;
	}
	
	public Integer getUserRegionId() {
		return userRegionId;
	}
	
	public void setUserRegionId(Integer userRegionId) {
		this.userRegionId = userRegionId;
	}
	
	public Integer getSysRole() {
		return sysRole;
	}
	
	public void setSysRole(Integer sysRole) {
		this.sysRole = sysRole;
	}
	
	public Integer getDeviceChannelId() {
		return deviceChannelId;
	}
	
	public void setDeviceChannelId(Integer deviceChannelId) {
		this.deviceChannelId = deviceChannelId;
	}
	
}