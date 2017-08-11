/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-4-28 下午1:33:14
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.webdevice.log.dao.impl;

import java.util.Date;

import org.hibernate.Criteria;
import org.hibernate.Query;

import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.hibernate.dao.BaseDao;
import com.hikvision.finance.extend.hibernate.qo.SqlQuery;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.common.util.SessionUtil;
import com.hikvision.finance.fms.model.Operlog;
import com.hikvision.finance.fms.webdevice.log.dao.ICollLogDao;
import com.hikvision.finance.fms.webdevice.log.dto.OperlogInfo;
import com.hikvision.finance.fms.webdevice.log.qo.OperlogsQo;

/**
 * <p></p>
 * @author xujiangfei 2016-4-28 下午1:33:14
 * @version V1.0 
 */
public class CollLogDaoImpl extends BaseDao<Operlog, OperlogsQo> implements ICollLogDao {
	
	/**
	 * 序列化ID
	 */
	private static final long serialVersionUID = 2521240285571456734L;
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.webdevice.log.dao.ICollLogDao#getDataPage(int, int, com.hikvision.finance.fms.model.Operlog)
	 */
	@Override
	public Page getDataPage(int limit, int start, OperlogInfo operlogInfo) {
		
		SqlQuery sqlQuery = new SqlQuery();
		StringBuffer sql = new StringBuffer("select userId, operTime, operType, note from Operlog where userId = ?");
		// 只显示该理财经理的操作日志
		sqlQuery.addParams(SessionUtil.getUserSession().getUserId());
		// 操作时间--开始时间
		if (operlogInfo.getStartoperTime() != null) {
			sql.append(" and operTime >= ?");
			sqlQuery.addParams(operlogInfo.getStartoperTime());
		}
		// 操作时间--结束时间
		if (operlogInfo.getEndoperTime() != null) {
			sql.append(" and operTime <= ?");
			sqlQuery.addParams(operlogInfo.getEndoperTime());
		}
		// 操作类型
		if (operlogInfo.getOperType() != null && operlogInfo.getOperType() != 0) {
			sql.append(" and operType = ?");
			sqlQuery.addParams(operlogInfo.getOperType());
		}
		sql.append(" order by id desc");
		
		// 设置sqlQuery属性
		sqlQuery.setSql(sql.toString());
		sqlQuery.setDtoClass(OperlogInfo.class);
		
		// 设置page 属性
		Page page = new Page();
		page.setCondition(sqlQuery);
		page.setPageNo(start);
		page.setPageSize(limit);
		page = this.queryPaginationBySql(page);
		
		return page;
	}
	
	/*
	 * 记录BS客户端操作日志
	 * @see com.hikvision.finance.fms.webdevice.log.service.ICollLogService#writeOperLog(java.lang.Integer, java.util.Date, int, java.lang.String)
	 */
	@Override
	public void writeOperLog(int operType, String note) {
		Operlog log = new Operlog();
		log.setUserId(SessionUtil.getUserSession().getUserId());
		log.setOperTime(new Date());
		log.setClientType(Constants.OperLogClientType.BROWSER);
		log.setOperType(operType);
		log.setNote(note);
		save(log);
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.webdevice.log.dao.ICollLogDao#deleteLog(java.util.Date)
	 */
	@Override
	public void deleteLog(Date thresholdDate) {
		String sql = "delete from Operlog where operTime < :thresholddate";
		Query query = createQuery(sql);
		query.setParameter("thresholddate", thresholdDate);
		query.executeUpdate();
	}
	
	/* 
	 * 记录CS客户端操作日志
	 * @see com.hikvision.finance.fms.webdevice.log.dao.ICollLogDao#writeOperLog(int, java.lang.String, java.lang.Integer)
	 */
	@Override
	public void writeOperLog(int operType, String note, Integer nUserId) {
		Operlog log = new Operlog();
		log.setUserId(nUserId);
		log.setOperTime(new Date());
		log.setClientType(Constants.OperLogClientType.CLIENT);
		log.setOperType(operType);
		log.setNote(note);
		save(log);
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.extend.hibernate.dao.BaseDao#buildCriteria(org.hibernate.Criteria, com.hikvision.finance.extend.hibernate.qo.BaseQo)
	 */
	@Override
	protected Criteria buildCriteria(Criteria criteria, OperlogsQo qo) {
		return criteria;
	}
	
}
