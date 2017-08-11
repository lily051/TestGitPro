/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月23日 下午4:55:47
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.product.dao;

import java.util.List;

import com.hikvision.finance.extend.hibernate.dao.IBaseDao;
import com.hikvision.finance.fms.model.Crtificateproductmapping;
import com.hikvision.finance.fms.model.Product;
import com.hikvision.finance.fms.modules.product.qo.CrtificateProductMappingQo;


/**
 * <p></p>
 * @author jinxindong 2016年3月23日 下午4:55:47
 * @version V1.0 
 */
public interface ICertProductMappingDao extends IBaseDao<Crtificateproductmapping,CrtificateProductMappingQo>{
	
	/**
	 * 根据证书ids获取产品的ids
	 * @author jinxindong 2016年3月28日 上午9:58:10
	 * @param id
	 * @return
	 */
	public List<Integer> getProductIdsbyCertId(Integer id);
	
	/**
	 * 通过产品证书id获取产品
	 * @author jinxindong 2016年3月29日 上午9:30:52
	 * @param id
	 * @return
	 */
	public List<Product> getProductbyCertId(Integer id);
	
	/**
	 * 保存关联关系
	 * @author jinxindong 2016年7月11日 下午8:10:14
	 * @param strsql
	 */
	public void saveMapping(String strsql);
	
	/**
	 * 删除关联关系
	 * @author jinxindong 2016年7月11日 下午8:10:23
	 * @param certId
	 * @param strsql
	 */
	public void deleteMapping(Integer certId,String strsql);
	
	/**
	 * 比save高效的保存方法
	 * @author jinxindong 2016年7月11日 下午8:10:30
	 * @param entity
	 */
	public void meger(Crtificateproductmapping entity);
}
