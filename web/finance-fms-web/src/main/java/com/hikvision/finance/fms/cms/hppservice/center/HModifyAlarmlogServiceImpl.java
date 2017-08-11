/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-3-21 下午2:53:29
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.cms.hppservice.center;

import org.apache.commons.lang.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.google.protobuf.RpcCallback;
import com.google.protobuf.RpcController;
import com.hikvision.finance.fms.cms.hpp.center.AlarmlogProto.Alarmlog;
import com.hikvision.finance.fms.cms.hpp.center.AlarmlogProto.AlarmlogInfo;
import com.hikvision.finance.fms.cms.hpp.center.AlarmlogProto.ModifyAlarmlogService;
import com.hikvision.finance.fms.cms.hpp.server.RspServerDataProto.RspServerData;
import com.hikvision.finance.fms.cms.hpp.type.CmdProto.CMD;

import com.hikvision.finance.fms.common.exception.CMSException;
import com.hikvision.finance.fms.common.util.Constants.RSP_RESULT;
import com.hikvision.finance.fms.modules.alarm.dao.IAlarmlogDao;
import com.ivms6.core.hpp.codec.HppConstants;
import com.hikvision.finance.core.util.DateUtils;

/**
 * <p></p>
 * @author xujiangfei 2016-3-21 下午2:53:29
 * @version V1.0 
 */
public class HModifyAlarmlogServiceImpl extends ModifyAlarmlogService{
	private static Logger logger = LoggerFactory.getLogger(HModifyAlarmlogServiceImpl.class);
	private IAlarmlogDao alarmlogDao;
	
	/* (non-Javadoc)--CMD_DT_ALARMLOG(获取报警状态)
	 * @see com.hikvision.finance.fms.cms.hpp.center.AlarmlogProto.ModifyAlarmlogService#qyeryAlarmlogID(com.google.protobuf.RpcController, com.hikvision.finance.fms.cms.hpp.center.AlarmlogProto.AlarmlogInfo, com.google.protobuf.RpcCallback)
	 */
	@Override
	public void qyeryAlarmlogID(RpcController controller, AlarmlogInfo request,RpcCallback<AlarmlogInfo> done) {
		logger.info("开始【获取报警状态】处理:cmd: 【" + CMD.CMD_DT_ALARMLOG_VALUE + "】; request:【" + request.toString() + "】");
		AlarmlogInfo.Builder rsp = AlarmlogInfo.newBuilder();
		Alarmlog.Builder alarmrsp = Alarmlog.newBuilder();
		
		try{
			//1.设置返回参数
			rsp.setCmd(CMD.CMD_DT_ALARMLOG_VALUE);
			//2.查找对应的报警信息
			com.hikvision.finance.fms.model.Alarmlog alarmModel = new com.hikvision.finance.fms.model.Alarmlog();
			alarmModel = alarmlogDao.getAlarmlogStatus(request);
			
			if(alarmModel != null){
				// 在数据库中的流水号
				if(alarmModel.getId()!=null){
					alarmrsp.setId(alarmModel.getId());
				}
				// 检测对象类型
				if(alarmModel.getCheckType()!=null){
					alarmrsp.setCheckType(alarmModel.getCheckType());
				}
				// 检测对象ID
				if(alarmModel.getCheckObjId()!=null){
					alarmrsp.setCheckObjId(alarmModel.getCheckObjId());
				}
				// 检查对象关联端口
				if(alarmModel.getCheckChan()!=null){
					alarmrsp.setCheckChan(alarmModel.getCheckChan());
				}
				// 关联对象
				if(alarmModel.getContactObjId()!=null){
					alarmrsp.setContactObjId(alarmModel.getContactObjId());
				}
				// 报警类型
				if(alarmModel.getAlarmType()!=null){
					alarmrsp.setAlarmType(alarmModel.getAlarmType());
				}
				// 报警最新时间
				if(alarmModel.getAlarmTime()!=null){
					alarmrsp.setAlarmTime(DateUtils.getStringDateTime("yyyy-MM-dd HH:mm:ss",alarmModel.getAlarmTime()));
				}
				// 报警开始时间
				if(alarmModel.getAlarmbeginTime()!=null){
					alarmrsp.setAlarmbeginTime(DateUtils.getStringDateTime("yyyy-MM-dd HH:mm:ss",alarmModel.getAlarmbeginTime()));// Coverity提示修改
				}
				// 报警来源IP
				if(StringUtils.isNotBlank(alarmModel.getStrHost())){
					alarmrsp.setStrHost(alarmModel.getStrHost());
				}
				// 报警详细内容
				if(StringUtils.isNotBlank(alarmModel.getComments())){
					alarmrsp.setComments(alarmModel.getComments());
				}
				// 报警区域
				if(alarmModel.getAlarmRegionId()!=null){
					alarmrsp.setAlarmRegionId(alarmModel.getAlarmRegionId());
				}
				 // 报警处理状态
				if(alarmModel.getStatus()!=null){
					alarmrsp.setStatus(alarmModel.getStatus());
				}
				 // 报警处理人姓名
				if(StringUtils.isNotBlank(alarmModel.getChecker())){
					alarmrsp.setChecker(alarmModel.getChecker());
				}
				// 报警处理人工号
				if(StringUtils.isNotBlank(alarmModel.getUserNo())){
					alarmrsp.setUserNo(alarmModel.getUserNo());
				}
				// 报警处理时间
				if(alarmModel.getDealTime()!=null){
					alarmrsp.setDealTime(DateUtils.getStringDateTime("yyyy-MM-dd HH:mm:ss",alarmModel.getDealTime()));
				}
				// 报警等级
				if(alarmModel.getAlarmlevel()!=null){
					alarmrsp.setAlarmlevel(alarmModel.getAlarmlevel());
				}
				// 报警GUID
				if(alarmModel.getStrGuid()!=null){
					alarmrsp.setStrGuid(alarmModel.getStrGuid());
				}
				
				rsp.setAlarmlog(alarmrsp);
			}else{
				throw new CMSException(CMSException.ALARM_NO_EXITS, "没有对应的报警信息！");
			}
			
			//3.结果处理
			done.run(rsp.build());
		}catch(CMSException e){
			done.run(rsp.build());
			logger.error("结束 获取报警状态 请求处理: 异常: 【" + e.getMessage() + "】",e);
			return;
		}
		
		logger.info("结束获取报警状态 请求处理:cmd: 【" + CMD.CMD_DT_ALARMLOG.getNumber() + "】; resultInfo:【"
		        + rsp.build().toString() + "】");
		
	}

