/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-4-17 下午5:06:17
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.webdevice.datacollect.service.impl;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import javax.servlet.http.HttpServletRequest;

import org.apache.commons.collections.CollectionUtils;
import org.apache.struts2.ServletActionContext;

import com.hikvision.cms.cache.manager.DataManager;
import com.hikvision.finance.core.util.DateUtils;
import com.hikvision.finance.core.util.SercurityUtil;
import com.hikvision.finance.core.util.StringUtils;
import com.hikvision.finance.extend.service.BaseServiceImpl;
import com.hikvision.finance.fms.common.cache.DataCache;
import com.hikvision.finance.fms.common.cache.HeartbeatCache;
import com.hikvision.finance.fms.common.init.FmsPropertyPlaceholder;
import com.hikvision.finance.fms.common.model.UserSession;
import com.hikvision.finance.fms.common.util.Base64Utils;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.common.util.Constants.OperLogType;
import com.hikvision.finance.fms.common.util.Constants.RecordStatus;
import com.hikvision.finance.fms.common.util.SessionUtil;
import com.hikvision.finance.fms.model.Device;
import com.hikvision.finance.fms.model.Devicechannel;
import com.hikvision.finance.fms.model.Prosaverecord;
import com.hikvision.finance.fms.model.Records;
import com.hikvision.finance.fms.model.Region;
import com.hikvision.finance.fms.model.Users;
import com.hikvision.finance.fms.modules.device.dao.IDeviceChannelDao;
import com.hikvision.finance.fms.modules.device.dao.IDeviceDao;
import com.hikvision.finance.fms.modules.record.dao.IProSaveRecordDao;
import com.hikvision.finance.fms.modules.region.dao.IRegionDao;
import com.hikvision.finance.fms.modules.user.dao.IUsersDao;
import com.hikvision.finance.fms.webdevice.datacollect.dao.IRecordsDao;
import com.hikvision.finance.fms.webdevice.datacollect.dto.BasicRecordInfo;
import com.hikvision.finance.fms.webdevice.datacollect.dto.DataCollectInfo;
import com.hikvision.finance.fms.webdevice.datacollect.dto.RecordsInfo;
import com.hikvision.finance.fms.webdevice.datacollect.qo.RecordsQo;
import com.hikvision.finance.fms.webdevice.datacollect.service.IDatacollectService;
import com.hikvision.finance.fms.webdevice.log.dao.ICollLogDao;
import com.hikvision.finance.fwork.exception.ExpectedException;
import com.hikvision.finance.fwork.util.AjaxData;
import com.hikvision.finance.fwork.util.ServletUtil;

/**
 * <p>录像数据service</p>
 * @author xujiangfei 2016-4-17 下午5:06:17
 * @version V1.0 
 */
public class DatacollectServiceImpl extends BaseServiceImpl<Records, RecordsQo, IRecordsDao> implements IDatacollectService {
	
	private IRecordsDao recordsDao;
	private ICollLogDao collLogDao;//记录日志
	private IDeviceDao deviceDao;
	private IDeviceChannelDao deviceChannelDao;
	private IRegionDao regionDao;
	private IProSaveRecordDao proSaveRecordDao;
	private DataManager dataManager; 
	private IUsersDao usersDao;
	
    
	/* 
	 * 获取通道List
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.webdevice.datacollect.service.IDatacollectService#getChannels()
	 */
	public List<Devicechannel> getChannels(String ip) {
		Integer userId = SessionUtil.getUserSession().getUserId();
		List<Devicechannel> devicechannelList = null;

		String strChoiceUsbIpc = FmsPropertyPlaceholder.getProperty("choiceUsbIpc");
		if (StringUtils.isEmpty(strChoiceUsbIpc)) {
			throw new ExpectedException("", "未获取到通道方式");
		}
		if ("ipc".equals(strChoiceUsbIpc)) {
			String isChannelIpBind = FmsPropertyPlaceholder.getProperty("channelIpBind");
			if (StringUtils.isEmpty(isChannelIpBind)) {
				throw new ExpectedException("", "未获取到通道ip绑定的状态");
			}
			if ("true".equals(isChannelIpBind)) {
				devicechannelList = deviceChannelDao.findBy("userIp", ip);
			} else {
				devicechannelList = deviceChannelDao.getChannels(userId);
			}			
		}
		return devicechannelList;
	}
	
