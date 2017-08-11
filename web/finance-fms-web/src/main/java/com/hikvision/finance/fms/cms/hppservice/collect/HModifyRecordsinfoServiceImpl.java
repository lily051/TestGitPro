/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-3-23 下午7:17:15
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.cms.hppservice.collect;

import java.net.InetSocketAddress;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.google.protobuf.RpcCallback;
import com.google.protobuf.RpcController;
import com.hikvision.finance.core.util.CollectionUtils;
import com.hikvision.finance.fms.cms.hpp.center.RecordsProto.ModifyRecordsinfoService;
import com.hikvision.finance.fms.cms.hpp.center.RecordsProto.Records;
import com.hikvision.finance.fms.cms.hpp.center.RecordsProto.Records.Builder;
import com.hikvision.finance.fms.cms.hpp.center.RecordsProto.RecordsList;
import com.hikvision.finance.fms.cms.hpp.center.RecordsProto.ReqRecords;
import com.hikvision.finance.fms.cms.hpp.server.RspServerDataProto.RspServerData;
import com.hikvision.finance.fms.cms.hpp.type.CmdProto.CMD;
import com.hikvision.finance.fms.cms.hppservice.model.RecordsVO;
import com.hikvision.finance.fms.common.exception.CMSException;
import com.hikvision.finance.fms.common.init.FmsPropertyPlaceholder;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.common.util.Constants.SysConfigType;
import com.hikvision.finance.fms.model.Cloudpoolmapping;
import com.hikvision.finance.fms.model.Cvrassociatemapping;
import com.hikvision.finance.fms.model.Device;
import com.hikvision.finance.fms.model.Prosaverecord;
import com.hikvision.finance.fms.model.Region;
import com.hikvision.finance.fms.model.Sysconfig;
import com.hikvision.finance.fms.modules.device.dao.ICloudpoolmappingDao;
import com.hikvision.finance.fms.modules.device.dao.ICvrassociatemappingDao;
import com.hikvision.finance.fms.modules.device.dao.IDeviceDao;
import com.hikvision.finance.fms.modules.device.qo.DeviceQo;
import com.hikvision.finance.fms.modules.record.dao.IProSaveRecordDao;
import com.hikvision.finance.fms.modules.region.dao.IRegionDao;
import com.hikvision.finance.fms.modules.sysconfig.dao.ISysconfigDao;
import com.hikvision.finance.fms.modules.user.dao.IUsersDao;
import com.hikvision.finance.fms.webdevice.datacollect.dao.IRecordsDao;
import com.hikvision.finance.fwork.exception.ExpectedException;
import com.ivms6.core.hpp.HppChannel;
import com.ivms6.core.hpp.codec.HppConstants;
import com.ivms6.core.hpp.service.RpcControllerImpl;
import com.ivms6.core.hpp.util.ContextUtil;
import com.ivms6.core.util.StringUtils;

/**
 * <p></p>
 * @author xujiangfei 2016-3-23 下午7:17:15
 * @version V1.0 
 */
public class HModifyRecordsinfoServiceImpl extends ModifyRecordsinfoService{

