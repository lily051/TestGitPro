/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月8日 下午4:42:52
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.log.dao.impl;

import java.util.ArrayList;
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
import com.hikvision.finance.fms.common.util.Constants.LoginStatus;
import com.hikvision.finance.fms.common.util.StringUtilExpand;
import com.hikvision.finance.fms.model.Logsys;
import com.hikvision.finance.fms.modules.log.dao.ILogsysDao;
import com.hikvision.finance.fms.modules.log.dto.LogsysInfo;
import com.hikvision.finance.fms.modules.log.qo.LogsysQo;
import com.hikvision.finance.fwork.exception.ExpectedException;


/**
 * <p></p>
 * @author jinxindong 2016年4月8日 下午4:42:52
 * @version V1.0 
 */
public class LogsysDaoImpl extends BaseDao<Logsys,LogsysQo> implements ILogsysDao{

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = -3892664593689460450L;

	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.hibernate.dao.BaseDao#buildCriteria(org.hibernate.Criteria, com.hikvision.finance.extend.hibernate.qo.BaseQo)
     */
    @Override
    protected Criteria buildCriteria(Criteria criteria, LogsysQo qo) {
	    // TODO Auto-generated method stub
	    return null;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.log.dao.ILogsysDao#getLogsysDataPage(java.lang.Integer, java.lang.Integer, com.hikvision.finance.fms.modules.log.dto.LogsysInfo)
     */
    @Override
    public Page getLogsysDataPage(Integer limit, Integer start, LogsysInfo logsysInfo) {
    	//1.封装查询条件
    	SqlQuery sqlQuery = conditionQuery(logsysInfo);
		
		//2.设置page 属性
		Page page = new Page();
		page.setCondition(sqlQuery);
		page.setPageNo(start);
		page.setPageSize(limit);
	    page = this.queryPaginationBySql(page);
	    return page;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.log.dao.ILogsysDao#exportLogsysExcel(com.hikvision.finance.fms.modules.log.dto.LogsysInfo)
     */
    @SuppressWarnings("unchecked")
    @Override
	public List<LogsysInfo> exportLogsysExcel(LogsysInfo logsysInfo) {
		// 1.封装查询条件
		SqlQuery sqlQuery = conditionQuery(logsysInfo);
		
		// 2.循环计入数据名称
		List<LogsysInfo> logsysInfos = (List<LogsysInfo>)this.queryListBySql(sqlQuery);
		List<LogsysInfo> logSysInfos = new ArrayList<LogsysInfo>();
		if (logsysInfos != null && logsysInfos.size() > 0) {
			Integer count = logsysInfos.size();
			if (count > Constants.EXCEL_MAX_NUM) {
				throw new ExpectedException("", "导出数据超过" + Constants.EXCEL_MAX_NUM + "条,无法导出");
			}
			for (LogsysInfo infoOld : logsysInfos) {
				if (LoginStatus.LOGIN_IN == infoOld.getNtype()) {
					infoOld.setNtypeName("登录");
				} else if (LoginStatus.LOGIN_OUT == infoOld.getNtype()) {
					infoOld.setNtypeName("登出");
				} else {
					infoOld.setNtypeName("未知");
				}
				if (0 == infoOld.getNmoduleType()) {
					infoOld.setNmoduleTypeName("中心管理员");
				} else if (1 == infoOld.getNmoduleType()) {
					infoOld.setNmoduleTypeName("理财经理");
				} else {
					infoOld.setNmoduleTypeName("未知");
				}
				logSysInfos.add(infoOld);
			}
		}
		return logSysInfos;
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.log.dao.ILogsysDao#deleteLog(java.util.Date)
	 */
	@Override
	public void deleteLog(Date thresholddate) {
		String sql = "delete from Logsys where strDate < :thresholddate";
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
	private SqlQuery conditionQuery(LogsysInfo logsysInfo){

		Date strDateStart = null;
		if(StringUtils.isNotBlank(logsysInfo.getStrDateStart())){
			strDateStart = DateUtils.getDateTime("yyyy-MM-dd HH:mm:ss", logsysInfo.getStrDateStart());// 业务时间 起始时间
		}
		Date strDateEnd = null;
		if(StringUtils.isNotBlank(logsysInfo.getStrDateEnd())){
			strDateEnd = DateUtils.getDateTime("yyyy-MM-dd HH:mm:ss", logsysInfo.getStrDateEnd());// 业务时间 结束时间	
		}
		
		String strUser = null == logsysInfo.getStrUser()?"":logsysInfo.getStrUser();// 操作用户
		Integer ntype = null == logsysInfo.getNtype()?null:logsysInfo.getNtype(); // 操作类型
		Integer nmoduleType = null == logsysInfo.getNmoduleType()?null:logsysInfo.getNmoduleType(); // 用户类型

    	
    	SqlQuery sqlQuery = new SqlQuery();
		StringBuffer sql = new StringBuffer("SELECT "
				+ "l.id AS id,"
				+ "l.nModuleType AS nmoduleType,"
				+ "l.nType AS ntype,"
				+ "l.strUser AS strUser,"
				+ "l.strDate AS strDate,"
				+ "l.strIP AS strIp,"
				+ "l.strVersionInfo AS strVersionInfo "
				+ "FROM logsys l "
				+ "where 1=1 ");

		if (ntype != null) {
			sql.append(" and l.nType = ? ");
			sqlQuery.addParams(ntype);
		}
		if (StringUtils.isNotBlank(strUser)) {
			sql.append(" and  l.strUser like ? escape '/'");
			sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(strUser) + "%");
		}
		if (nmoduleType!=null) {
			sql.append(" and  l.nmoduleType = ? ");
			sqlQuery.addParams(nmoduleType);
		}

		// 时间
		if (strDateStart != null) {
			sql.append(" and l.strDate >= ?");
			sqlQuery.addParams(strDateStart);
		}
		if (strDateEnd != null) {
			sql.append(" and l.strDate <= ?");
			sqlQuery.addParams(strDateEnd);
		}
		// 默认倒序
		sql.append(" order by l.id desc");
		// 设置sqlQuery属性
		sqlQuery.setSql(sql.toString());
		sqlQuery.setDtoClass(LogsysInfo.class);
		return sqlQuery;
	}
}
