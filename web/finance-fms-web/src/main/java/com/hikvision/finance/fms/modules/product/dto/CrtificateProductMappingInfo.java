/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月23日 下午5:58:19
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.product.dto;


/**
 * <p></p>
 * @author jinxindong 2016年3月23日 下午5:58:19
 * @version V1.0 
 */
public class CrtificateProductMappingInfo {
	private Integer id;
	private Integer ncrtificateId;
	private Integer nproductId;
	private Boolean status;
	
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
	
    public Boolean getStatus() {
    	return status;
    }
	
    public void setStatus(Boolean status) {
    	this.status = status;
    }
	
}
