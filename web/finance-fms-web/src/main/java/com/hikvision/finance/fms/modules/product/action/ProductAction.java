/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月17日 上午10:15:53
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.product.action;

import java.io.File;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.struts2.ServletActionContext;

import com.google.common.collect.Lists;
import com.google.common.collect.Maps;
import com.hikvision.finance.fms.common.excel.model.ExcelColumn;
import com.hikvision.finance.fms.common.excel.model.ExcelHead;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.model.Product;
import com.hikvision.finance.fms.model.Users;
import com.hikvision.finance.fms.modules.product.dto.CrtificateInfo;
import com.hikvision.finance.fms.modules.product.dto.ProductInfo;
import com.hikvision.finance.fms.modules.product.dto.ProducttypeInfo;
import com.hikvision.finance.fms.modules.product.service.IProductService;
import com.hikvision.finance.fms.modules.user.dto.UserInfo;
import com.hikvision.finance.fwork.exception.ExpectedException;
import com.hikvision.finance.fwork.www.action.BaseAction;

/**
 * <p>产品管理Action</p>
 * @author jinxindong 2016年3月17日 上午10:15:53
 * @version V1.0
 */
public class ProductAction extends BaseAction<Product> {
	
	/**
	 * 序列化ID
	 */
	private static final long serialVersionUID = -8644017428849343695L;
	private IProductService productService;
	private CrtificateInfo crtificateInfo;
	private ProductInfo productInfo;
	private ProducttypeInfo producttypeInfo;
	private List<Integer> ids;
	private List<CrtificateInfo> certList;
	private List<CrtificateInfo> certConnectInList;// 已关联
	private List<CrtificateInfo> certConnectOutList;// 未关联
	private List<Users> userList;// 复核员列表
	private UserInfo userInfo;
	private File proFile;// 导入机构的文件
	private String proFileFileName;// 导入机构的文件名
	private Map<String,Object> resMap; 
	
	
    public Map<String, Object> getResMap() {
    	return resMap;
    }

	
    public void setResMap(Map<String, Object> resMap) {
    	this.resMap = resMap;
    }
	/**
	 * 获取产品分页数据
	 * @author jinxindong 2016年3月21日 下午4:56:19
	 * @return
	 */
	public String getDataPage() {
		page = productService.getDataPage(limit, start, productInfo);
		return PAGE;
	}
	
