/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月7日 下午6:52:15
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.sale.dao.impl;

import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Map;

import org.hibernate.Criteria;
import org.hibernate.SQLQuery;

import com.google.common.collect.Maps;
import com.hikvision.finance.core.util.DateUtils;
import com.hikvision.finance.core.util.ListUtil;
import com.hikvision.finance.core.util.StringUtils;
import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.hibernate.dao.BaseDao;
import com.hikvision.finance.extend.hibernate.qo.SqlQuery;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.common.util.StringUtilExpand;
import com.hikvision.finance.fms.model.Clientinput;
import com.hikvision.finance.fms.model.Paramdictionary;
import com.hikvision.finance.fms.model.Region;
import com.hikvision.finance.fms.modules.analysis.dto.AnalysisInfo;
import com.hikvision.finance.fms.modules.card.dao.ICardDao;
import com.hikvision.finance.fms.modules.duerecord.dto.DueRecordInfo;
import com.hikvision.finance.fms.modules.product.dao.IProductTypeDao;
import com.hikvision.finance.fms.modules.region.dao.IRegionDao;
import com.hikvision.finance.fms.modules.sale.dao.IClientInputDao;
import com.hikvision.finance.fms.modules.sale.dto.SaleInfo;
import com.hikvision.finance.fms.modules.sale.qo.SaleQo;
import com.hikvision.finance.fwork.exception.ExpectedException;


/**
 * <p></p>
 * @author jinxindong 2016年4月7日 下午6:52:15
 * @version V1.0 
 */
public class ClientInputDaoImpl extends BaseDao<Clientinput,SaleQo> implements IClientInputDao{

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = -2766456641250320046L;

