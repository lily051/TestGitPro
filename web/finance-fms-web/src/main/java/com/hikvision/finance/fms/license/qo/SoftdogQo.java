/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月19日 上午10:51:41
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.license.qo;

import com.hikvision.finance.extend.hibernate.qo.BaseQo;


/**
 * <p></p>
 * @author jinxindong 2016年4月19日 上午10:51:41
 * @version V1.0 
 */
public class SoftdogQo extends BaseQo{
	/**
     * 序列化ID
     */
    private static final long serialVersionUID = -6310237916348284246L;
	private Integer nType;

	
    public Integer getnType() {
    	return nType;
    }

	
    public void setnType(Integer nType) {
    	this.nType = nType;
    }
	
}