	/* 
	 * 设置用户DVR信息和IPC通道信息
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.webdevice.datacollect.service.IDatacollectService#saveChannel(com.hikvision.finance.fms.webdevice.datacollect.dto.DataCollectInfo)
	 */
	public AjaxData saveChannel(DataCollectInfo dataCollectInfo) {
		Devicechannel devicechannel = deviceChannelDao.get(dataCollectInfo.getDeviceChannelId());
		if(HeartbeatCache.isChannelUsed(dataCollectInfo.getDeviceChannelId())){// 通道正在使用
			return new AjaxData(false).setFailureMsg("选择通道失败，其他用户正在使用该通道");
		}
		Device device = deviceDao.get(devicechannel.getNdeviceId());
		// 放到UserSession中，后续前台直接从session中取
		UserSession userSession = SessionUtil.getUserSession();
		userSession.setDeviceRegionId(device.getNregionId());
		userSession.setDeviceId(device.getId());
		userSession.setDeviceIp(device.getStrIp());
		userSession.setDevicePort(device.getNport());
		userSession.setDeviceUserName(device.getStrUser());
		String pass = SercurityUtil.decrypt(device.getStrPassword());
		userSession.setDevicePassword(Base64Utils.getBase64(pass));
		userSession.setStreamType(device.getNstreamType());
		userSession.setDeviceChannelId(devicechannel.getId());// 通道Id
		userSession.setDeviceChannelNo(devicechannel.getNchannelNo());// 通道号
		userSession.setDevicechannelName(devicechannel.getNchannelName());// 通道名
		// 传给前台
		RecordsInfo recordsInfo = new RecordsInfo();
		recordsInfo.setStrCollectIp(userSession.getDeviceIp());
		recordsInfo.setDvrPort(userSession.getDevicePort());
		recordsInfo.setDvrUserName(userSession.getDeviceUserName());
		recordsInfo.setDvrPwd(userSession.getDevicePassword());
		recordsInfo.setStreamType(userSession.getStreamType());
		recordsInfo.setNchannelNo(userSession.getDeviceChannelNo());
		recordsInfo.setNchannelName(userSession.getDevicechannelName());
		AjaxData ajaxData = new AjaxData();
		ajaxData.put("recordsInfo", recordsInfo);
		return ajaxData;
	}
	
