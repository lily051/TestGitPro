/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月28日 下午1:52:50
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.card.qo;

import com.hikvision.finance.extend.hibernate.qo.BaseQo;

/**
 * <p>
 * </p>
 * @author jinxindong 2016年3月28日 下午1:52:50
 * @version V1.0
 */
public class ParamdictionaryQo extends BaseQo {
	
	/**
	 * 序列化ID
	 */
	private static final long serialVersionUID = 308083872284726689L;
	private Integer id;
	private String strCode;
	private String strName;
	private Integer nsortId;
	private Integer enable;
	
	public Integer getId() {
		return id;
	}
	
	public void setId(Integer id) {
		this.id = id;
	}
	
	public String getStrCode() {
		return strCode;
	}
	
	public void setStrCode(String strCode) {
		this.strCode = strCode;
	}
	
	public String getStrName() {
		return strName;
	}
	
	public void setStrName(String strName) {
		this.strName = strName;
	}
	
	public Integer getNsortId() {
		return nsortId;
	}
	
	public void setNsortId(Integer nsortId) {
		this.nsortId = nsortId;
	}
	
	public Integer getEnable() {
		return enable;
	}
	
	public void setEnable(Integer enable) {
		this.enable = enable;
	}
	
}
