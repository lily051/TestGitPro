/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月17日 上午10:12:37
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.product.dao;

import java.util.List;

import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.hibernate.dao.IBaseDao;
import com.hikvision.finance.fms.model.Product;
import com.hikvision.finance.fms.modules.product.dto.ProductInfo;
import com.hikvision.finance.fms.modules.product.qo.ProductQo;


/**
 * <p></p>
 * @author jinxindong 2016年3月17日 上午10:12:37
 * @version V1.0 
 */
public interface IProductDao extends IBaseDao<Product,ProductQo>{

	/**
	 * 获取产品分页数据
	 * @author jinxindong 2016年3月28日 下午3:03:44
	 * @param limit
	 * @param start
	 * @param productInfo
	 * @return
	 */
	public Page getDataPage(Integer limit, Integer start, ProductInfo productInfo);
	
	/**
	 * 导出产品表格
	 * @author jinxindong 2016年4月20日 下午6:22:07
	 * @param productInfo
	 * @return
	 */
	public List<ProductInfo> exportProductsExcel(ProductInfo productInfo);
	
    /**
     * 通过产品证书id组成的字符串和产品类型Id获取可用的产品，for采集web&cs客户端
     * @author jinchenwu 2016年10月17日 上午11:17:01
     * @param ids
     * @param productTypeId
     * @param strCode //根据产品编码精确到产品
     * @return
     */
    public List<Product> getPermitProductList(String ids, Integer productTypeId, String strCode, Integer productId);
    
	
}
