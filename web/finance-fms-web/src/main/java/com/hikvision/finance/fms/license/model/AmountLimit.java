/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2015年8月25日 下午2:23:43
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.license.model;

import java.io.Serializable;


/**
 * @author jinxindong 2016年4月19日 下午3:40:43
 * @version V1.0
 */
public class AmountLimit implements Serializable{

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 3431276620247768102L;

	/**
	 * 信道限制数
	 */
	private Integer amtChannelNumLimit;

	
    public Integer getAmtChannelNumLimit() {
    	return amtChannelNumLimit;
    }

	
    public void setAmtChannelNumLimit(Integer amtChannelNumLimit) {
    	this.amtChannelNumLimit = amtChannelNumLimit;
    }

	



    
    
}
