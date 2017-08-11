/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月29日 下午4:35:23
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.user.qo;

import com.hikvision.finance.extend.hibernate.qo.BaseQo;


/**
 * <p></p>
 * @author jinxindong 2016年2月29日 下午4:35:23
 * @version V1.0 
 */
public class UserrolemappingQo extends BaseQo{
	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 7751865189308794659L;
	private Integer nuserId;
	private Integer nroleId;
	
	
    public Integer getNuserId() {
    	return nuserId;
    }
	
    public void setNuserId(Integer nuserId) {
    	this.nuserId = nuserId;
    }
	
    public Integer getNroleId() {
    	return nroleId;
    }
	
    public void setNroleId(Integer nroleId) {
    	this.nroleId = nroleId;
    }
	
}
