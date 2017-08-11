/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-4-17 下午4:36:54
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.webdevice.contact.dao.impl;

import org.hibernate.Criteria;

import com.hikvision.finance.core.util.StringUtils;
import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.hibernate.dao.BaseDao;
import com.hikvision.finance.extend.hibernate.qo.SqlQuery;
import com.hikvision.finance.fms.common.util.SessionUtil;
import com.hikvision.finance.fms.common.util.StringUtilExpand;
import com.hikvision.finance.fms.model.Businessorder;
import com.hikvision.finance.fms.model.Clientinput;
import com.hikvision.finance.fms.webdevice.contact.dao.IBusinessOrderDao;
import com.hikvision.finance.fms.webdevice.contact.dto.ClientinputInfo;
import com.hikvision.finance.fms.webdevice.contact.dto.RealOrdersInfo;
import com.hikvision.finance.fms.webdevice.contact.qo.BusinessOrderQo;

/**
 * <p>业务单数据dao</p>
 * @author xujiangfei 2016-4-17 下午4:36:54
 * @version V1.0 
 */
public class BusinessOrderDaoImpl extends BaseDao<Businessorder, BusinessOrderQo> implements IBusinessOrderDao{

	/**
	 * 序列化ID
	 */
	private static final long serialVersionUID = -7282824466169558645L;

