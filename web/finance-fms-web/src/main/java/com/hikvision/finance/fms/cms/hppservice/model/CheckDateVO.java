/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年10月25日 下午6:49:48
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.cms.hppservice.model;

/**
 * <p>
 * </p>
 * 登录多次错误后, 显示剩余的时间
 * 
 * @author zhoujiajun 2016年10月25日 下午6:49:48
 * @version V1.0
 */
public class CheckDateVO {

	private Long leftMintes;
	private Boolean canLogin;
	
	public Long getLeftMintes() {
		return leftMintes;
	}
	public void setLeftMintes(Long leftMintes) {
		this.leftMintes = leftMintes;
	}
	public Boolean getCanLogin() {
		return canLogin;
	}
	public void setCanLogin(Boolean canLogin) {
		this.canLogin = canLogin;
	}

	

}