	/*
	 * (non-Javadoc)保存正常录像信息
	 * @see com.hikvision.finance.fms.webdevice.datacollect.service.IDatacollectService#saveRecords()
	 */
	public void saveRecords(Records records,String ip) {
		Date dateTimeNow = new Date();
		String strDateTimeNow = DateUtils.getStringDateTime("yyyyMMddHHmmss", dateTimeNow);
		UserSession userSession = SessionUtil.getUserSession();
		if (records.getNrecordStatus() == Constants.RecordStatus.START_RECORD) {// 开始录像
			recordsDao.saveErrorRecordStatus(userSession.getUserId());// 对没有结束的录像设置为异常结束录像
			records.setNuserId(userSession.getUserId());// 用户Id
			records.setStrRecordName("录像" + strDateTimeNow);// 录像名称
			records.setNrecordType(Constants.Records.recordType.NORMAL_RECORD);// 录像类型
			records.setEndTime(null);
			
			String strChoiceUsbIpc = FmsPropertyPlaceholder.getProperty("choiceUsbIpc");
			if (StringUtils.isEmpty(strChoiceUsbIpc)) {
				throw new ExpectedException("", "未获取到通道方式");
			}
			
			if ("ipc".equals(strChoiceUsbIpc)) {// IPC-CVR
				records.setStreamNo(userSession.getDeviceIp() + "_" + userSession.getDevicePort() + "_"
				        + userSession.getDeviceChannelNo());// 通道流标识
				records.setNrecordFileType(Constants.Records.recordFileType.DVR_RECORD);
				records.setNcollectRegionId(userSession.getDeviceRegionId());// DVR/NVR的区域ID
				records.setNrepairDevId(userSession.getDeviceId());// DVR/NVR的ID
				records.setStrCollectIp(userSession.getDeviceIp());// DVR/NVR的IP
				records.setDvrPort(userSession.getDevicePort());// DVR/NVR的端口
				records.setDvrUserName(userSession.getDeviceUserName());// DVR/NVR的用户名
				String pass = Base64Utils.getFromBase64(userSession.getDevicePassword());
				records.setDvrPwd(SercurityUtil.encrypt(pass));// DVR/NVR的密码
				records.setNcollectChan(userSession.getDeviceChannelNo());// 通道号
			} else if ("usb".equals(strChoiceUsbIpc)) {// USB - KMS/对象存储
				records.setNcollectRegionId(userSession.getUserRegionId());// 用户的区域ID
				
				List<Device> deviceList = deviceDao.getUsbDeviceByNtype();// 获取所有usb模式下的设备list
				if (CollectionUtils.isNotEmpty(deviceList)) {
					records.setNrecordFileType(Constants.Records.recordFileType.LOCAL_RECORD);// 本地录像usb   不管是KMS还是对象存储，都存2，DC那边做相应的操作。
				} else {
					throw new ExpectedException("", "usb模式下，没有添加存储设备");
				}
				records.setStrIp(ip);// usb模式下存入本机的ip
			}
			recordsDao.save(records);
			// 记录操作日志
			String note = "录像名：" + records.getStrRecordName();
			collLogDao.writeOperLog(OperLogType.OperStartVideo, note);
		} else if (records.getNrecordStatus() == Constants.RecordStatus.END_RECORD) {// 结束录像
			String[] propertyName = {"nuserId","nrecordStatus"};
			Integer[] value = {userSession.getUserId(), Constants.RecordStatus.START_RECORD};
			List<Records> recordList = recordsDao.findBy(propertyName, value);
			if(recordList!=null && recordList.size()>0){
				Records newRecords = recordList.get(0);// 只会有一个满足条件
				newRecords.setEndTime(records.getEndTime());// 录像结束时间
				newRecords.setNrecordStatus(Constants.RecordStatus.END_RECORD);// 待关联
				newRecords.setStrUrl(records.getStrUrl());// USB模式结束时会修改url
				newRecords.setScreenshotSrc(newRecords.getScreenshotSrc()+","+records.getScreenshotSrc());
				// 记录操作日志
				String note = "录像名：" + newRecords.getStrRecordName();
				collLogDao.writeOperLog(OperLogType.OperEndVideo, note);
			}
			 
		} else {
			 throw new ExpectedException("", "录像失败");
		}
	}
	
	/* (non-Javadoc)存储补录录像信息
	 * @see com.hikvision.finance.fms.webdevice.datacollect.service.IDatacollectService#supplementVideo(com.hikvision.finance.fms.model.Records)
	 */
	@Override
	public void saveSupplementVideo(Records records) {
		Date dateTimeNow = new Date();
		String strDateTimeNow = DateUtils.getStringDateTime("yyMMddHHmmss", dateTimeNow);
		UserSession userSession = SessionUtil.getUserSession();
		records.setNuserId(userSession.getUserId());// 用户Id
		records.setStreamNo(userSession.getDeviceIp() + "_" + userSession.getDevicePort() + "_" + userSession.getDeviceChannelNo());// 通道流标识
		records.setStrRecordName("补录录像" + strDateTimeNow);// 录像名称
		records.setNrecordType(Constants.Records.recordType.REPAIR_RECORD);// 录像类型--补录
		records.setNrecordFileType(Constants.Records.recordFileType.DVR_RECORD);// 文件类型
		records.setStreamNo(userSession.getDeviceIp() + "_" + userSession.getDevicePort() + "_"
		        + userSession.getDeviceChannelNo());// 通道流标识
		records.setNrecordFileType(Constants.Records.recordFileType.DVR_RECORD);
		records.setNcollectRegionId(userSession.getDeviceRegionId());// DVR/NVR的区域ID
		records.setNrepairDevId(userSession.getDeviceId());// DVR/NVR的ID
		records.setStrCollectIp(userSession.getDeviceIp());// DVR/NVR的IP
		records.setDvrPort(userSession.getDevicePort());// DVR/NVR的端口
		records.setDvrUserName(userSession.getDeviceUserName());// DVR/NVR的用户名
		String pass = Base64Utils.getFromBase64(userSession.getDevicePassword());
		records.setDvrPwd(SercurityUtil.encrypt(pass));// DVR/NVR的密码
		records.setNcollectChan(userSession.getDeviceChannelNo());// 通道号
		records.setNrecordStatus(Constants.RecordStatus.END_RECORD);// 待关联
		recordsDao.save(records);
		// 记录操作日志
		String note = "录像名：" + records.getStrRecordName();
		collLogDao.writeOperLog(OperLogType.OperRepairVideo, note);
	}
	
