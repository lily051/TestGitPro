/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月3日 下午6:06:43
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.user.qo;

import com.hikvision.finance.extend.hibernate.qo.BaseQo;


/**
 * <p></p>
 * @author jinxindong 2016年3月3日 下午6:06:43
 * @version V1.0 
 */
public class UsercrtificatemappingQo extends BaseQo{

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 6533667201611975385L;
	private Integer ncrtificateId;
	private Integer userId;
	
    public Integer getNcrtificateId() {
    	return ncrtificateId;
    }
	
    public void setNcrtificateId(Integer ncrtificateId) {
    	this.ncrtificateId = ncrtificateId;
    }
	
    public Integer getUserId() {
    	return userId;
    }
	
    public void setUserId(Integer userId) {
    	this.userId = userId;
    }
}
