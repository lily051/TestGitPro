/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月17日 上午10:09:47
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.product.service;


import java.io.File;
import java.util.List;
import java.util.Map;

import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.service.IBaseService;
import com.hikvision.finance.fms.common.excel.model.ExcelHead;
import com.hikvision.finance.fms.model.Product;
import com.hikvision.finance.fms.model.Users;
import com.hikvision.finance.fms.modules.product.dto.CrtificateInfo;
import com.hikvision.finance.fms.modules.product.dto.ProductInfo;
import com.hikvision.finance.fms.modules.product.dto.ProducttypeInfo;
import com.hikvision.finance.fms.modules.product.qo.ProductQo;
import com.hikvision.finance.fms.modules.user.dto.UserInfo;
import com.hikvision.finance.fwork.util.AjaxData;


/**
 * <p>产品管理service接口</p>
 * @author jinxindong 2016年3月17日 上午10:09:47
 * @version V1.0 
 */
public interface IProductService extends IBaseService<Product, ProductQo>{
	
	/**
	 * 获取产品分页数据
	 * @author jinxindong 2016年3月28日 下午3:00:45
	 * @param limit
	 * @param start
	 * @param productInfo
	 * @return
	 */
	public Page getDataPage(Integer limit,Integer start,ProductInfo productInfo);
	
	/**
	 * 添加产品类型
	 * @author jinxindong 2016年3月21日 下午7:07:43
	 * @param producttypeInfo
	 * @return
	 */
	public AjaxData addProType(ProducttypeInfo producttypeInfo);
	
	/**
	 * 修改产品类型
	 * @author jinxindong 2016年3月21日 下午7:30:30
	 * @param producttypeInfo
	 * @return
	 */
	public AjaxData updateProType(ProducttypeInfo producttypeInfo);
	
	/**
	 * 删除产品类型
	 * @author jinxindong 2016年3月21日 下午7:35:29
	 * @param producttypeInfo
	 * @return
	 */
	public AjaxData deleteProType(ProducttypeInfo producttypeInfo);
	
	/**
	 * 添加产品
	 * @author jinxindong 2016年3月22日 上午11:00:14
	 * @param productInfo
	 * @return
	 */
	public AjaxData addProduct(ProductInfo productInfo);
	
	/**
	 * 更新产品
	 * @author jinxindong 2016年3月22日 上午11:03:42
	 * @param productInfo
	 * @return
	 */
	public AjaxData updateProduct(ProductInfo productInfo);
	
	/**
	 * 启用产品
	 * @author jinxindong 2016年3月22日 上午11:03:53
	 * @param ids
	 * @return
	 */
	public AjaxData enableProducts(List<Integer> ids);
	
	/**
	 * 禁用产品
	 * @author jinxindong 2016年3月22日 上午11:04:01
	 * @param ids
	 * @return
	 */
	public AjaxData disableProducts(List<Integer> ids);
	
	/**
	 * 获取复核员列表（中心管理员）
	 * @author jinxindong 2016年3月23日 上午9:24:02
	 * @param userInfo
	 * @return
	 */
	public List<Users> getCheckUsers(UserInfo userInfo);
	
	/**
	 * 指定复核员
	 * @author jinxindong 2016年3月23日 上午10:48:29
	 * @param ids
	 * @param userInfo
	 * @return
	 */
	public AjaxData addCheckUser(List<Integer> ids,UserInfo userInfo);
	
	/**
	 * 获取证书管理信息
	 * @author jinxindong 2016年3月23日 上午11:00:48
	 * @param crtificateInfo
	 * @return
	 */
	public List<CrtificateInfo> getCertDate(CrtificateInfo crtificateInfo);

	/**
	 * 添加证书
	 * @author jinxindong 2016年3月23日 上午11:22:47
	 * @param jsonStr
	 * @return
	 */
	public AjaxData addCertManager(String jsonStr);

	/**
	 * 通过证书id
	 * @author jinxindong 2016年3月23日 下午1:46:39
	 * @param certId
	 * @return
	 */
	public CrtificateInfo getCertByCertId(Integer certId);
	
	/**
	 * 更新证书
	 * @author jinxindong 2016年3月23日 下午1:48:51
	 * @param jsonStr
	 * @return
	 */
	public AjaxData updateCert(String jsonStr);
	
	/**
	 * 删除证书
	 * @author jinxindong 2016年3月23日 下午1:49:34
	 * @param certId
	 * @return
	 */
	public AjaxData deleteCert(Integer certId);

	/**
	 * 根据产品类型id获取产品类型
	 * @author jinxindong 2016年3月24日 下午7:25:23
	 * @param productTypeId
	 * @return
	 */
	public ProducttypeInfo getProTypeById(Integer productTypeId);
	
	/**
	 * 根据产品id获取产品
	 * @author jinxindong 2016年3月24日 下午7:25:53
	 * @param productId
	 * @return
	 */
	public ProductInfo getProductById(Integer productId);
	
	/**
	 * 根据产品id，获取已关联证书的列表
	 * @author jinxindong 2016年3月25日 上午10:49:05
	 * @param productId
	 * @return
	 */
	public List<CrtificateInfo> getCertListConn(Integer productId);
	
	/**
	 * 根据产品id，获取未关联证书的列表
	 * @author jinxindong 2016年3月25日 上午10:49:12
	 * @param productId
	 * @return
	 */
	public List<CrtificateInfo> getCertListConnOut(Integer productId);
	
	/**
	 * 提交添加或删除关联证书
	 * @author jinxindong 2016年3月25日 下午5:38:08
	 * @param productId
	 * @param ids
	 * @return
	 */
	public AjaxData addOrDelCertConnect(Integer productId,List<Integer> ids);
	
	/**
	 * 导出产品表格
	 * @author jinxindong 2016年4月20日 下午6:17:49
	 * @param productInfo
	 * @param titleName
	 * @param attrName
	 * @param path
	 * @return
	 */
	public boolean exportProductsExcel(ProductInfo productInfo, String[] titleName, String[] attrName, String path);
	
	/**
	 * 下载模板
	 * @author jinxindong 2016年4月22日 上午9:54:22
	 * @param path
	 * @return
	 */
	public boolean downLoadTemplate(String path);
	
	/**
	 * 导入表格
	 * @author jinxindong 2016年4月22日 下午2:29:16
	 * @param orgFile
	 * @param orgFileFileName
	 * @param excelHead
	 * @return
	 */
	public Map<String,Object> addProFromExcel(File orgFile, String orgFileFileName, ExcelHead excelHead);
	
	/**
	 * 导入表格数据字段校验
	 * @author jinxindong 2016年6月2日 上午11:47:29
	 * @param proFile
	 * @param proFileFileName
	 * @param excelHead
	 * @return
	 */
	public AjaxData addAndValidateExcel(File proFile, String proFileFileName, ExcelHead excelHead);
	
	/**
	 * 导入添加产品数据
	 * @author jinxindong 2016年4月22日 下午2:36:10
	 * @param productInfos
	 * @return
	 */
	public AjaxData addAllProducts(List<ProductInfo> productInfos);
	
	/**
	 * 理财经理获取有权限的产品
	 * @author jinchenwu 2016年4月23日 上午10:40:59
	 * @return
	 */
    public List<Product> getPermitProductList();
}
