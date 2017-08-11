/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2015年7月2日 下午2:55:26
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.sysdictionary.qo;

import java.util.List;

import com.hikvision.finance.extend.hibernate.qo.BaseQo;
import com.hikvision.finance.fms.common.util.Constants.SysDictionary;


/**
 * <p></p>
 * @author fuqunqing 2015年7月2日 下午2:55:26
 * @version V3.1  
 */
public class SysdictionaryQo extends BaseQo{

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 2034263217312931414L;
	
    /**
     * 大类型
     */
	private Integer nbigType;
	/**
	 * 类型
	 */
	private Integer ntype;
	/**
	 * 唯一key
	 */
	private Integer nkey;
	/**
	 * 厂商类型
	 */
	private Integer nmanType;
	/**
	 * 设备类型
	 */
	private Integer ndevType;
	
	/**
	 * 是否启用
	 */
	private Integer nenabled = SysDictionary.enable;
	/**
	 * 升序排序
	 */
	private boolean orderByNSortIdAsc = true;
	
	/**
	 * 字典类型列表
	 */
	private List<Integer> ntypes;
	
	/**
	 * 字典key列表
	 */
	private List<Integer> nkeys;
	/**
	 * 模糊查询strValue
	 */
	private String likeStrValue;
	/**
	 * 查询strValue
	 */
	private String strValue;
	
	/**不等于*/
	private Integer notId;
	
    public Integer getNbigType() {
    	return this.nbigType;
    }
	
    public void setNbigType(Integer nbigType) {
    	this.nbigType = nbigType;
    }
	
    public Integer getNtype() {
    	return this.ntype;
    }
	
    public void setNtype(Integer ntype) {
    	this.ntype = ntype;
    }
	
    public Integer getNkey() {
    	return this.nkey;
    }
	
    public void setNkey(Integer nkey) {
    	this.nkey = nkey;
    }
	
    public Integer getNmanType() {
    	return this.nmanType;
    }
	
    public void setNmanType(Integer nmanType) {
    	this.nmanType = nmanType;
    }
	
    public Integer getNdevType() {
    	return this.ndevType;
    }
	
    public void setNdevType(Integer ndevType) {
    	this.ndevType = ndevType;
    }


	public boolean isOrderByNSortIdAsc() {
	    return orderByNSortIdAsc;
    }

	public void setOrderByNSortIdAsc(boolean orderByNSortIdAsc) {
	    this.orderByNSortIdAsc = orderByNSortIdAsc;
    }

	public Integer getNenabled() {
	    return nenabled;
    }

	public void setNenabled(Integer nenabled) {
	    this.nenabled = nenabled;
    }

	
    public List<Integer> getNtypes() {
    	return ntypes;
    }

	
    public void setNtypes(List<Integer> ntypes) {
    	this.ntypes = ntypes;
    }

	
    public List<Integer> getNkeys() {
    	return nkeys;
    }

	
    public void setNkeys(List<Integer> nkeys) {
    	this.nkeys = nkeys;
    }

	public String getLikeStrValue() {
	    return likeStrValue;
    }

	public void setLikeStrValue(String likeStrValue) {
	    this.likeStrValue = likeStrValue;
    }

    public String getStrValue() {
    	return strValue;
    }

    public void setStrValue(String strValue) {
    	this.strValue = strValue;
    }

	public Integer getNotId() {
	    return notId;
    }

	public void setNotId(Integer notId) {
	    this.notId = notId;
    }
}
