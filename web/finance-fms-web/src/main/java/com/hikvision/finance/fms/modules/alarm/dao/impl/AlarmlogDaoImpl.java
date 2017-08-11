/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月6日 下午7:21:55
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.alarm.dao.impl;

import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import org.hibernate.Criteria;
import org.hibernate.Query;

import com.hikvision.finance.core.util.BeanUtils;
import com.hikvision.finance.core.util.DateUtils;
import com.hikvision.finance.core.util.StringUtils;
import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.hibernate.dao.BaseDao;
import com.hikvision.finance.extend.hibernate.qo.SqlQuery;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.model.Alarmlog;
import com.hikvision.finance.fms.model.Region;
import com.hikvision.finance.fms.modules.alarm.dao.IAlarmlogDao;
import com.hikvision.finance.fms.modules.alarm.dto.AlarmlogInfo;
import com.hikvision.finance.fms.modules.alarm.qo.AlarmlogQo;
import com.hikvision.finance.fms.modules.region.dao.IRegionDao;
import com.hikvision.finance.fwork.exception.ExpectedException;


/**
 * <p></p>
 * @author jinxindong 2016年4月6日 下午7:21:55
 * @version V1.0 
 */
public class AlarmlogDaoImpl extends BaseDao<Alarmlog,AlarmlogQo> implements IAlarmlogDao{

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = -5939266750424775973L;
    private IRegionDao regionDao;
	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.hibernate.dao.BaseDao#buildCriteria(org.hibernate.Criteria, com.hikvision.finance.extend.hibernate.qo.BaseQo)
     */
    @Override
    protected Criteria buildCriteria(Criteria criteria, AlarmlogQo qo) {
	    // TODO Auto-generated method stub
	    return null;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.alarm.dao.IAlarmlogDao#getAlarmDataPage(java.lang.Integer, java.lang.Integer, com.hikvision.finance.fms.modules.alarm.dto.AlarmlogInfo)
     */
    @Override
    public Page getAlarmDataPage(Integer limit, Integer start, AlarmlogInfo alarmlogInfo) {
    	Integer regionId = null == alarmlogInfo.getAlarmRegionId()?1:alarmlogInfo.getAlarmRegionId();
    	Integer isflag = null == alarmlogInfo.getIsFlag()?0:alarmlogInfo.getIsFlag();
		Region region = regionDao.get(regionId);
		String strPathLike = region.getStrPath();
		Date alarmbeginTimeStart = null;
		if(StringUtils.isNotBlank( alarmlogInfo.getAlarmbeginTimeStart())){
			alarmbeginTimeStart = DateUtils.getDateTime("yyyy-MM-dd HH:mm:ss", alarmlogInfo.getAlarmbeginTimeStart()) ;//报警发生开始 起始时间
		}
		Date alarmbeginTimeEnd = null;
		if(StringUtils.isNotBlank(alarmlogInfo.getAlarmbeginTimeEnd())){
			alarmbeginTimeEnd =  DateUtils.getDateTime("yyyy-MM-dd HH:mm:ss", alarmlogInfo.getAlarmbeginTimeEnd()) ;//报警发生开始  结束时间
		}
		
    	Integer alarmType = null == alarmlogInfo.getAlarmType()?null:alarmlogInfo.getAlarmType();//报警类型
    	Integer status = null == alarmlogInfo.getStatus()?null:alarmlogInfo.getStatus();//处理状态
    	   	
    	SqlQuery sqlQuery = new SqlQuery();
		StringBuffer sql = new StringBuffer("SELECT a.id AS id,a.`status` AS `status`,"
				+ "a.alarmType AS alarmType,a.alarmbeginTime AS alarmbeginTime,"
				+ "a.alarmTime AS alarmTime,a.strHost AS strHost,a.comments AS comments,"
				+ "a.dealcomment AS dealcomment,a.checker AS checker,"
				+ "a.userNo AS userNo,a.dealTime AS dealTime,a.alarmRegionId AS alarmRegionId "
				+ "FROM alarmlog a JOIN region reg ON a.alarmRegionId = reg.id where 1=1 ");
		
		if(isflag == Constants.CONS_1){//显示旗下所有子孙节点
			sql.append(" and reg.strPath like ? ");
			sqlQuery.addParams(strPathLike+"%");
		}else if(isflag == Constants.CONS_0){//只显示该节点
			sql.append(" and reg.strPath = ? ");
			sqlQuery.addParams(strPathLike);
		}
		
		if (null != alarmType) {
			sql.append(" and a.alarmType = ? ");
			sqlQuery.addParams(alarmType);
		}
		if (null != status) {
			sql.append(" and a.`status` = ? ");
			sqlQuery.addParams(status);
		}
		
		// 时间
		if (alarmbeginTimeStart != null) {
			sql.append(" and a.alarmbeginTime >= ?");
			sqlQuery.addParams(alarmbeginTimeStart);
		}
		if (alarmbeginTimeEnd != null) {
			sql.append("and a.alarmbeginTime <= ? ");
			sqlQuery.addParams(alarmbeginTimeEnd);
		}
		// 默认倒序
		sql.append(" order by a.`status` asc,a.id desc");
		// 设置sqlQuery属性
		sqlQuery.setSql(sql.toString());
		sqlQuery.setDtoClass(AlarmlogInfo.class);
		
		// 设置page 属性
		Page page = new Page();
		page.setCondition(sqlQuery);
		page.setPageNo(start);
		page.setPageSize(limit);
	    page = this.queryPaginationBySql(page);
	    return page;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.alarm.dao.IAlarmlogDao#getIndexAlarm(java.lang.Integer)
     */
    @SuppressWarnings("unchecked")
    @Override
    public List<AlarmlogInfo> getIndexAlarm(Integer limit) {
		String sql = "SELECT a.* FROM Alarmlog AS a  where a.status = 0 ORDER BY a.id DESC LIMIT ?";
		List<Alarmlog> list = (List<Alarmlog>)this.createSQLQuery(sql).addEntity(Alarmlog.class).setParameter(0, limit).list();
		List<AlarmlogInfo> alarmlogInfos = new ArrayList<AlarmlogInfo>();
		for(Alarmlog alarmlog:list){
			AlarmlogInfo info = new AlarmlogInfo();
			try {
				BeanUtils.copyNotNullProperties(info, alarmlog);
			} catch (Exception e) {
				throw new ExpectedException("", "拷贝bean失败[getIndexAlarm]");
			}
			alarmlogInfos.add(info);
		}
		return alarmlogInfos;

    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.alarm.dao.IAlarmlogDao#deleteLog(java.util.Date)
     */
    @Override
    public void deleteLog(Date thresholdDate) {
		String sql = "delete from Alarmlog where dealTime is not null and dealTime < :thresholddate";
		Query query = createQuery(sql);
		query.setParameter("thresholddate", thresholdDate);
		query.executeUpdate();
    }

	/* (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.alarm.dao.IAlarmlogDao#getAlarmlogStatus(com.hikvision.finance.fms.cms.hpp.center.AlarmlogProto.AlarmlogInfo)
	 */
	@Override
	public Alarmlog getAlarmlogStatus(
			com.hikvision.finance.fms.cms.hpp.center.AlarmlogProto.AlarmlogInfo request) {
		StringBuffer sql = new StringBuffer("select * from alarmlog al where al.alarmType = ");
		sql.append(request.getAlarmlog().getAlarmType());
		sql.append(" and al.checkChan =");
		sql.append(request.getAlarmlog().getCheckChan());
		sql.append(" and al.strHost =");
		sql.append("'");
		sql.append(request.getAlarmlog().getStrHost());
		sql.append("'");
		sql.append(" and al.comments =");
		sql.append("'");
		sql.append(request.getAlarmlog().getComments());
		sql.append("'");
		sql.append(" ORDER BY al.alarmTime desc LIMIT 1");
		return  (Alarmlog)this.createSQLQuery(sql.toString()).addEntity(Alarmlog.class).uniqueResult();
	}

	
    public IRegionDao getRegionDao() {
    	return regionDao;
    }

	
    public void setRegionDao(IRegionDao regionDao) {
    	this.regionDao = regionDao;
    }
	
	
}