    private IRegionDao regionDao ;
    private ICardDao cardDao;
    private IProductTypeDao productTypeDao ;
	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.hibernate.dao.BaseDao#buildCriteria(org.hibernate.Criteria, com.hikvision.finance.extend.hibernate.qo.BaseQo)
     */
    @Override
    protected Criteria buildCriteria(Criteria criteria, SaleQo qo) {
	    return criteria;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.sale.dao.IClientInputDao#getSaleDataPage(java.lang.Integer, java.lang.Integer, com.hikvision.finance.fms.modules.sale.dto.SaleInfo)
     */
    @Override
    public Page getSaleDataPage(Integer limit, Integer start, SaleInfo saleInfo) {
    	//1.查询条件封装
    	SqlQuery sqlQuery = conditionQuery(saleInfo);
		
		//2.设置page 属性
		Page page = new Page();
		page.setCondition(sqlQuery);
		page.setPageNo(start);
		page.setPageSize(limit);
	    page = this.queryPaginationBySql(page);
	    return page;
    }

	
    public IRegionDao getRegionDao() {
    	return regionDao;
    }

	
    public void setRegionDao(IRegionDao regionDao) {
    	this.regionDao = regionDao;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.sale.dao.IClientInputDao#getSaleDataList(java.lang.Integer, java.lang.Integer, com.hikvision.finance.fms.modules.sale.dto.SaleInfo)
     */
    @SuppressWarnings("unchecked")
    @Override
    public List<SaleInfo> getSaleDataList(SaleInfo saleInfo) {
    	//1.查询条件封装
    	SqlQuery sqlQuery = conditionQuery(saleInfo);
    	
		List<SaleInfo> saleInfos = (List<SaleInfo>)this.queryListBySql(sqlQuery);
		Integer count = saleInfos.size();		
		if(count > Constants.EXCEL_MAX_NUM){
			throw new ExpectedException("", "导出数据超过"+Constants.EXCEL_MAX_NUM+"条,无法导出");
		}
		
		//2.获取证件map
		List<Paramdictionary> paramdictionaries = cardDao.getAll();
		Map<Integer, Paramdictionary> cardMap = buildCardMap(paramdictionaries);

		//3.循环计入相关名称
		List<SaleInfo> saleInfoList = new ArrayList<SaleInfo>();
		for (SaleInfo info : saleInfos) {
			Paramdictionary paramdictionary = cardMap.get(info.getCreditId());
			info.setCreditCodeName(paramdictionary.getStrName());
			if(info.getStatus()==1){//投诉单
				info.setStatusName("投诉单");
			}else if(info.getStatus()==0){//签约单
				info.setStatusName("签约单");
			}else{
				info.setStatusName("未知");
			}
			saleInfoList.add(info);
		}
		
		return saleInfos;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.sale.dao.IClientInputDao#getSaleByAnalysisInfo(com.hikvision.finance.fms.modules.analysis.dto.AnalysisInfo)
     */
    @SuppressWarnings("unchecked")
    @Override
    public List<Clientinput> getSaleByAnalysisInfo(AnalysisInfo analysisInfo) {
    	Date recordStart =  DateUtils.getDateTime("yyyy-MM-dd HH:mm:ss", analysisInfo.getRecordStart()); //开始时间
    	Date recordEnd = DateUtils.getDateTime("yyyy-MM-dd HH:mm:ss", analysisInfo.getRecordEnd()); //结束时间
    	String sql = "SELECT * FROM Clientinput p where  p.businessTime >= ? AND p.businessTime <= ?";
		return  this.createSQLQuery(sql).addEntity(Clientinput.class).setParameter(0, recordStart).setParameter(1, recordEnd).list();
    }
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.sale.dao.IClientInputDao#getSaleByAnalysis(com.hikvision.finance.fms.modules.analysis.dto.AnalysisInfo, java.lang.String)
     */
    @Override
    public Integer getSaleByAnalysis(AnalysisInfo analysisInfo, String regionIds,int status) {
    	Date recordStart =  DateUtils.getDateTime("yyyy-MM-dd HH:mm:ss", analysisInfo.getRecordStart()); //开始时间
    	Date recordEnd = DateUtils.getDateTime("yyyy-MM-dd HH:mm:ss", analysisInfo.getRecordEnd()); //结束时间
    	String sql = "SELECT COUNT(id) FROM Clientinput p where  p.businessTime >= ? AND p.businessTime <= ?  AND p.`status` = ?  AND inputRegionId in ("+regionIds+")";
	    return  Integer.valueOf(this.createSQLQuery(sql).setParameter(0, recordStart).setParameter(1, recordEnd).setParameter(2, status).uniqueResult().toString());
    }
	
	
    /**
     * 查询条件封装方法
     * @author jinxindong 2016年5月18日 下午6:10:53
     * @param regionInfo
     * @return
     */
	private SqlQuery conditionQuery(SaleInfo saleInfo) {
		Integer regionId = null == saleInfo.getInputRegionId()?1:saleInfo.getInputRegionId();
		Integer isflag = null == saleInfo.getIsFlag()?0:saleInfo.getIsFlag();
		Region region = regionDao.get(regionId);
		String strPathLike = region.getStrPath();
		Date businessTimeStart = null;
		if(StringUtils.isNotBlank(saleInfo.getBusinessTimeStart())){
			businessTimeStart = DateUtils.getDateTime("yyyy-MM-dd HH:mm:ss", saleInfo.getBusinessTimeStart());// 业务时间 起始时间
		}
		Date businessTimeEnd = null;
		if(StringUtils.isNotBlank(saleInfo.getBusinessTimeEnd())){
			businessTimeEnd = DateUtils.getDateTime("yyyy-MM-dd HH:mm:ss", saleInfo.getBusinessTimeEnd());// 业务时间 结束时间	
		}
		String nproId = null == saleInfo.getNproId()?"":saleInfo.getNproId();// 业务单号
		String name = null == saleInfo.getName()?"":saleInfo.getName(); // 客户姓名 *
		String creditCode = null == saleInfo.getCreditCode()?"":saleInfo.getCreditCode();// 证件号
		String proName = null == saleInfo.getProName()?"":saleInfo.getProName();// 业务员姓名 *
		String proUserNo = null == saleInfo.getProUserNo()?"":saleInfo.getProUserNo();// 业务员工号
		String productName = null == saleInfo.getProductName()?"":saleInfo.getProductName();// 产品名称 *
		String productCode = null == saleInfo.getProductCode()?"":saleInfo.getProductCode();// 产品代码
		
		SqlQuery sqlQuery = new SqlQuery();
		StringBuffer sql = new StringBuffer("SELECT "
				+ "c.id AS id,"
				+ "c.nProId AS nproId,"
				+ "c.`name` AS `name`,"
				+ "c.creditId AS creditId,"
				+ "c.inputUser AS inputUser,"
				+ "c.inputRegionId AS inputRegionId,"
				+ "c.inputRegionCode AS inputRegionCode,"
				+ "c.inputRegionName AS inputRegionName,"
				+ "c.proName AS proName,"
				+ "c.proUserNo AS proUserNo,"
				+ "c.businessTime AS businessTime,"
				+ "c.strBuyMoney AS strBuyMoney,"
				+ "c.productId AS productId,"
				+ "c.productCode AS productCode,"
				+ "c.productName AS productName,"
				+ "c.productTypeId AS productTypeId,"
				+ "c.productTypeName AS productTypeName,"
				+ "c.productCode AS productCode,"				
				+ "c.`status` AS `status`,"				
				+ "c.strEndDate AS strEndDate ,"				
				+ "c.guaranteeYears AS guaranteeYears ,"			
				+ "c.guaranteeType AS guaranteeType ,"				
				+ "c.strGrade AS strGrade ,"				
				+ "c.strRisklevel AS strRisklevel ,"				
				+ "c.productSaveMonth AS productSaveMonth, "
				+ "c.productSaveType AS productSaveType, "
				+ "c.strCreditPhone AS strCreditPhone, "				
				+ "c.strBankNo AS strBankNo, "				
				+ "c.strRemarks AS strRemarks, "
				+ "c.lockClient AS lockClient, "
				+ "c.creditCode AS creditCode "				
				+ " FROM "
				+ "clientinput c JOIN region r ON c.inputRegionId = r.id"
				+ " where 1=1 ");
		
		if ( isflag == Constants.CONS_1) {// 显示旗下所有子孙节点
			sql.append(" and r.strPath like ? ");
			sqlQuery.addParams(strPathLike + "%");
		} else if (isflag == Constants.CONS_0) {// 只显示该节点
			sql.append(" and r.strPath = ? ");
			sqlQuery.addParams(strPathLike);
		}
		if (saleInfo.getLockClients() != null && saleInfo.getLockClients().size() > 0) {
			sql.append(" and  c.lockClient in ( "+ ListUtil.listToStr(saleInfo.getLockClients()) + " ) ");
		}
		
		if (saleInfo.getInputUser() != null && saleInfo.getInputUser() > 0) {
			sql.append(" and  c.inputUser = ?");
			sqlQuery.addParams(saleInfo.getInputUser());
		}
		if (StringUtils.isNotBlank(nproId)) {
			sql.append(" and  c.nProId like ?  escape '/'");
			sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(nproId) + "%");
		}
		if (StringUtils.isNotBlank(name)) {
			sql.append(" and  c.`name` like ?  escape '/'");
			sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(name) + "%");
		}
		if (StringUtils.isNotBlank(creditCode)) {
			sql.append(" and  c.creditCode like ?  escape '/'");
			sqlQuery.addParams("%" +StringUtilExpand.escapeSQLLike(creditCode) + "%");
		}
		if (StringUtils.isNotBlank(proName)) {
			sql.append(" and  c.proName like ?  escape '/'");
			sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(proName) + "%");
		}
		if (StringUtils.isNotBlank(proUserNo)) {
			sql.append(" and  c.proUserNo like ?  escape '/'");
			sqlQuery.addParams("%" +StringUtilExpand.escapeSQLLike(proUserNo)+"%");
		}
		if (StringUtils.isNotBlank(productName)) {
			sql.append(" and  c.productName like ?  escape '/'");
			sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(productName) + "%");
		}
		if (StringUtils.isNotBlank(productCode)) {
			sql.append(" and c.productCode like ?  escape '/'");
			sqlQuery.addParams("%" +StringUtilExpand.escapeSQLLike(productCode)+ "%");
		}
		if (saleInfo.getLockClient() != null && saleInfo.getLockClient() >= 0) {
			sql.append(" and c.lockClient = ?");
			sqlQuery.addParams(saleInfo.getLockClient());
		}
		// 时间
		if (businessTimeStart!=null) {
			sql.append(" and c.businessTime >= ?");
			sqlQuery.addParams(businessTimeStart);
		}
		if (businessTimeEnd!=null) {
			sql.append(" and c.businessTime <= ? ");
			sqlQuery.addParams(businessTimeEnd);
		}

