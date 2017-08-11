/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月17日 上午10:13:13
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.product.dao.impl;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import org.hibernate.Criteria;
import org.hibernate.criterion.Restrictions;

import com.google.common.collect.Maps;
import com.hikvision.finance.core.util.StringUtils;
import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.hibernate.dao.BaseDao;
import com.hikvision.finance.extend.hibernate.qo.SqlQuery;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.common.util.StringUtilExpand;
import com.hikvision.finance.fms.model.Crtificateproductmapping;
import com.hikvision.finance.fms.model.Product;
import com.hikvision.finance.fms.model.Producttype;
import com.hikvision.finance.fms.model.Users;
import com.hikvision.finance.fms.modules.product.dao.ICertProductMappingDao;
import com.hikvision.finance.fms.modules.product.dao.IProductDao;
import com.hikvision.finance.fms.modules.product.dao.IProductTypeDao;
import com.hikvision.finance.fms.modules.product.dto.ProductInfo;
import com.hikvision.finance.fms.modules.product.qo.ProductQo;
import com.hikvision.finance.fms.modules.user.dao.IUsersDao;
import com.hikvision.finance.fwork.exception.ExpectedException;



/**
 * <p>产品管理Dao层</p>
 * @author jinxindong 2016年3月17日 上午10:13:13
 * @version V1.0 
 */