	private IRecordsDao recordsDao;
	private IRegionDao regionDao;
	private IDeviceDao deviceDao;
	private IUsersDao usersDao;
	private IProSaveRecordDao proSaveRecordDao;
	private ICvrassociatemappingDao cvrassociatemappingDao;
	private ICloudpoolmappingDao cloudpoolmappingDao;
	private ISysconfigDao sysconfigDao;
	private static Logger logger = LoggerFactory.getLogger(HModifyRecordsinfoServiceImpl.class);
	/* (non-Javadoc)--CMD_DT_RECORDS_LIST(根据录像状态获取录像信息)
	 * @see com.hikvision.finance.fms.cms.hpp.collect.RecordsProto.ModifyRecordsinfoService#qyeryRecordsInfo(com.google.protobuf.RpcController, com.hikvision.finance.fms.cms.hpp.collect.RecordsProto.ReqRecords, com.google.protobuf.RpcCallback)
	 */
	@Override
	public void qyeryRecordsInfo(RpcController controller, ReqRecords request,RpcCallback<RecordsList> done) {
		
		//1.设置返回参数
		RecordsList.Builder rspRecordsList = RecordsList.newBuilder();
		rspRecordsList.setCmd(CMD.CMD_DT_RECORDS_LIST_VALUE);
		
		//获得当前登录的上传服务的ip
    	RpcControllerImpl rpcControllerImpl = (RpcControllerImpl)controller;
    	long sessionId = rpcControllerImpl.getSessionId();
    	HppChannel hppChannel = ContextUtil.getHppChannel(HppConstants.TYPE_SERVER + sessionId);
    	if(null == hppChannel){// Coverity提示需要加上非空判断
			done.run(rspRecordsList.build());
			logger.error("结束 根据录像状态获取录像信息  请求处理异常: hppChannel为空");
			return;
    	}
    	String clientIP = ((InetSocketAddress)hppChannel.getSession().getRemoteAddress()).getAddress().getHostAddress();
    	String Modelusb = FmsPropertyPlaceholder.getProperty("choiceUsbIpc");//"usb"：usb模式    "ipc"：ipc模式
    	if (StringUtils.isEmpty(Modelusb)) {
    		throw new CMSException(CMSException.UNKNOWN_ERR, "未获取到通道方式");
    	}
    	
		try{
			List<com.hikvision.finance.fms.model.Records> listRecords = proSaveRecordDao.getRecordsbyRecordsStatus(clientIP,Modelusb);
			if(listRecords==null){
				throw new CMSException(CMSException.RECORDS_NO_EXITS, "没有对应的录像信息！");
			}
			int k = 0;
			for(int i=0;i<listRecords.size();i++){
				Records.Builder rspRecords = Records.newBuilder();
				setrspRecords(listRecords.get(i),rspRecords);
				boolean bFindKmsorCvr = handleUsborIpc(listRecords.get(i),rspRecords);
				if(bFindKmsorCvr){
					rspRecordsList.addRecord(k, rspRecords);
					k++; 
				}
			}
			//2.结果处理
			done.run(rspRecordsList.build());
		}catch(CMSException e){
			done.run(rspRecordsList.build());
			logger.error("结束 根据录像状态获取录像信息  请求处理: 异常: 【" + e.getMessage() + "】",e);
			return;
		}
		logger.info("结束   根据录像状态获取录像信息  请求:cmd: 【" + CMD.CMD_DT_RECORDS_LIST.getNumber() + "】; resultInfo:【" + rspRecordsList.build().toString()
                + "】");
	}

