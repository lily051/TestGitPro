/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月6日 下午2:25:56
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.record.service.impl;

import java.io.Serializable;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Random;

import org.apache.log4j.Logger;

import com.hikvision.cms.cache.manager.DataManager;
import com.hikvision.finance.core.util.DateUtils;
import com.hikvision.finance.core.util.JsonUtils;
import com.hikvision.finance.core.util.ListUtil;
import com.hikvision.finance.core.util.SercurityUtil;
import com.hikvision.finance.core.util.StringUtils;
import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.service.BaseServiceImpl;
import com.hikvision.finance.fms.common.enums.ClientInputEnum;
import com.hikvision.finance.fms.common.enums.LogModuledEnum;
import com.hikvision.finance.fms.common.enums.ProSaveRecordEnum;
import com.hikvision.finance.fms.common.init.FmsPropertyPlaceholder;
import com.hikvision.finance.fms.common.util.Base64Utils;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.common.util.Constants.OperStatus;
import com.hikvision.finance.fms.common.util.ExcelUtil;
import com.hikvision.finance.fms.common.util.SessionUtil;
import com.hikvision.finance.fms.common.util.http.HttpPostUtil;
import com.hikvision.finance.fms.model.Clientinput;
import com.hikvision.finance.fms.model.Cloudpoolmapping;
import com.hikvision.finance.fms.model.Device;
import com.hikvision.finance.fms.model.Devicechannel;
import com.hikvision.finance.fms.model.Prosaverecord;
import com.hikvision.finance.fms.model.Recordlog;
import com.hikvision.finance.fms.modules.device.dao.ICloudpoolmappingDao;
import com.hikvision.finance.fms.modules.device.dao.IDeviceChannelDao;
import com.hikvision.finance.fms.modules.device.dao.IDeviceDao;
import com.hikvision.finance.fms.modules.device.dto.KmsInfo;
import com.hikvision.finance.fms.modules.log.dao.IConfigLogDao;
import com.hikvision.finance.fms.modules.log.dao.IRecordLogDao;
import com.hikvision.finance.fms.modules.record.dao.IProSaveRecordDao;
import com.hikvision.finance.fms.modules.record.dto.PlaybackInfo;
import com.hikvision.finance.fms.modules.record.dto.RecordDto;
import com.hikvision.finance.fms.modules.record.dto.RecordInfo;
import com.hikvision.finance.fms.modules.record.qo.ProSaveRecordQo;
import com.hikvision.finance.fms.modules.record.service.IRecordService;
import com.hikvision.finance.fms.modules.region.service.IRegionService;
import com.hikvision.finance.fms.modules.sale.dao.IClientInputDao;
import com.hikvision.finance.fms.modules.sale.dto.SaleInfo;
import com.hikvision.finance.fms.modules.sale.qo.SaleQo;
import com.hikvision.finance.fms.modules.sale.service.ISaleService;
import com.hikvision.finance.fwork.exception.ExpectedException;
import com.hikvision.finance.fwork.util.GlobalMessageUtil;

/**
 * <p>理财录像模块</p>
 * @author jinchenwu 2016年4月6日 下午2:25:56
 * @version V1.0
 */
public class RecordServiceImpl extends BaseServiceImpl<Prosaverecord, ProSaveRecordQo, IProSaveRecordDao> implements IRecordService {
	
