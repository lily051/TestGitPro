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
 * 录像的详细信息(DC业务单需求)
 * 
 * @author zhoujiajun 2016年10月25日 下午6:49:48
 * @version V1.0
 */
public class RecordsVO {

	private Integer nrepairDevId;
	private String strCollectIp;
	private Integer dvrPort;
	private String dvrUserName;
	private String dvrPwd;
	private Boolean isKMSDevice;

	public Integer getNrepairDevId() {
		return nrepairDevId;
	}

	public void setNrepairDevId(Integer nrepairDevId) {
		this.nrepairDevId = nrepairDevId;
	}

	public String getStrCollectIp() {
		return strCollectIp;
	}

	public void setStrCollectIp(String strCollectIp) {
		this.strCollectIp = strCollectIp;
	}

	public Integer getDvrPort() {
		return dvrPort;
	}

	public void setDvrPort(Integer dvrPort) {
		this.dvrPort = dvrPort;
	}

	public String getDvrUserName() {
		return dvrUserName;
	}

	public void setDvrUserName(String dvrUserName) {
		this.dvrUserName = dvrUserName;
	}

	public String getDvrPwd() {
		return dvrPwd;
	}

	public void setDvrPwd(String dvrPwd) {
		this.dvrPwd = dvrPwd;
	}

	public Boolean getIsKMSDevice() {
		return isKMSDevice;
	}

	public void setIsKMSDevice(Boolean isKMSDevice) {
		this.isKMSDevice = isKMSDevice;
	}

}