	/**
	 * @author xujiangfei 2016-6-3 下午6:30:56
	 * @param records
	 * @param rspRecords 
	 * @return
	 */
	private boolean handleUsborIpc(
			com.hikvision.finance.fms.model.Records records, Builder rspRecords) {
		boolean isSuccess = false;
		if(2==records.getNrecordFileType()){//2 代表本地录像--usb模式
			RecordsVO recordsNew =  setKMSInfo(records);
			if(recordsNew!=null ){
				rspRecords.setNRepairDevId(recordsNew.getNrepairDevId()); 
				if (recordsNew.getIsKMSDevice()) { //KMS
					// 文件类型，1-DVR录像，2-本地录像KMS  3-本地录像对象存储
					rspRecords.setNRecordFileType(2);
					rspRecords.setStrCollectIp(recordsNew.getStrCollectIp());
					rspRecords.setDvrPort(recordsNew.getDvrPort());
					rspRecords.setDvrUserName(recordsNew.getDvrUserName());
					rspRecords.setDvrPwd(recordsNew.getDvrPwd());
					//把这些数据回填到records表中
					updateRecords(recordsNew,records);
					//在表Prosaverecord中存KMS的id
					Prosaverecord prosaverecordOld = proSaveRecordDao.findUniqueBy("id", records.getnHistoryRecordId());
					prosaverecordOld.setCvr(recordsNew.getNrepairDevId()); // KMS的ID
					proSaveRecordDao.saveOrUpdate(prosaverecordOld);
					isSuccess = true;
				}else { //对象存储
					// 文件类型，1-DVR录像，2-本地录像KMS  3-本地录像对象存储(在web端只存在1和2, 但是服务那边要区分kms还是对象存储所以要加上3)
					rspRecords.setNRecordFileType(3);
					Device device = deviceDao.get(recordsNew.getNrepairDevId());
					rspRecords.setStrCloudIp(device.getStrIp());
					rspRecords.setNCloudPort(device.getNport());
					rspRecords.setStrAccessKey(device.getStrAccessKey());
					rspRecords.setStrSecretKey(device.getStrSecretKey());
					Cloudpoolmapping cloudpoolmapping = cloudpoolmappingDao.findUniqueBy("ndeviceId", device.getId());
					rspRecords.setStrBucket(cloudpoolmapping.getPoolName());
					//把这些数据回填到records表中
					//updateRecords(recordsNew,records);
					//在表Prosaverecord中存标准云-存储对象的id
					Prosaverecord prosaverecordOld = proSaveRecordDao.findUniqueBy("id", records.getnHistoryRecordId());
					prosaverecordOld.setCvr(recordsNew.getNrepairDevId()); // 标准云-存储对象的ID
					proSaveRecordDao.saveOrUpdate(prosaverecordOld);
					isSuccess = true;
				}
			}else{
				isSuccess = false;
			}
		} else{// 1代表DVR录像--ipc模式
			rspRecords.setNRecordFileType(1);
			// DVR的ID
			if(records.getNrepairDevId()!=null){
				rspRecords.setNRepairDevId(records.getNrepairDevId()); 
			}
			// DVR的IP
			if(records.getStrCollectIp()!=null){
				rspRecords.setStrCollectIp(records.getStrCollectIp());
			}
			// DVR的端口
			if(records.getDvrPort()!=null){
				rspRecords.setDvrPort(records.getDvrPort());
			}
			// DVR用户名
			if(records.getDvrUserName()!=null){
				rspRecords.setDvrUserName(records.getDvrUserName());
			}
			// DVR的密码
			if(records.getDvrPwd()!=null){
				rspRecords.setDvrPwd(records.getDvrPwd());
			}
			//根据dvr找到cvr的id然后存到prosaverecords.cvr字段，如果找不到对应的cvr的id（就是说dvr没有配置对应的cvr）抛出异常
			//后面的话要修改下，因为可能只抛出异常web断没有表现出来是什么问题
			String strChoiceUsbIpc = FmsPropertyPlaceholder.getProperty("choiceUsbIpc");
	    	if (StringUtils.isEmpty(strChoiceUsbIpc)) {
	    		throw new ExpectedException("", "未获取到通道方式");
	    	}
	    	if ("ipc".equals(strChoiceUsbIpc)) {
	    		String isNonCenterStorage = FmsPropertyPlaceholder.getProperty("nonCenterStorage");
	    		if (StringUtils.isEmpty(isNonCenterStorage)) {
		    		throw new ExpectedException("", "未获取到中心存储的状态");
		    	}
	    		if ("true".equals(isNonCenterStorage)) {//无中心存储
	    			isSuccess = true;
	    		} else {//有中心存储
	    			isSuccess = setPorsaverecordsCvr(records);
	    		}
	    	}
		}
		return isSuccess;
	}

