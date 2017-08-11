/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-4-17 下午5:02:51
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.webdevice.datacollect.dao.impl;

import java.util.List;

import org.hibernate.Criteria;
import org.hibernate.Query;

import com.hikvision.finance.extend.hibernate.dao.BaseDao;
import com.hikvision.finance.extend.hibernate.qo.SqlQuery;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.model.Records;
import com.hikvision.finance.fms.webdevice.datacollect.dao.IRecordsDao;
import com.hikvision.finance.fms.webdevice.datacollect.dto.BasicRecordInfo;
import com.hikvision.finance.fms.webdevice.datacollect.qo.RecordsQo;

/**
 * <p>录像数据dao</p>
 * @author xujiangfei 2016-4-17 下午5:02:51
 * @version V1.0 
 */
public class RecordsDaoImpl extends BaseDao<Records, RecordsQo> implements IRecordsDao{

	/**
	 * 序列化ID
	 */
	private static final long serialVersionUID = -119894751325155427L;

	/* 
	 * 设置异常结束录像状态
	 * (non-Javadoc)
     * @see com.hikvision.finance.fms.webdevice.datacollect.dao.IRecordsDao#saveErrorRecordStatus(java.lang.Integer)
     */
    @Override
    public void saveErrorRecordStatus(Integer userId) {
		String sql = "update Records set nrecordStatus = ? where nuserId = ? and nrecordStatus = ?";
		Query query = createQuery(sql);// hql语句
		query.setParameter(0, Constants.RecordStatus.END_ERROR_RECORD);
		query.setParameter(1, userId);
		query.setParameter(2, Constants.RecordStatus.START_RECORD);
		query.executeUpdate();
    }
	
	/*
	 * 获取视频文件列表
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.webdevice.datacollect.dao.IRecordsDao#getVideoList()
	 */
	@SuppressWarnings("unchecked")
	public List<BasicRecordInfo> getVideoList(Integer userId) {
		// 设置sqlQuery属性
		SqlQuery sqlQuery = new SqlQuery();
		StringBuilder sql = new StringBuilder("select id, nuserId, strRecordName, nrecordStatus, errorMsg from Records");
		if (userId != null) {
			sql.append(" where nuserId = ?");
			sqlQuery.addParams(userId);
		}
		sqlQuery.setSql(sql.toString());
		sqlQuery.setDtoClass(BasicRecordInfo.class);
		List<BasicRecordInfo> basicRecordInfoList = (List<BasicRecordInfo>)queryListBySql(sqlQuery);
		return basicRecordInfoList;
	}
	
	/*
	 * 删除每日录像，由定时器驱动执行
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.webdevice.datacollect.dao.IRecordsDao#deleteDailyRecord()
	 */
	public void deleteDailyRecord() {
		String sql2 = "delete from Records where nrecordStatus in (" + Constants.RecordStatus.UPLOADED + ","
		        + Constants.RecordStatus.DELETED + ")";
		Query query2 = createQuery(sql2);// hql语句
		query2.executeUpdate();
	}

	/* 删除录像信息(用户删除掉, 录像上传失败)，由定时器驱动执行
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.webdevice.datacollect.dao.IRecordsDao#deleteFailRecord()
	 */
	@Override
	public List<BasicRecordInfo> getFailRecord() {
		// 设置sqlQuery属性
		SqlQuery sqlQuery = new SqlQuery();
		//记录状态，0-开始记录，1-结束记录（未关联），2-已关联（未上传），3-上传中，4-已上传，8-异常结束，9-上传失败，100-已删除',
		StringBuilder sql = new StringBuilder("select id, nuserId, strRecordName, nrecordStatus, errorMsg from Records where nrecordStatus in (1, 8, 9)");
		sqlQuery.setSql(sql.toString());
		sqlQuery.setDtoClass(BasicRecordInfo.class);
		List<BasicRecordInfo> basicRecordInfoList = (List<BasicRecordInfo>)queryListBySql(sqlQuery);
		return basicRecordInfoList;
	}
	
	/* (non-Javadoc)
	 * @see com.hikvision.finance.extend.hibernate.dao.BaseDao#buildCriteria(org.hibernate.Criteria, com.hikvision.finance.extend.hibernate.qo.BaseQo)
	 */
	@Override
	protected Criteria buildCriteria(Criteria criteria, RecordsQo qo) {
		return criteria;
	}


}
