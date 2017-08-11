/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月3日 下午6:07:51
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.user.qo;

import com.hikvision.finance.extend.hibernate.qo.BaseQo;


/**
 * <p></p>
 * @author jinxindong 2016年3月3日 下午6:07:51
 * @version V1.0 
 */
public class UserregionmappingQo extends BaseQo{

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 7466596607333618852L;
	private Integer nuserId;
	private Integer nregionId;
	
    public Integer getNuserId() {
    	return nuserId;
    }
	
    public void setNuserId(Integer nuserId) {
    	this.nuserId = nuserId;
    }
	
    public Integer getNregionId() {
    	return nregionId;
    }
	
    public void setNregionId(Integer nregionId) {
    	this.nregionId = nregionId;
    }
	
	
}