	/**保存返回的录像数据 除了DCV的ip，端口，用户名，密码；或者kms的id
	 * @author xujiangfei 2016-6-3 下午6:11:26
	 * @param records
	 * @param rspRecords 
	 */
	private void setrspRecords(com.hikvision.finance.fms.model.Records records, Builder rspRecords) {
		// 在数据库中的流水号
		if(records.getId()!=null){
			rspRecords.setId(records.getId());
		}
		// 区域ID
		if(records.getNcollectRegionId()!=null){
			rspRecords.setNCollectRegionId(records.getNcollectRegionId());
		}
		// 记录所属用户
		if(records.getNuserId()!=null){
			rspRecords.setNUserID(records.getNuserId());
		}
		// 记录名称
		if(records.getStrRecordName()!=null){
			rspRecords.setStrRecordName(records.getStrRecordName());//// 记录名称
		}
		// 录像类型，1-正常录像，2-补录录像
		if(records.getNrecordType()!=null){
			rspRecords.setNRecordType(records.getNrecordType());// 记录类型（0：视频录像，1：电话录音）
		}
		// 记录状态，0-开始记录，1-结束记录（未关联），2-已关联（未上传），3-上传中，4-已上传，8-异常结束，9-上传失败，100-已删除
		if(records.getNrecordStatus()!=null){
			rspRecords.setNRecordStatus(records.getNrecordStatus());// 记录状态（0-开始记录，1-结束记录（未关联），2-已关联（未上传），3-上传中，4-已上传）
		}
		// 记录上传百分比
		if(records.getNupLoadPercent()!=null){
			rspRecords.setNUploadPercent(records.getNupLoadPercent());
		}
		// 录像开始时间
		if(records.getStartTime()!=null){
			rspRecords.setStartTime(new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(records.getStartTime()));
		}
		// 录像结束时间
		if(records.getEndTime()!=null){
			rspRecords.setEndTime(new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(records.getEndTime()));
		}
		// 采集DVR设备在CVR或者云存储上的唯一标识
		if(StringUtils.isNotBlank(records.getStreamNo())){
			rspRecords.setStreamNo(records.getStreamNo()); 
		}
		// DVR的通道号
		if(records.getNcollectChan()!=null){
			rspRecords.setNCollectChan(records.getNcollectChan());
		}
		// 本地录像路径
		if(records.getStrUrl()!=null){
			rspRecords.setStrUrl(records.getStrUrl());
		}
		// 截图存储路径
		if(records.getScreenshotSrc()!=null){
			rspRecords.setScreenshotSrc(records.getScreenshotSrc());
		}
		// 录像大小
		if(records.getNfileSize()!=null){
			rspRecords.setNFileSize(records.getNfileSize());
		}
		// 云存储的任务ID
		if(records.getnTaskId()!=null){
			rspRecords.setNTaskId(records.getnTaskId());
		}
		//录该条录像的设备所在电脑的ip
		if(records.getStrIp() !=null){
			rspRecords.setStrIp(records.getStrIp());
		}
	}

	/**
	 * @author xujiangfei 2016-5-31 下午7:56:39
	 * @param recordsNew
	 * @param records
	 */
	private void updateRecords(
			RecordsVO recordsNew,
			com.hikvision.finance.fms.model.Records recordOld) {
		recordOld.setNrepairDevId(recordsNew.getNrepairDevId());
		recordOld.setStrCollectIp(recordsNew.getStrCollectIp());
		recordOld.setDvrPort(recordsNew.getDvrPort());
		recordOld.setDvrUserName(recordsNew.getDvrUserName());
		recordOld.setDvrPwd(recordsNew.getDvrPwd());
		recordsDao.saveOrUpdate(recordOld);
	}

	/**
	 * @author xujiangfei 2016-5-31 下午7:40:20
	 * @param records
	 */
	private boolean setPorsaverecordsCvr(com.hikvision.finance.fms.model.Records records) {
		List<Cvrassociatemapping> cvrassociatemappingList =  cvrassociatemappingDao.findBy("ncollId", records.getNrepairDevId());
		boolean isflag = false;
		if(cvrassociatemappingList.size()>0){
			Cvrassociatemapping cvrassociatemapping = cvrassociatemappingList.get(0);	
			List<Prosaverecord> prosaverecordOlds = proSaveRecordDao.findBy("id", records.getnHistoryRecordId());
			if(prosaverecordOlds.size()>0){
				Prosaverecord prosaverecordOld=prosaverecordOlds.get(0);
				prosaverecordOld.setCvr(cvrassociatemapping.getNcvrId()); // CVR的ID
				proSaveRecordDao.saveOrUpdate(prosaverecordOld);
				isflag = true;
				if(prosaverecordOlds.size()>1){
				 for(int i=1;i<prosaverecordOlds.size();i++){
					if(prosaverecordOlds.get(i).getUpload()==0){
						proSaveRecordDao.delete(prosaverecordOlds.get(i));	
					}
				}
			  }
			}
		}else{
			logger.error("结束 根据录像状态获取录像信息  请求处理: 异常: 【没有配置对应的cvr,请给该dvr配置对应的cvr】");
			//错误信息存入records表，前台给出相应提示
    		com.hikvision.finance.fms.model.Records recordOld = recordsDao.get(records.getId());
    		recordOld.setErrorMsg("没有配置对应的cvr,请给该dvr配置对应的cvr！");
    		recordOld.setNrecordStatus(9);
    		recordsDao.saveOrUpdate(recordOld);
			
    		isflag = false;
		}
		return isflag;
	}

