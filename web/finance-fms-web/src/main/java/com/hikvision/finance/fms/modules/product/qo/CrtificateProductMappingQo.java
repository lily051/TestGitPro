/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月23日 下午4:56:43
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.product.qo;

import com.hikvision.finance.extend.hibernate.qo.BaseQo;


/**
 * <p></p>
 * @author jinxindong 2016年3月23日 下午4:56:43
 * @version V1.0 
 */
public class CrtificateProductMappingQo extends BaseQo{
	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 3287389927391323304L;
	private Integer id;
	private Integer ncrtificateId;
	private Integer nproductId;
	
    public Integer getId() {
    	return id;
    }
	
    public void setId(Integer id) {
    	this.id = id;
    }
	
    public Integer getNcrtificateId() {
    	return ncrtificateId;
    }
	
    public void setNcrtificateId(Integer ncrtificateId) {
    	this.ncrtificateId = ncrtificateId;
    }
	
    public Integer getNproductId() {
    	return nproductId;
    }
	
    public void setNproductId(Integer nproductId) {
    	this.nproductId = nproductId;
    }
	
}
