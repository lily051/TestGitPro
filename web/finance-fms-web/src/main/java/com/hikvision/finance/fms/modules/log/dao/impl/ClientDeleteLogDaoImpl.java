/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月8日 下午4:43:22
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.log.dao.impl;

import java.util.Date;
import java.util.List;

import org.hibernate.Criteria;
import org.hibernate.Query;

import com.hikvision.finance.core.util.DateUtils;
import com.hikvision.finance.core.util.StringUtils;
import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.hibernate.dao.BaseDao;
import com.hikvision.finance.extend.hibernate.qo.SqlQuery;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.common.util.StringUtilExpand;
import com.hikvision.finance.fms.model.Clientdeletelog;
import com.hikvision.finance.fms.modules.log.dao.IClientDeleteLogDao;
import com.hikvision.finance.fms.modules.log.dto.ClientdeletelogInfo;
import com.hikvision.finance.fms.modules.log.qo.ClientdeletelogQo;
import com.hikvision.finance.fwork.exception.ExpectedException;


/**
 * <p></p>
 * @author jinxindong 2016年4月8日 下午4:43:22
 * @version V1.0 
 */
public class ClientDeleteLogDaoImpl extends BaseDao<Clientdeletelog,ClientdeletelogQo> implements IClientDeleteLogDao{

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = -2851904656294863846L;

	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.hibernate.dao.BaseDao#buildCriteria(org.hibernate.Criteria, com.hikvision.finance.extend.hibernate.qo.BaseQo)
     */
    @Override
    protected Criteria buildCriteria(Criteria criteria, ClientdeletelogQo qo) {
	    // TODO Auto-generated method stub
	    return null;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.log.dao.IClientDeleteLogDao#getClientDelLogDataPage(java.lang.Integer, java.lang.Integer, com.hikvision.finance.fms.modules.log.dto.ClientdeletelogInfo)
     */
    @Override
    public Page getClientDelLogDataPage(Integer limit, Integer start, ClientdeletelogInfo clientdeletelogInfo) {
    	//1.封装查询条件
    	SqlQuery sqlQuery = conditionQuery(clientdeletelogInfo);
		
		//2.设置page 属性
		Page page = new Page();
		page.setCondition(sqlQuery);
		page.setPageNo(start);
		page.setPageSize(limit);
	    page = this.queryPaginationBySql(page);
	    return page;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.log.dao.IClientDeleteLogDao#exportDelLogExcel(com.hikvision.finance.fms.modules.log.dto.ClientdeletelogInfo, java.lang.String[], java.lang.String[], java.lang.String)
     */
    @SuppressWarnings("unchecked")
    @Override
	public List<ClientdeletelogInfo> exportDelLogExcel(ClientdeletelogInfo clientdeletelogInfo) {
    	//1.封装查询条件
		SqlQuery sqlQuery = conditionQuery(clientdeletelogInfo);
		
		//2.导出条件判断
		List<ClientdeletelogInfo> clientdeletelogInfos = (List<ClientdeletelogInfo>)this.queryListBySql(sqlQuery);
		Integer count = clientdeletelogInfos.size();
		if (count > Constants.EXCEL_MAX_NUM) {
			throw new ExpectedException("", "导出数据超过" + Constants.EXCEL_MAX_NUM + "条,无法导出");
		}
		return clientdeletelogInfos;
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.log.dao.IClientDeleteLogDao#deleteLog(java.util.Date)
	 */
	@Override
	public void deleteLog(Date thresholddate) {
		String sql = "delete from Clientdeletelog where inputTime < :thresholddate";
		Query query = createQuery(sql);
		query.setParameter("thresholddate", thresholddate);
		query.executeUpdate();
	}
	
	/**
	 * 封装查询条件
	 * @author jinxindong 2016年5月19日 下午4:53:20
	 * @param clientdeletelogInfo
	 * @return
	 */
	private SqlQuery conditionQuery(ClientdeletelogInfo clientdeletelogInfo){
		Date deleteTimeStart = null;
		if(StringUtils.isNotBlank(clientdeletelogInfo.getDeleteTimeStart())){
			deleteTimeStart = DateUtils.getDateTime("yyyy-MM-dd HH:mm:ss", clientdeletelogInfo.getDeleteTimeStart());// 业务时间 起始时间
		}
		Date deleteTimeEnd = null;
		if(StringUtils.isNotBlank(clientdeletelogInfo.getDeleteTimeEnd())){
			deleteTimeEnd = DateUtils.getDateTime("yyyy-MM-dd HH:mm:ss", clientdeletelogInfo.getDeleteTimeEnd());// 业务时间 结束时间	
		}
		
		String nproId = null == clientdeletelogInfo.getNproId()?"":clientdeletelogInfo.getNproId();// 业务单号
		String productName = null == clientdeletelogInfo.getProductName()?null:clientdeletelogInfo.getProductName();// 产品名称
		String creditId = null == clientdeletelogInfo.getCreditId()?"":clientdeletelogInfo.getCreditId();// 客户证件号
		String name = null == clientdeletelogInfo.getName()?"":clientdeletelogInfo.getName();// 客户姓名
    	
    	SqlQuery sqlQuery = new SqlQuery();
		StringBuffer sql = new StringBuffer("SELECT "
				+ "c.id AS id,"
				+ "c.nProId AS nproId,"
				+ "c.creditId AS creditId,"
				+ "c.productType AS productType,"
				+ "c.productName  AS productName,"
				+ "c.inputTime AS inputTime,"
				+ "c.inputUser AS inputUser,"
				+ "c.inputRegionId AS inputRegionId,"
				+ "c.deleteTime AS deleteTime,"
				+ "c.`name` AS `name` "
				+ "FROM clientdeletelog c "
				+ "where 1=1 ");
				
		if (StringUtils.isNotBlank(nproId)) {
			sql.append(" and  c.nProId like ?  escape '/'");
			sqlQuery.addParams("%" +StringUtilExpand.escapeSQLLike(nproId)+ "%");
		}
		if (StringUtils.isNotBlank(creditId)) {
			sql.append(" and c.creditId like ?  escape '/'");
			sqlQuery.addParams("%" +StringUtilExpand.escapeSQLLike(creditId)+ "%");
		}
		
		if (StringUtils.isNotBlank(name)) {
			sql.append(" and  c.`name` like ?  escape '/'");
			sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(name) + "%");
		}
		
		if (StringUtils.isNotBlank(productName)) {
			sql.append(" and  c.productName like ?  escape '/'");
			sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(productName) + "%");
		}
		
		// 时间
		if (deleteTimeStart != null) {
			sql.append(" and c.deleteTime >= ?");
			sqlQuery.addParams(deleteTimeStart);
		}
		if (deleteTimeEnd != null) {
			sql.append(" and c.deleteTime <= ? ");
			sqlQuery.addParams(deleteTimeEnd);
		}
		// 默认倒序
		sql.append(" order by c.id desc");
		// 设置sqlQuery属性
		sqlQuery.setSql(sql.toString());
		sqlQuery.setDtoClass(ClientdeletelogInfo.class);
		
		return sqlQuery;
	}
}
