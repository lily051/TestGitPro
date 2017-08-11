/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月17日 上午10:12:51
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.product.dao;

import java.util.List;

import com.hikvision.finance.extend.hibernate.dao.IBaseDao;
import com.hikvision.finance.fms.model.Producttype;
import com.hikvision.finance.fms.modules.product.qo.ProductTypeQo;

/**
 * <p>
 * </p>
 * @author jinxindong 2016年3月17日 上午10:12:51
 * @version V1.0
 */
public interface IProductTypeDao extends IBaseDao<Producttype, ProductTypeQo> {
	
	/**
	 * 获取strpath匹配的节点 子孙产品类型节点
	 * @author jinxindong 2016年3月22日 上午9:14:02
	 * @param producttype
	 * @return
	 */
	public List<Producttype> getLikeStrPath(Producttype producttype);
	
	/**
	 * 获取数据库中产品的类型
	 * @author jinxindong 2016年4月22日 上午11:16:23
	 * @return
	 */
	public List<String> getProductType();
	
	/**
	 * 获取指定类型的当前层级
	 * @author jinxindong 2016年5月16日 下午6:10:48
	 * @param regionId
	 * @return
	 */
	public int getRegLevel(int regionId);
}
