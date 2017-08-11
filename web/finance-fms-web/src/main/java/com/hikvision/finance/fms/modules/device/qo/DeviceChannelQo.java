/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月9日 上午10:32:34
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.device.qo;

import com.hikvision.finance.extend.hibernate.qo.BaseQo;


/**
 * <p></p>
 * @author jinxindong 2016年3月9日 上午10:32:34
 * @version V1.0 
 */
public class DeviceChannelQo extends BaseQo {
	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 1434807952543182556L;
	private Integer id;
	private Integer ndeviceId;
	private Integer nchannelNo;
	private String nchannelName;
	private Integer recordPlane;
	private Integer nregionId;
	private Integer nflag;
	
    public Integer getId() {
    	return id;
    }
	
    public void setId(Integer id) {
    	this.id = id;
    }
	
    public Integer getNdeviceId() {
    	return ndeviceId;
    }
	
    public void setNdeviceId(Integer ndeviceId) {
    	this.ndeviceId = ndeviceId;
    }
	
    public Integer getNchannelNo() {
    	return nchannelNo;
    }
	
    public void setNchannelNo(Integer nchannelNo) {
    	this.nchannelNo = nchannelNo;
    }
	
    public String getNchannelName() {
    	return nchannelName;
    }
	
    public void setNchannelName(String nchannelName) {
    	this.nchannelName = nchannelName;
    }
	
    public Integer getNflag() {
    	return nflag;
    }
	
    public void setNflag(Integer nflag) {
    	this.nflag = nflag;
    }

	
    public Integer getRecordPlane() {
    	return recordPlane;
    }

	
    public void setRecordPlane(Integer recordPlane) {
    	this.recordPlane = recordPlane;
    }

	
    public Integer getNregionId() {
    	return nregionId;
    }

	
    public void setNregionId(Integer nregionId) {
    	this.nregionId = nregionId;
    }
	
	
}
