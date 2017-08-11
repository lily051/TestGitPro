/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月23日 下午3:35:00
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.product.dto;

import java.util.List;


/**
 * <p></p>
 * @author jinxindong 2016年3月23日 下午3:35:00
 * @version V1.0 
 */
public class CrtificateDto {
	private CrtificateInfo certInfo;
	private List<CrtificateProductMappingInfo> certProductList;
	private List<ProducttypeInfo> producttypeList;
	//private List<CrtificateInfo> certInfoList;
	
    public CrtificateInfo getCertInfo() {
    	return certInfo;
    }
	
    public void setCertInfo(CrtificateInfo certInfo) {
    	this.certInfo = certInfo;
    }

    public List<CrtificateProductMappingInfo> getCertProductList() {
    	return certProductList;
    }

	
    public void setCertProductList(List<CrtificateProductMappingInfo> certProductList) {
    	this.certProductList = certProductList;
    }

	public List<ProducttypeInfo> getProducttypeList() {
    	return producttypeList;
    }

	
    public void setProducttypeList(List<ProducttypeInfo> producttypeList) {
    	this.producttypeList = producttypeList;
    }

	
/*    public List<CrtificateInfo> getCertInfoList() {
    	return certInfoList;
    }

	
    public void setCertInfoList(List<CrtificateInfo> certInfoList) {
    	this.certInfoList = certInfoList;
    }*/
	
	

	
	
}
