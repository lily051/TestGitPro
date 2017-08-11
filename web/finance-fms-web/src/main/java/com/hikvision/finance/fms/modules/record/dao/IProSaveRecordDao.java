/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月15日 下午1:54:58
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.record.dao;

import java.util.List;

import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.hibernate.dao.IBaseDao;
import com.hikvision.finance.fms.cms.hpp.center.RecordsProto.ReqRecords;
import com.hikvision.finance.fms.model.Clientinput;
import com.hikvision.finance.fms.model.Prosaverecord;
import com.hikvision.finance.fms.model.Records;
import com.hikvision.finance.fms.model.Sysconfig;
import com.hikvision.finance.fms.modules.analysis.dto.AnalysisInfo;
import com.hikvision.finance.fms.modules.record.dto.RecordInfo;
import com.hikvision.finance.fms.modules.record.qo.ProSaveRecordQo;


/**
 * <p></p>
 * @author jinxindong 2016年4月15日 下午1:54:58
 * @version V1.0 
 */
public interface IProSaveRecordDao extends IBaseDao<Prosaverecord,ProSaveRecordQo>{
	
	/**
	 * 获取理财录像分页数据
	 * @author jinchenwu 2016年4月6日 下午4:24:50
	 * @param recordInfo
	 * @param start
	 * @param limit
	 * @return
	 */
	public Page getDataPage(RecordInfo recordInfo, Integer start, Integer limit);
	
    /**
     * 获取理财录像List数据
     * @author jinchenwu 2016年4月7日 下午1:57:20
     * @param recordInfo
     * @return
     */
    public List<RecordInfo> getDataList(RecordInfo recordInfo);
    
    /**
     * 设置录像合规检查状态
     * @author jinchenwu 2016年4月7日 下午5:32:09
     * @param recordInfo
     */
	public void setRecordCheck(RecordInfo recordInfo);
	
	/**
	 * 设置录像延迟删除天数
	 * @author jinchenwu 2016年4月11日 下午4:37:23
	 * @param recordInfo
	 */
	public void setDelayDays(RecordInfo recordInfo);
	
	/**
	 * 查询业务单的录像数量
	 * @author zhoujiajun 2016年10月24日 上午11:22:14
	 * @param recordCount
	 * @return
	 */
	public List<RecordInfo> getVideoCount(RecordInfo recordInfo);
	
	
	/**
	 * 查询业务单的详细信息
	 * @author zhoujiajun 2016年10月27日 下午3:49:08
	 * @param recordInfo
	 * @return
	 */
    public List<RecordInfo> getClientRecordsList(RecordInfo recordInfo);
	
	/**
	 * 查询录像
	 * @author jinxindong 2016年4月15日 下午2:06:36
	 * @return
	 */
	public List<Prosaverecord> getRecordByCheck(AnalysisInfo analysisInfo);
	
	/**
	 * 查询签约单的录像
	 * @author jinxindong 2016年7月12日 上午10:42:38
	 * @param analysisInfo
	 * @param regionIds
	 * @param recordCheck
	 * @return
	 */
	public Integer getRecordByCheck(AnalysisInfo analysisInfo,String regionIds,int recordCheck);
	
	/**
	 * 首页查询录像
	 * @author jinxindong 2016年4月15日 下午2:06:36
	 * @return
	 */
	public List<Prosaverecord> getIndexRecordByCheck(AnalysisInfo analysisInfo);
	
	/**
	 * 首页查询本周合规录像
	 * @author jinxindong 2016年7月12日 上午11:42:03
	 * @param analysisInfo
	 * @return
	 */
	public Integer getIndexRecordByCheck(AnalysisInfo analysisInfo,String regionIds,int recordCheck);
	
	/********************以下是dc的dao移过来的***************************/
	
	/**
	 * 获取录像守卫时间
	 */
	public List<Sysconfig> getProsaverecoreTime();
	
	
	/**
	 * 获取过期录像信息
	 */
	public List<Clientinput> getOvertimeRecordInfo();


	/**
	 * @author xujiangfei 2016-3-23 上午10:36:24
	 * @param clientId
	 * @return
	 */
	public List<com.hikvision.finance.fms.model.Prosaverecord> getProsaverecordInfoByclientid(int clientId);

	/**根据录像状态获取录像信息
	 * @author xujiangfei 2016-3-28 下午2:16:08
	 * @param clientIP 
	 * @param modelusb 
	 * @param nRecordStatus
	 * @return
	 */
	public List<Records> getRecordsbyRecordsStatus(String clientIP, String modelusb);

	/**标记上传录像状态
	 * @author xujiangfei 2016-5-10 下午3:12:30
	 * @param request--上传服务传过来的records对象
	 * @param record-- 根据id
	 */
	public void updateRecordsInfoBystatus(ReqRecords request,
			com.hikvision.finance.fms.model.Records record);


	/**获取clientinput
	 * @author xujiangfei 2016-5-13 下午7:27:19
	 * @param clientId
	 * @return
	 */
	public Clientinput getClientinput(int clientId);


	/**--删除clientinput (更新表中nisPay=1)
	 * @author xujiangfei 2016-5-13 下午7:32:00
	 * @param id
	 */
	public void delClientinput(Integer id);


	/**记录删除日志（DC接口删除录像后会往clientdeletelog记录删除日志）
	 * @author xujiangfei 2016-5-26 下午8:31:49
	 * @param clientinput
	 */
	public void saveClientdeletelog(Clientinput clientinput);
	
	/**
	 * 获取过去7天的录像数据
	 * @author zhoujiajun 2016年11月2日 下午2:08:15
	 * @param clientIP
	 * @return
	 */
	public List<Prosaverecord> getRecordsFileList(String clientIP, Integer beforeDays);
	
}
