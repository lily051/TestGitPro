/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-3-17 上午11:05:12
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.cms.hppservice.center;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.google.protobuf.RpcCallback;
import com.google.protobuf.RpcController;
import com.hikvision.finance.core.util.StringUtils;
import com.hikvision.finance.fms.cms.hpp.center.ProsaverecordProto.ModifyProsaverecordService;
import com.hikvision.finance.fms.cms.hpp.center.ProsaverecordProto.Prosaverecord;
import com.hikvision.finance.fms.cms.hpp.center.ProsaverecordProto.ProsaverecordList;
import com.hikvision.finance.fms.cms.hpp.center.ProsaverecordProto.ReqProsaverecord;
import com.hikvision.finance.fms.cms.hpp.server.RspServerDataProto.RspServerData;
import com.hikvision.finance.fms.cms.hpp.type.CmdProto.CMD;
import com.hikvision.finance.fms.common.exception.CMSException;
import com.hikvision.finance.fms.common.init.FmsPropertyPlaceholder;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.model.Clientinput;
import com.hikvision.finance.fms.model.Cloudpoolmapping;
import com.hikvision.finance.fms.modules.device.dao.ICloudpoolmappingDao;
import com.hikvision.finance.fms.modules.record.dao.IProSaveRecordDao;
import com.hikvision.finance.fms.webdevice.datacollect.dao.IRecordsDao;
import com.hikvision.finance.fwork.exception.ExpectedException;

/**
 * <p></p>
 * @author xujiangfei 2016-3-17 上午11:05:12
 * @version V1.0 
 */
public class HProsaverecordServiceImpl extends ModifyProsaverecordService{
	private IProSaveRecordDao proSaveRecordDao;
	private static Logger logger = LoggerFactory.getLogger(HProsaverecordServiceImpl.class);
	private IRecordsDao recordsDao;
	private ICloudpoolmappingDao cloudpoolmappingDao;

	/* (non-Javadoc)//CMD_WDT_RECORD_INFO--保存录像信息
	 * @see com.hikvision.finance.fms.cms.hpp.center.ProsaverecordProto.ModifyProsaverecordService#saveProsaverecordInfo(com.google.protobuf.RpcController, com.hikvision.finance.fms.cms.hpp.center.ProsaverecordProto.Prosaverecord, com.google.protobuf.RpcCallback)
	 */
	@Override
	public void saveProsaverecordInfo(RpcController controller,Prosaverecord request, RpcCallback<RspServerData> done) {
	}

