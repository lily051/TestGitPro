/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月30日 下午7:16:18
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.sysconfig.dto;


/**
 * <p>安全策略包装类</p>
 * @author jinxindong 2016年3月30日 下午7:16:18
 * @version V1.0 
 */
public class PasswordParamInfo {
	
	private String pwdFreshDays;// 密码保鲜天数
	private String pwdModfiyDays; // 密码修改提示天数
	private String defaultPassword; // 平台初始密码
	private String passwordSecruityLevel; // 密码安全等级
	private String loginTimes;// 登录次数
	private String lockTime;// 登录锁定时间
	
	public String getPwdFreshDays() {
		return pwdFreshDays;
	}
	
	public void setPwdFreshDays(String pwdFreshDays) {
		this.pwdFreshDays = pwdFreshDays;
	}
	
	public String getPwdModfiyDays() {
		return pwdModfiyDays;
	}
	
	public void setPwdModfiyDays(String pwdModfiyDays) {
		this.pwdModfiyDays = pwdModfiyDays;
	}
	
	public String getDefaultPassword() {
		return defaultPassword;
	}
	
	public void setDefaultPassword(String defaultPassword) {
		this.defaultPassword = defaultPassword;
	}
	
	public String getPasswordSecruityLevel() {
		return passwordSecruityLevel;
	}
	
	public void setPasswordSecruityLevel(String passwordSecruityLevel) {
		this.passwordSecruityLevel = passwordSecruityLevel;
	}
	
	public String getLoginTimes() {
		return loginTimes;
	}
	
	public void setLoginTimes(String loginTimes) {
		this.loginTimes = loginTimes;
	}
	
	public String getLockTime() {
		return lockTime;
	}
	
	public void setLockTime(String lockTime) {
		this.lockTime = lockTime;
	}
	
}