	/* (non-Javadoc)--CMD_WDT_ALARMLOG_INFO(添加报警日志信息)
	 * @see com.hikvision.finance.fms.cms.hpp.center.AlarmlogProto.ModifyAlarmlogService#insertAlarmlogInfo(com.google.protobuf.RpcController, com.hikvision.finance.fms.cms.hpp.center.AlarmlogProto.AlarmlogInfo, com.google.protobuf.RpcCallback)
	 */
	@Override
	public void insertAlarmlogInfo(RpcController controller,AlarmlogInfo request, RpcCallback<RspServerData> done) {
		logger.info("开始【添加报警日志信息】处理:cmd: 【" + CMD.CMD_WDT_ALARMLOG_INFO_VALUE + "】; request:【" + request.toString() + "】");
		RspServerData.Builder rsp = RspServerData.newBuilder();
		
		try{
			//1.设置返回参数
			rsp.setCmd(CMD.CMD_WDT_ALARMLOG_INFO_VALUE);
			
			
			Alarmlog req = request.getAlarmlog();//proto中的报警
			if(req == null){
				throw new CMSException(CMSException.OBJECT_REQ_ISNULL, "添加报警日志信息的请求对象为空！！");
			}
			com.hikvision.finance.fms.model.Alarmlog alarmlog =new com.hikvision.finance.fms.model.Alarmlog();
			//添加报警信息
			// 检测对象类型
			if(req.getCheckType()>-1){
				alarmlog.setCheckType(req.getCheckType());
			}
			// 检测对象ID
			if(req.getCheckObjId()>-1){
				alarmlog.setCheckObjId(req.getCheckObjId());
			}
			// 检查对象关联端口
			if(req.getCheckChan()>-1){
				alarmlog.setCheckChan(req.getCheckChan());
			}
			// 关联对象
			if(req.getContactObjId()>-1){
				alarmlog.setContactObjId(req.getContactObjId());
			}
			// 报警类型
			if(req.getAlarmType()>-1){
				alarmlog.setAlarmType(req.getAlarmType());
			}
			// 报警最新时间
			if(StringUtils.isNotEmpty(req.getAlarmTime())){
				alarmlog.setAlarmTime(DateUtils.getDateTime("yyyy-MM-dd HH:mm:ss",req.getAlarmTime()));
			}
			
			if(StringUtils.isNotEmpty(req.getAlarmbeginTime())){
				alarmlog.setAlarmbeginTime(DateUtils.getDateTime("yyyy-MM-dd HH:mm:ss",req.getAlarmbeginTime()));
			}
			if(req.getAlarmRegionId()>-1){
				alarmlog.setAlarmRegionId(req.getAlarmRegionId());
			}
			if(req.getComments()!=null){
				alarmlog.setComments(req.getComments());
			}
			if(req.getStrHost()!=null){
				alarmlog.setStrHost(req.getStrHost());
			}
			if(req.getStatus()>-1){
				alarmlog.setStatus(req.getStatus());
			}
			
			alarmlogDao.save(alarmlog);
			
			//处理结果
			rsp.setResult(RSP_RESULT.SUCCESS);
			rsp.setErrorMsg("添加报警日志信息！");
			rsp.setDataId(HppConstants.RESULT_OK);
			done.run(rsp.build());
		}catch(CMSException e){
			rsp.setResult(e.getErrorCode());
			rsp.setDataId(HppConstants.RESULT_ERROR);
			rsp.setErrorMsg(e.getMessage());
			done.run(rsp.build());
			
			logger.error("结束 添加报警日志信息  请求: 异常: 【" + e.getMessage() + "】", e);
			return;
		}
		
		logger.info("结束  添加报警日志信息  请求:cmd: 【" + CMD.CMD_WDT_ALARMLOG_INFO.getNumber() + "】; resultInfo:【" + rsp.build().toString()
                + "】");
		
	}

