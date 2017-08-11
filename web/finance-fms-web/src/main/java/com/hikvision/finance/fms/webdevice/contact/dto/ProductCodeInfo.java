/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月23日 上午11:01:47
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.webdevice.contact.dto;

import com.hikvision.finance.fms.model.Product;

/**
 * <p>理财经理模糊搜索产品编码专用，减少字段，提高速度</p>
 * @author jinchenwu 2016年4月23日 上午11:01:47
 * @version V1.0 
 */
public class ProductCodeInfo {
	
	private Integer value;// 产品Id
	private String label;// 产品编码
	private Product product;
	
	public Integer getValue() {
		return value;
	}
	
	public void setValue(Integer value) {
		this.value = value;
	}
	
	public String getLabel() {
		return label;
	}
	
	public void setLabel(String label) {
		this.label = label;
	}

	
    public Product getProduct() {
    	return product;
    }

	
    public void setProduct(Product product) {
    	this.product = product;
    }
	
}