	/**
	 * 产品类型添加界面
	 * @author jinxindong 2016年3月21日 下午4:56:51
	 * @return
	 */
	public String toAddProTypePage() {
		if (producttypeInfo == null || producttypeInfo.getId() == null) {
			throw new ExpectedException("", "未获取产品类型的ID");
		}
		Integer nparentId = producttypeInfo.getId();
		ProducttypeInfo producttype = productService.getProTypeById(nparentId);
		producttypeInfo.setNparentName(producttype.getStrName());
		producttypeInfo.setPid(nparentId);
		operPage = "/modules/product/dialogs/addProTypePage.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 添加产品类型
	 * @author jinxindong 2016年3月21日 下午5:54:45
	 * @return
	 */
	public String addProType() {
		ajaxData = productService.addProType(producttypeInfo);
		return AJAX;
	}
	
	/**
	 * 跳到类型修改界面
	 * @author jinxindong 2016年3月21日 下午5:56:31
	 * @return
	 */
	public String toUpdateProTypePage() {
		//根据id获取产品类型信息在页面回显
		producttypeInfo = productService.getProTypeById(producttypeInfo.getId());
		operPage = "/modules/product/dialogs/editProTypePage.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 修改产品类型
	 * @author jinxindong 2016年3月21日 下午5:57:05
	 * @return
	 */
	public String updateProType() {
		if (producttypeInfo == null || producttypeInfo.getId() == null) {
			throw new ExpectedException("", "未获取到产品类型ID");
		}
		ajaxData = productService.updateProType(producttypeInfo);
		return AJAX;
	}
	
	/**
	 * 删除产品类型 （删除父节点，全部子节点删除 但是注意：能删的只有节点没有产品且子孙节点没有产品）
	 * @author jinxindong 2016年3月21日 下午5:57:56
	 * @return
	 */
	public String deleteProType() {
		if (producttypeInfo == null || producttypeInfo.getId() == null) {
			throw new ExpectedException("", "未获取到产品类型ID");
		}
		ajaxData = productService.deleteProType(producttypeInfo);
		return AJAX;
	}
	
	/**
	 * 产品添加界面
	 * @author jinxindong 2016年3月21日 下午4:57:25
	 * @return
	 */
	public String toAddProPage() {
		if (productInfo == null || productInfo.getPid() == null) {
			throw new ExpectedException("", "未获取产品类型ID");
		}
		Integer nparentId = productInfo.getPid();
		ProducttypeInfo producttype = productService.getProTypeById(nparentId);
		//类型的名称和id放到添加页面
		productInfo.setPid(nparentId);
		productInfo.setProductTypeName(producttype.getStrName());
		operPage = "/modules/product/dialogs/toAddProPage.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 添加产品
	 * @author jinxindong 2016年3月21日 下午5:59:19
	 * @return
	 */
	public String addProduct() {
		ajaxData = productService.addProduct(productInfo);
		return AJAX;
	}
	
	/**
	 * 跳到产品修改界面
	 * @author jinxindong 2016年3月21日 下午5:59:45
	 * @return
	 */
	public String toUpdateProPage() {
		if (productInfo == null || productInfo.getId() == null) {
			throw new ExpectedException("", "未获取产品的ID");
		}
		productInfo = productService.getProductById(productInfo.getId());
		operPage = "/modules/product/dialogs/toEditProPage.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 修改产品
	 * @author jinxindong 2016年3月21日 下午6:00:08
	 * @return
	 */
	public String updateProduct() {
		if (productInfo == null || productInfo.getId() == null) {
			throw new ExpectedException("", "未获取到产品ID");
		}
		ajaxData = productService.updateProduct(productInfo);
		return AJAX;
	}
	
	/**
	 * 解禁用户（启用）
	 * @author jinxindong 2016年3月21日 下午6:00:53
	 * @return
	 */
	public String enableProducts() {
		ajaxData = productService.enableProducts(ids);
		return AJAX;
	}
	
	/**
	 * 禁用用户
	 * @author jinxindong 2016年3月21日 下午6:01:01
	 * @return
	 */
	public String disableProducts() {
		ajaxData = productService.disableProducts(ids);
		return AJAX;
	}
	
	/**
	 * 调到指定复核员界面
	 * @author jinxindong 2016年3月21日 下午6:02:05
	 * @return
	 */
	public String toAddCheckUserPage() {
		operPage = "/modules/product/dialogs/assignUserPage.jsp";
		return DISPATCHER;
	}

	/**
	 * 到证书详情页面
	 * @author jinxindong 2016年6月16日 下午3:37:24
	 * @return
	 */
	public String toCertDetailPage() {
		if (crtificateInfo == null || crtificateInfo.getId() == null) {
			throw new ExpectedException("", "未获取到证书ID");
		}
		crtificateInfo = productService.getCertByCertId(crtificateInfo.getId());
		operPage = "/modules/product/dialogs/certificateDetail.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 查询指定复核员
	 * @author jinxindong 2016年3月21日 下午6:02:05
	 * @return
	 */
	public String getCheckUsers() {
		// 获取复核员列表（中心管理员）
		userList = productService.getCheckUsers(userInfo);
		ajaxData.put("userList",userList);
		return AJAX;
	}
	
	/**
	 * 提交指定复核员
	 * @author jinxindong 2016年3月21日 下午6:02:32
	 * @return
	 */
	public String addCheckUser() {
		if (userInfo == null || userInfo.getId() == null) {
			throw new ExpectedException("", "未获取到复核员ID");
		}
		ajaxData = productService.addCheckUser(ids, userInfo);
		return AJAX;
	}
	
	/**
	 * 调到证书管理界面
	 * @author jinxindong 2016年3月21日 下午6:03:24
	 * @return
	 */
	public String toCertManagerPage() {
		//certList = productService.getCertDate(crtificateInfo);
		operPage = "/modules/product/dialogs/certificatePage.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 证书管理界面（带搜索参数）
	 * @author jinxindong 2016年3月21日 下午6:03:24
	 * @return
	 */
	public String getCertDate() {
		certList = productService.getCertDate(crtificateInfo);
		operPage = "/modules/product/dialogs/certificatePageDialog.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 跳转到添加证书页面
	 * @author jinxindong 2016年3月21日 下午6:04:13
	 * @return
	 */
	public String toAddCertManagerPage() {
		operPage = "/modules/product/dialogs/addCertificatePage.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 添加证书
	 * @author jinxindong 2016年3月21日 下午6:05:28
	 * @return
	 */
	public String addCertManager() {
		ajaxData = productService.addCertManager(jsonStr);
		return AJAX;
	}
	
	/**
	 * 跳转到编辑证书页面
	 * @author jinxindong 2016年3月21日 下午6:06:01
	 * @return
	 */
	public String toUpdateCertManagerPage() {
		if (crtificateInfo == null || crtificateInfo.getId() == null) {
			throw new ExpectedException("", "未获取到证书ID");
		}
		crtificateInfo = productService.getCertByCertId(crtificateInfo.getId());
		operPage = "/modules/product/dialogs/editCertificatePage.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 编辑证书
	 * @author jinxindong 2016年3月21日 下午6:08:08
	 * @return
	 */
	public String updateCert() {
		if (crtificateInfo == null || crtificateInfo.getId() == null) {
			throw new ExpectedException("", "未获取到证书ID");
		}
		ajaxData = productService.updateCert(jsonStr);
		return AJAX;
	}
	
	/**
	 * 删除证书
	 * @author jinxindong 2016年3月21日 下午6:08:13
	 * @return
	 */
	public String deleteCert() {
		if (crtificateInfo == null || crtificateInfo.getId() == null) {
			throw new ExpectedException("", "未获取到证书ID");
		}
		ajaxData = productService.deleteCert(crtificateInfo.getId());
		return AJAX;
	}
	
	/**
	 * 跳转到关联证书界面（查询关联和未关联的证书）
	 * @author jinxindong 2016年3月21日 下午6:09:08
	 * @return
	 */
	public String toCertConnectPage() {
		if (productInfo == null || productInfo.getId() == null) {
			throw new ExpectedException("", "未获取到产品ID");
		}
		productInfo = productService.getProductById(productInfo.getId());
		certConnectInList = productService.getCertListConn(productInfo.getId());// 已关联private List<CrtificateInfo>
		certConnectOutList = productService.getCertListConnOut(productInfo.getId());// 未关联
		operPage = "/modules/product/dialogs/relateProPage.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 提交添加或删除关联证书
	 * @author jinxindong 2016年3月25日 下午12:53:46
	 * @return
	 */
	public String addOrDelCertConnect() {
		//通过证书的状态判断添加或者删除 jsonStr
		ajaxData = productService.addOrDelCertConnect(productInfo.getId(),ids);
		return AJAX;
	}
	
	
	/**
	 * 导出产品列表
	 * @author jinxindong 2016年4月20日 下午5:03:47
	 * @return
	 */
	public String exportProductsExcel() {
		String[] titleNames = new String[] {"产品名称", "状态",  "产品编码", "发行机构", "风险等级", "产品类型", "到期日", "保障期限","类型","视频保存时间","类型","是否关联证书"};
		String[] attrNames = new String[] {"strName", "nenabledName",  "strCode", "strGrade", "strRisklevelName", "productTypeName", "strEndDate","guaranteeYears", "strGuaranteeType", "saveYears", "strSaveType","relateStatusName"};
		String rootpath = ServletActionContext.getServletContext().getRealPath("/");
		String path = Constants.FILEUPLOAD + "excel/" + "prductsQuery.xls";
		if(productService.exportProductsExcel( productInfo, titleNames, attrNames, rootpath + path)) {
			ajaxData.put("url", "/" + path);
    		success = true;
		} else {
			msg = "导出失败";
    		success = false;
		}
		return AJAX;
	}
	
	/**
	 * 跳转到下载模板界面
	 * @author jinxindong 2016年4月22日 上午9:14:52
	 * @return
	 */
	public String toCheckOutPage() {
		operPage = "/modules/product/dialogs/import.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 下载模板
	 * @author jinxindong 2016年4月19日 下午4:45:21
	 * @return
	 */
	public String downLoadTemplate() {
		String rootpath = ServletActionContext.getServletContext().getRealPath("/");
		//String path = Constants.FILEUPLOAD + "excel/" + "prductsTemplate.xls";
		String path ="modules/product/doc/" + "productTemplate.xls";
		String path1 = Constants.FILEUPLOAD + "excel/" + "productTemplate.xls";
		if(productService.downLoadTemplate(rootpath + path)) {
			ajaxData.put("url", "/" + path1);
    		success = true;
		} else {
			msg = "下载失败";
    		success = false;
		}
		return AJAX;
	}
	
	/**
	 * 上传Excel 
	 * @author jinxindong 2016年4月22日 上午9:23:30
	 * @return
	 */
	@SuppressWarnings({"serial", "unchecked"})
    public String productUploadExcel() {
		resMap = new HashMap<String, Object>(); 
		if (this.proFile != null) {
		// 1、excel的每行显示名称-数据库名称
		List<ExcelColumn> excelColumns = Lists.newArrayList();
		excelColumns.add(new ExcelColumn(0, "productTypeName", "产品类型(必填)"));
		excelColumns.add(new ExcelColumn(1, "strName", "产品名称(必填)"));
		excelColumns.add(new ExcelColumn(2, "strCode", "产品代码(必填)"));
		excelColumns.add(new ExcelColumn(3, "strGrade", "发行机构(必填)"));
		excelColumns.add(new ExcelColumn(4, "strRisklevel", "风险等级(必填)"));
		excelColumns.add(new ExcelColumn(5, "strEndDate", "到期日(2选1/2016-01-01)"));
		excelColumns.add(new ExcelColumn(6, "guaranteeYears", "保障期限(2选1/年)"));
		excelColumns.add(new ExcelColumn(7, "guaranteeType", "类型"));
		excelColumns.add(new ExcelColumn(8, "saveYears", "视频保存时间(必填/年)"));
		excelColumns.add(new ExcelColumn(9, "saveType", "类型"));
		
		excelColumns.add(new ExcelColumn(10, "wordTemplate", "话术模板(非必填)"));
		
		// 2、机构级别中显示的名称-数据库中整数
		Map<String, Integer> levelMap = new HashMap<String, Integer>(){
			{
				put(Constants.RiskLevel.RISKLEVEL_ONE, 1); 
				put(Constants.RiskLevel.RISKLEVEL_TWO, 2); 
				put(Constants.RiskLevel.RISKLEVEL_THREE, 3); 
				put(Constants.RiskLevel.RISKLEVEL_FOUR, 4); 
				put(Constants.RiskLevel.RISKLEVEL_FIVE, 5); 
				put(Constants.RiskLevel.RISKLEVEL_SIX, 6); 
				put(Constants.RiskLevel.RISKLEVEL_NONE, 0);
			}
		};
		Map<String, Integer> saveType = new HashMap<String, Integer>(){
			{
				put("年", 0);    put("月", 1); put("日", 2);
			}
		};
		// 3、excel中显示的信息转换为数据库中的值
		Map<String, Map<?, ?>> excelColumnsConvertMap = Maps.newHashMap();
		excelColumnsConvertMap.put("strRisklevel", levelMap);
		excelColumnsConvertMap.put("guaranteeType", saveType);
		excelColumnsConvertMap.put("saveType", saveType);
		// 4、组装excel信息
		ExcelHead excelHead = new ExcelHead();
		excelHead.setRowCount(1);// 模板头部所占行数
		excelHead.setColumnCount(9);
		excelHead.setColumns(excelColumns);// 列的定义
		excelHead.setColumnsConvertMap(excelColumnsConvertMap);//列的转换
		//List<ProductInfo> productInfos = productService.addProFromExcel(proFile, proFileFileName, excelHead);
		//Map<String,Object> prosMap = productService.addProFromExcel(proFile, proFileFileName, excelHead);		
		ajaxData = productService.addAndValidateExcel(proFile, proFileFileName, excelHead);
		if(ajaxData.getSuccess()){
			//批量添加prosList
			List<ProductInfo> productInfos = (List<ProductInfo>)ajaxData.getData().get("prosList");
			ajaxData = productService.addAllProducts(productInfos);
		}
		resMap.put("errorNo", ajaxData.getSuccess());
		resMap.put("errorInfo", ajaxData.getMsg());
	}
		return "upload";
	}
	
	/**
	 * 导入的编码有重复，则跳转到该页面展示
	 * @author jinxindong 2016年4月22日 下午4:03:10
	 * @return
	 */
	public String toAlreadyImport() {
		operPage = "/modules/product/dialogs/alreadyImport.jsp";
		return DISPATCHER;
	}
	// ******************************************getter/setter*************************************************
	
	public IProductService getProductService() {
		return productService;
	}
	
	public void setProductService(IProductService productService) {
		this.productService = productService;
	}
	
	public CrtificateInfo getCrtificateInfo() {
		return crtificateInfo;
	}
	
	public void setCrtificateInfo(CrtificateInfo crtificateInfo) {
		this.crtificateInfo = crtificateInfo;
	}
	
	public ProductInfo getProductInfo() {
		return productInfo;
	}
	
	public void setProductInfo(ProductInfo productInfo) {
		this.productInfo = productInfo;
	}
	
	public ProducttypeInfo getProducttypeInfo() {
		return producttypeInfo;
	}
	
	public void setProducttypeInfo(ProducttypeInfo producttypeInfo) {
		this.producttypeInfo = producttypeInfo;
	}
	
	public List<Integer> getIds() {
		return ids;
	}
	
	public void setIds(List<Integer> ids) {
		this.ids = ids;
	}
	
	public List<CrtificateInfo> getCertList() {
		return certList;
	}
	
	public void setCertList(List<CrtificateInfo> certList) {
		this.certList = certList;
	}
	
	public List<CrtificateInfo> getCertConnectInList() {
		return certConnectInList;
	}
	
	public void setCertConnectInList(List<CrtificateInfo> certConnectInList) {
		this.certConnectInList = certConnectInList;
	}
	
	public List<CrtificateInfo> getCertConnectOutList() {
		return certConnectOutList;
	}
	
	public void setCertConnectOutList(List<CrtificateInfo> certConnectOutList) {
		this.certConnectOutList = certConnectOutList;
	}
	
	public List<Users> getUserList() {
		return userList;
	}
	
	public void setUserList(List<Users> userList) {
		this.userList = userList;
	}
	
	public UserInfo getUserInfo() {
		return userInfo;
	}
	
	public void setUserInfo(UserInfo userInfo) {
		this.userInfo = userInfo;
	}

	
    public File getProFile() {
    	return proFile;
    }

	
    public void setProFile(File proFile) {
    	this.proFile = proFile;
    }

	
    public String getProFileFileName() {
    	return proFileFileName;
    }

	
    public void setProFileFileName(String proFileFileName) {
    	this.proFileFileName = proFileFileName;
    }
	
}
