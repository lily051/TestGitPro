/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月5日 下午4:34:33
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.device.dto;


/**
 * <p></p>
 * @author jinxindong 2016年4月5日 下午4:34:33
 * @version V1.0 
 */
public class CloudPoolInfo {
	private Integer id;
	private Integer ndeviceId;
	private Integer poolId;
	private String poolName;
	private Integer poolType;
	private Integer poolSize;
	private Integer poolFreeSize;
	private Integer checkedPoolId;//已选分券
	
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
	
    public Integer getPoolId() {
    	return poolId;
    }
	
    public void setPoolId(Integer poolId) {
    	this.poolId = poolId;
    }
	
    public String getPoolName() {
    	return poolName;
    }
	
    public void setPoolName(String poolName) {
    	this.poolName = poolName;
    }
	
    public Integer getPoolType() {
    	return poolType;
    }
	
    public void setPoolType(Integer poolType) {
    	this.poolType = poolType;
    }
	
    public Integer getPoolSize() {
    	return poolSize;
    }
	
    public void setPoolSize(Integer poolSize) {
    	this.poolSize = poolSize;
    }
	
    public Integer getPoolFreeSize() {
    	return poolFreeSize;
    }
	
    public void setPoolFreeSize(Integer poolFreeSize) {
    	this.poolFreeSize = poolFreeSize;
    }

	
    public Integer getCheckedPoolId() {
    	return checkedPoolId;
    }

	
    public void setCheckedPoolId(Integer checkedPoolId) {
    	this.checkedPoolId = checkedPoolId;
    }
	
	
}
