/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月20日 上午9:26:25
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.user.model;


/**
 * <p></p>
 * @author jinxindong 2016年4月20日 上午9:26:25
 * @version V1.0 
 */
public class RecordPojo {
	private Integer camType;//监控点类型，0-DVR，1-NVR，2-USB
	private Integer channelId;//通道表id
	private String camName;//监控点名称
	private Integer devType;//设备类型  海康
	private Integer regionId;//监控点所在区域id
	private String devIp;//设备ip
	private Integer devPort;//设备端口
	private String user;//设备用户名
	private String pwd;//设备密码
	private Integer channel;//通道编号
	private Integer realPlayType;//码流类型
	
    public Integer getCamType() {
    	return camType;
    }
	
    public void setCamType(Integer camType) {
    	this.camType = camType;
    }
	
    public Integer getChannelId() {
    	return channelId;
    }
	
    public void setChannelId(Integer channelId) {
    	this.channelId = channelId;
    }
	
    public String getCamName() {
    	return camName;
    }
	
    public void setCamName(String camName) {
    	this.camName = camName;
    }
	
    public Integer getDevType() {
    	return devType;
    }
	
    public void setDevType(Integer devType) {
    	this.devType = devType;
    }
	
    public Integer getRegionId() {
    	return regionId;
    }
	
    public void setRegionId(Integer regionId) {
    	this.regionId = regionId;
    }
	
    public String getDevIp() {
    	return devIp;
    }
	
    public void setDevIp(String devIp) {
    	this.devIp = devIp;
    }
	
    public Integer getDevPort() {
    	return devPort;
    }
	
    public void setDevPort(Integer devPort) {
    	this.devPort = devPort;
    }
	
    public String getUser() {
    	return user;
    }
	
    public void setUser(String user) {
    	this.user = user;
    }
	
    public String getPwd() {
    	return pwd;
    }
	
    public void setPwd(String pwd) {
    	this.pwd = pwd;
    }
	
    public Integer getChannel() {
    	return channel;
    }
	
    public void setChannel(Integer channel) {
    	this.channel = channel;
    }

	
    public Integer getRealPlayType() {
    	return realPlayType;
    }

	
    public void setRealPlayType(Integer realPlayType) {
    	this.realPlayType = realPlayType;
    }
	
	
	
}