	/* (non-Javadoc)上传录像(重新上传)
	 * @see com.hikvision.finance.fms.webdevice.datacollect.service.IDatacollectService#uploadRecord(java.lang.String)
	 */
	@Override
	public AjaxData uploadRecord(String recordids) {
		AjaxData ajaxData = new AjaxData(false);
		String[] strlist = recordids.split(",");
		for(String recordid :strlist){
			recordsDao.updateById(Integer.valueOf(recordid), "nrecordStatus", Constants.RecordStatus.CONTRACTED);
		}
		return ajaxData;
	}
	
	/* 
	 * 删除录像记录
	 * @see com.hikvision.finance.fms.webdevice.datacollect.service.IDatacollectService#deleteChooseRecordsByids(java.lang.String)
	 */
	@Override
	public AjaxData deleteRecords(List<String> idList) {
		String note = "删除" + idList.size() + "条录像";
		for (String id : idList) {
			Records record = recordsDao.get(Integer.valueOf(id));
			if (null == record) {
				return new AjaxData(false).setFailureMsg("录像不存在");
			}
			if (Integer.valueOf(RecordStatus.CONTRACTED).equals(record.getNrecordStatus())
			        || Integer.valueOf(RecordStatus.UPLOADING).equals(record.getNrecordStatus())
			        || Integer.valueOf(RecordStatus.UPLOADED).equals(record.getNrecordStatus())
			        || Integer.valueOf(RecordStatus.UPLOAD_FAIL).equals(record.getNrecordStatus())) {
				return new AjaxData(false).setFailureMsg("不能删除已关联的录像");
			}
			recordsDao.deleteById(id);
			note += "，" + record.getStrRecordName();
		}
		// 记录操作日志
		collLogDao.writeOperLog(OperLogType.OperDelVideo, note);
		return new AjaxData();
	}
	
	/* 
	 * 修改录像名称
	 * (non-Javadoc)
     * @see com.hikvision.finance.fms.webdevice.datacollect.service.IDatacollectService#editRecordsName(com.hikvision.finance.fms.model.Records)
     */
    @Override
    public void editRecordsName(Records records) {
    	Records newRecord = recordsDao.get(records.getId());
    	// 记录操作日志
    	String note = "原录像名：" + newRecord.getStrRecordName();
    	newRecord.setStrRecordName(records.getStrRecordName());
    	note += "，新录像名：" + records.getStrRecordName();
		collLogDao.writeOperLog(OperLogType.OperEditVideo, note);
    }
    
