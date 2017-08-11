/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月8日 下午4:42:33
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
import com.hikvision.finance.fms.common.util.Constants.RecordType;
import com.hikvision.finance.fms.common.util.StringUtilExpand;
import com.hikvision.finance.fms.model.Recordlog;
import com.hikvision.finance.fms.model.Region;
import com.hikvision.finance.fms.modules.log.dao.IRecordLogDao;
import com.hikvision.finance.fms.modules.log.dto.RecordlogInfo;
import com.hikvision.finance.fms.modules.log.qo.RecordlogQo;
import com.hikvision.finance.fms.modules.region.dao.IRegionDao;
import com.hikvision.finance.fwork.exception.ExpectedException;


/**
 * <p></p>
 * @author jinxindong 2016年4月8日 下午4:42:33
 * @version V1.0 
 */
public class RecordLogDaoImpl extends BaseDao<Recordlog,RecordlogQo> implements IRecordLogDao{



	/**
     * 序列化ID
     */
    private static final long serialVersionUID = -7986979679847932309L;

    private IRegionDao regionDao;
	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.hibernate.dao.BaseDao#buildCriteria(org.hibernate.Criteria, com.hikvision.finance.extend.hibernate.qo.BaseQo)
     */
    @Override
    protected Criteria buildCriteria(Criteria criteria, RecordlogQo qo) {
	    // TODO Auto-generated method stub
	    return null;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.log.dao.IRecordLogDao#getRecordLogDataPage(java.lang.Integer, java.lang.Integer, com.hikvision.finance.fms.modules.log.dto.RecordlogInfo)
     */
    @Override
    public Page getRecordLogDataPage(Integer limit, Integer start, RecordlogInfo recordlogInfo) {
    	//1.封装查询条件
    	SqlQuery sqlQuery = conditionQuery(recordlogInfo);
		
		//2.设置page 属性
		Page page = new Page();
		page.setCondition(sqlQuery);
		page.setPageNo(start);
		page.setPageSize(limit);
	    page = this.queryPaginationBySql(page);
	    return page;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.log.dao.IRecordLogDao#exportRecordLogExcel(com.hikvision.finance.fms.modules.log.dto.RecordlogInfo)
     */
    @SuppressWarnings("unchecked")
    @Override
    public List<RecordlogInfo> exportRecordLogExcel(RecordlogInfo recordlogInfo) {
    	
    	//1.封装查询条件
    	SqlQuery sqlQuery = conditionQuery(recordlogInfo);
    	
    	//2.循环计入数据名称
		List<RecordlogInfo> recordlogInfos = (List<RecordlogInfo>)this.queryListBySql(sqlQuery);
		List<RecordlogInfo> recordLogInfos = new ArrayList<RecordlogInfo>();
		Integer count = recordlogInfos.size();		
		if(count > Constants.EXCEL_MAX_NUM){
			throw new ExpectedException("", "导出数据超过"+Constants.EXCEL_MAX_NUM+"条,无法导出");
		}
		
		if(recordlogInfos.size()>0){
			for(RecordlogInfo infoOld :recordlogInfos){
				if(RecordType.RECORD_SIGN==infoOld.getRecordType()){
					infoOld.setRecordTypeName("签约录像");
				}else if(RecordType.RECORD_COMPLAINNT==infoOld.getRecordType()){
					infoOld.setRecordTypeName("投诉录像");
				}else{
					infoOld.setRecordTypeName("未知");
				}
				recordLogInfos.add(infoOld);
			}
		}
		
		return recordLogInfos;
    }
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.log.dao.IRecordLogDao#deleteLog(java.util.Date)
	 */
	@Override
	public void deleteLog(Date thresholddate) {
		String sql = "delete from Recordlog where operDate < :thresholddate";
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
	private SqlQuery conditionQuery(RecordlogInfo recordlogInfo){

		Integer regionId = recordlogInfo.getRegionId();
    	Integer isflag = null == recordlogInfo.getIsFlag()?0:recordlogInfo.getIsFlag();
		Region region = regionDao.get(regionId);
		String strPathLike = region.getStrPath();
		
		Date operDateStart = null;
		if(StringUtils.isNotBlank(recordlogInfo.getOperDateStart())){
			operDateStart = DateUtils.getDateTime("yyyy-MM-dd HH:mm:ss", recordlogInfo.getOperDateStart());// 业务时间 起始时间
		}
		Date operDateEnd = null;
		if(StringUtils.isNotBlank(recordlogInfo.getOperDateEnd())){
			operDateEnd = DateUtils.getDateTime("yyyy-MM-dd HH:mm:ss", recordlogInfo.getOperDateEnd());// 业务时间 结束时间	
		}
		
    	String checkUserName = null == recordlogInfo.getCheckUserName()?"":recordlogInfo.getCheckUserName();//操作用户
    	Integer recordType = null == recordlogInfo.getRecordType()?null:recordlogInfo.getRecordType();  //录像类型
    	String nproId = null == recordlogInfo.getNproId()?"":recordlogInfo.getNproId();//业务单号
    	String recordName = null == recordlogInfo.getRecordName()?"":recordlogInfo.getRecordName();//录像名称
    	
    	SqlQuery sqlQuery = new SqlQuery();
		StringBuffer sql = new StringBuffer("SELECT r.id AS id,"
				+ "r.recordName AS recordName,"
				+ "r.recordType AS recordType,"
				+ "r.nProId AS nproId,"
				+ "r.remark AS remark,"
				+ "r.operDate AS operDate,"
				+ "r.checkUserName AS checkUserName"
				+ " FROM "
				+ "recordlog r JOIN region reg ON r.regionId = reg.id "
				+ "where 1=1 ");
		
		if(isflag == Constants.CONS_1){//显示旗下所有子孙节点
			sql.append(" and reg.strPath like ? ");
			sqlQuery.addParams(strPathLike+"%");
		}else if(isflag == Constants.CONS_0){//只显示该节点
			sql.append(" and reg.strPath = ? ");
			sqlQuery.addParams(strPathLike);
		}
		
		if (StringUtils.isNotBlank(nproId)) {
			sql.append(" and  r.nProId like ?  escape '/'");
			sqlQuery.addParams("%"+StringUtilExpand.escapeSQLLike(nproId)+"%");
		}
		if (recordType!=null) {
			sql.append(" and r.recordType = ? ");
			sqlQuery.addParams(recordType);
		}
		if (StringUtils.isNotBlank(checkUserName)) {
			sql.append(" and  r.checkUserName like ?  escape '/'");
			sqlQuery.addParams("%"+StringUtilExpand.escapeSQLLike(checkUserName)+"%");
		}
		if (StringUtils.isNotBlank(recordName)) {
			sql.append(" and  r.recordName like ?  escape '/'");
			sqlQuery.addParams("%"+StringUtilExpand.escapeSQLLike(recordName)+"%");
		}
		
		// 时间
		if (operDateStart != null) {
			sql.append(" and r.operDate >= ? ");
			sqlQuery.addParams(operDateStart);
		}
		if (operDateEnd != null) {
			sql.append(" and r.operDate <= ? ");
			sqlQuery.addParams(operDateEnd);
		}
		// 默认倒序
		sql.append(" order by r.id desc");
		// 设置sqlQuery属性
		sqlQuery.setSql(sql.toString());
		sqlQuery.setDtoClass(RecordlogInfo.class);
		return sqlQuery;
	}
	// ================================getter/setter==============================================
	public IRegionDao getRegionDao() {
		return regionDao;
	}
	
	public void setRegionDao(IRegionDao regionDao) {
		this.regionDao = regionDao;
	}
	
}
