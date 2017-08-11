/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月15日 下午1:55:36
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.record.dao.impl;

import java.util.Date;
import java.util.List;

import org.hibernate.Criteria;
import org.hibernate.Query;
import org.hibernate.SQLQuery;

import com.hikvision.finance.core.util.DateUtils;
import com.hikvision.finance.core.util.ListUtil;
import com.hikvision.finance.core.util.StringUtils;
import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.hibernate.dao.BaseDao;
import com.hikvision.finance.extend.hibernate.qo.SqlQuery;
import com.hikvision.finance.fms.cms.hpp.center.RecordsProto.ReqRecords;
import com.hikvision.finance.fms.common.enums.ProSaveRecordEnum;
import com.hikvision.finance.fms.common.util.StringUtilExpand;
import com.hikvision.finance.fms.model.Clientinput;
import com.hikvision.finance.fms.model.Prosaverecord;
import com.hikvision.finance.fms.model.Records;
import com.hikvision.finance.fms.model.Sysconfig;
import com.hikvision.finance.fms.modules.analysis.dto.AnalysisInfo;
import com.hikvision.finance.fms.modules.record.dao.IProSaveRecordDao;
import com.hikvision.finance.fms.modules.record.dto.RecordInfo;
import com.hikvision.finance.fms.modules.record.qo.ProSaveRecordQo;


/**
 * <p></p>
 * @author jinxindong 2016年4月15日 下午1:55:36
 * @version V1.0 
 */
