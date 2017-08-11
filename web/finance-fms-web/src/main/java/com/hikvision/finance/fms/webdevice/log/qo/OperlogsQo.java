/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-4-27 下午8:06:58
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.webdevice.log.qo;

import java.util.Date;

import com.hikvision.finance.extend.hibernate.qo.BaseQo;

/**
 * <p></p>
 * @author xujiangfei 2016-4-27 下午8:06:58
 * @version V1.0 
 */
public class OperlogsQo extends BaseQo{

	/**
	 * 序列化ID
	 */
	private static final long serialVersionUID = 3102887230541317857L;
	private Integer id;
	private Integer ntype;
	private Integer nsubType;
	private Integer npriority;
	private Date dtOccurtime;
	private Integer nregionId;
	private Integer ncollectorId;
	private Integer nuserId;
	private String strAppendInfo;
	
	public Integer getId() {
		return id;
	}
	public void setId(Integer id) {
		this.id = id;
	}
	public Integer getNtype() {
		return ntype;
	}
	public void setNtype(Integer ntype) {
		this.ntype = ntype;
	}
	public Integer getNsubType() {
		return nsubType;
	}
	public void setNsubType(Integer nsubType) {
		this.nsubType = nsubType;
	}
	public Integer getNpriority() {
		return npriority;
	}
	public void setNpriority(Integer npriority) {
		this.npriority = npriority;
	}
	public Date getDtOccurtime() {
		return dtOccurtime;
	}
	public void setDtOccurtime(Date dtOccurtime) {
		this.dtOccurtime = dtOccurtime;
	}
	public Integer getNregionId() {
		return nregionId;
	}
	public void setNregionId(Integer nregionId) {
		this.nregionId = nregionId;
	}
	public Integer getNcollectorId() {
		return ncollectorId;
	}
	public void setNcollectorId(Integer ncollectorId) {
		this.ncollectorId = ncollectorId;
	}
	public Integer getNuserId() {
		return nuserId;
	}
	public void setNuserId(Integer nuserId) {
		this.nuserId = nuserId;
	}
	public String getStrAppendInfo() {
		return strAppendInfo;
	}
	public void setStrAppendInfo(String strAppendInfo) {
		this.strAppendInfo = strAppendInfo;
	}
	
	
	
}
