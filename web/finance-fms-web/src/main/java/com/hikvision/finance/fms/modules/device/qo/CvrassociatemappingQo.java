/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月5日 下午6:20:44
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.device.qo;

import com.hikvision.finance.extend.hibernate.qo.BaseQo;


/**
 * <p></p>
 * @author jinxindong 2016年4月5日 下午6:20:44
 * @version V1.0 
 */
public class CvrassociatemappingQo extends BaseQo{
	/**
     * 序列化ID
     */
    private static final long serialVersionUID = -7636514315107966437L;
	private Integer id;
	private Integer ncvrId;
	private Integer ncollId;
	private Integer ntype;
	private Integer groupIndex;
	
    public Integer getId() {
    	return id;
    }
	
    public void setId(Integer id) {
    	this.id = id;
    }
	
    public Integer getNcvrId() {
    	return ncvrId;
    }
	
    public void setNcvrId(Integer ncvrId) {
    	this.ncvrId = ncvrId;
    }
	
    public Integer getNcollId() {
    	return ncollId;
    }
	
    public void setNcollId(Integer ncollId) {
    	this.ncollId = ncollId;
    }
	
    public Integer getNtype() {
    	return ntype;
    }
	
    public void setNtype(Integer ntype) {
    	this.ntype = ntype;
    }
	
    public Integer getGroupIndex() {
    	return groupIndex;
    }
	
    public void setGroupIndex(Integer groupIndex) {
    	this.groupIndex = groupIndex;
    }
	
}
