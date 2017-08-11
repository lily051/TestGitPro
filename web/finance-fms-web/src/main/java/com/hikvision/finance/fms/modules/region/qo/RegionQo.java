/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月25日 上午9:50:41
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.region.qo;

import com.hikvision.finance.extend.hibernate.qo.BaseQo;

/**
 * <p>
 * </p>
 * @author jinxindong 2016年2月25日 上午9:50:41
 * @version V1.0
 */
public class RegionQo extends BaseQo {
	
	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 2606144734020231092L;
	private String strName; // 名称
	private String strInterAreaCode; // 区域号
	private Integer nparentId; // 父组织id
	private String strPathLike; // 路径
	private String strCode;
	private Integer nsortId;
	private Integer notId;
	
	/**
	 * 是否根据nSortId 升序排序
	 */
	private boolean orderByNSortIdAsc = false;
	
	

	
    public String getStrName() {
    	return strName;
    }
	
    public void setStrName(String strName) {
    	this.strName = strName;
    }
	
    public String getStrInterAreaCode() {
    	return strInterAreaCode;
    }
	
    public void setStrInterAreaCode(String strInterAreaCode) {
    	this.strInterAreaCode = strInterAreaCode;
    }
	
    public Integer getNparentId() {
    	return nparentId;
    }
	
    public void setNparentId(Integer nparentId) {
    	this.nparentId = nparentId;
    }
	

	

	
    public String getStrPathLike() {
    	return strPathLike;
    }
	
    public void setStrPathLike(String strPathLike) {
    	this.strPathLike = strPathLike;
    }

	
    public boolean isOrderByNSortIdAsc() {
    	return orderByNSortIdAsc;
    }

	
    public void setOrderByNSortIdAsc(boolean orderByNSortIdAsc) {
    	this.orderByNSortIdAsc = orderByNSortIdAsc;
    }

	
    public String getStrCode() {
    	return strCode;
    }

	
    public void setStrCode(String strCode) {
    	this.strCode = strCode;
    }

	
    public Integer getNsortId() {
    	return nsortId;
    }

	
    public void setNsortId(Integer nsortId) {
    	this.nsortId = nsortId;
    }

	
    public Integer getNotId() {
    	return notId;
    }

	
    public void setNotId(Integer notId) {
    	this.notId = notId;
    }
	
	
	
}