		// 默认倒序
		sql.append(" order by c.id desc");
		// 设置sqlQuery属性
		sqlQuery.setSql(sql.toString());
		sqlQuery.setDtoClass(SaleInfo.class);
		return sqlQuery;
	}
	
	
	/**
     * 查询条件封装方法
     * @author jinxindong 2016年5月18日 下午6:10:53
     * @param regionInfo
     * @return
     */
	private SqlQuery hisConditionQuery(SaleInfo saleInfo) {
		Date businessTimeStart = null;
		if(StringUtils.isNotBlank(saleInfo.getBusinessTimeStart())){
			businessTimeStart = DateUtils.getDateTime("yyyy-MM-dd HH:mm:ss", saleInfo.getBusinessTimeStart());// 业务时间 起始时间
		}
		Date businessTimeEnd = null;
		if(StringUtils.isNotBlank(saleInfo.getBusinessTimeEnd())){
			businessTimeEnd = DateUtils.getDateTime("yyyy-MM-dd HH:mm:ss", saleInfo.getBusinessTimeEnd());// 业务时间 结束时间	
		}
		String nproId = null == saleInfo.getNproId()?"":saleInfo.getNproId();// 业务单号
		String name = null == saleInfo.getName()?"":saleInfo.getName(); // 客户姓名 *
		SqlQuery sqlQuery = new SqlQuery();
		StringBuffer sql = new StringBuffer("SELECT "
				+ "c.id AS id,"
				+ "c.nProId AS nproId,"
				+ "c.`name` AS `name`,"
				+ "c.creditId AS creditId,"
				+ "c.inputUser AS inputUser,"
				+ "c.inputRegionId AS inputRegionId,"
				+ "c.inputRegionCode AS inputRegionCode,"
				+ "c.inputRegionName AS inputRegionName,"
				+ "c.proName AS proName,"
				+ "c.proUserNo AS proUserNo,"
				+ "c.businessTime AS businessTime,"
				+ "c.strBuyMoney AS strBuyMoney,"
				+ "c.productId AS productId,"
				+ "c.productCode AS productCode,"
				+ "c.productName AS productName,"
				+ "c.productTypeId AS productTypeId,"
				+ "c.productTypeName AS productTypeName,"
				+ "c.productCode AS productCode,"				
				+ "c.`status` AS `status`,"				
				+ "c.strEndDate AS strEndDate ,"				
				+ "c.guaranteeYears AS guaranteeYears ,"			
				+ "c.guaranteeType AS guaranteeType ,"				
				+ "c.strGrade AS strGrade ,"				
				+ "c.strRisklevel AS strRisklevel ,"				
				+ "c.productSaveMonth AS productSaveMonth, "
				+ "c.productSaveType AS productSaveType, "
				+ "c.strCreditPhone AS strCreditPhone, "				
				+ "c.strBankNo AS strBankNo, "				
				+ "c.strRemarks AS strRemarks, "
				+ "c.lockClient AS lockClient, "
				+ "c.creditCode AS creditCode "				
				+ " FROM "
				+ "clientinput c JOIN region r ON c.inputRegionId = r.id"
				+ " where 1=1 ");
		
		if (saleInfo.getInputUser() != null && saleInfo.getInputUser() > 0) {
			sql.append(" and  c.inputUser = ?");
			sqlQuery.addParams(saleInfo.getInputUser());
		}
		if (StringUtils.isNotBlank(nproId)) {
			sql.append(" and  c.nProId like ?  escape '/'");
			sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(nproId) + "%");
		}
		if (StringUtils.isNotBlank(name)) {
			sql.append(" and  c.`name` like ?  escape '/'");
			sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(name) + "%");
		}
		// 时间
		if (businessTimeStart!=null) {
			sql.append(" and c.businessTime >= ?");
			sqlQuery.addParams(businessTimeStart);
		}
		if (businessTimeEnd!=null) {
			sql.append(" and c.businessTime <= ? ");
			sqlQuery.addParams(businessTimeEnd);
		}
		// 默认倒序
		sql.append(" order by c.id desc");
		// 设置sqlQuery属性
		sqlQuery.setSql(sql.toString());
		sqlQuery.setDtoClass(SaleInfo.class);
		return sqlQuery;
	}
	
	/**
	 * 封装证件Map
	 * @author jinxindong 2016年5月19日 下午6:41:07
	 * @param paramdictionaries
	 * @return
	 */
	private Map<Integer, Paramdictionary> buildCardMap(List<Paramdictionary> paramdictionaries) {
		Map<Integer, Paramdictionary> cardMap = Maps.newHashMap();
		for (Paramdictionary paramdictionary : paramdictionaries) {
			cardMap.put(paramdictionary.getId(), paramdictionary);
		}
		return cardMap;
	}
	
	/******************************以下为到期管理模块**********************************/
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.duerecord.dao.IClientInputDao#getDataPage(com.hikvision.finance.fms.modules.duerecord.dto.DueRecordInfo, java.lang.Integer, java.lang.Integer)
	 */
	public Page getDataPage(DueRecordInfo dueRecordInfo, Integer start, Integer limit) {
		SqlQuery sqlQuery = creatDueRecordSqlQuery(dueRecordInfo, 0);
		// 设置page 属性
		Page page = new Page();
		page.setCondition(sqlQuery);
		page.setPageNo(start);
		page.setPageSize(limit);
		page = this.queryPaginationBySql(page);
		return page;
	}
	
	/* 
	 * (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.duerecord.dao.IClientInputDao#getVideoNumList(java.util.List)
     */
    @SuppressWarnings("unchecked")
    @Override
    public List<DueRecordInfo> getVideoNumList(List<Integer> ids) {
		// 设置sqlQuery属性
		SqlQuery sqlQuery = new SqlQuery();
		String sql = "select c.id as busiId, (select count(p.id) from prosaverecord p where p.clientId = c.id) as videoNum from clientinput c where c.id in (" + ListUtil.listToStr(ids) + ")";
		sqlQuery.setSql(sql);
		sqlQuery.setDtoClass(DueRecordInfo.class);
		List<DueRecordInfo> dueRecordInfoList = (List<DueRecordInfo>)queryListBySql(sqlQuery);
		return dueRecordInfoList;
    }
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.duerecord.dao.IClientInputDao#getDataList(com.hikvision.finance.fms.modules.duerecord.dto.DueRecordInfo)
	 */
	@SuppressWarnings("unchecked")
    public List<DueRecordInfo> getDataList(DueRecordInfo dueRecordInfo, Integer limit) {
		SqlQuery sqlQuery = creatDueRecordSqlQuery(dueRecordInfo, 1);
		if (limit != null) {
			sqlQuery.setSql(sqlQuery.getSql() + " limit " + limit);
		}
		List<DueRecordInfo> dueRecordInfoList = (List<DueRecordInfo>)queryListBySql(sqlQuery);
		return dueRecordInfoList;
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.duerecord.dao.IClientInputDao#setDelayDays(com.hikvision.finance.fms.modules.duerecord.dto.DueRecordInfo)
	 */
	public void setDelayDays(DueRecordInfo dueRecordInfo) {
		String sql = "update clientinput set strVideoEndDate = DATE_ADD(strVideoEndDate, INTERVAL " + dueRecordInfo.getDelayDays()
		        + " DAY), delaydays = delaydays + " + dueRecordInfo.getDelayDays() + " where id in (" + ListUtil.listToStr(dueRecordInfo.getBusiIds()) + ")";
		SQLQuery sqlQuery = this.createSQLQuery(sql);
		sqlQuery.executeUpdate();
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.duerecord.dao.IClientInputDao#setReadStatus(com.hikvision.finance.fms.modules.duerecord.dto.DueRecordInfo)
	 */
	public void setReadStatus(DueRecordInfo dueRecordInfo){
		String sql = "update clientinput set nisRead = " + Constants.BusiReadStatus.READED + " where id in(" + ListUtil.listToStr(dueRecordInfo.getBusiIds()) + ")";
		SQLQuery sqlQuery = this.createSQLQuery(sql);
		sqlQuery.executeUpdate();
	}
	
	/**
	 * 拼装sql查询语句的私有方法
	 * @author jinchenwu 2016年4月7日 下午8:21:31
	 * @param dueRecordInfo
	 * @return
	 */
	private SqlQuery creatDueRecordSqlQuery(DueRecordInfo dueRecordInfo, int type) {
		// 设置sqlQuery属性
		SqlQuery sqlQuery = new SqlQuery();
		StringBuilder sql = new StringBuilder(
		        "select c.id as busiId,"// 业务单id
		        + " c.nProId as nproId,"// 业务单号
		        + " c.inputRegionName as inputRegionName,"// 交易区域名称
		        + " c.businessTime as businessTime,"// 办理时间
		        + " c.proName as proName,"// 业务员姓名
		        + " c.name as name,"// 客户姓名
		        + " c.productName as productName,"// 产品名称
		        + " c.nisPay as nisPay,"// 录像状态
		        + " c.nisRead as nisRead,"// 是否已读
		        + " c.strVideoEndDate as strVideoEndDate");// 视频到期日
		        if(type == 1){// 0-分页查询，1-非分页查询，分页查询不能执行子查询 
		        	sql.append(", (select count(id) from prosaverecord where clientId = c.id) as videoNum");// 录像个数
		        }
		        sql.append(" from clientinput c where 1 = 1");
		if (dueRecordInfo != null) {
			// 删除提示天数
			if (dueRecordInfo.getThresholdDate() != null) {
				sql.append(" and c.strVideoEndDate <= ?");
				sqlQuery.addParams(dueRecordInfo.getThresholdDate());
			}
			// 筛选区域
			if(StringUtils.isNotBlank(dueRecordInfo.getStrRegionIds())){
				sql.append(" and c.inputRegionId in (" + dueRecordInfo.getStrRegionIds() + ")");
			}
			// 录像状态
			if (dueRecordInfo.getNisPay() != null) {
				sql.append(" and c.nisPay = ?");
				sqlQuery.addParams(dueRecordInfo.getNisPay());
			}
			// 已读未读
			if (dueRecordInfo.getNisRead() != null) {
				sql.append(" and c.nisRead = ?");
				sqlQuery.addParams(dueRecordInfo.getNisRead());
			}
			// 业务单号
			if (StringUtils.isNotBlank(dueRecordInfo.getNproId())) {
				sql.append(" and c.nProId like ? escape '/'");
				sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(dueRecordInfo.getNproId()) + "%");
			}
			// 业务员姓名
			if (StringUtils.isNotBlank(dueRecordInfo.getProName())) {
				sql.append(" and c.proName like ? escape '/'");
				sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(dueRecordInfo.getProName()) + "%");
			}
			// 客户姓名
			if (StringUtils.isNotBlank(dueRecordInfo.getName())) {
				sql.append(" and c.name like ? escape '/'");
				sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(dueRecordInfo.getName()) + "%");
			}
			// 产品名称
			if (StringUtils.isNotBlank(dueRecordInfo.getProductName())) {
				sql.append(" and c.productName like ? escape '/'");
				sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(dueRecordInfo.getProductName()) + "%");
			}

			sql.append(" order by c.nisRead asc, c.nisPay asc, c.strVideoEndDate asc");
		}
		sqlQuery.setSql(sql.toString());
		sqlQuery.setDtoClass(DueRecordInfo.class);
		return sqlQuery;
	}
	
	public ICardDao getCardDao() {
		return cardDao;
	}
	
	public void setCardDao(ICardDao cardDao) {
		this.cardDao = cardDao;
	}
	
	public IProductTypeDao getProductTypeDao() {
		return productTypeDao;
	}
	
	public void setProductTypeDao(IProductTypeDao productTypeDao) {
		this.productTypeDao = productTypeDao;
	}

	/* (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.sale.dao.IClientInputDao#getHisClientInputList(com.hikvision.finance.fms.modules.sale.dto.SaleInfo)
	 */
	@Override
	public List<SaleInfo> getHisClientInputList(SaleInfo saleInfo) {
		//1.查询条件封装
    	SqlQuery sqlQuery = hisConditionQuery(saleInfo);
		List<SaleInfo> saleInfos = (List<SaleInfo>)this.queryListBySql(sqlQuery);
		return saleInfos;
	}
	
}
