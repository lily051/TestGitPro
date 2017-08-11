/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月29日 下午2:47:29
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.role.qo;

import com.hikvision.finance.extend.hibernate.qo.BaseQo;


/**
 * <p></p>
 * @author jinxindong 2016年2月29日 下午2:47:29
 * @version V1.0 
 */
public class RoleresourcemappingQo extends BaseQo{

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 8694880806727553121L;
	private Integer nroleId;
	private Integer nresourceId;
	private Integer nresType;
	
    public Integer getNroleId() {
    	return nroleId;
    }
	
    public void setNroleId(Integer nroleId) {
    	this.nroleId = nroleId;
    }
	
    public Integer getNresourceId() {
    	return nresourceId;
    }
	
    public void setNresourceId(Integer nresourceId) {
    	this.nresourceId = nresourceId;
    }
	
    public Integer getNresType() {
    	return nresType;
    }
	
    public void setNresType(Integer nresType) {
    	this.nresType = nresType;
    }
	
	
}