    /* 
     * 添加本地录像文件信息（USB录像文件）
     * (non-Javadoc)
     * @see com.hikvision.finance.fms.webdevice.datacollect.service.IDatacollectService#saveLocalRecord(com.hikvision.finance.fms.model.Records)
     */
    @Override
    public AjaxData saveLocalRecord(Records records) {
		Date dateTimeNow = new Date();
		String strDateTimeNow = DateUtils.getStringDateTime("yyyyMMddHHmmss", dateTimeNow);
		records.setStrRecordName("离线录像" + strDateTimeNow);
    	records.setNcollectRegionId(SessionUtil.getUserSession().getUserRegionId());// 用户的区域ID
    	records.setNuserId(SessionUtil.getUserSession().getUserId());// 业务员ID
		records.setNrecordType(Constants.Records.recordType.REPAIR_RECORD);// 暂时设为补录类型
    	records.setNrecordFileType(Constants.Records.recordFileType.LOCAL_RECORD);// 本地录像文件
    	records.setNrecordStatus(Constants.RecordStatus.END_RECORD);// 置为录像结束状态
		HttpServletRequest request = ServletActionContext.getRequest();
    	records.setStrIp(ServletUtil.getRemortIP(request));
		try {
//			String[] fullUrlArray = records.getStrUrl().split("_");
//			String fileUrl = fullUrlArray[fullUrlArray.length - 1];// 获取时间和文件后缀名
//			String[] fileNameArray = fileUrl.split("\\.");
//			String timeUrl = fileNameArray[0];// 获取开始时间和结束时间
//			String[] timeUrlArray = timeUrl.split("-");
			Date currentTime = new Date();
			records.setStartTime(currentTime);
			records.setEndTime(currentTime);
		} catch (Exception e) {
			return new AjaxData(false).setFailureMsg("视频文件名称格式错误");
		}
//    	setKMSInfo(records);// 设置KMS信息
    	recordsDao.save(records);
		// 记录操作日志
		String note = "录像名：" + records.getStrRecordName();
		collLogDao.writeOperLog(OperLogType.OperAddLocalVideo, note);
	    return new AjaxData();
    }
	
	/*
	 * 获取视频文件列表（records表）,直接读数据库, 或者从缓存中读
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.webdevice.datacollect.service.IDatacollectService#getVideoList()
	 */
	@SuppressWarnings("unchecked")
    @Override
	public List<BasicRecordInfo> getVideoList(int type) {
		List<BasicRecordInfo> basicRecordInfoList = new ArrayList<BasicRecordInfo>();
		int userId = Integer.valueOf(SessionUtil.getHikSession().getUserId());
		if (type == 1) {
			basicRecordInfoList = recordsDao.getVideoList(userId);
		} else if (type == 2) {
			List<BasicRecordInfo> allBasicRecordInfoList = (List<BasicRecordInfo>)DataCache.getCache("allBasicRecordInfoList",
			        10000);// 缓存失效时间是10秒，必须小于前端定时器间隔，否则数据可能有错
			for (BasicRecordInfo basicRecordInfo : allBasicRecordInfoList) {
				if (userId == basicRecordInfo.getNuserId()) {
					basicRecordInfoList.add(basicRecordInfo);
				}
			}
		}
		return basicRecordInfoList;
	}
    