	/* (non-Javadoc)--CMD_MDT_RECORDS_STATUS(标记上传录像状态)
	 * @see com.hikvision.finance.fms.cms.hpp.collect.RecordsProto.ModifyRecordsinfoService#updateRecordsInfoBystatus(com.google.protobuf.RpcController, com.hikvision.finance.fms.cms.hpp.collect.RecordsProto.ReqRecords, com.google.protobuf.RpcCallback)
	 */
	@Override
	public void updateRecordsInfoBystatus(RpcController controller,ReqRecords request, RpcCallback<RspServerData> done) {
		logger.info("开始【标记上传录像状态 】处理:cmd: 【" + CMD.CMD_MDT_RECORDS_STATUS_VALUE + "】; request:【" + request.toString() + "】");
		
		RspServerData.Builder rsp = RspServerData.newBuilder();
		//1.设置返回参数
		
		try{
			//2.处理结果
			com.hikvision.finance.fms.model.Records record =  recordsDao.get(request.getId());
			proSaveRecordDao.updateRecordsInfoBystatus(request,record);
				
		}catch(CMSException e){
			rsp.setResult(0);
			rsp.setErrorMsg("failed to send message");
			done.run(rsp.build());
			logger.error("结束 标记上传录像状态   请求处理: 异常: 【" + e.getMessage() + "】",e);
			return;
		}
		logger.info("结束   标记上传录像状态   请求:cmd: 【" + CMD.CMD_MDT_RECORDS_STATUS.getNumber() + "】; resultInfo:【" + rsp.build().toString()
                + "】");
		
	}

	/* (non-Javadoc)--CMD_DT_RECORDS_INFO(根据录像ID获取录像信息)
	 * @see com.hikvision.finance.fms.cms.hpp.collect.RecordsProto.ModifyRecordsinfoService#queryRecordsInfoByid(com.google.protobuf.RpcController, com.hikvision.finance.fms.cms.hpp.collect.RecordsProto.ReqRecords, com.google.protobuf.RpcCallback)
	 */
	@Override
	public void queryRecordsInfoByid(RpcController controller,ReqRecords request, RpcCallback<RecordsList> done) {
		
	}

