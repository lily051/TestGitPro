/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月17日 上午10:10:09
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.product.service.impl;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

import org.apache.poi.hssf.usermodel.DVConstraint;
import org.apache.poi.hssf.usermodel.HSSFDataValidation;
import org.apache.poi.hssf.util.CellRangeAddressList;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.apache.poi.ss.usermodel.WorkbookFactory;
import org.apache.struts2.ServletActionContext;

import com.google.common.collect.Lists;
import com.google.common.collect.Maps;
import com.hikvision.cms.cache.manager.DataLoadException;
import com.hikvision.cms.cache.manager.DataManager;
import com.hikvision.finance.core.util.BeanUtils;
import com.hikvision.finance.core.util.JsonUtils;
import com.hikvision.finance.core.util.ListUtil;
import com.hikvision.finance.core.util.StringUtils;
import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.service.BaseServiceImpl;
import com.hikvision.finance.fms.common.enums.LogModuledEnum;
import com.hikvision.finance.fms.common.excel.ExcelHelper;
import com.hikvision.finance.fms.common.excel.ExcelHelperFactory;
import com.hikvision.finance.fms.common.excel.model.ExcelHead;
import com.hikvision.finance.fms.common.model.UserSession;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.common.util.Constants.CacheKeyPrefix;
import com.hikvision.finance.fms.common.util.Constants.OperStatus;
import com.hikvision.finance.fms.common.util.Constants.SysConfigType;
import com.hikvision.finance.fms.common.util.Constants.SysDictionary;
import com.hikvision.finance.fms.common.util.ExcelUtil;
import com.hikvision.finance.fms.common.util.SessionUtil;
import com.hikvision.finance.fms.common.util.StringUtilExpand;
import com.hikvision.finance.fms.common.util.ValidateUtil;
import com.hikvision.finance.fms.model.Crtificate;
import com.hikvision.finance.fms.model.Crtificateproductmapping;
import com.hikvision.finance.fms.model.Product;
import com.hikvision.finance.fms.model.Producttype;
import com.hikvision.finance.fms.model.Usercrtificatemapping;
import com.hikvision.finance.fms.model.Users;
import com.hikvision.finance.fms.modules.log.dao.IConfigLogDao;
import com.hikvision.finance.fms.modules.product.dao.ICertDao;
import com.hikvision.finance.fms.modules.product.dao.ICertProductMappingDao;
import com.hikvision.finance.fms.modules.product.dao.IProductDao;
import com.hikvision.finance.fms.modules.product.dao.IProductTypeDao;
import com.hikvision.finance.fms.modules.product.dto.CrtificateDto;
import com.hikvision.finance.fms.modules.product.dto.CrtificateInfo;
import com.hikvision.finance.fms.modules.product.dto.CrtificateProductMappingInfo;
import com.hikvision.finance.fms.modules.product.dto.ProductInfo;
import com.hikvision.finance.fms.modules.product.dto.ProducttypeInfo;
import com.hikvision.finance.fms.modules.product.qo.ProductQo;
import com.hikvision.finance.fms.modules.product.qo.ProductTypeQo;
import com.hikvision.finance.fms.modules.product.service.IProductService;
import com.hikvision.finance.fms.modules.user.dao.IUsercrtificatemappingDao;
import com.hikvision.finance.fms.modules.user.dao.IUsersDao;
import com.hikvision.finance.fms.modules.user.dto.UserInfo;
import com.hikvision.finance.fwork.exception.ExpectedException;
import com.hikvision.finance.fwork.util.AjaxData;
import com.hikvision.finance.fwork.util.GlobalMessageUtil;

/**
 * <p>产品管理Service层</p>
 * @author jinxindong 2016年3月17日 上午10:10:09
 * @version V1.0
 */
@SuppressWarnings("deprecation")
public class ProductServiceImpl extends BaseServiceImpl<Product, ProductQo, IProductDao> implements IProductService {
	