	/* 
	 * 打开回放页面
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.webdevice.datacollect.service.IDatacollectService#getrecordsPlaybackbyid(java.lang.Integer)
	 */
	@Override
	public RecordsInfo getrecordsPlaybackbyid(Integer id, int type) {
		RecordsInfo recordsInfo = new RecordsInfo();
		if (type == 1) {// 视频文件列表中回放
			Records records = get(id);
			recordsInfo.setId(id);// 返回给前端，前端需要
			recordsInfo.setStrCollectIp(records.getStrCollectIp());// ip
			recordsInfo.setDvrPort(records.getDvrPort());// 端口
			recordsInfo.setDvrUserName(records.getDvrUserName());// 用户名
			String pass = SercurityUtil.decrypt(records.getDvrPwd());
			if (StringUtils.isNotBlank(pass)) {
				recordsInfo.setDvrPwd(Base64Utils.getBase64(pass));// 密码
			}
			recordsInfo.setNcollectChan(records.getNcollectChan());// 通道号
			recordsInfo.setStartTime(DateUtils.getStringDateTime(DateUtils.yyyy_MM_dd_HH_mm_ss, records.getStartTime()));
			if (records.getEndTime() == null) {// 异常录像
				recordsInfo.setEndTime(DateUtils.getStringDateTime(DateUtils.yyyy_MM_dd_HH_mm_ss, DateUtils.addHours(records.getStartTime(), 3)));// 默认加3小时
			}else{
				recordsInfo.setEndTime(DateUtils.getStringDateTime(DateUtils.yyyy_MM_dd_HH_mm_ss, records.getEndTime()));
			}
			recordsInfo.setStrUrl(records.getStrUrl());
			List<Devicechannel> devicechannels = deviceChannelDao.findBy(new String[]{ "ndeviceId","nchannelNo"}, new Object[]{records.getNrepairDevId(),records.getNcollectChan()});
			if(devicechannels!=null && devicechannels.size()>0){
				recordsInfo.setNchannelName(devicechannels.get(0).getNchannelName());
			}
		} else if (type == 2) {// 业务关联中回放
			Prosaverecord prosaverecord = proSaveRecordDao.get(id);
			
			String strChoiceUsbIpc = FmsPropertyPlaceholder.getProperty("choiceUsbIpc");
			if (StringUtils.isEmpty(strChoiceUsbIpc)) {
				throw new ExpectedException("", "未获取到通道方式");
			}
			try {
				if("ipc".equals(strChoiceUsbIpc)){// IPC-CVR
	            	Device device = deviceDao.get(prosaverecord.getDevId());// DVR信息
	            	if (device == null) {
	            		throw new ExpectedException("","找不到DVR/NVR设备信息");
	            	}
	            	recordsInfo.setStrCollectIp(device.getStrIp());// ip
	            	recordsInfo.setDvrPort(device.getNport());// 端口
	            	recordsInfo.setDvrUserName(device.getStrUser());// 用户名
	        		String pass = SercurityUtil.decrypt(device.getStrPassword());
	            	recordsInfo.setDvrPwd(Base64Utils.getBase64(pass));// 密码
	            	recordsInfo.setNcollectChan(prosaverecord.getDevChannel());// 通道号
	            	recordsInfo.setStartTime(DateUtils.getStringDateTime(DateUtils.yyyy_MM_dd_HH_mm_ss, prosaverecord.getStartTime()));// 录像开始时间
	            	recordsInfo.setEndTime(DateUtils.getStringDateTime(DateUtils.yyyy_MM_dd_HH_mm_ss, prosaverecord.getEndTime()));// 录像结束时间
	            	List<Devicechannel> devicechannels = deviceChannelDao.findBy(new String[]{ "ndeviceId","nchannelNo"}, new Object[]{prosaverecord.getDevId(),prosaverecord.getDevChannel()});
	            	if(devicechannels!=null && devicechannels.size()>0){
	            		recordsInfo.setNchannelName(devicechannels.get(0).getNchannelName());
	            	}
				} else if ("usb".equals(strChoiceUsbIpc)) {// USB-KMS
					recordsInfo.setStrUrl(prosaverecord.getStrUrl());// USB本地文件路径
				}
			 } catch (Exception e) {
	            	throw new ExpectedException("", "打开回放页面异常");
	         }
		}		
		return recordsInfo;
	}
	
	/* 
	 * 异常录像添加结束时间
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.webdevice.datacollect.service.IDatacollectService#saveEndTime(com.hikvision.finance.fms.model.Records)
	 */
	public void saveEndTime(Records records){
		Records newRecord = recordsDao.get(records.getId());
		newRecord.setEndTime(records.getEndTime());// 设置录像结束时间
		newRecord.setNrecordStatus(Constants.RecordStatus.END_RECORD);// 录像状态修改为待关联
		recordsDao.update(newRecord);
		// 记录操作日志
		String strEndDate = DateUtils.getStringDateTime(newRecord.getEndTime());
		String note = "录像名：" + newRecord.getStrRecordName() + "，录像结束时间：" + strEndDate;
		collLogDao.writeOperLog(OperLogType.OperHandleAbnormalVideo, note);
	}
	
	/*
	 * 删除每日录像，由定时器驱动执行
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.webdevice.datacollect.service.IDatacollectService#deleteDailyRecord()
	 */
	public void deleteDailyRecord(){
		recordsDao.deleteDailyRecord();
	}
	