	/* (non-Javadoc)--CMD_MDT_UPLOAD_PERCENT(更新上传进度)
	 * @see com.hikvision.finance.fms.cms.hpp.collect.RecordsProto.ModifyRecordsinfoService#updateUpLoadPercent(com.google.protobuf.RpcController, com.hikvision.finance.fms.cms.hpp.collect.RecordsProto.ReqRecords, com.google.protobuf.RpcCallback)
	 */
	@Override
	public void updateUpLoadPercent(RpcController controller,ReqRecords request, RpcCallback<RspServerData> done) {
	
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
	
	private RecordsVO setKMSInfo(com.hikvision.finance.fms.model.Records records) {
		Device device = null;
		RecordsVO records_temp = null;
		com.hikvision.finance.fms.model.Records recordOld = recordsDao.get(records.getId());
		Prosaverecord dbProsaverecord = proSaveRecordDao.get(recordOld.getnHistoryRecordId());
		Region userRegion = regionDao.get(dbProsaverecord.getRegionId());//得到区域
		if(null == userRegion){
			logger.error("获取区域信息失败");
    		//错误信息存入records表
    		recordOld.setErrorMsg("获取区域信息失败，无法上传！");
    		recordOld.setNrecordStatus(9);
    		recordsDao.saveOrUpdate(recordOld);
    		return null;
		}
		String strInterAreaCode = userRegion.getStrPath();
		String[] interAreaCodeArray = strInterAreaCode.split("-");
		DeviceQo deviceQo = new DeviceQo();
		List<Integer> ntypes = new ArrayList<Integer>();
		ntypes.add(Constants.DeviceType.DEV_TYPE_CLOUD_STORAGE_OBJECT_STORAGE);
		ntypes.add(Constants.DeviceType.DEV_TYPE_KMS);
		deviceQo.setNtypes(ntypes);
		for (int i = interAreaCodeArray.length - 1; i >= 0; i--) {// 一般层级不会太多，hql查询暂时放到for循环里
			Region region = regionDao.findUniqueBy("strInterAreaCode", interAreaCodeArray[i]);
			deviceQo.setNregionId(region.getId());
			List<Device> deviceList = deviceDao.queryList(deviceQo);
			if (CollectionUtils.isNotEmpty(deviceList)) {
				// 遍历循环优先云存储
				for (Device dbDevice : deviceList) {
					if (Constants.DeviceType.DEV_TYPE_CLOUD_STORAGE_OBJECT_STORAGE == dbDevice.getNtype()) { //云存储-对象存储
						device = dbDevice;
						Cloudpoolmapping c = (Cloudpoolmapping) cloudpoolmappingDao.findUniqueBy("ndeviceId", device.getId());
			        	if(null == c){
			        		logger.error("结束 根据录像状态获取录像信息  请求处理: 异常: 【云存储-对象存储没有获取分卷信息，无法上传！】");
			        		//错误信息存入records表，前台给出相应提示
			        		recordOld.setErrorMsg("云存储-对象存储没有获取分卷信息，无法上传！");
			        		recordOld.setNrecordStatus(9);
			        		recordsDao.saveOrUpdate(recordOld);
			        		return null;
			        	}
			        	records_temp = new RecordsVO();
			    		records_temp.setNrepairDevId(device.getId());// 对象云的ID
			    		records_temp.setIsKMSDevice(false);
			    		return records_temp;
					}else if (Constants.DeviceType.DEV_TYPE_KMS == dbDevice.getNtype()) { //KMS
						device = dbDevice;
						continue;
					}
				}
				if (device == null) {//KMS
		    		logger.error("结束 根据录像状态获取录像信息  请求处理: 异常: 【没有配置对应的KMS或者云存储-对象存储,请给该录像文件配置对应的KMS或者云存储-对象存储！】");
		    		//错误信息存入records表，前台给出相应提示
		    		recordOld.setErrorMsg("没有配置对应的KMS或者云存储-对象存储！");
		    		recordOld.setNrecordStatus(9);
		    		recordsDao.saveOrUpdate(recordOld);
		    	}else{
		    		records_temp = new RecordsVO();
		    		records_temp.setNrepairDevId(device.getId());// KMS的ID
		        	records_temp.setStrCollectIp(device.getStrIp());// KMS的IP
		        	records_temp.setDvrPort(device.getNport());// KMS的端口
		        	records_temp.setDvrUserName(device.getStrUser());// KMS的用户名
		        	records_temp.setDvrPwd(device.getStrPassword());// KMS的密码
		        	records_temp.setIsKMSDevice(true);
		    	}
				break;
			}
		} 
    	return records_temp;
	}

	/* (non-Javadoc)
	 * @see com.hikvision.finance.fms.cms.hpp.center.RecordsProto.ModifyRecordsinfoService#queryRecordsFileInfo(com.google.protobuf.RpcController, com.hikvision.finance.fms.cms.hpp.center.RecordsProto.ReqRecords, com.google.protobuf.RpcCallback)
	 */
	@Override
	public void queryRecordsFileInfo(RpcController controller, ReqRecords request, RpcCallback<RecordsList> done) {
		// 1.设置返回参数
		RecordsList.Builder rspRecordsList = RecordsList.newBuilder();
		rspRecordsList.setCmd(CMD.CMD_DT_RECORDS_LIST_EX_VALUE);
		
		// 获得当前登录的上传服务的ip
		RpcControllerImpl rpcControllerImpl = (RpcControllerImpl)controller;
		long sessionId = rpcControllerImpl.getSessionId();
		HppChannel hppChannel = ContextUtil.getHppChannel(HppConstants.TYPE_SERVER + sessionId);
		if(null == hppChannel){// Coverity提示需要加上非空判断
			done.run(rspRecordsList.build());
			logger.error("结束 更新业务单信息  请求处理异常: hppChannel为空");
			return;
    	}
		String clientIP = ((InetSocketAddress)hppChannel.getSession().getRemoteAddress()).getAddress().getHostAddress();
		String Modelusb = FmsPropertyPlaceholder.getProperty("choiceUsbIpc");//"usb"：usb模式    "ipc"：ipc模式
    	if (StringUtils.isEmpty(Modelusb)) {
    		throw new ExpectedException("", "未获取到通道方式");
    	}
		
		try {
			if("usb".equals(Modelusb)){
				Sysconfig sysconfig = sysconfigDao.findUniqueBy("nkey", SysConfigType.BEFORE_DAYS);
				if (sysconfig == null) {
					done.run(rspRecordsList.build());
					logger.error("获取系统参数信息(USB删除n天前视频)异常");
					return;
				}
				List<com.hikvision.finance.fms.model.Prosaverecord> listRecords = proSaveRecordDao.getRecordsFileList(clientIP, Integer.valueOf(sysconfig.getStrValue()));
				if(CollectionUtils.isNotEmpty(listRecords)){
					for (com.hikvision.finance.fms.model.Prosaverecord prosaverecord : listRecords) {
						Records.Builder rspRecords = Records.newBuilder();
						if (StringUtils.isNotBlank(prosaverecord.getStrUrl())){
							rspRecords.setStrUrl(prosaverecord.getStrUrl());
						}
						if (StringUtils.isNotBlank(prosaverecord.getFileName())){
							rspRecords.setStrRecordName(prosaverecord.getFileName());
						}
						rspRecords.setNRecordStatus(Constants.RecordStatus.UPLOADED);
						rspRecordsList.addRecord(rspRecords);
					}
				}
			}
			// 2.结果处理
			done.run(rspRecordsList.build());
		} catch (CMSException e) {
			done.run(rspRecordsList.build());
			logger.error("结束请求待删除本地文件结束: 异常: 【" + e.getMessage() + "】", e);
			return;
		}
		logger.info("结束请求待删除本地文件结束请求:cmd: 【" + CMD.CMD_DT_RECORDS_LIST_EX_VALUE + "】; resultInfo:【"
				+ rspRecordsList.build().toString() + "】");
	}
	
	// setter ---getter
	public ISysconfigDao getSysconfigDao() {
		return sysconfigDao;
	}
	
	public void setSysconfigDao(ISysconfigDao sysconfigDao) {
		this.sysconfigDao = sysconfigDao;
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
	
	public IRegionDao getRegionDao() {
		return regionDao;
	}
	
	public void setRegionDao(IRegionDao regionDao) {
		this.regionDao = regionDao;
	}
	
	public IDeviceDao getDeviceDao() {
		return deviceDao;
	}
	
	public void setDeviceDao(IDeviceDao deviceDao) {
		this.deviceDao = deviceDao;
	}
	
	public IUsersDao getUsersDao() {
		return usersDao;
	}
	
	public void setUsersDao(IUsersDao usersDao) {
		this.usersDao = usersDao;
	}
	
	public ICvrassociatemappingDao getCvrassociatemappingDao() {
		return cvrassociatemappingDao;
	}
	
	public void setCvrassociatemappingDao(ICvrassociatemappingDao cvrassociatemappingDao) {
		this.cvrassociatemappingDao = cvrassociatemappingDao;
	}
	
	public ICloudpoolmappingDao getCloudpoolmappingDao() {
		return cloudpoolmappingDao;
	}
	
	public void setCloudpoolmappingDao(ICloudpoolmappingDao cloudpoolmappingDao) {
		this.cloudpoolmappingDao = cloudpoolmappingDao;
	}
}