	/* (non-Javadoc)//CMD_DT_RECORD_LIST--获取录像信息
	 * @see com.hikvision.finance.fms.cms.hpp.center.ProsaverecordProto.ModifyProsaverecordService#queryProsaverecordInfo(com.google.protobuf.RpcController, com.hikvision.finance.fms.cms.hpp.center.ProsaverecordProto.ReqProsaverecord, com.google.protobuf.RpcCallback)
	 */
	@Override
	public void queryProsaverecordInfo(RpcController controller,ReqProsaverecord request, RpcCallback<ProsaverecordList> done) {
		logger.info("开始【获取录像信息】处理:cmd: 【" + CMD.CMD_DT_RECORD_LIST_VALUE + "】; request:【" + request.toString() + "】");
		
		ProsaverecordList.Builder rspProsaverecordList = ProsaverecordList.newBuilder();
		rspProsaverecordList.setCmd(CMD.CMD_DT_RECORD_LIST_VALUE);
		
		try{
			List<com.hikvision.finance.fms.model.Prosaverecord>  prosaverecord = proSaveRecordDao.getProsaverecordInfoByclientid(request.getClientId());
			if(prosaverecord==null){
				throw new CMSException(CMSException.RECORDS_NO_EXITS, "没有对应的录像信息！");
			}
			
			String srtChoiceUsbIpc = FmsPropertyPlaceholder.getProperty("choiceUsbIpc");
			if (StringUtils.isEmpty(srtChoiceUsbIpc)) {
				throw new ExpectedException("", "未获取到通道方式");
			}
			for(int i = 0;i<prosaverecord.size();i++){
				Prosaverecord.Builder rspProsaverecord = Prosaverecord.newBuilder();
				
				if(prosaverecord.get(i).getId()!=null){
					rspProsaverecord.setId(prosaverecord.get(i).getId());
				}
				if(prosaverecord.get(i).getClientId()!=null){
					rspProsaverecord.setClientId(prosaverecord.get(i).getClientId());
				}
				if(prosaverecord.get(i).getCvr()!=null){
					rspProsaverecord.setCvr(prosaverecord.get(i).getCvr());
				}
				if(prosaverecord.get(i).getStartTime()!=null){
					rspProsaverecord.setStartTime(new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(prosaverecord.get(i).getStartTime()));
				}
				if(prosaverecord.get(i).getEndTime()!=null){
					rspProsaverecord.setEndTime(new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(prosaverecord.get(i).getEndTime()));
				}
				if(StringUtils.isNotBlank(prosaverecord.get(i).getStreamNo())){
					rspProsaverecord.setStreamNo(prosaverecord.get(i).getStreamNo());
				}
				if(prosaverecord.get(i).getUpload()!=null){
					rspProsaverecord.setUpload(prosaverecord.get(i).getUpload());
				}
				if(prosaverecord.get(i).getDevId() != null){
					rspProsaverecord.setDevId(prosaverecord.get(i).getDevId());
				}
				if(prosaverecord.get(i).getDevChannel() != null){
					rspProsaverecord.setDevChannel(prosaverecord.get(i).getDevChannel());
				}
				
				if ("usb".equals(srtChoiceUsbIpc)) {
					Cloudpoolmapping cloudpoolmapping = cloudpoolmappingDao.findUniqueBy("ndeviceId", prosaverecord.get(i).getCvr());
					String strBucket = cloudpoolmapping.getPoolName();
					if (StringUtils.isNotEmpty(strBucket)) {
						rspProsaverecord.setStrBucket(strBucket);
					}
				}
				rspProsaverecordList.addProsaverecord(i, rspProsaverecord);
			}
			
			//处理结果
			done.run(rspProsaverecordList.build());
		}catch(CMSException e){
			done.run(rspProsaverecordList.build());
			logger.error("结束  获取录像信息  请求处理: 异常: 【" + e.getMessage() + "】",e);
			return;
		}
		logger.info("结束  获取录像信息  请求:cmd: 【" + CMD.CMD_DT_RECORD_LIST.getNumber() + "】; resultInfo:【" + rspProsaverecordList.build().toString()
                + "】");
	}