	/* (non-Javadoc)--CMD_MDT_ALARMTIME(更新报警日志信息)
	 * @see com.hikvision.finance.fms.cms.hpp.center.AlarmlogProto.ModifyAlarmlogService#updateAlarmlogInfo(com.google.protobuf.RpcController, com.hikvision.finance.fms.cms.hpp.center.AlarmlogProto.AlarmlogInfo, com.google.protobuf.RpcCallback)
	 */
	@Override
	public void updateAlarmlogInfo(RpcController controller,AlarmlogInfo request, RpcCallback<RspServerData> done) {
		logger.info("开始【更新报警日志信息】处理:cmd: 【" + CMD.CMD_MDT_ALARMTIME_VALUE + "】; request:【" + request.toString() + "】");
		RspServerData.Builder rsp = RspServerData.newBuilder();
		
		try{
			//1.设置返回参数
			rsp.setCmd(CMD.CMD_MDT_ALARMTIME_VALUE);
			
			//2.查找对应的报警信息
			com.hikvision.finance.fms.model.Alarmlog alarmModel = null;//new com.hikvision.finance.fms.model.Alarmlog()
			alarmModel = alarmlogDao.get(request.getAlarmlog().getId());
			if(alarmModel != null){
				alarmModel.setAlarmTime(DateUtils.getDateTime("yyyy-MM-dd HH:mm:ss",request.getAlarmlog().getAlarmTime()));
				alarmlogDao.update(alarmModel);
				
				//处理结果
				rsp.setResult(RSP_RESULT.SUCCESS);
				rsp.setErrorMsg("更新报警信息成功！");
				rsp.setDataId(HppConstants.RESULT_OK);
				done.run(rsp.build());
			}else{
				throw new CMSException(CMSException.ALARM_NO_EXITS, "没有对应的报警信息！");
			}
		}catch(CMSException e){
			rsp.setResult(e.getErrorCode());
			rsp.setDataId(HppConstants.RESULT_ERROR);
			rsp.setErrorMsg(e.getMessage());
			done.run(rsp.build());
			
			logger.error("结束 更新报警信息  请求: 异常: 【" + e.getMessage() + "】", e);
			return;
		}
	
		logger.info("结束  更新报警信息  请求:cmd: 【" + CMD.CMD_MDT_ALARMTIME.getNumber() + "】; resultInfo:【" + rsp.build().toString()
                + "】");
		
	}

	//getter---  setter
	public IAlarmlogDao getAlarmlogDao() {
		return alarmlogDao;
	}

	public void setAlarmlogDao(IAlarmlogDao alarmlogDao) {
		this.alarmlogDao = alarmlogDao;
	}

}