	/**
	 * 设置用户相关的KMS信息
	 * @author jinchenwu 2016年5月7日 下午12:39:03
	 * @return
	 */
	@SuppressWarnings("unused")
    private void setKMSInfo(Records records) {
		Device device = null;
		Integer userRegionId = SessionUtil.getUserSession().getUserRegionId();
		Region userRegion = regionDao.get(userRegionId);
		String strInterAreaCode = userRegion.getStrPath();
		String[] interAreaCodeArray = strInterAreaCode.split("-");
		for (int i = interAreaCodeArray.length; i > 0; i--) {// 一般层级不会太多，hql查询暂时放到for循环里
			Region region = regionDao.findUniqueBy("strInterAreaCode", interAreaCodeArray[i-1]);
			List<Device> deviceList = deviceDao.findBy(new String[] {"nregionId", "ntype"}, new Integer[] {region.getId(),
			        Constants.DeviceType.DEV_TYPE_KMS});
			if (deviceList != null && deviceList.size() > 0) {
				device = deviceList.get(0);// 临时做法，取第一个
				break;
			}
		}
    	if (device == null) {
    		throw new ExpectedException("", "没有相关的KMS信息");
    	}
    	records.setNrepairDevId(device.getId());// KMS的ID
    	records.setStrCollectIp(device.getStrIp());// KMS的IP
    	records.setDvrPort(device.getNport());// KMS的端口
    	records.setDvrUserName(device.getStrUser());// KMS的用户名
    	records.setDvrPwd(device.getStrPassword());// KMS的密码
	}
	
	/* 删除录像信息(用户删除掉, 录像上传失败)，由定时器驱动执行
	 * @see com.hikvision.finance.fms.webdevice.datacollect.service.IDatacollectService#deleteFailRecord()
	 */
	@Override
	public void deleteFailRecord() {
		List<BasicRecordInfo> recordInfos = recordsDao.getFailRecord();
		if (CollectionUtils.isNotEmpty(recordInfos)) {
			for (BasicRecordInfo recordInfo : recordInfos) {
				Users user = usersDao.get(recordInfo.getNuserId());
				if (user == null) {
					recordsDao.deleteById(recordInfo.getId());
				}
			}
		}
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.extend.service.IBaseService#deleteByIds(java.io.Serializable[])
	 */
	@Override
	public void deleteByIds(Serializable[] paramArrayOfSerializable) {
		
	}
	
	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.service.BaseServiceImpl#getDao()
     */
    @Override
    protected IRecordsDao getDao() {
	    return recordsDao;
    }
	
	// **************************setter--getter************************************//
	
	public IDeviceDao getDeviceDao() {
		return deviceDao;
	}
	
	public void setDeviceDao(IDeviceDao deviceDao) {
		this.deviceDao = deviceDao;
	}
	
	public IDeviceChannelDao getDeviceChannelDao() {
		return deviceChannelDao;
	}
	
	public void setDeviceChannelDao(IDeviceChannelDao deviceChannelDao) {
		this.deviceChannelDao = deviceChannelDao;
	}
	
	public IRegionDao getRegionDao() {
		return regionDao;
	}
	
	public void setRegionDao(IRegionDao regionDao) {
		this.regionDao = regionDao;
	}
	
	public DataManager getDataManager() {
		return dataManager;
	}
	
	public void setDataManager(DataManager dataManager) {
		this.dataManager = dataManager;
	}
	
	public ICollLogDao getCollLogDao() {
		return collLogDao;
	}
	
	public void setCollLogDao(ICollLogDao collLogDao) {
		this.collLogDao = collLogDao;
	}
	
	public IProSaveRecordDao getProSaveRecordDao() {
		return proSaveRecordDao;
	}
	
	public void setProSaveRecordDao(IProSaveRecordDao proSaveRecordDao) {
		this.proSaveRecordDao = proSaveRecordDao;
	}
	
	public IRecordsDao getRecordsDao() {
		return recordsDao;
	}
	
	public void setRecordsDao(IRecordsDao recordsDao) {
		this.recordsDao = recordsDao;
	}

	public IUsersDao getUsersDao() {
		return usersDao;
	}

	public void setUsersDao(IUsersDao usersDao) {
		this.usersDao = usersDao;
	}
	
}
