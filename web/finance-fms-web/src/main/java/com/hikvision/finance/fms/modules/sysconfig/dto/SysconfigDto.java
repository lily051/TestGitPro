/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月9日 下午4:47:03
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.sysconfig.dto;

/**
 * <p>
 * </p>
 * 
 * @author jinxindong 2016年3月9日 下午4:47:03
 * @version V1.0
 */
public class SysconfigDto {
	private String passwordDefault; // 平台初始密码
	private String passwordSecruityLevel;// 密码安全等级
	private String passwordFresh; // 密码保鲜期，0表示不启用
	private String sysTitle; // 标题
	private String copyRight; // 版权描述
	private String imgLogo; // 图片logo
	private String imgLogin; // 图片登录
	private String imgBar; // 图片导航栏
	private String osdEnabled; // osd是否启用
	private String usbmodelinfo; // usb模式
	private String failCount; // 登陆错误最大次数
	private String distanceMinutes; // 账户锁定的分钟数

	public String getPasswordDefault() {
		return passwordDefault;
	}

	public void setPasswordDefault(String passwordDefault) {
		this.passwordDefault = passwordDefault;
	}

	public String getPasswordSecruityLevel() {
		return passwordSecruityLevel;
	}

	public void setPasswordSecruityLevel(String passwordSecruityLevel) {
		this.passwordSecruityLevel = passwordSecruityLevel;
	}

	public String getSysTitle() {
		return sysTitle;
	}

	public void setSysTitle(String sysTitle) {
		this.sysTitle = sysTitle;
	}

	public String getCopyRight() {
		return copyRight;
	}

	public void setCopyRight(String copyRight) {
		this.copyRight = copyRight;
	}

	public String getImgLogo() {
		return imgLogo;
	}

	public void setImgLogo(String imgLogo) {
		this.imgLogo = imgLogo;
	}

	public String getImgLogin() {
		return imgLogin;
	}

	public void setImgLogin(String imgLogin) {
		this.imgLogin = imgLogin;
	}

	public String getImgBar() {
		return imgBar;
	}

	public void setImgBar(String imgBar) {
		this.imgBar = imgBar;
	}

	public String getPasswordFresh() {
		return passwordFresh;
	}

	public void setPasswordFresh(String passwordFresh) {
		this.passwordFresh = passwordFresh;
	}

	public String getOsdEnabled() {
		return osdEnabled;
	}

	public void setOsdEnabled(String osdEnabled) {
		this.osdEnabled = osdEnabled;
	}

	public String getUsbmodelinfo() {
		return usbmodelinfo;
	}

	public void setUsbmodelinfo(String usbmodelinfo) {
		this.usbmodelinfo = usbmodelinfo;
	}

	public String getFailCount() {
		return failCount;
	}

	public void setFailCount(String failCount) {
		this.failCount = failCount;
	}

	public String getDistanceMinutes() {
		return distanceMinutes;
	}

	public void setDistanceMinutes(String distanceMinutes) {
		this.distanceMinutes = distanceMinutes;
	}

}
