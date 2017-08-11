/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月15日 下午6:56:48
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.record.dto;


/**
 * <p>用于控件获取回放信息(RecordInfo之外的字段)</p>
 * @author jinchenwu 2016年4月15日 下午6:56:48
 * @version V1.0 
 */
public class PlaybackInfo {
	
	private Integer type;// 设备类型
	private String ip;// CVR的IP
	private Integer port;// CVR的端口
	private String user;// CVR的用户名
	private String password;// CVR的密码
	private String channelName;// 通道名称
	private String strAccessKey;//云存储-对象存储访问KEY
	private String strSecretKey;//云存储-对象存储加密KEY
	
	public String getPassword() {
		return password;
	}
	
	public void setPassword(String password) {
		this.password = password;
	}
	
	public String getIp() {
		return ip;
	}
	
	public void setIp(String ip) {
		this.ip = ip;
	}
	
	public Integer getPort() {
		return port;
	}
	
	public void setPort(Integer port) {
		this.port = port;
	}
	
	public String getUser() {
		return user;
	}
	
	public void setUser(String user) {
		this.user = user;
	}
	
	public Integer getType() {
		return type;
	}
	
	public void setType(Integer type) {
		this.type = type;
	}

	
    public String getChannelName() {
    	return channelName;
    }

	
    public void setChannelName(String channelName) {
    	this.channelName = channelName;
    }
    
    public String getStrAccessKey() {
		return strAccessKey;
	}
    public void setStrAccessKey(String strAccessKey) {
		this.strAccessKey = strAccessKey;
	}
    
    public String getStrSecretKey() {
		return strSecretKey;
	}
    public void setStrSecretKey(String strSecretKey) {
		this.strSecretKey = strSecretKey;
	}
	
}
