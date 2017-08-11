/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2015年8月17日 上午10:15:09
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.common.model;

import java.util.Date;

import javax.persistence.Column;
import javax.persistence.MappedSuperclass;

import org.apache.struts2.json.annotations.JSON;


/**
 * 数据库表实体类的超类，table通用字段集成
 * @author jinxindong 2016年2月22日 下午2:09:23
 * @version V1.0
 */
@MappedSuperclass
public abstract class BaseEntity implements java.io.Serializable{

   
	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 1627237261775982018L;
	/**
	 * 记录创建者id
	 */
	private Integer ncreatorId;
	/**
	 * 创建时间
	 */
	private Date dtCreateTime;
	/**
	 * 更新时间
	 */
	private Date dtUpdateTime;

	@Column(name = "ncreatorId")
	public Integer getNcreatorId() {
		return this.ncreatorId;
	}
	
	public void setNcreatorId(Integer ncreatorId) {
		this.ncreatorId = ncreatorId;
	}
	
	@JSON(format="yyyy-MM-dd HH:mm:ss")
	@Column(name = "dtCreateTime", length = 19)
	public Date getDtCreateTime() {
		return this.dtCreateTime;
	}
	
	public void setDtCreateTime(Date dtCreateTime) {
		this.dtCreateTime = dtCreateTime;
	}
	
	@JSON(format="yyyy-MM-dd HH:mm:ss")
	@Column(name = "dtUpdateTime", length = 19)
	public Date getDtUpdateTime() {
		return this.dtUpdateTime;
	}
	
	public void setDtUpdateTime(Date dtUpdateTime) {
		this.dtUpdateTime = dtUpdateTime;
	}
}