public class ProSaveRecordDaoImpl extends BaseDao<Prosaverecord,ProSaveRecordQo> implements IProSaveRecordDao{

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 8256442665559515888L;

	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.hibernate.dao.BaseDao#buildCriteria(org.hibernate.Criteria, com.hikvision.finance.extend.hibernate.qo.BaseQo)
     */
    @Override
    protected Criteria buildCriteria(Criteria criteria, ProSaveRecordQo qo) {
	    return criteria;
    }
    
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.record.dao.IProSaveRecordDao#getDataPage(com.hikvision.finance.fms.modules.record.dto.RecordInfo, java.lang.Integer,
	 * java.lang.Integer)
	 */
	public Page getDataPage(RecordInfo recordInfo, Integer start, Integer limit) {
		SqlQuery sqlQuery = creatRecordSqlQuery(recordInfo);
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
	 * @see com.hikvision.finance.fms.modules.record.dao.IProSaveRecordDao#getDataList(com.hikvision.finance.fms.modules.record.dto.RecordInfo)
	 */
	@SuppressWarnings("unchecked")
	public List<RecordInfo> getDataList(RecordInfo recordInfo) {
		SqlQuery sqlQuery = creatRecordSqlQuery(recordInfo);
		List<RecordInfo> recordInfoList = (List<RecordInfo>)queryListBySql(sqlQuery);
		return recordInfoList;
	}
	
	/* (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.record.dao.IProSaveRecordDao#getClientRecordsList(com.hikvision.finance.fms.modules.record.dto.RecordInfo)
	 */
	@SuppressWarnings("unchecked")
	public List<RecordInfo> getClientRecordsList(RecordInfo recordInfo) {
		
		SqlQuery sqlQuery = createClientRecordsSqlQuery(recordInfo);
		List<RecordInfo> recordInfoList = (List<RecordInfo>)queryListBySql(sqlQuery);
		return recordInfoList;
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.record.dao.IProSaveRecordDao#setRecordCheck(com.hikvision.finance.fms.modules.record.dto.RecordInfo)
	 */
	public void setRecordCheck(RecordInfo recordInfo) {
		String hql = "update Prosaverecord set recordCheck = ?, checkRemark = ? where id = ?";
		Query query = this.createQuery(hql,
		        new Object[] {recordInfo.getRecordCheck(), recordInfo.getCheckRemark(), recordInfo.getVideoId()});
		query.executeUpdate();
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.record.dao.IProSaveRecordDao#setDelayDays(com.hikvision.finance.fms.modules.record.dto.RecordInfo)
	 */
	public void setDelayDays(RecordInfo recordInfo) {
		String sql = "update clientinput set strVideoEndDate = DATE_ADD(strVideoEndDate, INTERVAL " + recordInfo.getDelayDays()
		        + " DAY), delaydays = delaydays + " + recordInfo.getDelayDays() + " where id in (" + ListUtil.listToStr(recordInfo.getBusiIds()) + ")";
		SQLQuery sqlQuery = this.createSQLQuery(sql);
		sqlQuery.executeUpdate();
	}
	
	/**
	 * 拼装sql查询语句的私有方法
	 * @author jinchenwu 2016年4月7日 下午1:44:22
	 * @param recordInfo
	 * @return
	 */
	private SqlQuery creatRecordSqlQuery(RecordInfo recordInfo) {
		
		// 设置sqlQuery属性，sql语句，不是hql
		SqlQuery sqlQuery = new SqlQuery();
		StringBuilder sql = new StringBuilder(
		        "select c.id as busiId,"
		        + " p.id as videoId,"
		        + " p.fileName as fileName,"
		        + " c.inputRegionId as inputRegionId,"// 交易网点Id
		        + " c.inputRegionCode as inputRegionCode,"// 交易区域编码
		        + " c.inputRegionName as inputRegionName,"// 交易网点名称
		        + " c.status as status,"
		        + " c.nProId as nproId,"
		        + " c.lockClient as lockClient,"		        
		        + " c.businessTime as businessTime,"// 办理时间
		        + " c.strVideoEndDate as strVideoEndDate,"
		        + " c.proUserNo as proUserNo,"
		        + " c.proName as proName,"// 业务员姓名
		        + " c.productId as productId,"//产品ID
		        + " c.productCode as productCode,"
		        + " c.productName as productName,"// 产品名称
		        + " c.productTypeId as productTypeId,"
		        + " c.productTypeName as productTypeName,"// 产品类型名称
		        + " c.name as name,"// 客户姓名
		        + " c.strCreditPhone as strCreditPhone,"// 客户电话
		        + " c.creditCode as creditCode,"// 客户证件号
		        + " c.creditId as creditId,"// 客户证件类型
		        + " c.strBuyMoney as strBuyMoney,"
		        + " c.strBankNo as strBankNo,"// 银行卡号
		        + " c.strGrade as strGrade,"// 产品发行机构
		        + " p.recordCheck as recordCheck,"
		        + " p.stateType as stateType,"
		        + " p.upload as upload,"
		        + " p.checkRemark as checkRemark,"
		        + " p.devChannel as channel,"
		        + " p.devId as cdevId,"
		        + " p.cvr as devId,"
		        + " p.streamNo as streamNo,"
		        + " p.startTime as startTime,"
		        + " p.endTime as endTime,"
		        + " p.rtspPath as rtspPath,"
		        + " p.nFileSize as nFileSize,"
		        + " p.strUrl as strUrl"
		        + " from prosaverecord p join clientinput c"
		        + " on p.clientId = c.id");
		//and c.nisPay = ? === > sqlQuery.addParams(Constants.VideoDueStatus.NOT_DUE);// 只显示未兑付的录像
		if (recordInfo != null) {
			if(StringUtils.isNotBlank(recordInfo.getStrRegionIds())){
				sql.append(" and p.regionId in (" + recordInfo.getStrRegionIds() + ")");
			}
			if (recordInfo.getVideoId() != null){
				sql.append(" and p.id = ?");
				sqlQuery.addParams(recordInfo.getVideoId());
			}
			if (recordInfo.getLockClients() != null && recordInfo.getLockClients().size() > 0) {
				sql.append(" and  c.lockClient in ( "+ ListUtil.listToStr(recordInfo.getLockClients()) + " ) ");
			}
			// 决定是签约录像还是投诉录像
			if (recordInfo.getStateType() != null) {
				sql.append(" and p.stateType = ?");
				sqlQuery.addParams(recordInfo.getStateType());
			}
			// 以下两个if判断用于约束录像开始时间的范围
			if (recordInfo.getStartTime() != null) {
				sql.append(" and p.startTime >= ?");
				sqlQuery.addParams(recordInfo.getStartTime());
			}
			if (recordInfo.getEndTime() != null) {
				sql.append(" and p.startTime <= ?");
				sqlQuery.addParams(recordInfo.getEndTime());
			}
			// 是否合规
			if (recordInfo.getRecordCheck() != null) {
				sql.append(" and p.recordCheck = ?");
				sqlQuery.addParams(recordInfo.getRecordCheck());
			}
			// 是否存在纠纷
			if (recordInfo.getStatus() != null) {
				sql.append(" and c.status = ?");
				sqlQuery.addParams(recordInfo.getStatus());
			}
			if (StringUtils.isNotBlank(recordInfo.getProductCode())) {
				sql.append(" and c.productCode like ? escape '/'");
				sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(recordInfo.getProductCode()) + "%");
			}
			if (StringUtils.isNotBlank(recordInfo.getProductName())) {
				sql.append(" and c.productName like ? escape '/'");
				sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(recordInfo.getProductName()) + "%");
			}
			if (StringUtils.isNotBlank(recordInfo.getNproId())) {
				sql.append(" and c.nProId like ? escape '/'");
				sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(recordInfo.getNproId()) + "%");
			}
			if (StringUtils.isNotBlank(recordInfo.getProUserNo())) {
				sql.append(" and c.proUserNo like ? escape '/'");
				sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(recordInfo.getProUserNo()) + "%");
			}
			if (StringUtils.isNotBlank(recordInfo.getProName())) {
				sql.append(" and c.proName like ? escape '/'");
				sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(recordInfo.getProName()) + "%");
			}
			if (StringUtils.isNotBlank(recordInfo.getCreditCode())) {
				sql.append(" and c.creditCode like ? escape '/'");
				sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(recordInfo.getCreditCode()) + "%");
			}
			if (StringUtils.isNotBlank(recordInfo.getName())) {
				sql.append(" and c.name like ? escape '/'");
				sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(recordInfo.getName()) + "%");
			}
			if (recordInfo.getInputUser() != null && recordInfo.getInputUser() > 0) {
				sql.append(" and  c.inputUser = ?");
				sqlQuery.addParams(recordInfo.getInputUser());
			}
			// 是否已上传
			if (recordInfo.getUpload() != null) {
				sql.append(" and p.upload = ?");
				sqlQuery.addParams(recordInfo.getUpload());
			}
			sql.append(" order by p.id desc");
		}
		sqlQuery.setSql(sql.toString());
		sqlQuery.setDtoClass(RecordInfo.class);
		return sqlQuery;
	}
	
	
	
	private SqlQuery createClientRecordsSqlQuery(RecordInfo recordInfo) {
		
		// 设置sqlQuery属性，sql语句，不是hql
		SqlQuery sqlQuery = new SqlQuery();
		StringBuilder sql = new StringBuilder(
		        "select c.id as busiId,"
		        + " p.id as videoId,"
		        + " p.fileName as fileName,"
		        + " c.inputRegionId as inputRegionId,"// 交易网点Id
		        + " c.inputRegionCode as inputRegionCode,"// 交易区域编码
		        + " c.inputRegionName as inputRegionName,"// 交易网点名称
		        + " c.status as status,"
		        + " c.nProId as nproId,"
		        + " c.lockClient as lockClient,"		        
		        + " c.businessTime as businessTime,"// 办理时间
		        + " c.strVideoEndDate as strVideoEndDate,"
		        + " c.proUserNo as proUserNo,"
		        + " c.proName as proName,"// 业务员姓名
		        + " c.productId as productId,"//产品ID
		        + " c.productCode as productCode,"
		        + " c.productName as productName,"// 产品名称
		        + " c.productTypeId as productTypeId,"
		        + " c.productTypeName as productTypeName,"// 产品类型名称
		        + " c.name as name,"// 客户姓名
		        + " c.strCreditPhone as strCreditPhone,"// 客户电话
		        + " c.creditId as creditId,"// 客户证件类型
		        + " c.creditCode as creditCode,"// 客户证件号
		        + " c.strBuyMoney as strBuyMoney,"
		        + " c.strBankNo as strBankNo,"// 银行卡号
		        + " c.strGrade as strGrade,"// 产品发行机构
		        + " p.recordCheck as recordCheck,"
		        + " p.stateType as stateType,"
		        + " p.upload as upload,"
		        + " p.checkRemark as checkRemark,"
		        + " p.devChannel as channel,"
		        + " p.devId as cdevId,"
		        + " p.cvr as devId,"
		        + " p.streamNo as streamNo,"
		        + " p.screenshotSrc as screenshotSrc,"
		        + " p.startTime as startTime,"
		        + " p.endTime as endTime,"
		        + " p.rtspPath as rtspPath,"
		        + " p.nFileSize as nFileSize,"
		        + " p.strUrl as strUrl"
		        + " from prosaverecord p join clientinput c"
		        + " on p.clientId = c.id");
		if (recordInfo != null) {
			if (recordInfo.getStartTime() != null) {
				sql.append(" and c.businessTime >= ?");
				sqlQuery.addParams(recordInfo.getStartTime());
			}
			if (recordInfo.getEndTime() != null) {
				sql.append(" and c.businessTime <= ?");
				sqlQuery.addParams(recordInfo.getEndTime());
			}
			if (StringUtils.isNotBlank(recordInfo.getProductName())) {
				sql.append(" and c.productName like ? escape '/'");
				sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(recordInfo.getProductName()) + "%");
			}
			if (StringUtils.isNotBlank(recordInfo.getName())) {
				sql.append(" and c.name like ? escape '/'");
				sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(recordInfo.getName()) + "%");
			}
			if (recordInfo.getInputUser() != null && recordInfo.getInputUser() > 0) {
				sql.append(" and  c.inputUser = ?");
				sqlQuery.addParams(recordInfo.getInputUser());
			}
			sql.append(" order by p.id desc");
		}
		sqlQuery.setSql(sql.toString());
		sqlQuery.setDtoClass(RecordInfo.class);
		return sqlQuery;
	}
	
	/* (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.record.dao.IProSaveRecordDao#getDataCount(com.hikvision.finance.fms.modules.record.dto.RecordCount)
	 */
	@SuppressWarnings("unchecked")
	public List<RecordInfo> getVideoCount(RecordInfo recordInfo) {
		
		SqlQuery sqlQuery = creatRecordCountSqlQuery(recordInfo);
		List<RecordInfo> list = (List<RecordInfo>)this.queryListBySql(sqlQuery);
		return list;
	}
	
	/**
	 * 查询业务单的录像数量
	 * @author zhoujiajun 2016年10月24日 上午11:14:30
	 * @param recordInfo
	 * @return
	 */
	private SqlQuery creatRecordCountSqlQuery(RecordInfo recordInfo) {

		// 设置sqlQuery属性，sql语句，不是hql
		SqlQuery sqlQuery = new SqlQuery();
		StringBuilder sql = new StringBuilder(
		        "SELECT p.clientId as busiId, "
		        + " count(p.clientId) as videoCount"
		        + " from prosaverecord p join clientinput c "
		        + " on p.clientId = c.id "
		        + " where 1 = 1");
		if (recordInfo != null) {
			if(recordInfo.getInputUser() != null && recordInfo.getInputUser() > 0){
				sql.append(" and c.inputUser = ?");
				sqlQuery.addParams(recordInfo.getInputUser());
			}
			if(StringUtils.isNotBlank(recordInfo.getProductName())){
				sql.append(" and c.productName like ? escape '/'");
				sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(recordInfo.getProductName()) + "%");
			}
			if(StringUtils.isNotBlank(recordInfo.getName())){
				sql.append(" and c.name like ? escape '/'");
				sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(recordInfo.getName()) + "%");
			}
			if (recordInfo.getStartTime() != null) {
				sql.append(" and c.businessTime >= ?");
				sqlQuery.addParams(recordInfo.getStartTime());
			}
			if (recordInfo.getEndTime() != null) {
				sql.append(" and c.businessTime <= ?");
				sqlQuery.addParams(recordInfo.getEndTime());
			}
		}
		sql.append(" GROUP BY p.clientId;");
		sqlQuery.setSql(sql.toString());
		sqlQuery.setDtoClass(RecordInfo.class);
		return sqlQuery;
	}

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.analysis.dao.IProSaveRecordDao#getRecordByCheck()
     */
    @SuppressWarnings("unchecked")
    @Override
    public List<Prosaverecord> getRecordByCheck(AnalysisInfo analysisInfo) {
    	Date recordStart =  DateUtils.getDateTime("yyyy-MM-dd HH:mm:ss", analysisInfo.getRecordStart()); //开始时间
    	Date recordEnd = DateUtils.getDateTime("yyyy-MM-dd HH:mm:ss", analysisInfo.getRecordEnd()); //结束时间
    	String sql = "SELECT * FROM Prosaverecord p where stateType = 1 and p.endTime >= ? AND p.endTime <= ?";
		return  this.createSQLQuery(sql).addEntity(Prosaverecord.class).setParameter(0, recordStart).setParameter(1, recordEnd).list();
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.analysis.dao.IProSaveRecordDao#getIndexRecordByCheck(com.hikvision.finance.fms.modules.analysis.dto.AnalysisInfo)
     */
    @SuppressWarnings("unchecked")
    @Override
    public List<Prosaverecord> getIndexRecordByCheck(AnalysisInfo analysisInfo) {
    	String sql = "SELECT * FROM prosaverecord p WHERE stateType = 1 and YEARWEEK(date_format(p.endTime,'%Y-%m-%d')) = YEARWEEK(now())";
		return  this.createSQLQuery(sql).addEntity(Prosaverecord.class).list();
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.analysis.dao.IProSaveRecordDao#getRecordByCheck(com.hikvision.finance.fms.modules.analysis.dto.AnalysisInfo, java.lang.String, int)
     */
    @Override
	public Integer getRecordByCheck(AnalysisInfo analysisInfo, String regionIds, int recordCheck) {
		Date recordStart = DateUtils.getDateTime("yyyy-MM-dd HH:mm:ss", analysisInfo.getRecordStart()); // 开始时间
		Date recordEnd = DateUtils.getDateTime("yyyy-MM-dd HH:mm:ss", analysisInfo.getRecordEnd()); // 结束时间
		String sql = "SELECT COUNT(id) FROM Prosaverecord p where stateType = 1 and p.endTime >= ? AND p.endTime <= ? AND p.recordCheck = ? AND p.regionId in (" + regionIds + ")";
		return Integer.valueOf(this.createSQLQuery(sql).setParameter(0, recordStart).setParameter(1, recordEnd).setParameter(2, recordCheck).uniqueResult().toString());
	}

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.analysis.dao.IProSaveRecordDao#getIndexRecordByCheck(com.hikvision.finance.fms.modules.analysis.dto.AnalysisInfo, java.lang.String, int)
     */
    @Override
    public Integer getIndexRecordByCheck(AnalysisInfo analysisInfo, String regionIds, int recordCheck) {
		String sql = "SELECT COUNT(id) FROM Prosaverecord p where stateType = 1 and  YEARWEEK(date_format(p.endTime,'%Y-%m-%d')) = YEARWEEK(now()) AND p.recordCheck = ? AND p.regionId in (" + regionIds + ")";
		return Integer.valueOf(this.createSQLQuery(sql).setParameter(0, recordCheck).uniqueResult().toString());
    }
    
    /********************以下是dc的dao移过来的***************************/
    
	/* (non-Javadoc)
	 * @see com.hikvision.finance.fms.cms.dao.IProsaverecordDao#getProsaverecoreTime()
	 */
	@SuppressWarnings("unchecked")
	@Override
	public List<Sysconfig> getProsaverecoreTime() {
		StringBuffer sql = new StringBuffer("SELECT * FROM sysconfig where nKey in ('6104','6105','6106') ");
		return this.createSQLQuery(sql.toString()).addEntity(Sysconfig.class).list();
		
	}

	/* (non-Javadoc)
	 * @see com.hikvision.finance.fms.cms.dao.IProsaverecordDao#getOvertimeRecordInfo(java.lang.Integer)
	 */
	@SuppressWarnings("unchecked")
	@Override
	public List<Clientinput> getOvertimeRecordInfo() {
		StringBuffer sql = new StringBuffer("SELECT * FROM clientinput ci WHERE ci.nisPay = 0 and ci.strVideoEndDate < ");
		sql.append("date_format(NOW(),"); 
		sql.append("'");
		sql.append("%Y-%m-%d");
		sql.append("'");
		sql.append(")");
		return this.createSQLQuery(sql.toString()).addEntity(Clientinput.class).list();
	}

	/* (non-Javadoc)
	 * @see com.hikvision.finance.fms.cms.dao.IProsaverecordDao#getProsaverecordInfoByclientid(int)
	 */
	@SuppressWarnings("unchecked")
	@Override
	public List<Prosaverecord> getProsaverecordInfoByclientid(int clientId) {
		StringBuffer sql = new StringBuffer("SELECT * FROM prosaverecord WHERE clientID = ?");
		return this.createSQLQuery(sql.toString()).addEntity(Prosaverecord.class).setParameter(0, clientId).list();
	}

	/* (non-Javadoc)
	 * @see com.hikvision.finance.fms.cms.dao.IProsaverecordDao#updateRecordsInfoBystatus(int, int)
	 */
	@Override
	public void updateRecordsInfoBystatus(ReqRecords request,com.hikvision.finance.fms.model.Records record) {
		StringBuffer sql = new StringBuffer();
		if(9==request.getNRecordStatus()){  //9--表示是视频上传失败
			sql.append("UPDATE Records SET nUpLoadPercent = 0, nRecordStatus = 9, errorMsg= ");
			sql.append("'").append(request.getErrorMsg()).append("'");
			sql.append(" WHERE id = ").append(request.getId());
		}else if(3==request.getNRecordStatus()){ //3--表示视频为上传中
			sql.append("UPDATE Records SET nRecordStatus = 3, nUpLoadPercent = "); 
			sql.append(request.getNUpLoadPercent());
			sql.append(" ,nTaskId = ").append(request.getNTaskId());
			sql.append(" WHERE id = ").append(request.getId());
		}else if(4==request.getNRecordStatus()){ //4--表示视频已上传，然后把对应的prosaverecords表中的upload设置为1
			sql.append("update records re,prosaverecord pro set re.nRecordStatus = 4, pro.upload = 1");
			if (StringUtils.isNotBlank(request.getStreamNo())) {
				sql.append(" ,pro.streamNo = ").append("'").append(request.getStreamNo()).append("'");
			}
			if (StringUtils.isNotBlank(request.getStartTime())) {
				sql.append(" ,pro.startTime = ").append("'").append(request.getStartTime()).append("'");
			}
			if (StringUtils.isNotBlank(request.getEndTime())) {
				sql.append(" ,pro.endTime = ").append("'").append(request.getEndTime()).append("'");
			}
			if (StringUtils.isNotBlank(request.getRtspPath())) {
				sql.append(" ,pro.rtspPath =").append("'").append(request.getRtspPath()).append("'");
			}
			if (request.getNFileSize() > 0) {
				sql.append(" ,pro.nFileSize=").append(request.getNFileSize());
			}
			sql.append(" where re.nHistoryRecordId = pro.id and re.id = ");
			sql.append(request.getId());
		}
		Query query = createSQLQuery(sql.toString());
		query.executeUpdate();
		
	}

	/* (non-Javadoc)
	 * @see com.hikvision.finance.fms.cms.dao.IProsaverecordDao#getClientinput(int)
	 */
	@Override
	public Clientinput getClientinput(int clientId) {
		StringBuffer sql = new StringBuffer("select * from clientinput where id = ?");
		return (Clientinput) this.createSQLQuery(sql.toString()).addEntity(Clientinput.class).setParameter(0, clientId).uniqueResult();
	}

	/* (non-Javadoc)
	 * @see com.hikvision.finance.fms.cms.dao.IProsaverecordDao#delClientinput(java.lang.Integer)
	 */
	@Override
	public void delClientinput(Integer id) {
		StringBuffer sql = new StringBuffer("UPDATE clientinput set nisPay = 1 where id = ?");
		this.createSQLQuery(sql.toString()).setParameter(0, id).executeUpdate();
		
	}

	/* (non-Javadoc)
	 * @see com.hikvision.finance.fms.cms.dao.IProsaverecordDao#getRecordsbyRecordsStatus(java.lang.String, java.lang.String)
	 */
	@SuppressWarnings("unchecked")
	@Override
	public List<Records> getRecordsbyRecordsStatus(String clientIP,
			String modelusb) {
		StringBuffer sql = new StringBuffer();// Coverity提示初始化不能为null
		if("usb".equals(modelusb)){//USB模式
			sql = new StringBuffer("SELECT * FROM Records WHERE nRecordStatus in (2,3) and strIp = ");//降序  
			sql.append("'");
			sql.append(clientIP);
			sql.append("'");
			sql.append(" ORDER BY nRecordStatus desc");
		}else if("ipc".equals(modelusb)){
			sql = new StringBuffer("SELECT * FROM Records WHERE nRecordStatus in (2,3) ORDER BY nRecordStatus desc LIMIT 50");//降序
		}
		return this.createSQLQuery(sql.toString()).addEntity(Records.class).list();
	}

	/* (non-Javadoc)
	 * @see com.hikvision.finance.fms.cms.dao.IProsaverecordDao#saveClientdeletelog(com.hikvision.finance.fms.model.Clientinput)
	 */
	@Override
	public void saveClientdeletelog(Clientinput clientinput) {
		StringBuffer sql = new StringBuffer("insert into clientdeletelog (nProId,`name`,creditId,productName,inputTime,inputUser,inputRegionId,deleteTime) VALUES");
		sql.append(" (?,?,?,?,?,?,?,now())");
		this.createSQLQuery(sql.toString()).setParameter(0, clientinput.getNproId()).setParameter(1, clientinput.getName())
		.setParameter(2, clientinput.getCreditCode()).setParameter(3, clientinput.getProductName())
		.setParameter(4, clientinput.getInputTime()).setParameter(5, clientinput.getProName())
		.setParameter(6, clientinput.getInputRegionId())
		.executeUpdate();
	}
	
	public List<Prosaverecord> getRecordsFileList(String clientIP, Integer beforeDays) {
    	String startDate = DateUtils.getStringDateTime(DateUtils.yyyy_MM_dd_HH_mm_ss, DateUtils.addDays(new Date(), -beforeDays));
    	String endDate = DateUtils.getStringDateTime(DateUtils.yyyy_MM_dd_HH_mm_ss, DateUtils.addDays(new Date(), -beforeDays * 2));
    	StringBuffer sql = new StringBuffer("SELECT * FROM prosaverecord WHERE startTime <=? and startTime >? and upload =? and devIp = ? ");//降序  
		return this.createSQLQuery(sql.toString()).addEntity(Prosaverecord.class).setParameter(0, startDate).setParameter(1, endDate).setParameter(2, ProSaveRecordEnum.VideoUploadStatus.UPLOADED.getNType()).setParameter(3, clientIP).list();
	}
	
}