public class ProductDaoImpl extends BaseDao<Product,ProductQo> implements IProductDao{

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 8491008093291258495L;
	private IProductTypeDao  productTypeDao;
	private IUsersDao usersDao;
	private ICertProductMappingDao certProductMappingDao;
	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.hibernate.dao.BaseDao#buildCriteria(org.hibernate.Criteria, com.hikvision.finance.extend.hibernate.qo.BaseQo)
     */
    @Override
    protected Criteria buildCriteria(Criteria criteria, ProductQo qo) {
	    // TODO Auto-generated method stub
    	if (qo != null) {
    		if (qo.getId() != null) {
    			criteria.add(Restrictions.eq("id", qo.getId()));
    		}
    		if (qo.getStrCode() != null) {
    			criteria.add(Restrictions.eq("strCode", qo.getStrCode()));
    		}
    		if (qo.getNenabled() != null) {
    			criteria.add(Restrictions.eq("nenabled", qo.getNenabled()));
    		}
    	}
	    return criteria;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.product.dao.IProductDao#getDataPage(java.lang.Integer, java.lang.Integer, com.hikvision.finance.fms.modules.product.dto.ProductInfo)
     */
    @SuppressWarnings("unchecked")
    @Override
	public Page getDataPage(Integer limit, Integer start, ProductInfo productInfo) {
		// 1.查询条件封装
		SqlQuery sqlQuery = conditionQuery(productInfo);
		
		// 2.设置page 属性
		Page page = new Page();
		page.setCondition(sqlQuery);
		page.setPageNo(start);
		page.setPageSize(limit);
		page = this.queryPaginationBySql(page);
		
		// 3.获取用户信息map
		List<Users> users = usersDao.getAll();
		Map<Integer, Users> userMap = buildUserMap(users);
		
		// 4.获取产品类型map
		List<Producttype> producttypes = productTypeDao.getAll();
		Map<Integer, Producttype> proTypeMap = buildProtypeMap(producttypes);
		
		// 5.循环迭代 添加关联未关联字样和复合员名字,产品类型名字
		List<ProductInfo> productInfos = (List<ProductInfo>)page.getResult();
		List<ProductInfo> productInfoList = new ArrayList<ProductInfo>();
		for (ProductInfo productOld : productInfos) {
			// 计入复核员名称
			if (productOld.getCheckUser() != null) {
				Users user = userMap.get(productOld.getCheckUser());
				if (user != null) {
					productOld.setCheckUserName(user.getStrName());
				} else {
					productOld.setCheckUserName(null);
				}
				
			}
			// 计入产品类型名称
			Producttype producttype = proTypeMap.get(productOld.getPid());
			productOld.setProductTypeName(producttype.getStrName());
			// 计入关联关系
			List<Crtificateproductmapping> crtificateproductmappings = certProductMappingDao.findBy("nproductId",
			        productOld.getId());
			if (crtificateproductmappings != null && crtificateproductmappings.size() > 0) {
				productOld.setRelateStatus(Constants.CONS_1);// 关联
			} else {
				productOld.setRelateStatus(Constants.CONS_0);// 未关联
			}
			productInfoList.add(productOld);
		}
		page.setResult(productInfoList);
		return page;
	}

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.product.dao.IProductDao#exportProductsExcel(com.hikvision.finance.fms.modules.product.dto.ProductInfo)
     */
    @SuppressWarnings("unchecked")
    @Override
	public List<ProductInfo> exportProductsExcel(ProductInfo productInfo) {
		// 1.查询条件封装
		SqlQuery sqlQuery = conditionQuery(productInfo);
		
		// 2.获取用户信息map
		// List<Users> users = usersDao.getAll();
		// Map<Integer, Users> userMap = buildUserMap(users);
		
		// 3.获取产品类型map
		List<Producttype> producttypes = productTypeDao.getAll();
		Map<Integer, Producttype> proTypeMap = buildProtypeMap(producttypes);
		
		// 5.循环迭代 添加关联未关联字样和复合员名字,产品类型名字
		List<ProductInfo> productInfos = (List<ProductInfo>)this.queryListBySql(sqlQuery);
		Integer count = productInfos.size();
		if (count > Constants.EXCEL_MAX_NUM) {
			throw new ExpectedException("", "导出数据超过" + Constants.EXCEL_MAX_NUM + "条,无法导出");
		}
		
		List<ProductInfo> productInfoList = new ArrayList<ProductInfo>();
		for (ProductInfo productOld : productInfos) {
			// 计入风险等级
			if ("1".equals(productOld.getStrRisklevel())) {
				productOld.setStrRisklevelName(Constants.RiskLevel.RISKLEVEL_ONE);
			} else if ("2".equals(productOld.getStrRisklevel())) {
				productOld.setStrRisklevelName(Constants.RiskLevel.RISKLEVEL_TWO);
			} else if ("3".equals(productOld.getStrRisklevel())) {
				productOld.setStrRisklevelName(Constants.RiskLevel.RISKLEVEL_THREE);
			} else if ("4".equals(productOld.getStrRisklevel())) {
				productOld.setStrRisklevelName(Constants.RiskLevel.RISKLEVEL_FOUR);
			} else if ("5".equals(productOld.getStrRisklevel())) {
				productOld.setStrRisklevelName(Constants.RiskLevel.RISKLEVEL_FIVE);
			}else if ("6".equals(productOld.getStrRisklevel())) {
				productOld.setStrRisklevelName(Constants.RiskLevel.RISKLEVEL_SIX);
			} else if ("0".equals(productOld.getStrRisklevel())) {
				productOld.setStrRisklevelName(Constants.RiskLevel.RISKLEVEL_NONE);
			}else {
				productOld.setStrRisklevelName("未知");
			}
			
			if(0 == productOld.getNenabled()){
				productOld.setNenabledName("禁用");
			}else if(1 == productOld.getNenabled()){
				productOld.setNenabledName("启用");
			}else{
				productOld.setNenabledName("未知");
			}
			
			if(Constants.ProductKeepType.YEAR == productOld.getGuaranteeType()){
				productOld.setStrGuaranteeType("年");
			}else if(Constants.ProductKeepType.MONTH == productOld.getGuaranteeType()){
				productOld.setStrGuaranteeType("月");
			}else if(Constants.ProductKeepType.DAY == productOld.getGuaranteeType()){
				productOld.setStrGuaranteeType("日");
			}
			
			if(Constants.ProductKeepType.YEAR == productOld.getSaveType()){
				productOld.setStrSaveType("年");
			}else if(Constants.ProductKeepType.MONTH == productOld.getSaveType()){
				productOld.setStrSaveType("月");
			}else if(Constants.ProductKeepType.DAY == productOld.getSaveType()){
				productOld.setStrSaveType("日");
			}
			
			// 计入产品类型名称
			Producttype producttype = proTypeMap.get(productOld.getPid());
			productOld.setProductTypeName(producttype.getStrName());
			// 计入关联关系
			List<Crtificateproductmapping> crtificateproductmappings = certProductMappingDao.findBy("nproductId",
			        productOld.getId());
			if (crtificateproductmappings != null && crtificateproductmappings.size() > 0) {
				productOld.setRelateStatusName("关联");// 关联
			} else {
				productOld.setRelateStatusName("未关联");// 未关联
			}
			productInfoList.add(productOld);
		}
		return productInfoList;
	}

    /* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.product.dao.IProductDao#getPermitProductList(java.lang.String, java.lang.Integer)
     */
    @SuppressWarnings("unchecked")
    public List<Product> getPermitProductList(String ids, Integer productTypeId, String strCode, Integer productId){
    	SqlQuery sqlQuery = new SqlQuery();
		StringBuilder sql = new StringBuilder(
				"select distinct p.id as id,"
				+ " p.pId as pid,"
				+ " p.strCode as strCode,"
				+ " p.strName as strName,"
				+ " p.saveType as saveType,"
				+ " p.saveYears as saveYears,"
				+ " p.guaranteeType as guaranteeType,"
				+ " p.guaranteeYears as guaranteeYears,"
				+ " p.strEndDate as strEndDate,"
				+ " p.strGrade as strGrade,"
				+ " p.strRisklevel as strRisklevel"
				+ " from product p");
        if (StringUtils.isNotBlank(ids)) {				
			sql.append(
				", crtificateproductmapping c"
				+ " where c.nCrtificateId in (" + ids + ") and c.nProductId = p.id and"
			);
		} else {
			sql.append(" where");
		}
		sql.append(" (p.strEndDate is null or p.strEndDate > curdate()) and p.nEnabled = 1");
		if (null != productTypeId && productTypeId > 0) {
			sql.append(" and p.pId = ?");
			sqlQuery.addParams(productTypeId);
		}
		if (StringUtils.isNotBlank(strCode)) {
			sql.append(" and p.strCode = ?");
			sqlQuery.addParams(strCode);
		}
		if (null != productId && productId > 0) {
			sql.append(" and p.id = ?");
			sqlQuery.addParams(productId);
		}
		sqlQuery.setSql(sql.toString());
		sqlQuery.setDtoClass(Product.class);
		List<Product> productList = (List<Product>)queryListBySql(sqlQuery);
		return productList;
	}
    
    /**
     * 封装查询条件
     * @author jinxindong 2016年5月19日 下午3:07:45
     * @param productInfo
     * @return
     */
	private SqlQuery conditionQuery(ProductInfo productInfo) {
		SqlQuery sqlQuery = new SqlQuery();
		String strName = "";
		String strCode = "";
		String strRisklevel = null;
		Integer nenabled = null;
		Integer pid = null;
		if (productInfo != null) {
			strName = null == productInfo.getStrName()?"":productInfo.getStrName();
			strCode = null == productInfo.getStrCode()?"":productInfo.getStrCode();
			strRisklevel = null == productInfo.getStrRisklevel()?"":productInfo.getStrRisklevel();
			nenabled = null == productInfo.getNenabled()?null:productInfo.getNenabled();
			pid = Constants.ROOT_PRO_ID == productInfo.getPid()?null:productInfo.getPid();
		}
		StringBuffer sql = new StringBuffer( " SELECT p.id as id,"
				+ "p.pid as pid,"
				+ "p.strCode as strCode,"
				+ "p.strName as strName,"
				+ "p.nEnabled as nenabled,"
				+ "p.strGrade as strGrade,"
				+ "p.strRisklevel as strRisklevel,"
				+ "p.strEndDate as strEndDate,"
				+ "p.guaranteeType as guaranteeType,"
				+ "p.guaranteeYears as guaranteeYears,"
				+ "p.saveType as saveType,"
				+ "p.saveYears as saveYears,"
				+ "p.checkUser as checkUser FROM product p WHERE 1 = 1  ");
		
		if (StringUtils.isNotBlank(strCode)) {
			sql.append(" and p.strCode like ?  escape '/'");
			sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(strCode) + "%");
		}
		if (StringUtils.isNotBlank(strRisklevel)) {
			sql.append(" and p.strRisklevel = ? ");
			sqlQuery.addParams(strRisklevel);
		}
		if (StringUtils.isNotBlank(strName)) {
			sql.append(" and p.strName like ?  escape '/'");
			sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(strName) + "%");
		}
		if (nenabled != null) {
			sql.append(" and p.nEnabled = ? ");
			sqlQuery.addParams(nenabled);
		}
		