	/* (non-Javadoc)//CMD_DDT_RECORD_INFO--删除录像信息
	 * @see com.hikvision.finance.fms.cms.hpp.center.ProsaverecordProto.ModifyProsaverecordService#deleteProsaverecordByid(com.google.protobuf.RpcController, com.hikvision.finance.fms.cms.hpp.center.ProsaverecordProto.ReqProsaverecord, com.google.protobuf.RpcCallback)
	 */
	@Override
	public void deleteProsaverecordByid(RpcController controller,ReqProsaverecord request, RpcCallback<RspServerData> done) {
		
		logger.info("开始【删除录像信息】处理:cmd: 【" + CMD.CMD_DDT_RECORD_INFO_VALUE + "】; request:【" + request.toString() + "】");
		RspServerData.Builder rsp = RspServerData.newBuilder();
		try{
			//1.设置返回参数
			rsp.setCmd(CMD.CMD_DDT_RECORD_INFO_VALUE);
			
			//2.根据prosaverecord表的id来删除对应的数据
			if (request.getId() != 0) {
				recordsDao.deleteBy("nHistoryRecordId", request.getId());// 删除records表
				proSaveRecordDao.deleteById(request.getId());// 删除prosaverecords
				rsp.setResult(Constants.ResultStatus.RESULT_SUCCESS);
			} else {
				rsp.setErrorMsg("ID为"+request.getId()+"的录像没有被删除");
				rsp.setResult(Constants.ResultStatus.RESULT_FAILE);
			}
			done.run(rsp.build());
		}catch(CMSException e){
			rsp.setErrorMsg("ID为"+request.getId()+"的录像没有被删除");
			rsp.setResult(Constants.ResultStatus.RESULT_FAILE);
			logger.error("结束  获取录像信息  请求处理: 异常: 【" + e.getMessage() + "】",e);
			done.run(rsp.build());
			return;
		}
		
		logger.info("结束  删除录像信息  请求:cmd: 【" + CMD.CMD_DDT_RECORD_INFO.getNumber() + "】; resultInfo:【" + rsp.build().toString()
                + "】");
		
	}
	/* (non-Javadoc)--CMD_MDT_OVERTIMERECORD--标记过期录像记录
	 * @see com.hikvision.finance.fms.cms.hpp.center.ProsaverecordProto.ModifyProsaverecordService#markClientinputByid(com.google.protobuf.RpcController, com.hikvision.finance.fms.cms.hpp.center.ProsaverecordProto.ReqProsaverecord, com.google.protobuf.RpcCallback)
	 */
	@Override
	public void markClientinputByid(RpcController controller,
			ReqProsaverecord request, RpcCallback<RspServerData> done) {
		logger.info("开始【标记过期录像记录】处理:cmd: 【" + CMD.CMD_MDT_OVERTIMERECORD + "】; request:【" + request.toString() + "】");
		RspServerData.Builder rsp = RspServerData.newBuilder();
		try{
			//1.设置返回参数
			rsp.setCmd(CMD.CMD_DDT_RECORD_INFO_VALUE);
			
			//2.更新clientinput表中的nisPay = 1
			if(request.getClientId()!=0){
				Clientinput clientinput = proSaveRecordDao.getClientinput(request.getClientId());
				if(clientinput!=null){
					proSaveRecordDao.delClientinput(clientinput.getId());//更新clientinput表中的nisPay = 1
					proSaveRecordDao.saveClientdeletelog(clientinput);//记录删除日志
					rsp.setResult(Constants.ResultStatus.RESULT_SUCCESS);
				}else{
					rsp.setResult(Constants.ResultStatus.RESULT_FAILE);
				}
			}
			done.run(rsp.build());
		}catch(CMSException e){
			rsp.setErrorMsg("ID为"+request.getId()+"的录像没有被删除");
			rsp.setResult(Constants.ResultStatus.RESULT_FAILE);
			logger.error("结束  获取录像信息  请求处理: 异常: 【" + e.getMessage() + "】",e);
			done.run(rsp.build());
			return;
		}
		
	}
	

	/**
	 * 
	 * @author xujiangfei 2016-3-21 下午2:20:04
	 * @param strDate--字符串的日期
	 * @param strDateformat--要转变成的日期格式
	 * @return
	 */
	public Date changeDage(String strDate,String strDateformat){
		Date date = new Date();
		try {
			date = new SimpleDateFormat(strDateformat).parse(strDate);
		} catch (ParseException e) {
			e.printStackTrace();
		}
		
		return date;
	}

	//getter   setter
	public ICloudpoolmappingDao getCloudpoolmappingDao() {
		return cloudpoolmappingDao;
	}
	
	public void setCloudpoolmappingDao(ICloudpoolmappingDao cloudpoolmappingDao) {
		this.cloudpoolmappingDao = cloudpoolmappingDao;
	}
	
	public IRecordsDao getRecordsDao() {
		return recordsDao;
	}
	
	public void setRecordsDao(IRecordsDao recordsDao) {
		this.recordsDao = recordsDao;
	}
	
	public IProSaveRecordDao getProSaveRecordDao() {
		return proSaveRecordDao;
	}
	
	public void setProSaveRecordDao(IProSaveRecordDao proSaveRecordDao) {
		this.proSaveRecordDao = proSaveRecordDao;
	}
}
