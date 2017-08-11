/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月8日 下午6:57:59
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.device.dto;

/**
 * <p>
 * json转对象类
 * </p>
 * @author jinxindong 2016年4月8日 下午6:57:59
 * @version V1.0
 */
public class KmsInfo {
	
	private String node;// 操作ip
	private Integer ret;
	private String msg;// 操作备注
	private String token;// token值
	private String freeSpace;
	private String totalSpace;
	private String strBeginTime;
	private String strEndTime;
	
	public String getNode() {
		return node;
	}
	
	public void setNode(String node) {
		this.node = node;
	}
	
	public Integer getRet() {
		return ret;
	}
	
	public void setRet(Integer ret) {
		this.ret = ret;
	}
	
	public String getMsg() {
		return msg;
	}
	
	public void setMsg(String msg) {
		this.msg = msg;
	}
	
	public String getToken() {
		return token;
	}
	
	public void setToken(String token) {
		this.token = token;
	}
	
	public String getFreeSpace() {
		return freeSpace;
	}
	
	public void setFreeSpace(String freeSpace) {
		this.freeSpace = freeSpace;
	}
	
	public String getTotalSpace() {
		return totalSpace;
	}
	
	public void setTotalSpace(String totalSpace) {
		this.totalSpace = totalSpace;
	}
	
	public String getStrBeginTime() {
		return strBeginTime;
	}
	
	public void setStrBeginTime(String strBeginTime) {
		this.strBeginTime = strBeginTime;
	}
	
	public String getStrEndTime() {
		return strEndTime;
	}
	
	public void setStrEndTime(String strEndTime) {
		this.strEndTime = strEndTime;
	}
	
}