		if (null != pid) {
			sql.append(" and p.pid = ? ");
			sqlQuery.addParams(pid);
		}
		// 默认倒序
		sql.append(" order by p.id desc");
		// 设置sqlQuery属性
		sqlQuery.setSql(sql.toString());
		sqlQuery.setDtoClass(ProductInfo.class);
		return sqlQuery;
	}
	
	/**
	 * 封装用户信息map
	 * @author jinxindong 2016年5月19日 下午3:17:28
	 * @param users
	 * @return
	 */
	private Map<Integer, Users> buildUserMap(List<Users> users) {
		Map<Integer, Users> userMap = Maps.newHashMap();
		for (Users user : users) {
			userMap.put(user.getId(), user);
		}
		return userMap;
	}
	
	/**
	 * 封装产品类型map
	 * @author jinxindong 2016年5月19日 下午3:17:09
	 * @param producttypes
	 * @return
	 */
	private Map<Integer, Producttype> buildProtypeMap(List<Producttype> producttypes) {
		Map<Integer, Producttype> proTypeMap = Maps.newHashMap();
		for (Producttype producttype : producttypes) {
			proTypeMap.put(producttype.getId(), producttype);
		}
		return proTypeMap;
	}
	
    public IProductTypeDao getProductTypeDao() {
    	return productTypeDao;
    }

	
    public void setProductTypeDao(IProductTypeDao productTypeDao) {
    	this.productTypeDao = productTypeDao;
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
	
}
