/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月31日 下午2:42:52
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.sysconfig.dto;


/**
 * <p>平台参数信息包装类</p>
 * @author jinxindong 2016年3月31日 下午2:42:52
 * @version V1.0 
 */
public class PlatformParamInfo {
	private  String platformDiscribe;//平台描述文字
	private  String platformCopyright;//平台版权文字
	private  String imgBar;//导航栏logo
	private  String imgLogo;////登录页LOGO图标
	private  String imgLogin;//登录页图片
	
    public String getPlatformDiscribe() {
    	return platformDiscribe;
    }
	
    public void setPlatformDiscribe(String platformDiscribe) {
    	this.platformDiscribe = platformDiscribe;
    }
	
    public String getPlatformCopyright() {
    	return platformCopyright;
    }
	
    public void setPlatformCopyright(String platformCopyright) {
    	this.platformCopyright = platformCopyright;
    }
	
    public String getImgLogo() {
    	return imgLogo;
    }
	
    public void setImgLogo(String imgLogo) {
    	this.imgLogo = imgLogo;
    }
	
    public String getImgBar() {
    	return imgBar;
    }
	
    public void setImgBar(String imgBar) {
    	this.imgBar = imgBar;
    }
	
    public String getImgLogin() {
    	return imgLogin;
    }
	
    public void setImgLogin(String imgLogin) {
    	this.imgLogin = imgLogin;
    }

	
	
	
}