	/* 
	 * 录入投诉单和选择历史业务单时获取历史业务单
	 * @see com.hikvision.finance.fms.webdevice.contact.dao.IBusinessOrderDao#getDataPage(int, int, java.lang.String)
	 */
	@Override
	public Page getDataPage(int limit, int start, Clientinput clientinput, Integer type) {
		SqlQuery sqlQuery = new SqlQuery();
		StringBuffer sql = new StringBuffer("select"
				+ " cli.id as id,"
				+ " cli.nProId as nproId,"// 业务单号
				+ " cli.status as status,"// 业务类型
				+ " cli.name as name,"// 客户姓名
				+ " cli.creditCode as creditCode,"// 客户证件号
				+ " p.strName as creditName,"// 客户证件类型
				+ " cli.businessTime as businessTime,"
				+ " cli.productName as productName,"
				+ " cli.guaranteeType as guaranteeType,"
				+ " cli.guaranteeYears as guaranteeYears,"
				+ " cli.name as name,"
				+ " cli.productCode as productCode,"
				+ " cli.proUserNo as proUserNo,"
				+ " cli.strGrade as strGrade,"
				+ " cli.strEndDate as strEndDate,"
				+ " cli.strVideoEndDate as strVideoEndDate,"
				+ " cli.productSaveType as productSaveType,"
				+ " cli.productSaveMonth as productSaveMonth,"
				+ " cli.lockClient as lockClient" // 业务单审批状态
				+ " from clientinput cli left join paramdictionary p"
				+ " on cli.creditId = p.id "
				+ "where 1 = 1 "
				+ "and cli.nisPay=0 ");
		//查询条件--业务单的流水号
		if (StringUtils.isNotBlank(clientinput.getNproId())) {
			sql.append(" and cli.nProId like ? escape '/'");
			sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(clientinput.getNproId()) + "%");
		}
		//客户证件号
		if (StringUtils.isNotBlank(clientinput.getCreditCode())) {
			sql.append(" and cli.creditCode like ? escape '/'");
			sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(clientinput.getCreditCode()) + "%");
		}
		//客户姓名
		if (StringUtils.isNotBlank(clientinput.getName())) {
			sql.append(" and cli.name like ? escape '/'");
			sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(clientinput.getName()) + "%");
		}
		//区分历史业务单(签约单获取到的业务单type=1, 投诉单获取到的业务单type=2)
		//关联业务单只能获取审批退回的 ,投诉单获取到所有的业务单
		if (type != null && type == 1) { 
			sql.append(" and cli.lockClient = 2 ");
			sql.append(" and cli.inputUser = ? ");
			sqlQuery.addParams(SessionUtil.getHikSession().getUserId());
		}
		sql.append(" order by cli.id desc");
		
		// 设置sqlQuery属性
		sqlQuery.setSql(sql.toString());
		sqlQuery.setDtoClass(ClientinputInfo.class);
		
		// 设置page 属性
		Page page = new Page();
		page.setCondition(sqlQuery);
		page.setPageNo(start);
		page.setPageSize(limit);
		page = this.queryPaginationBySql(page);
		return page;
	}

	/* 
	 * 获取录像信息和业务信息分页数据
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.webdevice.contact.dao.IBusinessOrderDao#getRecordsDataPage(int, int, com.hikvision.finance.fms.webdevice.contact.dto.RealOrdersInfo)
	 */
	@Override
	public Page getRecordsDataPage(int limit, int start, RealOrdersInfo realOrdersInfo) {
		SqlQuery sqlQuery = new SqlQuery();
		StringBuffer sql = new StringBuffer("SELECT pro.id as proid,"// prosaverecord 表的id
				+ " pro.fileName as strRecordName,"// 录像的名称
				+ " pro.startTime as startTime,"// 记录开始时间
				+ " pro.endTime as endTime,"// 记录结束时间
				+ " pro.stateType as stateType,"// 录像类型
				+ " cli.id as cliid,"// clientinput表的id
				+ " cli.nProId as nProId,"// 业务单号
				+ " cli.status as status ,"// 签约单的类型--签约，投诉
				+ " cli.name as customername,"// 客户姓名
				+ " cli.productName as productName,"// 产品名称
				+ " cli.strBuyMoney as strBuyMoney,"// 购买金额
				+ " cli.businessTime dtBusinessTime,"// 业务办理时间
				+ " cli.lockClient as lockClient" //  业务单审批状态
                + " from prosaverecord pro join clientinput cli on pro.clientId = cli.id"
                + " where pro.userId = ?");// 理财经理的id
		sqlQuery.addParams(SessionUtil.getHikSession().getUserId());
		if (realOrdersInfo.getStartTime()!=null) {
			sql.append(" and pro.startTime > ?");
			sqlQuery.addParams(realOrdersInfo.getStartTime());
		}
		if (realOrdersInfo.getEndTime()!=null) {
			sql.append(" and pro.startTime < ?");
			sqlQuery.addParams(realOrdersInfo.getEndTime());
		}
		// 客户名称
		if (StringUtils.isNotBlank(realOrdersInfo.getCustomername())) {
			sql.append(" and cli.name like ? escape '/'");
			sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(realOrdersInfo.getCustomername()) + "%");
		}
		// 录像名称
		if (StringUtils.isNotBlank(realOrdersInfo.getStrRecordName())) {
			sql.append(" and pro.fileName like ? escape '/'");
			sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(realOrdersInfo.getStrRecordName()) + "%");
		}
		// 产品名称
		if (StringUtils.isNotBlank(realOrdersInfo.getProductName())) {
			sql.append(" and cli.productName like ? escape '/'");
			sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(realOrdersInfo.getProductName()) + "%");
		}
		sql.append(" order by cli.id desc");
		// 设置sqlQuery属性
		sqlQuery.setSql(sql.toString());
		sqlQuery.setDtoClass(RealOrdersInfo.class);
		// 设置page 属性
		Page page = new Page();
		page.setCondition(sqlQuery);
		page.setPageNo(start);
		page.setPageSize(limit);
		page = this.queryPaginationBySql(page);
		return page;
	}
	
	/* (non-Javadoc)
	 * @see com.hikvision.finance.extend.hibernate.dao.BaseDao#buildCriteria(org.hibernate.Criteria, com.hikvision.finance.extend.hibernate.qo.BaseQo)
	 */
	@Override
	protected Criteria buildCriteria(Criteria criteria, BusinessOrderQo qo) {
		return criteria;
	}

}
