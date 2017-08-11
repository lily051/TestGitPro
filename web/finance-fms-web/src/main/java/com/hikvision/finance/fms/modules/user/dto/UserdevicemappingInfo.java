/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月17日 下午6:30:25
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.user.dto;


/**
 * <p></p>
 * @author jinxindong 2016年3月17日 下午6:30:25
 * @version V1.0 
 */
public class UserdevicemappingInfo {
	private Integer id;
	private Integer nuserId;
	private Integer devId;
	private Integer devType;
	private Integer devChannel;
	private Boolean status;
	
    public Integer getId() {
    	return id;
    }
	
    public void setId(Integer id) {
    	this.id = id;
    }
	
    public Integer getNuserId() {
    	return nuserId;
    }
	
    public void setNuserId(Integer nuserId) {
    	this.nuserId = nuserId;
    }
	
    public Integer getDevId() {
    	return devId;
    }
	
    public void setDevId(Integer devId) {
    	this.devId = devId;
    }
	
    public Integer getDevType() {
    	return devType;
    }
	
    public void setDevType(Integer devType) {
    	this.devType = devType;
    }
	
    public Integer getDevChannel() {
    	return devChannel;
    }
	
    public void setDevChannel(Integer devChannel) {
    	this.devChannel = devChannel;
    }
	
    public Boolean getStatus() {
    	return status;
    }
	
    public void setStatus(Boolean status) {
    	this.status = status;
    }
	
	
}