	private IProductTypeDao  productTypeDao;
	private IProductDao productDao;
	private ICertDao certDao;
	private IUsersDao usersDao;
	private ICertProductMappingDao certProductMappingDao;
	private IUsercrtificatemappingDao usercrtificatemappingDao;
	private DataManager dataManager;
	private IConfigLogDao configLogDao;
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.extend.service.IBaseService#deleteByIds(java.io.Serializable[])
	 */
	@Override
	public void deleteByIds(Serializable[] paramArrayOfSerializable) {
				
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.extend.service.BaseServiceImpl#getDao()
	 */
	@Override
	protected IProductDao getDao() {
		 return this.getProductDao();
	}

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.product.service.IProductService#getDataPage(java.lang.Integer, java.lang.Integer, com.hikvision.finance.fms.modules.product.dto.ProductInfo)
     */
    @Override
	public Page getDataPage(Integer limit, Integer start, ProductInfo productInfo) {
		// 获取分页数据
    	return  productDao.getDataPage(limit, start, productInfo);	    	
	}

	
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.product.service.IProductService#addProType(com.hikvision.finance.fms.modules.product.dto.ProducttypeInfo)
     */
    @Override
	public AjaxData addProType(ProducttypeInfo producttypeInfo) {
		
		AjaxData ajaxData = new AjaxData(false);
		String aeraCode = getRegSeril(producttypeInfo.getPid());
		// 验证唯一性
		List<Producttype> producttypes =  productTypeDao.findBy("strName", producttypeInfo.getStrName().trim());
		if(producttypes != null && producttypes.size()>0){
			return ajaxData.setFailureMsg("已存在产品类型名称为" + producttypeInfo.getStrName());
		}
		
		List<Producttype> producttypes1 =  productTypeDao.findBy("strInterAreaCode",aeraCode);
		if(producttypes1 != null && producttypes1.size()>0){
			return ajaxData.setFailureMsg("已存在产品类型编码为" + producttypeInfo.getStrInterAreaCode());
		}
		
		
		// 3，若添加的为组织，验证最大层级
		if (getOrgLevel(producttypeInfo.getPid()) >= Constants.RegNum.REGION_NUM) {
			ajaxData.setFailureMsg("产品类型" + productTypeDao.get(producttypeInfo.getPid()).getStrName() + "已达到最大层级");
			return ajaxData;
		}
		
		Producttype producttype = new Producttype();
		producttype.setPid(producttypeInfo.getPid());
		producttype.setStrName(producttypeInfo.getStrName());
		producttype.setNsortId(getNsort());
		producttype.setStrInterAreaCode(aeraCode);
		producttype.setStrPath(getStrPath(producttypeInfo.getPid(), aeraCode));
		productTypeDao.save(producttype);
		configLogDao.setLogContent(OperStatus.ADD, LogModuledEnum.PRODUCT.getCode(),GlobalMessageUtil.getMessage("log.proType.add",new String[] {producttypeInfo.getStrName()}));
		return ajaxData;
	}

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.product.service.IProductService#updateProType(com.hikvision.finance.fms.modules.product.dto.ProducttypeInfo)
     */
    @Override
    public AjaxData updateProType(ProducttypeInfo producttypeInfo) {
	 
    	AjaxData ajaxData = new AjaxData(false);
    	Producttype producttype = productTypeDao.get(producttypeInfo.getId());
    	
		List<Producttype> producttypes =  productTypeDao.findBy("strName", producttypeInfo.getStrName().trim());
		if(producttypes != null && producttypes.size()>0){
			for (Producttype d : producttypes) {
				if (!d.getId().equals(producttype.getId())) {
					return ajaxData.setFailureMsg("已存在产品类型名称为" + producttypeInfo.getStrName());
				}
			}			
		}
		
    	producttype.setStrName(producttypeInfo.getStrName());
    	productTypeDao.update(producttype);
    	configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.PRODUCT.getCode(),GlobalMessageUtil.getMessage("log.proType.update",new String[] {producttypeInfo.getStrName()}));
	    return ajaxData;
    }
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.product.service.IProductService#deleteProType(com.hikvision.finance.fms.modules.product.dto.ProducttypeInfo)
     */
    @Override
    public AjaxData deleteProType(ProducttypeInfo producttypeInfo) {
    	
    	AjaxData ajaxData = new AjaxData(false);
    	List<String> names = new ArrayList<String>();
    	//能删的只有节点没有产品且子孙节点没有产品
    	Producttype producttype = productTypeDao.get(producttypeInfo.getId());
    	List<Producttype> producttypes = productTypeDao.getLikeStrPath(producttype);//获取子孙产品类型节点
		for (Producttype pt : producttypes) {
			List<Product> products = productDao.findBy("pid", pt.getId());
			if (products != null && products.size() > 0) {
				ajaxData.setFailureMsg("产品类型或子类型下挂有产品,不能删除");
				return ajaxData;
			}
		}
		//删除子孙产品类型节点
    	for(Producttype pt : producttypes){
    		Producttype proName = productTypeDao.get(pt.getId());
    		names.add(proName.getStrName());
    		productTypeDao.delete(pt);
    	}
    	configLogDao.setLogContent(OperStatus.DELETE, LogModuledEnum.PRODUCT.getCode(),GlobalMessageUtil.getMessage("log.proType.delete",new String[] {ListUtil.listToStr(names)}));
	    return ajaxData;
    }
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.product.service.IProductService#addProduct(com.hikvision.finance.fms.modules.product.dto.ProductInfo)
     */
    @Override
    public AjaxData addProduct(ProductInfo productInfo) {
    	AjaxData ajaxData = new AjaxData(false);
		Product product = new Product();	
		// 校验产品名称唯一性
		AjaxData isExist1 = isExistPro(2,productInfo.getStrName());
		if (isExist1.getSuccess()) {
			ajaxData.setFailureMsg("已存在产品名称为" + productInfo.getStrName());
			return ajaxData;
		}
		
		// 校验产品编码唯一性
		AjaxData isExist = isExistPro(1,productInfo.getStrCode());
		if (isExist.getSuccess()) {
			ajaxData.setFailureMsg("已存在产品编码为" + productInfo.getStrCode());
			return ajaxData;
		}
		
		
		product.setStrCode(productInfo.getStrCode());//产品编码
		product.setPid(productInfo.getPid());//类型id
		product.setStrName(productInfo.getStrName());//产品名称
		product.setStrGrade(productInfo.getStrGrade());//发行机构
		product.setStrRisklevel(productInfo.getStrRisklevel());//风险等级
		product.setWordTemplate(null == productInfo.getWordTemplate()?"":productInfo.getWordTemplate());//话术模板wordTemplate
		product.setNenabled(SysDictionary.disable);//添加产品默认不可用 nEnabled
		UserSession userSession = SessionUtil.getUserSession();
		product.setImportUser(userSession.getUserId());//生成产品用户 importUser
		product.setImportTime(new Date());//生成产品时间  importTime
		//product.setProductStatus(ProductStatus.discheck);//默认未指定复核员
		product.setSaveYears(null == productInfo.getSaveYears()?null:productInfo.getSaveYears());//视频保存时间 saveYears
		product.setSaveType(null == productInfo.getSaveYears()?Constants.ProductKeepType.MONTH:productInfo.getSaveType());
		//到期日 strEndDate（2选1） 保障期限 guaranteeYears（2选1）
		if(productInfo.getStrEndDate()!=null){
			product.setStrEndDate(productInfo.getStrEndDate());
			product.setGuaranteeYears(null);
			product.setGuaranteeType(Constants.ProductKeepType.MONTH);
		}else{
			product.setGuaranteeType(productInfo.getGuaranteeType());
			product.setGuaranteeYears(productInfo.getGuaranteeYears());
		}
		productDao.save(product);
		configLogDao.setLogContent(OperStatus.ADD, LogModuledEnum.PRODUCT.getCode(),GlobalMessageUtil.getMessage("log.pro.add",new String[] {productInfo.getStrName()}));
		return ajaxData;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.product.service.IProductService#updateProduct(com.hikvision.finance.fms.modules.product.dto.ProductInfo)
     */
    @Override
    public AjaxData updateProduct(ProductInfo productInfo) {
    	AjaxData ajaxData = new AjaxData(false);
		Product product = productDao.get(productInfo.getId());
		try {
			String value = dataManager.get(CacheKeyPrefix.PREFIX_SYSCONFIG + SysConfigType.PRODUCT_REVIEW);
			if ("1".equals(value)) {
				UserSession userSession = SessionUtil.getUserSession();
				if((userSession.getUserId()!=1) && (!product.getImportUser().equals(userSession.getUserId()))){
					return ajaxData.setFailureMsg("非本人添加的产品,无权修改");
				}
			}
		} catch (DataLoadException e) {
			throw new ExpectedException("", "获取复核功能配置失败");
		}
		List<Product> products =  productDao.findBy("strName", productInfo.getStrName().trim());
		if(products != null && products.size()>0){
			for (Product d : products) {
				if (!d.getId().equals(product.getId())) {
					return ajaxData.setFailureMsg("已存在产品名称为" + productInfo.getStrName());
				}
			}			
		}
		
		List<Product> products1 =  productDao.findBy("strCode", productInfo.getStrCode().trim());
		if(products1 != null && products1.size()>0){
			for (Product d : products1) {
				if (!d.getId().equals(product.getId())) {
					return ajaxData.setFailureMsg("已存在产品编码为" + productInfo.getStrCode());
				}
			}			
		}
		
		
		product.setStrCode(productInfo.getStrCode());//产品编码
		product.setStrName(productInfo.getStrName());//产品名称
		product.setStrGrade(productInfo.getStrGrade());//发行机构
		product.setStrRisklevel(productInfo.getStrRisklevel());//风险等级
		product.setWordTemplate(null == productInfo.getWordTemplate()?"":productInfo.getWordTemplate());//话术模板wordTemplate
		product.setNenabled(SysDictionary.disable);//修改产品会设置成不可用 nEnabled
		product.setSaveYears(null == productInfo.getSaveYears()?null:productInfo.getSaveYears());//视频保存时间 saveYears
		product.setSaveType(null == productInfo.getSaveYears()?Constants.ProductKeepType.MONTH:productInfo.getSaveType());
		//到期日 strEndDate（2选1） 保障期限 guaranteeYears（2选1）
		if(productInfo.getStrEndDate()!=null){
			product.setStrEndDate(productInfo.getStrEndDate());
			product.setGuaranteeYears(null);
			product.setGuaranteeType(Constants.ProductKeepType.MONTH);
		}else{
			product.setStrEndDate(null);
			product.setGuaranteeYears(productInfo.getGuaranteeYears());
			product.setGuaranteeType(productInfo.getGuaranteeType());
		}
		productDao.update(product);
		configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.PRODUCT.getCode(),GlobalMessageUtil.getMessage("log.pro.update",new String[] {productInfo.getStrName()}));
		return ajaxData;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.product.service.IProductService#enableProducts(java.util.List)
     */
    @Override
	public AjaxData enableProducts(List<Integer> ids) {
		AjaxData ajaxData = new AjaxData(false);
		// 根据平台配置如果没有指定复核员功能，那么谁都可以启用 ,如果配置了指定复核员，则需要按规则执行
		List<String> names = new ArrayList<String>();
		try {
			String value = dataManager.get(CacheKeyPrefix.PREFIX_SYSCONFIG + SysConfigType.PRODUCT_REVIEW);
			if ("1".equals(value)) {
				// 判断管理员可启用 否则只有复核员可启用（如果复核员为空不能启用）
				UserSession userSession = SessionUtil.getUserSession();
				if (userSession.isAdmin()) {
					for (Integer id : ids) {
						Product product = productDao.get(id);
						product.setNenabled(SysDictionary.enable);
						productDao.update(product);
						names.add(product.getStrName());
					}
				} else {
					for (Integer id : ids) {
						Product product = productDao.get(id);
						if (product.getCheckUser() == null || !userSession.getUserId().equals(product.getCheckUser())) {
							ajaxData.setFailureMsg("您无权启用或含有无权启用的产品");
							return ajaxData;
						}
					}
					for (Integer id : ids) {
						Product product = productDao.get(id);
						product.setNenabled(SysDictionary.enable);
						productDao.update(product);
						names.add(product.getStrName());
					}
				}
			} else {
				for (Integer id : ids) {
					Product product = productDao.get(id);
					product.setNenabled(SysDictionary.enable);
					productDao.update(product);
					names.add(product.getStrName());
				}
			}
			
		} catch (DataLoadException e) {
			throw new ExpectedException("", "启用产品失败：获取复核功能配置失败");
		}
		if(ids.size()>5){
			configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.PRODUCT.getCode(),GlobalMessageUtil.getMessage("log.pro.enable.batch"));
		}else{
			configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.PRODUCT.getCode(),GlobalMessageUtil.getMessage("log.pro.enable",new String[] {ListUtil.listToStr(names)}));	
		}
		return ajaxData;
	}

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.product.service.IProductService#disableProducts(java.util.List)
     */
    @Override
    public AjaxData disableProducts(List<Integer> ids) {
    	AjaxData ajaxData = new AjaxData(false);
		// 根据平台配置如果没有指定复核员功能，那么谁都可以启用 ,如果配置了指定复核员，则需要按规则执行
		List<String> names = new ArrayList<String>();
		try {
			String value = dataManager.get(CacheKeyPrefix.PREFIX_SYSCONFIG + SysConfigType.PRODUCT_REVIEW);
			if (String.valueOf(SysDictionary.enable).equals(value)) {
				// 判断管理员可启用 否则只有复核员可启用禁用（如果复核员为空不能启用）
				UserSession userSession = SessionUtil.getUserSession();
				if (userSession.isAdmin()) {
					for (Integer id : ids) {
						Product product = productDao.get(id);
						product.setNenabled(SysDictionary.disable);
						productDao.update(product);
						names.add(product.getStrName());
					}
				} else {
					for (Integer id : ids) {
						Product product = productDao.get(id);
						if (product.getCheckUser() == null || !userSession.getUserId().equals(product.getCheckUser())) {
							ajaxData.setFailureMsg("您无权禁用或含有无权禁用的产品");
							return ajaxData;
						}
					}
					for (Integer id : ids) {
						Product product = productDao.get(id);
						product.setNenabled(SysDictionary.disable);
						productDao.update(product);
						names.add(product.getStrName());
					}
				}
			} else {
				for (Integer id : ids) {
					Product product = productDao.get(id);
					product.setNenabled(SysDictionary.disable);
					productDao.update(product);
					names.add(product.getStrName());
				}
			}
			
		} catch (DataLoadException e) {
			throw new ExpectedException("", "禁用产品失败：获取复核功能配置失败");
		}
		if(ids.size()>5){
			configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.PRODUCT.getCode(),GlobalMessageUtil.getMessage("log.pro.disable.batch"));
		}else{
			configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.PRODUCT.getCode(),GlobalMessageUtil.getMessage("log.pro.disable",new String[] {ListUtil.listToStr(names)}));	
		}
	    return ajaxData;
    }
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.product.service.IProductService#getCheckUsers(com.hikvision.finance.fms.modules.user.dto.UserInfo)
     */
    @Override
    public List<Users> getCheckUsers(UserInfo userInfo) {
    	String  userName = "";
	    if(userInfo!=null && userInfo.getStrName()!=null){
	    	userName = userInfo.getStrName();	
	    }
	    List<Users> users = usersDao.getCheckUsers(userName);
	    List<Users> userNes = new  ArrayList<Users>();
	    UserSession userSession = SessionUtil.getUserSession();
	    if(users!=null && users.size()>0){
		    for(Users user:users){
		    	if("admin".equals(user.getStrName()) || userSession.getUserId().equals(user.getId())){
		    		continue;
		    	}
		    	userNes.add(user);
		    }
	    }

	    return  userNes;
    }
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.product.service.IProductService#appointCheckUsers(java.util.List)
     */
    @Override
    public AjaxData addCheckUser(List<Integer> ids,UserInfo userInfo) {
	    // PJ03D2016040708 夏嘉蔚缺陷单 1.非自己添加的产品，能够重新指定复核员，不合理
    	StringBuilder names = new StringBuilder();
    	AjaxData ajaxData = new AjaxData(false);
    	 UserSession userSession = SessionUtil.getUserSession();
    	for(Integer id : ids){
    		Product product = productDao.get(id);
    		if((userSession.getUserId()!=1) && (!userSession.getUserId().equals(product.getImportUser()))){
    			ajaxData.setFailureMsg("所选含有非本人添加的产品,不能指定复核员");
    			 return ajaxData;
    		}
    	}
    	
    	for(Integer id : ids){
    		Product product = productDao.get(id);
    		product.setCheckUser(userInfo.getId());
    		productDao.update(product);
    		names.append(product.getStrName()+" ");
    	}
    	if(ids.size()>0){
    		configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.PRODUCT.getCode(),GlobalMessageUtil.getMessage("log.pro.check.batch"));
    	}else{
    		configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.PRODUCT.getCode(),GlobalMessageUtil.getMessage("log.pro.check",new String[] {names.toString()}));	
    	}
	    return ajaxData;
    }
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.product.service.IProductService#getCertDate(com.hikvision.finance.fms.modules.product.dto.CrtificateInfo)
     */
    @Override
	public List<CrtificateInfo> getCertDate(CrtificateInfo crtificateInfo) {
		List<Crtificate> crtificates = null;
		// 获取证书列表，如果证书名不为空则需要模糊查询
		if (crtificateInfo != null && StringUtils.isNotBlank(crtificateInfo.getStrName())) {
			crtificates = certDao.getCertByName(crtificateInfo.getStrName());
		} else {
			crtificates = certDao.getAll();
		}
		List<CrtificateInfo> crtificateInfos = new ArrayList<CrtificateInfo>();
		for (Crtificate crtificate : crtificates) {
			CrtificateInfo info = new CrtificateInfo();
			List<Product> productList = new ArrayList<Product>();
			try {
				BeanUtils.copyNotNullProperties(info, crtificate);
			} catch (Exception e) {
				throw new ExpectedException("", "拷贝bean失败[getCertDate]");
			}
			productList = certProductMappingDao.getProductbyCertId(crtificate.getId());
			StringBuilder builder = new StringBuilder();
			for(Product product :productList){
				builder.append(product.getStrName()+" ; ");
			}
			info.setProductNameList(builder.toString());
			crtificateInfos.add(info);
		}
		return crtificateInfos;
	}
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.product.service.IProductService#addCertManager(com.hikvision.finance.fms.modules.product.dto.CrtificateInfo)
     */
    @Override
	public AjaxData addCertManager(String jsonStr) {
		AjaxData ajaxData = new AjaxData(false);
		CrtificateDto certDto = JsonUtils.json2Object(jsonStr, CrtificateDto.class);
		CrtificateInfo crtificateInfo = certDto.getCertInfo();
		
		List<Crtificate> crtificates =  certDao.findBy("strName", crtificateInfo.getStrName().trim());
		if(crtificates != null && crtificates.size()>0){
			return ajaxData.setFailureMsg("已存在证书名称为" + crtificateInfo.getStrName());
		}
		List<Crtificate> crtificates1 =  certDao.findBy("strCode", crtificateInfo.getStrCode().trim());
		if(crtificates1 != null && crtificates1.size()>0){
			return ajaxData.setFailureMsg("已存在证书编码为" + crtificateInfo.getStrCode());
		}
		
		Crtificate crtificate = new Crtificate();
		crtificate.setStrCode(crtificateInfo.getStrCode());
		crtificate.setStrName(crtificateInfo.getStrName());
		Integer certId = (Integer)certDao.save(crtificate);// 保存证书信息
		configLogDao.setLogContent(OperStatus.ADD, LogModuledEnum.PRODUCT.getCode(),GlobalMessageUtil.getMessage("log.cert.add",new String[] {crtificateInfo.getStrName()}));
		// 1.1：保存证书下的产品
		List<CrtificateProductMappingInfo> crtificateproductmappingInfos = certDto.getCertProductList();
		StringBuffer buffer1 = new StringBuffer();
		StringBuffer buffer2 = new StringBuffer();
		int i=0,j=0;
		if (crtificateproductmappingInfos != null) {
			for (CrtificateProductMappingInfo crtificateproductmappingInfo : crtificateproductmappingInfos) {
				if(crtificateproductmappingInfo.getStatus()){//添加操作时：节点为true的添加，false的不用管
					buffer1.append("("+crtificateproductmappingInfo.getNproductId()+", "+certId+"),");
					i++;
				}
			}
		}
		if(i>0){
			String strsql = buffer1.deleteCharAt(buffer1.length() - 1).toString();
			certProductMappingDao.saveMapping(strsql);	
		}
		//1.2：如果有类型节点传过来，则遍历类型子孙节点的产品，有产品就维护产品证书的关系
		List<ProducttypeInfo> producttypeInfos = certDto.getProducttypeList();
		
		if ( producttypeInfos != null && producttypeInfos.size()>0) {
			//组装所有产品的map
			List<Product> productList = productDao.getAll();
			Map<Integer,List<Product>> typeProductMap = TypeProductMap(productList);
			for (ProducttypeInfo producttypeInfo : producttypeInfos) {
				if (producttypeInfo.getStatus()) {//添加操作时：节点为true的添加，false的不用管
					// 获取每个类型节点
					Producttype producttype = productTypeDao.get(producttypeInfo.getId());
					// 获取每个类型节点的子孙节点
					List<Producttype> producttypes = productTypeDao.getLikeStrPath(producttype);
					// 对每个类型节点的子孙节点找到产品
					for (Producttype pt : producttypes) {
						List<Product> products = typeProductMap.get(pt.getId());
						// 如果子孙节点有产品(不为null)则添加证书和产品的关系
						if (products != null) {
							for (Product product : products) {
								buffer2.append("("+product.getId()+", "+certId+"),");
								j++;
							}
						}
					}
				}
			}
			if(j>0){
				String strsql2 = buffer2.deleteCharAt(buffer2.length() - 1).toString();
				certProductMappingDao.saveMapping(strsql2);	
			}
		}
		return ajaxData;
	}
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.product.service.IProductService#getCertByCertId(java.lang.Integer)
     */
    @Override
	public CrtificateInfo getCertByCertId(Integer certId) {
		Crtificate crtificate = certDao.get(certId);
		CrtificateInfo info = new CrtificateInfo();
		try {
			BeanUtils.copyNotNullProperties(info, crtificate);
		} catch (Exception e) {
			throw new ExpectedException("", "拷贝bean失败[getCertByCertId]");
		}
		return info;
	}

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.product.service.IProductService#updateCert(com.hikvision.finance.fms.modules.product.dto.CrtificateInfo)
     */
    @Override
    public AjaxData updateCert(String jsonStr) {
		AjaxData ajaxData = new AjaxData(false);
		CrtificateDto certDto = JsonUtils.json2Object(jsonStr, CrtificateDto.class);
		CrtificateInfo crtificateInfo = certDto.getCertInfo();
		Crtificate crtificate = certDao.get(crtificateInfo.getId());
		Integer certId = crtificateInfo.getId();
		List<Crtificate> crtificates =  certDao.findBy("strName", crtificateInfo.getStrName().trim());
		if(crtificates != null && crtificates.size()>0){
			for (Crtificate d : crtificates) {
				if (!d.getId().equals(crtificateInfo.getId())) {
					return ajaxData.setFailureMsg("已存在证书名称为" + crtificateInfo.getStrName());
				}
			}			
		}
		List<Crtificate> crtificates1 =  certDao.findBy("strCode", crtificateInfo.getStrCode().trim());
		if(crtificates1 != null && crtificates1.size()>0){
			for (Crtificate d1 : crtificates1) {
				if (!d1.getId().equals(crtificateInfo.getId())) {
					return ajaxData.setFailureMsg("已存在证书编码为" + crtificateInfo.getStrCode());
				}
			}			
		}
		crtificate.setStrCode(crtificateInfo.getStrCode());
		crtificate.setStrName(crtificateInfo.getStrName());
		certDao.update(crtificate);// 更新证书信息
		configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.PRODUCT.getCode(),GlobalMessageUtil.getMessage("log.cert.update",new String[] {crtificateInfo.getStrName()}));
		// 1.1：修改证书下的产品	
		StringBuffer buffer1 = new StringBuffer();
		StringBuffer buffer2 = new StringBuffer();
		StringBuffer buffer3 = new StringBuffer();
		StringBuffer buffer4 = new StringBuffer();
		int i=0,j=0,k=0,m=0;
		List<CrtificateProductMappingInfo> crtificateproductmappingInfos = certDto.getCertProductList();
		if (crtificateproductmappingInfos != null && crtificateproductmappingInfos.size()>0) {
			for (CrtificateProductMappingInfo crtificateproductmappingInfo : crtificateproductmappingInfos) {
				Boolean status = crtificateproductmappingInfo.getStatus();
				buffer1.append(crtificateproductmappingInfo.getNproductId()+",");
				k++;
				//certProductMappingDao.deleteBy(new String[]{"ncrtificateId","nproductId"}, new Object[]{certId,crtificateproductmappingInfo.getNproductId()});
				//true 添加 false 删除
				if(status){
					buffer2.append("("+crtificateproductmappingInfo.getNproductId()+", "+certId+"),");
					i++;	
				}
			}
			if(k>0){
				certProductMappingDao.deleteMapping(certId, buffer1.deleteCharAt(buffer1.length() - 1).toString());	
			}
			if(i>0){
				String strsql1 = buffer2.deleteCharAt(buffer2.length() - 1).toString();
				certProductMappingDao.saveMapping(strsql1);	
			}
		}
		// 1.2：如果有类型节点传过来，则遍历类型子孙节点的产品，有产品就修改维护产品证书的关系
		List<ProducttypeInfo> producttypeInfos = certDto.getProducttypeList();
		if (producttypeInfos != null && producttypeInfos.size()>0) {
			// 组装所有产品的map
			List<Product> productList = productDao.getAll();
			Map<Integer, List<Product>> typeProductMap = TypeProductMap(productList);
			for (ProducttypeInfo producttypeInfo : producttypeInfos) {
				Boolean status = producttypeInfo.getStatus();
				// 获取每个类型节点
				Producttype producttype = productTypeDao.get(producttypeInfo.getId());
				// 获取每个类型节点的子孙节点
				List<Producttype> producttypes = productTypeDao.getLikeStrPath(producttype);
				// 对每个类型节点的子孙节点找到产品
				for (Producttype pt : producttypes) {
					List<Product> products = typeProductMap.get(pt.getId());
					// 如果子孙节点有产品(不为null)则添加证书和产品的关系
					if (products != null) {
						for (Product product : products) {
							buffer3.append(product.getId()+",");
							m++;
							//certProductMappingDao.deleteBy(new String[] {"ncrtificateId", "nproductId"}, new Object[] {certId, product.getId()});
							if (status) {
								buffer4.append("("+product.getId()+", "+certId+"),");
								j++;
							} 
						}
					}
				}
			}
			if(m>0){
				certProductMappingDao.deleteMapping(certId, buffer3.deleteCharAt(buffer3.length() - 1).toString());	
			}
			if(j>0){
				String strsql2 = buffer4.deleteCharAt(buffer4.length() - 1).toString();
				certProductMappingDao.saveMapping(strsql2);	
			}
		}
		return ajaxData;
	}

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.product.service.IProductService#deleteCert(java.lang.Integer)
     */
    @Override
    public AjaxData deleteCert(Integer certId) {
    	AjaxData ajaxData = new AjaxData(false);
    	//先判断证书有没有关联产品，有关联得不能删除证书
    	Crtificate crtificate = certDao.get(certId);
    	List<Crtificateproductmapping> crtificateproductmappings = certProductMappingDao.findBy("ncrtificateId", crtificate.getId());
    	if(crtificateproductmappings!=null && crtificateproductmappings.size()>0){
    		ajaxData.setFailureMsg("该证书关联了相关产品,不能直接删除");
    		return ajaxData;
    	}
    	//删除用户和证书的关系
    	usercrtificatemappingDao.deleteBy("ncrtificateId", certId);
    	certDao.deleteById(certId);
    	configLogDao.setLogContent(OperStatus.DELETE, LogModuledEnum.PRODUCT.getCode(),GlobalMessageUtil.getMessage("log.cert.delete",new String[] {crtificate.getStrName()}));
	    return ajaxData;
    }
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.product.service.IProductService#getProTypeById(java.lang.Integer)
     */
    @Override
    public ProducttypeInfo getProTypeById(Integer productTypeId) {
    	Producttype producttype = productTypeDao.get(productTypeId);
		ProducttypeInfo info = new ProducttypeInfo();
		try {
			BeanUtils.copyNotNullProperties(info, producttype);
		} catch (Exception e) {
			throw new ExpectedException("", "拷贝bean失败[getProTypeById]");
		}

		if (Constants.ROOT_PRO_ID != productTypeId) {
			String nparentName = productTypeDao.get(info.getPid()).getStrName();
			info.setNparentName(nparentName);
		}
		return info;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.product.service.IProductService#getProductById(java.lang.Integer)
     */
    @Override
	public ProductInfo getProductById(Integer productId) {
		Product product = productDao.get(productId);
		ProductInfo info = new ProductInfo();
		try {
			BeanUtils.copyNotNullProperties(info, product);
		} catch (Exception e) {
			throw new ExpectedException("", "拷贝bean失败[getProductById]");
		}		
		String nparentName = productTypeDao.get(product.getPid()).getStrName();
		info.setProductTypeName(nparentName);		
		return info;
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.product.service.IProductService#getCertListConn(java.lang.Integer)
	 */
    @Override
	public List<CrtificateInfo> getCertListConn(Integer productId) {
		List<CrtificateInfo> crtificateInfos = new ArrayList<CrtificateInfo>();
		List<Crtificateproductmapping> cpms = certProductMappingDao.findBy("nproductId", productId);
		for (Crtificateproductmapping cpm : cpms) {
			Crtificate crtificate = this.certDao.get(cpm.getNcrtificateId());
			CrtificateInfo info = new CrtificateInfo();
			try {
				BeanUtils.copyNotNullProperties(info, crtificate);
			} catch (Exception e) {
				throw new ExpectedException("", "拷贝bean失败[getCertListConn]");
			}
			crtificateInfos.add(info);
		}
		return crtificateInfos;
	}

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.product.service.IProductService#getCertListConnOut(java.lang.Integer)
     */
    @Override
	public List<CrtificateInfo> getCertListConnOut(Integer productId) {
		List<CrtificateInfo> crtificateInfos = new ArrayList<CrtificateInfo>();
		// 获取未关联的证书=全部证书id-已关联的证书
		List<Crtificateproductmapping> cpms = certProductMappingDao.findBy("nproductId", productId);
		List<Crtificate> allCertList = certDao.getAll();
		List<Integer> allCertListIds = buildCertIds(allCertList);// 全部证书的ids
		List<Integer> cpmListIds = buildCertIdsByCp(cpms);// 关联证书的ids
		allCertListIds.removeAll(cpmListIds);// 全部证书的ids-关联证书的ids
		
		// 通过id获取未关联证书的信息
		for (Integer cid : allCertListIds) {
			Crtificate crtificate = certDao.get(cid);
			CrtificateInfo info = new CrtificateInfo();
			try {
				BeanUtils.copyNotNullProperties(info, crtificate);
			} catch (Exception e) {
				throw new ExpectedException("", "拷贝bean失败[getCertListConnOut]");
			}
			crtificateInfos.add(info);
		}
		
		return crtificateInfos;
	}
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.product.service.IProductService#addOrDelCertConnect(java.lang.String)
     */
	@Override
	public AjaxData addOrDelCertConnect(Integer productId, List<Integer> ids) {
		AjaxData ajaxData = new AjaxData(false);
		// 先删除后添加
		certProductMappingDao.deleteBy("nproductId", productId);
		if (ids != null && ids.size() > 0) {
			for (Integer certId : ids) {
				Crtificateproductmapping crtificateproductmapping = new Crtificateproductmapping();
				crtificateproductmapping.setNcrtificateId(certId);
				crtificateproductmapping.setNproductId(productId);
				certProductMappingDao.save(crtificateproductmapping);
			}
		}
		Product product = productDao.get(productId);
		configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.PRODUCT.getCode(),GlobalMessageUtil.getMessage("log.pro.cert",new String[] {product.getStrName()}));
		return ajaxData;
	}
    /***************************************************私有方法***************************************************/
	/**
	 * 获取排序号
	 * @author jinxindong 2016年2月26日 上午9:39:20
	 * @return
	 */
	private Integer getNsort() {
		ProductTypeQo proTypeQo = new ProductTypeQo();
		Integer nsort = (Integer)productTypeDao.maxProperty("nsortId", proTypeQo) + 1;
		return nsort;
	}
	
	/**
	 * 获取组织路径编码
	 * @author jinxindong 2016年3月22日 上午9:19:48
	 * @param panrentId
	 * @param areaCode
	 * @return
	 */
    private String getStrPath(Integer panrentId, String areaCode) {
		Producttype producttype = productTypeDao.get(panrentId);
		if (producttype != null && producttype.getStrPath() != null) {
			areaCode = producttype.getStrPath() + "-" + areaCode;
		}
		return areaCode;
	}
    
    /**
     * 获取areaCode
     * @author jinxindong 2016年3月22日 上午10:01:42
     * @param nparentId
     * @return
     */
    private String getRegSeril(Integer nparentId) {
		String ex = null;
		String code = null;
		boolean isExistCode = false;
		// 控制单元
		if (nparentId == 1) {
			ex = "100000";
		} else {
			Producttype reg = (Producttype)productTypeDao.get(nparentId);
			ex = StringUtilExpand.getLengthStr(reg.getStrInterAreaCode(), 6);
		}
		// B规则：区域代码： 1000000000+6位随机数+90
		code = ex + "0000" + StringUtilExpand.getSixRandomSeril() + 90;
		// 查找是否存在
		ProductTypeQo productTypeQo = new ProductTypeQo();
		productTypeQo.setStrInterAreaCode(code);
		Producttype producttype = productTypeDao.queryUnique(productTypeQo);
		if (producttype != null) {
			isExistCode = true;
		}
		// 如果存在，重新生成
		while (isExistCode) {
			code = ex + "0000" + StringUtilExpand.getSixRandomSeril() + 90;
			productTypeQo.setStrInterAreaCode(code);
			producttype = productTypeDao.queryUnique(productTypeQo);
			if (producttype != null) {
				isExistCode = true;
			}
		}
		return code;
	}
    

	/**
	 * 组装map <产品类型id,产品list>
	 * @author jinxindong 2016年3月23日 下午6:26:10
	 * @param productList
	 * @return
	 */
	private Map<Integer,List<Product>> TypeProductMap(List<Product> productList){
		Map<Integer, List<Product>> productMapChildren = Maps.newHashMap();
		for (Product product : productList) {
			Integer parentId = product.getPid();
			List<Product> list = productMapChildren.get(parentId);
			if (list == null) {
				list = new ArrayList<Product>();
				productMapChildren.put(parentId, list);
			}
			list.add(product);
		}
		return productMapChildren;
		
	}
	
	/**
	 * 传入 证书信息  获取证书列表的id list
	 * @author jinxindong 2016年3月25日 下午12:32:52
	 * @param certProductList
	 * @return
	 */
	private List<Integer> buildCertIds(List<Crtificate> certProductList){
		List<Integer> integers = new ArrayList<Integer>();
		for(Crtificate info:certProductList){
			Integer cid = info.getId();
			integers.add(cid);
		}
		return integers;
	}
	
	/**
	 * 传入 证书产品关联信息 获取证书列表的id list
	 * @author jinxindong 2016年3月25日 下午12:32:52
	 * @param certProductList
	 * @return
	 */
	private List<Integer> buildCertIdsByCp(List<Crtificateproductmapping> certProductList){
		List<Integer> integers = new ArrayList<Integer>();
		for(Crtificateproductmapping info:certProductList){
			Integer cid = info.getNcrtificateId();
			integers.add(cid);
		}
		return integers;
	}
	
	/**
	 * 验证是否存在相同条件的其他类型
	 * @author jinxindong 2016年3月28日 下午3:48:43
	 * @param type
	 * @param value
	 * @param notId
	 * @return
	 */
	@SuppressWarnings("unused")
    private AjaxData isExistReg(Integer type, String value, Integer notId) {
		AjaxData ajaxData = new AjaxData(false);
		ProductTypeQo qo = new ProductTypeQo();
		switch (type) {
			case 1:
				qo.setStrName(value);
				break;
			case 2:
				qo.setStrInterAreaCode(value);
				break;
			default:
				break;
		}
		Producttype reg = productTypeDao.queryUnique(qo);
		if (reg == null) {
			ajaxData.setSuccess(false);
		}
		return ajaxData;
	}
	
	/**
	 * 判断产品编码、产品名称是否唯一
	 * @author jinxindong 2016年4月22日 下午3:28:28
	 * @param value
	 * @return
	 */
	private AjaxData isExistPro(Integer type, String value) {
		AjaxData ajaxData = new AjaxData(false);
		List<Product> products = null;
		if(type == 1){
			products = productDao.findBy("strCode", value);
		}else if(type == 2){
			products = productDao.findBy("strName", value);
		}
		if (products == null || products.size()<1) {
			ajaxData.setSuccess(false);
		}
		return ajaxData;
	}
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.product.service.IProductService#exportProductsExcel(com.hikvision.finance.fms.modules.product.dto.ProductInfo, java.lang.String[], java.lang.String[], java.lang.String)
     */
    @Override
	public boolean exportProductsExcel(ProductInfo productInfo, String[] titleName, String[] attrName, String path) {
		// 1，依次准备标题、标题映射属性、Sheet以及对应内容
		List<String[]> titleNames = new ArrayList<String[]>();
		List<String[]> attrNames = new ArrayList<String[]>();
		LinkedHashMap<String, List<?>> map = new LinkedHashMap<String, List<?>>();
		titleNames.add(titleName);
		attrNames.add(attrName);
		List<ProductInfo> productInfos = productDao.exportProductsExcel(productInfo);
		map.put("Sheet1", productInfos);
		// 2，创建Excel的数据对象并导出表格
		ExcelUtil.ExcelExportData setInfo = new ExcelUtil.ExcelExportData();
		setInfo.setColumnNames(titleNames);
		setInfo.setFieldNames(attrNames);
		setInfo.setDataMap(map);
		boolean exportResult = false;
		try {
			exportResult = ExcelUtil.export2File(setInfo, path);
			configLogDao.setLogContent(OperStatus.EXPORT, LogModuledEnum.PRODUCT.getCode(),GlobalMessageUtil.getMessage("log.pro.export"));
		} catch (Exception e) {
			e.printStackTrace();
		}
		return exportResult;
	}
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.product.service.IProductService#downLoadTemplate(com.hikvision.finance.fms.modules.product.dto.ProductInfo, java.lang.String[], java.lang.String[], java.lang.String)
     */
    @Override
	public boolean downLoadTemplate(String path) {
		// 输入输出流
		InputStream is = null;
		FileOutputStream out = null;
		try {
			is = new FileInputStream(path);
			// 创建工作空间
			Workbook wb = WorkbookFactory.create(is);
			Sheet sheet = wb.getSheetAt(0);
			// 查询下拉框内容
			String[] riskLevel = {
									Constants.RiskLevel.RISKLEVEL_ONE,Constants.RiskLevel.RISKLEVEL_TWO,
									Constants.RiskLevel.RISKLEVEL_THREE,Constants.RiskLevel.RISKLEVEL_FOUR,
									Constants.RiskLevel.RISKLEVEL_FIVE,Constants.RiskLevel.RISKLEVEL_SIX,
									Constants.RiskLevel.RISKLEVEL_NONE
								};
			sheet = setHSSFValidation(sheet, riskLevel, 1, 2000, 4, 4);
			 //3，创建文件（输出目录）
			String rootpath = ServletActionContext.getServletContext().getRealPath("/");
			String path1 = Constants.FILEUPLOAD + "excel/" + "productTemplate.xls";
			File saveFile = new File(rootpath+Constants.FILEUPLOAD + "excel/");
			if(!saveFile.exists()) {
				saveFile.mkdirs();
			}
			out = new FileOutputStream(rootpath+path1);
			//写出文件
			wb.write(out);						
		} catch (Exception e) {
			e.printStackTrace();
			throw new ExpectedException("", "下载模板出错:" + e.getMessage());
		}finally {
			try {
				if (null != is) {
					is.close();
				}
				if (null != out) {
					out.close();
				}
            } catch (IOException e) {
	            e.printStackTrace();
            }
		}
		return true;
	}
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.product.service.IProductService#addProFromExcel(java.io.File, java.lang.String, com.hikvision.finance.fms.common.excel.model.ExcelHead)
     */
    @Override
	public Map<String, Object> addProFromExcel(File orgFile, String orgFileFileName, ExcelHead excelHead) {
		Map<String, Object> prosMap = Maps.newHashMap();
		prosMap.put("errorNo", true);
		List<ProductInfo> pros = Lists.newArrayList();
		ExcelHelper<ProductInfo> excelHelper = ExcelHelperFactory.createExcelHelper(orgFileFileName);
		pros = excelHelper.importToObjectList(excelHead, orgFile, ProductInfo.class);
		Map<String, ProductInfo> map = new HashMap<String, ProductInfo>();
		Map<String, ProductInfo> map1 = new HashMap<String, ProductInfo>();
		// 校验导入表格中，编码是否有重复
		if (null != pros) {// 因coverity检测出空指针判断缺陷添加
			for (ProductInfo info : pros) {
				String code = info.getStrCode();
				String strName = info.getStrName();
				ProductInfo info2 = map.get(code);
				ProductInfo info3 = map1.get(strName);
				if (info2 != null) {
					prosMap.put("errorNo", false);
					prosMap.put("errorInfo", "导入表格中产品编码有重复,无法导入");
					break;
					// throw new ExpectedException("", "导入表格中编码有重复,无法导入");
				}
				if (info3 != null) {
					prosMap.put("errorNo", false);
					prosMap.put("errorInfo", "导入表格中产品名称有重复,无法导入");
					break;
					// throw new ExpectedException("", "导入表格中编码有重复,无法导入");
				}
				map.put(code, info);
				map1.put(strName, info);
			}
		}
		
		prosMap.put("prosList", pros);
		return prosMap;
	}
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.product.service.IProductService#addAndValidateExcel(java.io.File, java.lang.String, com.hikvision.finance.fms.common.excel.model.ExcelHead)
     */
    @Override
	public AjaxData addAndValidateExcel(File proFile, String proFileFileName, ExcelHead excelHead) {
		AjaxData ajaxData = new AjaxData(false);
		Map<String, Object> prosMap = Maps.newHashMap();
		List<ProductInfo> pros = Lists.newArrayList();
		try {
			ExcelHelper<ProductInfo> excelHelper = ExcelHelperFactory.createExcelHelper(proFileFileName);
			pros = excelHelper.importToObjectList(excelHead, proFile, ProductInfo.class);
			List<Object> mapNameList = Lists.newArrayList();// 存放表格内产品名称
			List<Object> mapCodeList = Lists.newArrayList();// 存放表格内产品编码
			if (pros == null || pros.size() < 1) {
				ajaxData.setFailureMsg("导入的表格内关键数据缺失或无数据");
				return ajaxData;
			}
			for (ProductInfo info : pros) {
				// 判断必填项是否为空
				if (StringUtils.isBlank(info.getProductTypeName())) {
					ajaxData.setFailureMsg("产品类型列数据不全");
					return ajaxData;
				}
				if (StringUtils.isBlank(info.getStrName())) {
					ajaxData.setFailureMsg("产品名称列数据不全");
					return ajaxData;
				}
				if (StringUtils.isBlank(info.getStrCode())) {
					ajaxData.setFailureMsg("产品代码列数据不全");
					return ajaxData;
				}
				if (StringUtils.isBlank(info.getStrGrade())) {
					ajaxData.setFailureMsg("发行机构列数据不全");
					return ajaxData;
				}
				if (info.getSaveYears() == null) {
					ajaxData.setFailureMsg("视频保存时间列数据不全");
					return ajaxData;
				}
				
				// 校验表格内产品名称和产品编码是否重复
				if (mapNameList.contains(info.getStrName())) {
					ajaxData.setFailureMsg("产品名称列表中存在重复数据：" + info.getStrName());
					return ajaxData;
				}
				
				if (mapCodeList.contains(info.getStrCode())) {
					ajaxData.setFailureMsg("产品编码列表中存在重复数据：" + info.getStrCode());
					return ajaxData;
				}
				mapNameList.add(info.getStrName());
				mapCodeList.add(info.getStrCode());
			}
		} catch (Exception e) {
			ajaxData.setFailureMsg("导入文件的内容格式有误,请检查文件内容是否合规");
			System.out.println(e.getMessage());
			return ajaxData;
		}
		prosMap.put("prosList", pros);
		ajaxData.setData(prosMap);
		return ajaxData;
	}
    
    
    /**
     * 设置下拉
     * @author jinxindong 2016年4月22日 上午11:38:21
     * @param sheet
     * @param textlist
     * @param firstRow
     * @param endRow
     * @param firstCol
     * @param endCol
     * @return
     */
    public Sheet setHSSFValidation(Sheet sheet, String[] textlist, int firstRow, int endRow, int firstCol, int endCol) {
		// 加载下拉列表内容
		DVConstraint constraint = DVConstraint.createExplicitListConstraint(textlist);
		// 设置数据有效性加载在哪个单元格上,四个参数分别是：起始行、终止行、起始列、终止列
		CellRangeAddressList regions = new CellRangeAddressList(firstRow, endRow, firstCol, endCol);
		// 数据有效性对象
		HSSFDataValidation data_validation_list = new HSSFDataValidation(regions, constraint);
		sheet.addValidationData(data_validation_list);
		return sheet;
	}
 
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.product.service.IProductService#addAllProducts(java.util.List)
     */
    @Override
    public AjaxData addAllProducts(List<ProductInfo> productInfos) {
    	AjaxData ajaxData = new AjaxData(false);
    	List<Product> objects = new ArrayList<Product>();
    	StringBuilder builder = new StringBuilder();
    	//查询产品类型组装map
    	Map<String, Producttype> map = new HashMap<String, Producttype>();
    	List<Producttype> producttypes = productTypeDao.getAll();
    	for(Producttype producttype: producttypes){
    		map.put(producttype.getStrName(), producttype);
    	}
    	//查询产品编码组装类型
    	Map<String, Product> productMap = new HashMap<String, Product>();
    	Map<String, Product> productMap1 = new HashMap<String, Product>();
    	List<Product>  products = productDao.getAll();
    	for(Product product: products){
    		productMap.put(product.getStrCode(), product);
    		productMap1.put(product.getStrName(), product);
    	}
    	//数据有效性校验
    	for(ProductInfo  productInfo:productInfos){
    		if(!validateCoulm(productInfo)){
    			ajaxData.setFailureMsg("文件内容含非法数据或有特殊字符\\/:*?\"<|'%>&");
    			return ajaxData;
    		}	
    	}
    	int sucessNum=0;
    	int faileNum=0;
    	for(ProductInfo  productInfo:productInfos){
    		//判断编码是否和数据库重复 如果为null表示数据库中不存在，可添加
    		Product product1 =productMap.get(productInfo.getStrCode());
    		if(product1 != null){
    			faileNum++;
    			builder.append(productInfo.getStrName()+",产品编码:"+productInfo.getStrCode()+" 已存在&");
    			continue;
    		}
    		
    		Product product2 =productMap1.get(productInfo.getStrName());
    		if(product2 != null){
    			faileNum++;
    			builder.append(productInfo.getStrName()+",产品名称:"+productInfo.getStrName()+" 已存在&");
    			continue;
    		}
    		//对导入的产品类型 进行判断
			Producttype producttype = map.get(productInfo.getProductTypeName());
			if (producttype == null) {
				faileNum++;
				builder.append(productInfo.getStrName()+",产品类型:"+productInfo.getProductTypeName() + " 不存在&");
				continue;
			}
			
    		Product product = new Product();
    		product.setPid(map.get(productInfo.getProductTypeName()).getId());//类型id
    		product.setStrCode(productInfo.getStrCode());//产品编码   		
    		product.setStrName(productInfo.getStrName());//产品名称
    		product.setStrGrade(productInfo.getStrGrade());//发行机构
    		product.setStrRisklevel(productInfo.getStrRisklevel());//风险等级
    		product.setWordTemplate(null == productInfo.getWordTemplate()?"":productInfo.getWordTemplate());//话术模板wordTemplate
    		product.setNenabled(SysDictionary.disable);//添加产品默认不可用 nEnabled
    		UserSession userSession = SessionUtil.getUserSession();
    		product.setImportUser(userSession.getUserId());//生成产品用户 importUser
    		product.setImportTime(new Date());//生成产品时间  importTime
    		//product.setProductStatus(ProductStatus.discheck);//默认未指定复核员
    		product.setSaveYears(0 == productInfo.getSaveYears()?null:productInfo.getSaveYears());//视频保存时间 saveYears
    		product.setSaveType(null == productInfo.getSaveYears()?Constants.ProductKeepType.MONTH:productInfo.getSaveType());
    		//到期日 strEndDate（2选1） 保障期限 guaranteeYears（2选1）
    		if(productInfo.getStrEndDate()!=null){
    			product.setStrEndDate(productInfo.getStrEndDate());
    			product.setGuaranteeYears(null);
    			product.setGuaranteeType(Constants.ProductKeepType.MONTH);
    		}else{
    			product.setGuaranteeType(productInfo.getGuaranteeType());
    			product.setGuaranteeYears(productInfo.getGuaranteeYears());
    		}
    		
    		objects.add(product);
    		sucessNum++;
    	}
    	productDao.saveAll(objects);
    	if (sucessNum > 0) {
    		configLogDao.setLogContent(OperStatus.IMPORT, LogModuledEnum.PRODUCT.getCode(),GlobalMessageUtil.getMessage("log.pro.import"));	
    	}else{
    		ajaxData.setSuccess(false);
    	}
    	if(faileNum>0){
    		ajaxData.setSuccess(true);
    		ajaxData.setMsg("导入失败的产品：失败个数( "+faileNum+" )个。&"+builder.toString());
    	}   
		return ajaxData;
    }

    /*
     * 通过产品证书id组成的字符串和产品类型Id获取可用的产品，for采集web&cs客户端
     * @see com.hikvision.finance.fms.webdevice.contact.service.IContactService#getPermitProductList()
     */
    public List<Product> getPermitProductList(){
		UserSession usersession = SessionUtil.getUserSession();
		Integer userId = usersession.getUserId();
		List<Usercrtificatemapping> usercrtificatemappingList = usercrtificatemappingDao.findBy("userId", userId);
		String crtificateIds = new String();
		if(usercrtificatemappingList!=null&&usercrtificatemappingList.size()>0){// 用户有关联证书，则获取证书
			List<Integer> crtificateIdList = new ArrayList<Integer>();
			for (Usercrtificatemapping usercrtificatemapping : usercrtificatemappingList) {
				crtificateIdList.add(usercrtificatemapping.getNcrtificateId());
			}
			crtificateIds = ListUtil.listToStr(crtificateIdList);// 证书id
		}
		List<Product> permitProductList = productDao.getPermitProductList(crtificateIds, null, null, null);// 要显示的产品List
    	return permitProductList;
    }
	
    
    /**
     * 获取指定组织的当前层级
     * @author jinxindong 2016年5月4日 下午3:14:30
     * @param regionId
     * @return
     */
    private int getOrgLevel(int regionId) {
    	return productTypeDao.getRegLevel(regionId);
    }
    
    /**
     * 校验导入的数据
     * @author jinxindong 2016年6月15日 下午5:02:37
     * @param productInfo
     * @return
     */
	private Boolean validateCoulm(ProductInfo productInfo) {
		if(ValidateUtil.validateChar(productInfo.getProductTypeName())){
			return false;
		}
		if(ValidateUtil.validateChar(productInfo.getStrCode())){
			return false;
		}
		if(ValidateUtil.validateChar(productInfo.getStrName())){
			return false;
		}
		if(ValidateUtil.validateChar(productInfo.getStrGrade())){
			return false;
		}
		if(ValidateUtil.validateChar(productInfo.getProductTypeName())){
			return false;
		}
		if(ValidateUtil.validateChar(productInfo.getWordTemplate())){
			return false;
		}		
		return true;
	}
	// =======================================getter/setter================================================================
	public IProductTypeDao getProductTypeDao() {
		return productTypeDao;
	}
	
	public void setProductTypeDao(IProductTypeDao productTypeDao) {
		this.productTypeDao = productTypeDao;
	}
	
	public IProductDao getProductDao() {
		return productDao;
	}
	
	public void setProductDao(IProductDao productDao) {
		this.productDao = productDao;
	}
	
	public ICertDao getCertDao() {
		return certDao;
	}
	
	public void setCertDao(ICertDao certDao) {
		this.certDao = certDao;
	}
	
	public IUsersDao getUsersDao() {
		return usersDao;
	}
	
	public void setUsersDao(IUsersDao usersDao) {
		this.usersDao = usersDao;
	}
	
	public ICertProductMappingDao getCertProductMappingDao() {
		return certProductMappingDao;
	}
	
	public void setCertProductMappingDao(ICertProductMappingDao certProductMappingDao) {
		this.certProductMappingDao = certProductMappingDao;
	}
	
	public IUsercrtificatemappingDao getUsercrtificatemappingDao() {
		return usercrtificatemappingDao;
	}
	
	public void setUsercrtificatemappingDao(IUsercrtificatemappingDao usercrtificatemappingDao) {
		this.usercrtificatemappingDao = usercrtificatemappingDao;
	}

	
    public DataManager getDataManager() {
    	return dataManager;
    }

	
    public void setDataManager(DataManager dataManager) {
    	this.dataManager = dataManager;
    }

	
    public IConfigLogDao getConfigLogDao() {
    	return configLogDao;
    }

	
    public void setConfigLogDao(IConfigLogDao configLogDao) {
    	this.configLogDao = configLogDao;
    }

	
}