	private static final Logger logger = Logger.getLogger(RecordServiceImpl.class);
	private IProSaveRecordDao proSaveRecordDao;
	private IRegionService regionService;
	private IDeviceDao deviceDao;
	private IDeviceChannelDao deviceChannelDao;
	private IClientInputDao clientInputDao;
	private ISaleService saleService;
	private IRecordLogDao recordLogDao;
	private IConfigLogDao configLogDao;
	private ICloudpoolmappingDao cloudpoolmappingDao;
	private DataManager dataManager;
	private static String rtspPath = "/kms/services/rest/dataInfoService/getRtspPath";//获取KMS上录像准确的信息
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.record.service.IRecordService#getDataPage(com.hikvision.finance.fms.modules.record.dto.RecordInfo,
	 * java.lang.Integer, java.lang.Integer)
	 */
	@SuppressWarnings("unchecked")
    public Page getDataPage(RecordInfo recordInfo, Integer start, Integer limit) {
		String strRegionIds = regionService.getLikeStrPathIds(recordInfo.getRegionId(), recordInfo.getIsFlag());
		recordInfo.setStrRegionIds(strRegionIds);// 设置过滤的区域节点
		Page page = proSaveRecordDao.getDataPage(recordInfo, start, limit);
		List<RecordInfo> recordInfoList = (List<RecordInfo>)page.getResult();
		changeRecordInfoList(recordInfoList);
		return page;
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.record.service.IRecordService#saveLogAndGetData(com.hikvision.finance.fms.modules.record.dto.RecordInfo)
	 */
	public RecordDto saveLogAndGetData(RecordInfo recordInfo) {
		RecordDto recordDto = new RecordDto();
		// 获取录像信息
		List<RecordInfo> recordInfoList = proSaveRecordDao.getDataList(recordInfo);
		RecordInfo newRecordInfo = recordInfoList.get(0);
		
		String strChoiceUsbIpc = FmsPropertyPlaceholder.getProperty("choiceUsbIpc");
		if (StringUtils.isEmpty(strChoiceUsbIpc)) {
			throw new ExpectedException("", "未获取到通道方式");
		}
		Device device = null;
		if ("ipc".equals(strChoiceUsbIpc)) {
			String isNonCenterStorage = FmsPropertyPlaceholder.getProperty("nonCenterStorage");
			if (StringUtils.isEmpty(isNonCenterStorage)) {
				throw new ExpectedException("", "未获取到中心存储的状态");
			}
			if ("true".equals(isNonCenterStorage)) {//无中心
				device = deviceDao.get(newRecordInfo.getCdevId());//cdevId是采集设备ID
			} else {//有中心
				// 获取回放需要的设备信息
				if(newRecordInfo.getDevId() == null){
					throw new ExpectedException("", "设备信息不存在，无法回放视频");
				}
				device = deviceDao.get(newRecordInfo.getDevId());//devId是存储设备ID
			}
		} else {
			// 获取回放需要的设备信息
			if(newRecordInfo.getDevId() == null){
				throw new ExpectedException("", "设备信息不存在，无法回放视频");
			}
			device = deviceDao.get(newRecordInfo.getDevId());
		}
		
		if(device==null){
			throw new ExpectedException("", "设备信息不存在，无法回放视频");
		}
		//由于新的KMS不会传给WEB录像的开始时间和结束时间，所以在合规检查时要web去KMS上获取对应记录的开始和结束时间，再把时间更新到数据库
		if(Constants.DeviceType.DEV_TYPE_KMS == device.getNtype() && (null == newRecordInfo.getIsUpdateKmsTime() || 0 == newRecordInfo.getIsUpdateKmsTime())){//1、判断只有KMS类型的录像才去获取2、只有没有获取过KMS时间的记录才去获取
			String strIp = device.getStrIp();
			Integer nport = device.getNport();
			String kmsRecordId = newRecordInfo.getStreamNo();
			//通过录像在KMS上的ID查询到录像的真实信息
			String infoString = HttpPostUtil.sendGet(" http://"+strIp+":"+nport+rtspPath, "id="+kmsRecordId);
			KmsInfo tokenInfo = JsonUtils.json2Object(infoString, KmsInfo.class);
			if(tokenInfo.getRet()==Constants.CONS_0){//等于0的时候是操作成功，否则给出获取KMS信息失败
				if(tokenInfo.getStrBeginTime().equals(tokenInfo.getStrEndTime())){//如果开始时间和结束时间相等，则KMS还没有解析好时间，给出获取失败的提示
					throw new ExpectedException("", "获取kms时间失败["+tokenInfo.getMsg()+"]");
				}else{//否则将获取的真实时间传给控件，和更新到数据库，同时将是否更新KMS时间标记字段（IsUpdateKmsTime）变成1
					String strBeginTime = tokenInfo.getStrBeginTime();
					String strEndTime = tokenInfo.getStrEndTime();
					SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
					Date strBeginDate;
					Date strEndDate;
					try {
						strBeginDate = sdf.parse(strBeginTime);
						strEndDate = sdf.parse(strEndTime);
					} catch (ParseException e) {
						e.printStackTrace();
						throw new ExpectedException("", "获取的KMS时间转换失败");
					}
					newRecordInfo.setStartTime(strBeginDate);
					newRecordInfo.setEndTime(strEndDate);
					//将时间保存到数据库对应的记录上
					Prosaverecord p = proSaveRecordDao.get(recordInfo.getVideoId());
					p.setStartTime(strBeginDate);
					p.setEndTime(strEndDate);
					p.setIsUpdateKmsTime(1);
					proSaveRecordDao.update(p);
				}
				
			}else{
				throw new ExpectedException("", "获取kms信息失败["+tokenInfo.getMsg()+"]");
			}
		}
		
		if ("usb".equals(strChoiceUsbIpc) && Constants.DeviceType.DEV_TYPE_CLOUD_STORAGE_OBJECT_STORAGE == device.getNtype()) {//USB模式下,云存储-对象存储需设置strBucket
			Cloudpoolmapping cloudpoolmapping = cloudpoolmappingDao.findUniqueBy("ndeviceId", device.getId());
			if (null == cloudpoolmapping) {
				throw new ExpectedException("", "未获取到分卷信息");
			}
			newRecordInfo.setStrBucket(cloudpoolmapping.getPoolName());//设置对象存储回放时所需的strBucket			
		}
		
		Prosaverecord prosaverecord = proSaveRecordDao.get(recordInfo.getVideoId());
		Clientinput clientinput = clientInputDao.get(prosaverecord.getClientId());
		newRecordInfo.setLockClient(clientinput.getLockClient());
		
		List<Prosaverecord> prosaverecordList = proSaveRecordDao.findBy("clientId", prosaverecord.getClientId());
		if (prosaverecord.getStateType() == Constants.RecordType.RECORD_COMPLAINNT) {//投诉录像 不能审批
			newRecordInfo.setIsCanApproval(0);
		} else {//签约录像  默认能审批;业务单包含上传失败的录像，则不能审批。
			newRecordInfo.setIsCanApproval(1);
			for (Prosaverecord psr : prosaverecordList) {
				if (psr.getUpload() == Constants.VideoUploadStatus.NOT_UPLOAD) {
					newRecordInfo.setIsCanApproval(0);//不能审批
					break;
				}
			}
		}
		
		recordDto.setRecordInfo(newRecordInfo);
		
		PlaybackInfo playbackInfo = new PlaybackInfo();
		switch(device.getNtype()){
			case Constants.DeviceType.DEV_TYPE_CVR:
				playbackInfo.setType(1);
				break;
			case Constants.DeviceType.DEV_TYPE_CLOUD_STORAGE:
				playbackInfo.setType(2);
				break;
			case Constants.DeviceType.DEV_TYPE_KMS:
				playbackInfo.setType(3);
				break;
			case Constants.DeviceType.DEV_TYPE_CLOUD_STORAGE_OBJECT_STORAGE:
				playbackInfo.setType(4);
				break;
			case Constants.DeviceType.DEV_TYPE_DVR:
			case Constants.DeviceType.DEV_TYPE_NVR:
				playbackInfo.setType(0); // DVR/NVR
				break;
		}
		playbackInfo.setIp(device.getStrIp());
		playbackInfo.setPort(device.getNport());
		
		if ("usb".equals(strChoiceUsbIpc) && StringUtils.isNotEmpty(device.getStrAccessKey()) && StringUtils.isNotEmpty(device.getStrSecretKey())) {//USB模式下，有访问key以及加密key的时候
			String strAccessKey = SercurityUtil.decrypt(device.getStrAccessKey());
			playbackInfo.setStrAccessKey(Base64Utils.getBase64(strAccessKey));
			String strSecretKey = SercurityUtil.decrypt(device.getStrSecretKey());
			playbackInfo.setStrSecretKey(Base64Utils.getBase64(strSecretKey));
		}
		playbackInfo.setUser(device.getStrUser());
		//通道名字
		if(newRecordInfo.getCdevId()==null){
			playbackInfo.setChannelName("其他");
		}else{
			List<Devicechannel> devicechannels = deviceChannelDao.findBy(new String[]{"ndeviceId","nchannelNo"}, new Object[]{newRecordInfo.getCdevId(),newRecordInfo.getChannel()});
			if(devicechannels!=null && devicechannels.size()>0){
				playbackInfo.setChannelName(devicechannels.get(0).getNchannelName());
			}else{
				playbackInfo.setChannelName("其他");
			}	
		}
		String pass = SercurityUtil.decrypt(device.getStrPassword());
		playbackInfo.setPassword(Base64Utils.getBase64(pass));
		recordDto.setPlaybackInfo(playbackInfo);
		
		// 记录回放日志
		Recordlog recordLog = new Recordlog();
		recordLog.setRegionId(newRecordInfo.getInputRegionId().toString());// 交易区域Id
    	recordLog.setNproId(newRecordInfo.getNproId());// 业务单号
    	recordLog.setRecordName(newRecordInfo.getFileName());// 录像名称
    	recordLog.setRecordType(newRecordInfo.getStateType());// 录像类型
    	recordLog.setOperDate(new Date());// 回放时间
    	recordLog.setCheckUserName(SessionUtil.getUserSession().getUserName());// 操作用户姓名
		recordLogDao.save(recordLog);
		
		return recordDto;
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.record.service.IRecordService#getRandomData(com.hikvision.finance.fms.modules.record.dto.RecordInfo)
	 */
	@SuppressWarnings("unchecked")
    public RecordInfo getRandomData(RecordInfo recordInfo) {
		recordInfo.setUpload(Constants.VideoUploadStatus.UPLOADED);// 随机检查只查询已上传的录像
		String strRegionIds = regionService.getLikeStrPathIds(recordInfo.getRegionId(), recordInfo.getIsFlag());
		recordInfo.setStrRegionIds(strRegionIds);// 设置过滤的区域节点
		List<RecordInfo> recordInfoList = null;
		if(recordInfo.getPageLenth()>20){
			Random random = new Random();
			int randomNum = random.nextInt(recordInfo.getPageLenth());
			recordInfoList = (List<RecordInfo>)proSaveRecordDao.getDataPage(recordInfo, randomNum, 20).getResult();
		}else{
			recordInfoList = proSaveRecordDao.getDataList(recordInfo);	
		}
		RecordInfo randomRecordInfo = new RecordInfo();
		if (recordInfoList.size() == 0) {
			return null;
		}
		Random random = new Random();
		int randomNum = random.nextInt(recordInfoList.size());
		randomRecordInfo = recordInfoList.get(randomNum);
		return randomRecordInfo;
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.record.service.IRecordService#getClientinput(com.hikvision.finance.fms.modules.record.dto.RecordInfo)
	 */
	public SaleInfo getOrderInfo(SaleInfo saleInfo){
		return saleService.getBizOrderInfo(saleInfo);
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.record.service.IRecordService#saveRecordCheck(com.hikvision.finance.fms.modules.record.dto.RecordInfo)
	 */
	public void saveRecordCheck(RecordInfo recordInfo) {
		Prosaverecord prosaverecord = proSaveRecordDao.get(recordInfo.getVideoId());
		Clientinput clientinput = clientInputDao.get(prosaverecord.getClientId());
		if (recordInfo.getLockClient() != null && recordInfo.getLockClient() >= 0) {
			clientinput.setLockClient(recordInfo.getLockClient()); 
		}
		proSaveRecordDao.setRecordCheck(recordInfo);
		String fileName = proSaveRecordDao.get(recordInfo.getVideoId()).getFileName();
		configLogDao.setLogContent(
		        OperStatus.UPDATE,
		        LogModuledEnum.RECORD.getCode(),
		        GlobalMessageUtil.getMessage("log.record.setStatus",
		                new String[] {fileName, ProSaveRecordEnum.RecordCheckStatus.getStrByN(recordInfo.getRecordCheck())}));
	}

	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.record.service.IRecordService#saveDelayDays(com.hikvision.finance.fms.modules.record.dto.RecordInfo)
	 */
	public void saveDelayDays(RecordInfo recordInfo) {
		proSaveRecordDao.setDelayDays(recordInfo);
		// 写配置日志
		SaleQo saleQo = new SaleQo();
		saleQo.setIds(recordInfo.getBusiIds());
		List<Clientinput> clientinputList = clientInputDao.queryList(saleQo);
		List<String> proIdList = new ArrayList<String>();
		for (Clientinput clientinput : clientinputList) {
			proIdList.add(clientinput.getNproId());
		}
		if(recordInfo.getBusiIds().size()>5){
			configLogDao.setLogContent(OperStatus.UPDATE,LogModuledEnum.RECORD.getCode(),GlobalMessageUtil.getMessage("log.record.set.batch",new String[] {recordInfo.getDelayDays().toString()}));
		}else{
			configLogDao.setLogContent(OperStatus.UPDATE,LogModuledEnum.RECORD.getCode(),GlobalMessageUtil.getMessage("log.record.set", new String[] {ListUtil.listToStr(proIdList),recordInfo.getDelayDays().toString()}));
		}
	}
	
    /* 
     * (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.record.service.IRecordService#saveLogAndexportExcel(com.hikvision.finance.fms.modules.record.dto.RecordInfo, java.lang.String)
     */
    public boolean saveLogAndexportExcel(RecordInfo recordInfo, String path) {
		String strRegionIds = regionService.getLikeStrPathIds(recordInfo.getRegionId(),recordInfo.getIsFlag());
		recordInfo.setStrRegionIds(strRegionIds);// 设置过滤的区域节点
    	String[] titleName = null;
    	String[] attrName = null;
    	if(recordInfo.getStateType() == Constants.RecordType.RECORD_SIGN){// 签约录像
    		attrName = new String[] {"fileName", "nproId", "inputRegionName", "startTimeString", "proUserNo", "proName", "name", "creditCode", "strBankNo", "productCode", "productName", "strVideoEndDate", "recordLength", "strUpload", "strRecordCheck", "strStatus"};
    		titleName = new String[] {"录像名称", "业务单号", "交易网点", "录像时间", "业务员工号", "业务员姓名", "客户姓名", "客户证件号", "银行卡号", "产品编码", "产品名称", "录像到期时间", "录像时长", "是否上传", "是否合规", "是否存在纠纷"};
    	}else{// 投诉录像
    		attrName = new String[] {"fileName", "nproId", "inputRegionName", "startTimeString", "proUserNo", "proName", "name", "creditCode", "strBankNo", "productCode", "productName", "strVideoEndDate", "recordLength", "strUpload", "strRecordCheck", "checkRemark"};
    		titleName = new String[] {"录像名称", "业务单号", "交易网点", "录像时间", "业务员工号", "业务员姓名", "客户姓名", "客户证件号", "银行卡号", "产品编码", "产品名称", "录像到期时间", "录像时长", "是否上传", "录像状态", "处理意见"};
    	}
		// 1，依次准备标题、标题映射属性、Sheet以及对应内容
		List<String[]> titleNameList = new ArrayList<String[]>();
		List<String[]> attrNameList = new ArrayList<String[]>();
		titleNameList.add(titleName);
		attrNameList.add(attrName);
		LinkedHashMap<String, List<?>> map = new LinkedHashMap<String, List<?>>();
		List<RecordInfo> recordInfoList1 = proSaveRecordDao.getDataList(recordInfo);
		if(recordInfoList1.size() > Constants.EXCEL_MAX_NUM){
			throw new ExpectedException("", "导出数据超过"+Constants.EXCEL_MAX_NUM+"条，无法导出");
		}
		List<RecordInfo> recordInfoList = new ArrayList<RecordInfo>();
		for(RecordInfo info: recordInfoList1){
			String date = DateUtils.getStringDateTime(DateUtils.yyyy_MM_dd_HH_mm_ss, info.getStartTime());
			info.setStartTimeString(date);
			recordInfoList.add(info);
		}
		changeRecordInfoList(recordInfoList);
		map.put("Sheet1", recordInfoList);
		// 2，创建Excel的数据对象并导出表格
		ExcelUtil.ExcelExportData setInfo = new ExcelUtil.ExcelExportData();
		setInfo.setColumnNames(titleNameList);
		setInfo.setFieldNames(attrNameList);
		setInfo.setDataMap(map);
		boolean exportResult = false;
		try {
			exportResult = ExcelUtil.export2File(setInfo, path);
			configLogDao.setLogContent(OperStatus.EXPORT, LogModuledEnum.RECORD.getCode(), GlobalMessageUtil.getMessage("log.record.export"));
		} catch (Exception e) {
			logger.error("导出异常", e);
		}
		return exportResult;
		
    }
    
    /**
     * 对从数据库中读出的录像信息和业务信息进行加工
     * @author jinchenwu 2016年4月21日 下午3:25:35
     * @param recordInfoList
     */
    private void changeRecordInfoList(List<RecordInfo> recordInfoList){
		for (RecordInfo newRecordInfo : recordInfoList) {
			newRecordInfo.setStrUpload(ProSaveRecordEnum.VideoUploadStatus.getStrByN(newRecordInfo.getUpload()));
			newRecordInfo.setStrStatus(ClientInputEnum.BusiComplainStatus.getStrByN(newRecordInfo.getStatus()));
			newRecordInfo.setStrRecordCheck(ProSaveRecordEnum.RecordCheckStatus.getStrByN(newRecordInfo.getRecordCheck()));
			setRecordLegnth(newRecordInfo);
		}
    }
    
    /**
     * 设置录像时长
     * @author jinchenwu 2016年4月21日 下午3:25:25
     * @param recordInfo
     */
    private void setRecordLegnth(RecordInfo recordInfo){
		int nRecordLength = (int)((recordInfo.getEndTime().getTime() - recordInfo.getStartTime().getTime()) / 1000);
		int minutes = nRecordLength/60;
		int seconds = nRecordLength%60;
		StringBuilder recordLegnth = new StringBuilder();
		if (minutes > 0) {
			recordLegnth.append(minutes + "分");
		}
		if (seconds >= 10) {
			recordLegnth.append(seconds % 60 + "秒");
		} else {
			recordLegnth.append("0" + seconds + "秒");
		}
		recordInfo.setRecordLength(recordLegnth.toString());
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
    protected IProSaveRecordDao getDao() {
	    return null;
    }
	
	
	// =======================================getter/setter================================================================

	public IRegionService getRegionService() {
		return regionService;
	}
	
	public void setRegionService(IRegionService regionService) {
		this.regionService = regionService;
	}
	
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
	
	public ISaleService getSaleService() {
		return saleService;
	}
	
	public void setSaleService(ISaleService saleService) {
		this.saleService = saleService;
	}
	
	public IRecordLogDao getRecordLogDao() {
		return recordLogDao;
	}
	
	public void setRecordLogDao(IRecordLogDao recordLogDao) {
		this.recordLogDao = recordLogDao;
	}
	
	public IConfigLogDao getConfigLogDao() {
		return configLogDao;
	}
	
	public void setConfigLogDao(IConfigLogDao configLogDao) {
		this.configLogDao = configLogDao;
	}
	
	public ICloudpoolmappingDao getCloudpoolmappingDao() {
		return cloudpoolmappingDao;
	}
	
	public void setCloudpoolmappingDao(ICloudpoolmappingDao cloudpoolmappingDao) {
		this.cloudpoolmappingDao = cloudpoolmappingDao;
	}
	
	public DataManager getDataManager() {
		return dataManager;
	}
	
	public void setDataManager(DataManager dataManager) {
		this.dataManager = dataManager;
	}
	
	public IClientInputDao getClientInputDao() {
		return clientInputDao;
	}
	
	public void setClientInputDao(IClientInputDao clientInputDao) {
		this.clientInputDao = clientInputDao;
	}
	
	public IProSaveRecordDao getProSaveRecordDao() {
		return proSaveRecordDao;
	}
	
	public void setProSaveRecordDao(IProSaveRecordDao proSaveRecordDao) {
		this.proSaveRecordDao = proSaveRecordDao;
	}
	
}
