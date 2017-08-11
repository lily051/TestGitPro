/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月9日 上午10:23:57
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.device.service.impl;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

import org.apache.poi.hssf.usermodel.DVConstraint;
import org.apache.poi.hssf.usermodel.HSSFDataValidation;
import org.apache.poi.hssf.util.CellRangeAddressList;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.apache.poi.ss.usermodel.WorkbookFactory;
import org.apache.struts2.ServletActionContext;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.google.common.collect.Lists;
import com.google.common.collect.Maps;
import com.hikvision.cms.cache.manager.DataManager;
import com.hikvision.finance.core.util.BeanUtils;
import com.hikvision.finance.core.util.CollectionUtils;
import com.hikvision.finance.core.util.JsonUtils;
import com.hikvision.finance.core.util.ListUtil;
import com.hikvision.finance.core.util.SercurityUtil;
import com.hikvision.finance.core.util.StringUtils;
import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.service.BaseServiceImpl;
import com.hikvision.finance.fms.common.enums.LogModuledEnum;
import com.hikvision.finance.fms.common.excel.ExcelHelper;
import com.hikvision.finance.fms.common.excel.ExcelHelperFactory;
import com.hikvision.finance.fms.common.excel.model.ExcelHead;
import com.hikvision.finance.fms.common.init.FmsPropertyPlaceholder;
import com.hikvision.finance.fms.common.util.Base64Utils;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.common.util.Constants.OperStatus;
import com.hikvision.finance.fms.common.util.ExcelUtil;
import com.hikvision.finance.fms.common.util.ValidateUtil;
import com.hikvision.finance.fms.license.dao.ISoftdogDao;
import com.hikvision.finance.fms.model.Clientinput;
import com.hikvision.finance.fms.model.Cloudpoolmapping;
import com.hikvision.finance.fms.model.Cvrassociatemapping;
import com.hikvision.finance.fms.model.Device;
import com.hikvision.finance.fms.model.Devicechannel;
import com.hikvision.finance.fms.model.Prosaverecord;
import com.hikvision.finance.fms.model.Records;
import com.hikvision.finance.fms.model.Region;
import com.hikvision.finance.fms.modules.device.dao.ICloudpoolmappingDao;
import com.hikvision.finance.fms.modules.device.dao.ICvrassociatemappingDao;
import com.hikvision.finance.fms.modules.device.dao.IDeviceChannelDao;
import com.hikvision.finance.fms.modules.device.dao.IDeviceDao;
import com.hikvision.finance.fms.modules.device.dto.CloudPoolInfo;
import com.hikvision.finance.fms.modules.device.dto.DeviceChannelInfo;
import com.hikvision.finance.fms.modules.device.dto.DeviceDto;
import com.hikvision.finance.fms.modules.device.dto.DeviceInfo;
import com.hikvision.finance.fms.modules.device.qo.DeviceQo;
import com.hikvision.finance.fms.modules.device.service.IDeviceService;
import com.hikvision.finance.fms.modules.log.dao.IConfigLogDao;
import com.hikvision.finance.fms.modules.record.dao.IProSaveRecordDao;
import com.hikvision.finance.fms.modules.region.dao.IRegionDao;
import com.hikvision.finance.fms.modules.sale.dao.IClientInputDao;
import com.hikvision.finance.fms.modules.user.dao.IUserdevicemappingDao;
import com.hikvision.finance.fms.modules.user.model.RecordPojo;
import com.hikvision.finance.fms.webdevice.datacollect.dao.IRecordsDao;
import com.hikvision.finance.fwork.exception.ExpectedException;
import com.hikvision.finance.fwork.util.AjaxData;
import com.hikvision.finance.fwork.util.GlobalMessageUtil;


/**
 * <p></p>
 * @author jinxindong 2016年3月9日 上午10:23:57
 * @version V1.0 
 */
@SuppressWarnings("deprecation")
public class DeviceServiceImpl extends BaseServiceImpl<Device, DeviceQo, IDeviceDao> implements IDeviceService {

	private static final Logger logger = LoggerFactory.getLogger(DeviceServiceImpl.class);
	private IDeviceDao deviceDao;
	private IRegionDao regionDao;
	private IDeviceChannelDao deviceChannelDao;
	private ICloudpoolmappingDao cloudpoolmappingDao;
	private ICvrassociatemappingDao cvrassociatemappingDao;
	private IUserdevicemappingDao userdevicemappingDao;
	private ISoftdogDao softdogDao;
	private IConfigLogDao configLogDao;
	private IProSaveRecordDao proSaveRecordDao;
	private IClientInputDao clientInputDao;
	private IRecordsDao recordsDao;
	private DataManager dataManager;
	
	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.service.IBaseService#deleteByIds(java.io.Serializable[])
     */
    @Override
    public void deleteByIds(Serializable[] paramArrayOfSerializable) {
	    
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.service.BaseServiceImpl#getDao()
     */
    @Override
    protected IDeviceDao getDao() {
	    return this.getDeviceDao();
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.device.service.IDeviceService#getStoDataPage(java.lang.Integer, java.lang.Integer, com.hikvision.finance.fms.modules.device.dto.DeviceInfo)
     */
    @Override
	public Page getStoDataPage(Integer limit, Integer start, DeviceInfo deviceInfo) {
		return deviceDao.getStoDataPage(limit, start, deviceInfo);
	}

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.device.service.IDeviceService#addSto(com.hikvision.finance.fms.modules.device.dto.DeviceInfo)
     */
    @Override
	public AjaxData addSto(DeviceInfo deviceInfo, String jsonStr) {
		AjaxData ajaxData = new AjaxData(false);
		DeviceDto deviceDto = JsonUtils.json2Object(jsonStr, DeviceDto.class);
		List<CloudPoolInfo> cloudPoolInfos = deviceDto.getCloudPoolInfoList();
		Integer uiTotalSpace = 0;// 总空间
		Integer uiFreeSpace = 0;// 总的剩余空间
		//编码 名称  ip+端口唯一约束校验
		List<Device> devices =  deviceDao.findBy("strName", deviceInfo.getStrName().trim());
		if(devices != null && devices.size()>0){
			return ajaxData.setFailureMsg("已存在设备名称为" + deviceInfo.getStrName());
		}
		List<Device> devices1 =  deviceDao.findBy("strIndexCode", deviceInfo.getStrIndexCode().trim());
		if(devices1 != null &&  devices1.size()>0){
			return ajaxData.setFailureMsg("已存在设备编码为" + deviceInfo.getStrIndexCode());
		}
		
		List<Device> devices2 = deviceDao.findBy(new String[]{"strIp","nport"}, new Object[]{deviceInfo.getStrIp().trim(),deviceInfo.getNport()});
		if(devices2 != null &&  devices2.size()>0){
			return ajaxData.setFailureMsg("IP、端口组合有重复");
		}
		Device device = new Device();
		device.setNregionId(deviceInfo.getNregionId());// 区域id
		device.setNtype(deviceInfo.getNtype());// 设备类型
		device.setStrName(deviceInfo.getStrName());// 设备名称
		device.setStrIndexCode(deviceInfo.getStrIndexCode());// 设备编码
		device.setStrManufacture(deviceInfo.getStrManufacture());// 设备厂商
		device.setStrIp(deviceInfo.getStrIp());// 设备ip
		device.setNport(deviceInfo.getNport());// 设备端口
		device.setStrUser(deviceInfo.getStrUser());// 设备用户名
		device.setOnline(Constants.CONS_0);
		String pass = Base64Utils.getFromBase64(deviceInfo.getStrPassword());
		device.setStrPassword(SercurityUtil.encrypt(pass));// 设备密码
		if (StringUtils.isNotEmpty(deviceInfo.getAccessKey()) &&  StringUtils.isNotEmpty(deviceInfo.getSecretKey())) {
			device.setStrPasswordLevel(Constants.PasswordSecurityLevel.NO_PWD);// 对象存储无密码
		} else {
			device.setStrPasswordLevel(deviceInfo.getStrPasswordLevel());// 设备密码等级
		}
		device.setNtpEnable(deviceInfo.getNtpEnable());
		
		String accessKey = Base64Utils.getFromBase64(deviceInfo.getAccessKey());
		String secretKey = Base64Utils.getFromBase64(deviceInfo.getSecretKey());
		device.setStrAccessKey(SercurityUtil.encrypt(accessKey));
		device.setStrSecretKey(SercurityUtil.encrypt(secretKey));
		
		Integer deviceId = (Integer)deviceDao.save(device);
		
		String strChoiceUsbIpc = FmsPropertyPlaceholder.getProperty("choiceUsbIpc");
		if (StringUtils.isEmpty(strChoiceUsbIpc)) {
			throw new ExpectedException("", "未获取到通道方式");
		}
		if(cloudPoolInfos!=null && cloudPoolInfos.size()>0){
			for (CloudPoolInfo cloudPoolInfo : cloudPoolInfos) {
				//计算总的和剩余的容量
				uiTotalSpace = uiTotalSpace + cloudPoolInfo.getPoolSize();
				uiFreeSpace = uiFreeSpace + cloudPoolInfo.getPoolFreeSize();
				// 添加存储池
				Cloudpoolmapping cloudpoolmapping = new Cloudpoolmapping();
				cloudpoolmapping.setNdeviceId(deviceId);
				cloudpoolmapping.setPoolId(cloudPoolInfo.getPoolId());
				cloudpoolmapping.setPoolName(cloudPoolInfo.getPoolName());
				cloudpoolmapping.setPoolType(cloudPoolInfo.getPoolType());
				cloudpoolmapping.setPoolFreeSize(cloudPoolInfo.getPoolFreeSize());
				cloudpoolmapping.setPoolSize(cloudPoolInfo.getPoolSize());
				cloudpoolmappingDao.save(cloudpoolmapping);
			}
			Device deviceOld = deviceDao.get(deviceId);
			deviceOld.setUiFreeSpace(uiFreeSpace);
			deviceOld.setUiTotalSpace(uiTotalSpace);
			deviceDao.update(deviceOld);
		} else if ("usb".equals(strChoiceUsbIpc) && Constants.DeviceType.DEV_TYPE_CLOUD_STORAGE_OBJECT_STORAGE == device.getNtype()) {
			ajaxData.put("isObjectStorage", "yes");
		}
		
//		String strChoiceUsbIpc = FmsPropertyPlaceholder.getProperty("choiceUsbIpc");
//		if (StringUtils.isEmpty(strChoiceUsbIpc)) {
//			throw new ExpectedException("", "未获取到通道方式");
//		}
//		if ("usb".equals(strChoiceUsbIpc) && Constants.DeviceType.DEV_TYPE_CLOUD_STORAGE_OBJECT_STORAGE == device.getNtype()) {//USB模式下,云存储-对象存储需设置strBucket
//			Cloudpoolmapping cloudpoolmapping = cloudpoolmappingDao.findUniqueBy("ndeviceId", deviceId);
//			if (null == cloudpoolmapping) {
//				throw new ExpectedException("", "未获取到分卷信息");
//			}
//		}
		configLogDao.setLogContent(OperStatus.ADD, LogModuledEnum.DEVICE.getCode(),GlobalMessageUtil.getMessage("log.stodev.add",new String[] {deviceInfo.getStrName()}));
		return ajaxData;
	}

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.device.service.IDeviceService#getStoDevById()
     */
    @Override
	public DeviceInfo getStoDevById(Integer deviceId) {
		DeviceInfo deviceInfo = new DeviceInfo();
		Device device = deviceDao.get(deviceId);
		try {
			BeanUtils.copyNotNullProperties(deviceInfo, device);
		} catch (Exception e) {
			throw new ExpectedException("", "拷贝bean失败[getStoDevById]");
		}
		String regionName = regionDao.get(deviceInfo.getNregionId()).getStrName();
		deviceInfo.setRegionName(regionName);//添加区域名字在前段显示
		String pass = Base64Utils.getBase64(SercurityUtil.decrypt(deviceInfo.getStrPassword()));
		deviceInfo.setStrPassword(pass);
		
		if (StringUtils.isNotEmpty(device.getStrAccessKey()) && StringUtils.isNotEmpty(device.getStrSecretKey())) {
			String accessKey = Base64Utils.getBase64(SercurityUtil.decrypt(device.getStrAccessKey()));
			deviceInfo.setAccessKey(accessKey);
			String secretKey = Base64Utils.getBase64(SercurityUtil.decrypt(device.getStrSecretKey()));
			deviceInfo.setSecretKey(secretKey);			
		} else {
			deviceInfo.setAccessKey("");
			deviceInfo.setSecretKey("");
		}
		
		//设置存储池信息
		List<Cloudpoolmapping> cloudpoolmappings = cloudpoolmappingDao.findBy("ndeviceId", device.getId());
		List<DeviceChannelInfo> cloudpoolInfos = new ArrayList<DeviceChannelInfo>();
		for(Cloudpoolmapping cloudpoolmapping :cloudpoolmappings){
			DeviceChannelInfo cloudpoolInfo = new DeviceChannelInfo();
			String groupIndex = cloudpoolmapping.getPoolName()+":剩余"+cloudpoolmapping.getPoolFreeSize()+"G"+"(共:"+cloudpoolmapping.getPoolSize()+"G)";
			//复用deviceChannelInfo显示存储池信息
			cloudpoolInfo.setNchannelName(groupIndex);
			cloudpoolInfo.setNchannelNo(cloudpoolmapping.getPoolId());
			cloudpoolInfos.add(cloudpoolInfo);
		}
		deviceInfo.setDeviceChannelList(cloudpoolInfos);
		
		return deviceInfo;
	}

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.device.service.IDeviceService#updateSto(com.hikvision.finance.fms.modules.device.dto.DeviceInfo)
     */
    @Override
    public AjaxData updateSto(DeviceInfo deviceInfo,String jsonStr) {
    	AjaxData ajaxData = new AjaxData(false);
		DeviceDto deviceDto = JsonUtils.json2Object(jsonStr, DeviceDto.class);
		List<CloudPoolInfo> cloudPoolInfos = deviceDto.getCloudPoolInfoList();
		Device device = deviceDao.get(deviceInfo.getId());
		device.setNregionId(deviceInfo.getNregionId());// 区域id
		
		List<Device> devices =  deviceDao.findBy("strName", deviceInfo.getStrName().trim());
		if(devices != null && devices.size()>0){
			for (Device d : devices) {
				if (!d.getId().equals(device.getId())) {
					return ajaxData.setFailureMsg("已存在设备名称为" + deviceInfo.getStrName());
				}
			}			
		}
		
		List<Device> devices1 =  deviceDao.findBy("strIndexCode", deviceInfo.getStrIndexCode().trim());
		if(devices1 != null &&  devices1.size()>0){
			for (Device d : devices1) {
				if (!d.getId().equals(device.getId())) {
					return ajaxData.setFailureMsg("已存在设备编码为" + deviceInfo.getStrIndexCode());
				}
			}				
		}
		
		List<Device> devices2 = deviceDao.findBy(new String[]{"strIp","nport"}, new Object[]{deviceInfo.getStrIp().trim(),deviceInfo.getNport()});
		if(devices2 != null &&  devices2.size()>0){
			for (Device d : devices2) {
				if (!d.getId().equals(device.getId())) {
					return ajaxData.setFailureMsg("IP、端口组合有重复");
				}
			}	
		}
		
		device.setNtype(deviceInfo.getNtype());// 设备类型
		device.setStrName(deviceInfo.getStrName());// 设备名称
		device.setStrIndexCode(deviceInfo.getStrIndexCode());// 设备编码
		device.setStrManufacture(deviceInfo.getStrManufacture());// 设备厂商
		device.setStrIp(deviceInfo.getStrIp());// 设备ip
		device.setNport(deviceInfo.getNport());// 设备端口
		device.setStrUser(deviceInfo.getStrUser());// 设备用户名
		String pass = Base64Utils.getFromBase64(deviceInfo.getStrPassword());
		device.setStrPassword(SercurityUtil.encrypt(pass));// 设备密码
		if (StringUtils.isNotEmpty(deviceInfo.getAccessKey()) &&  StringUtils.isNotEmpty(deviceInfo.getSecretKey())) {
			device.setStrPasswordLevel(Constants.PasswordSecurityLevel.NO_PWD);// 对象存储无密码
		} else {
			device.setStrPasswordLevel(deviceInfo.getStrPasswordLevel());// 设备密码等级
		}
		device.setNtpEnable(deviceInfo.getNtpEnable());
		
		String accessKey = Base64Utils.getFromBase64(deviceInfo.getAccessKey());
		String secretKey = Base64Utils.getFromBase64(deviceInfo.getSecretKey());
		device.setStrAccessKey(SercurityUtil.encrypt(accessKey));
		device.setStrSecretKey(SercurityUtil.encrypt(secretKey));
		
		//标记存储池传空时  是否更改原有存储池信息    0不需要(没有点重新获取)    1需要
		if(deviceInfo.getFlag()==1){
			updateStoragePool(device, cloudPoolInfos);
		}
		
		String strChoiceUsbIpc = FmsPropertyPlaceholder.getProperty("choiceUsbIpc");
		if (StringUtils.isEmpty(strChoiceUsbIpc)) {
			throw new ExpectedException("", "未获取到通道方式");
		}
//		if ("usb".equals(strChoiceUsbIpc) && Constants.DeviceType.DEV_TYPE_CLOUD_STORAGE_OBJECT_STORAGE == device.getNtype()) {
//			Cloudpoolmapping cloudpoolmapping = cloudpoolmappingDao.findUniqueBy("ndeviceId", device.getId());
//			if (null == cloudpoolmapping) {
//				throw new ExpectedException("", "未获取到分卷信息");
//			}
//		}
		
		deviceDao.update(device);
		if ("usb".equals(strChoiceUsbIpc) && Constants.DeviceType.DEV_TYPE_CLOUD_STORAGE_OBJECT_STORAGE == device.getNtype() && device.getUiTotalSpace() == null) {
			ajaxData.put("isObjectStorage", "yes");
		}
		configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.DEVICE.getCode(),GlobalMessageUtil.getMessage("log.stodev.update",new String[] {deviceInfo.getStrName()}));
		return ajaxData;
    }

    
   /**
    * 更新存储池信息
    * @author fuqunqing 2016年9月21日 上午11:23:48
    * @param device
    * @param cloudPoolInfos
    */
   private void updateStoragePool(Device device, List<CloudPoolInfo> cloudPoolInfos){
	   	Integer uiTotalSpace = 0;// 总空间
		Integer uiFreeSpace = 0;// 总的剩余空间
		List<Cloudpoolmapping> cloudpoololds = cloudpoolmappingDao.findBy("ndeviceId", device.getId());
		Map<Integer, Cloudpoolmapping> poolmapOld = Maps.newHashMap();
		for(Cloudpoolmapping cloudpoolold: cloudpoololds){
			poolmapOld.put(cloudpoolold.getPoolId(), cloudpoolold);
		}
		
		List<Cloudpoolmapping> cloudpooladd = Lists.newArrayList();
		List<Cloudpoolmapping> cloudpoolupt = Lists.newArrayList();
		List<Cloudpoolmapping> cloudpooldel = Lists.newArrayList();
		for (CloudPoolInfo cloudPoolInfo : cloudPoolInfos) {
			//计算总的和剩余的容量
			uiTotalSpace += cloudPoolInfo.getPoolSize();
			uiFreeSpace += cloudPoolInfo.getPoolFreeSize();
			if(poolmapOld.containsKey(cloudPoolInfo.getPoolId())){
				//update
				Cloudpoolmapping cloudpool = poolmapOld.get(cloudPoolInfo.getPoolId());
				cloudpool.setPoolName(cloudPoolInfo.getPoolName());
				cloudpool.setPoolType(cloudPoolInfo.getPoolType());
				cloudpool.setPoolFreeSize(cloudPoolInfo.getPoolFreeSize());
				cloudpool.setPoolSize(cloudPoolInfo.getPoolSize());
				cloudpoolupt.add(cloudpool);
				poolmapOld.remove(cloudPoolInfo.getPoolId());
			}else{
				//add
				Cloudpoolmapping cloudpool = new Cloudpoolmapping();
				cloudpool.setNdeviceId(device.getId());
				cloudpool.setPoolId(cloudPoolInfo.getPoolId());
				cloudpool.setPoolName(cloudPoolInfo.getPoolName());
				cloudpool.setPoolType(cloudPoolInfo.getPoolType());
				cloudpool.setPoolFreeSize(cloudPoolInfo.getPoolFreeSize());
				cloudpool.setPoolSize(cloudPoolInfo.getPoolSize());
				cloudpooladd.add(cloudpool);
			}
		}
		cloudpooldel = new ArrayList<Cloudpoolmapping>(poolmapOld.values());
		
		//更新存储池信息
		cloudpoolmappingDao.deleteAll(cloudpooldel);
		cloudpoolmappingDao.saveAll(cloudpooladd);
		cloudpoolmappingDao.updateAll(cloudpoolupt);

		for(Cloudpoolmapping pool: cloudpooldel){
			//删除采集和存储的关联关系
			cvrassociatemappingDao.deleteBy(new String[]{"ncvrId","groupIndex"}, new Object[]{pool.getNdeviceId(),pool.getPoolId()});
		}
		
		device.setUiFreeSpace(uiFreeSpace);
		device.setUiTotalSpace(uiTotalSpace);
   }
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.device.service.IDeviceService#deleteStos(java.util.List)
     */
    @Override
	public AjaxData deleteStos(List<Integer> ids) {
		AjaxData ajaxData = new AjaxData(false);
		List<String> names = new ArrayList<String>();		
		for (Integer devId : ids) {
			//判断存储设备中是否含有录像
			List<Prosaverecord>  prosaverecords = proSaveRecordDao.findBy("cvr",devId);
			if (prosaverecords != null && prosaverecords.size() > 0) {
				for(Prosaverecord prosaverecord:prosaverecords){
					List<Clientinput> clientinputs = clientInputDao.findBy(new String[]{"id","nisPay"},new Object[]{prosaverecord.getClientId(),0} );
					if (clientinputs != null && clientinputs.size() > 0) {
						throw new ExpectedException("", "选中的设备中,含有未删除业务录像的设备");
					}
				}
			}
						
			//判断和采集设备关联关系
			List<Cvrassociatemapping> cvrassociatemappings = cvrassociatemappingDao.findBy("ncvrId", devId);
			if(cvrassociatemappings!=null && cvrassociatemappings.size()>0){
				throw new ExpectedException("", "选中的设备中,含有和采集设备已关联的存储设备,请先解除关联关系");
			}
			Device device = deviceDao.get(devId);
		    //删除存储设备
			deviceDao.deleteById(devId);
			//删除存储池
			cloudpoolmappingDao.deleteBy("ndeviceId", devId);
			//删除集中存储设备和采集设备关系
			//cvrassociatemappingDao.deleteBy("ncvrId", devId);
			names.add(device.getStrName());
		}
		if(ids.size()>5){
			configLogDao.setLogContent(OperStatus.DELETE, LogModuledEnum.DEVICE.getCode(),GlobalMessageUtil.getMessage("log.stodev.delete.batch"));
		}else{
			configLogDao.setLogContent(OperStatus.DELETE, LogModuledEnum.DEVICE.getCode(),GlobalMessageUtil.getMessage("log.stodev.delete",new String[] {ListUtil.listToStr(names)}));	
		}
		return ajaxData;
	}
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.device.service.IDeviceService#getCollDataPage(java.lang.Integer, java.lang.Integer, com.hikvision.finance.fms.modules.device.dto.DeviceInfo)
     */
    @Override
    public Page getCollDataPage(Integer limit, Integer start, DeviceInfo deviceInfo) {
    	return deviceDao.getCollDataPage(limit, start, deviceInfo);
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.device.service.IDeviceService#addColl(com.hikvision.finance.fms.modules.device.dto.DeviceInfo, java.lang.String)
     */
    @Override
	public AjaxData addColl(DeviceInfo deviceInfo, String jsonStr) {
		AjaxData ajaxData = new AjaxData(false);
		DeviceDto deviceDto = JsonUtils.json2Object(jsonStr, DeviceDto.class);
		List<DeviceChannelInfo> channelInfos = deviceDto.getDeviceChannelList();
		//编码 名称  ip+端口唯一约束校验
		List<Device> devices =  deviceDao.findBy("strName", deviceInfo.getStrName().trim());
		if(devices != null && devices.size()>0){
			return ajaxData.setFailureMsg("已存在设备名称为" + deviceInfo.getStrName());
		}
		List<Device> devices1 =  deviceDao.findBy("strIndexCode", deviceInfo.getStrIndexCode().trim());
		if(devices1 != null &&  devices1.size()>0){
			return ajaxData.setFailureMsg("已存在设备编码为" + deviceInfo.getStrIndexCode());
		}
		
		List<Device> devices2 = deviceDao.findBy(new String[]{"strIp","nport"}, new Object[]{deviceInfo.getStrIp().trim(),deviceInfo.getNport()});
		if(devices2 != null &&  devices2.size()>0){
			return ajaxData.setFailureMsg("IP、端口组合有重复");
		}
		// 1、保存采集设备基本信息
		Device device = new Device();
		device.setNregionId(deviceInfo.getNregionId());// 区域id
		device.setNtype(deviceInfo.getNtype());// 设备类型
		device.setStrName(deviceInfo.getStrName());// 设备名称
		device.setStrIndexCode(deviceInfo.getStrIndexCode());// 设备编码
		device.setStrManufacture(deviceInfo.getStrManufacture());// 设备厂商
		device.setStrIp(deviceInfo.getStrIp());// 设备ip
		device.setNport(deviceInfo.getNport());// 设备端口
		device.setOnline(Constants.CONS_0);
		device.setNstreamType(deviceInfo.getNstreamType());// nStreamType设备码流
		device.setStrUser(deviceInfo.getStrUser());// 设备用户名
		String pass = Base64Utils.getFromBase64(deviceInfo.getStrPassword());
		device.setStrPassword(SercurityUtil.encrypt(pass));// 设备密码
		device.setStrPasswordLevel(deviceInfo.getStrPasswordLevel());// 设备密码等级
		device.setTelChanNum(deviceInfo.getTelChanNum());// ip通道个数telChanNum
		device.setNchanNum(deviceInfo.getNchanNum());// 模拟通道个数nChanNum
		device.setNtpEnable(deviceInfo.getNtpEnable());
		Integer deviceId = (Integer)deviceDao.save(device);
		configLogDao.setLogContent(OperStatus.ADD, LogModuledEnum.DEVICE.getCode(),GlobalMessageUtil.getMessage("log.colldev.add",new String[] {deviceInfo.getStrName()}));
		
		String strChoiceUsbIpc = FmsPropertyPlaceholder.getProperty("choiceUsbIpc");
		if (StringUtils.isEmpty(strChoiceUsbIpc)) {
			return ajaxData.setFailureMsg("未获取到通道方式");
		}
		
		String isNonCenterStorage = FmsPropertyPlaceholder.getProperty("nonCenterStorage");
		if (StringUtils.isEmpty(isNonCenterStorage)) {
			return ajaxData.setFailureMsg("未获取到中心存储的状态");
		}
		// 2、关联的存储设备CvrAssociateMapping表插入数据
		if(deviceInfo.getNdevStoId()!=null  && "ipc".equals(strChoiceUsbIpc) && !"true".equals(isNonCenterStorage)){
			Cvrassociatemapping cvrassociatemapping = new Cvrassociatemapping();
			cvrassociatemapping.setNcollId(deviceId);// 采集设备id
			cvrassociatemapping.setNcvrId(deviceInfo.getNdevStoId());// 存储设备id
			cvrassociatemapping.setNtype(deviceInfo.getNstreamType());// 码流类型
			if(deviceInfo.getGroupIndexId()==null){
				throw new ExpectedException("", "请关联分卷信息"); 
			}
			cvrassociatemapping.setGroupIndex(deviceInfo.getGroupIndexId());// 存储设备分券
			cvrassociatemappingDao.save(cvrassociatemapping);
		}
			
		// 3、信道表DeviceChannel插入数据
		if (CollectionUtils.isNotEmpty(channelInfos)) {
			//判断通道是否超过限制，抛出异常处理
			int channelLimt = softdogDao.getAuthoriseDeviceChannel();//上限阈值
			int channelAll = deviceChannelDao.getAll().size()+channelInfos.size();
			if(channelLimt < channelAll){
				throw new ExpectedException("", "通道数量超过授权上限： "+channelLimt+" 条");
			}
			for (DeviceChannelInfo channelInfo : channelInfos) {
				Devicechannel devicechannel = new Devicechannel();
				devicechannel.setNchannelNo(channelInfo.getNchannelNo());// 信道号
				devicechannel.setNchannelName(channelInfo.getNchannelName());// 信道名称
				devicechannel.setNdeviceId(deviceId);// 采集设备id
				devicechannel.setNregionId(deviceInfo.getNregionId());// 所属区域id
				deviceChannelDao.save(devicechannel);
			}
		}
		
		
		return ajaxData;
	}

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.device.service.IDeviceService#getCollDevById(java.lang.Integer)
     */
    @Override
    public DeviceInfo getCollDevById(Integer deviceId) {
	    //1、通过设备采集id获取单个采集设备信息
		DeviceInfo deviceInfo = new DeviceInfo();
		Device device = deviceDao.get(deviceId);
		try {
			BeanUtils.copyNotNullProperties(deviceInfo, device);
		} catch (Exception e) {
			throw new ExpectedException("", "拷贝bean失败[getCollDevById]");
		}
		String regionName = regionDao.get(deviceInfo.getNregionId()).getStrName();
		deviceInfo.setRegionName(regionName);//添加区域名字在前段显示
		String pass = Base64Utils.getBase64(SercurityUtil.decrypt(deviceInfo.getStrPassword()));
		deviceInfo.setStrPassword(pass);
		//2、通过采集设备id获取挂靠的存储设备
		Cvrassociatemapping cvrassociatemapping = cvrassociatemappingDao.findUniqueBy("ncollId", deviceId);
		if(cvrassociatemapping!=null){
			//关联的存储设备信息
			deviceInfo.setNdevStoId(cvrassociatemapping.getNcvrId());
			Device deviceBy = deviceDao.get(cvrassociatemapping.getNcvrId());
			if(deviceBy!=null){
				deviceInfo.setNdevStoName(deviceBy.getStrName());
				deviceInfo.setGroupIndexId(cvrassociatemapping.getGroupIndex());
				List<Cloudpoolmapping> cloudpoolmappings = cloudpoolmappingDao.findBy("ndeviceId", deviceBy.getId());
				Cloudpoolmapping cloudpool = null;
				String groupIndex = "";
				for(Cloudpoolmapping cloudpoolmapping :cloudpoolmappings){
					if( cvrassociatemapping.getGroupIndex().equals(cloudpoolmapping.getPoolId())){
						cloudpool = cloudpoolmapping;
						groupIndex = cloudpool.getPoolName()+":剩余"+cloudpool.getPoolFreeSize()+"G"+"(共:"+cloudpool.getPoolSize()+"G)";
						break;
					}
				}
				deviceInfo.setGroupIndex(groupIndex);
			}
		}
		
		
		//3、查询信道list
		List<DeviceChannelInfo> deviceChannelInfoList = new ArrayList<DeviceChannelInfo>();
		List<Devicechannel> deviceChannelList = deviceChannelDao.findBy("ndeviceId", deviceId);
		for(Devicechannel devicechannel : deviceChannelList){
			DeviceChannelInfo deviceChannelInfo = new DeviceChannelInfo();
			try {
				BeanUtils.copyNotNullProperties(deviceChannelInfo, devicechannel);
			} catch (Exception e) {
				throw new ExpectedException("", "拷贝bean失败[getCollDevById]");
			}
			deviceChannelInfoList.add(deviceChannelInfo);
		}
		
		deviceInfo.setDeviceChannelList(deviceChannelInfoList);
		
		return deviceInfo;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.device.service.IDeviceService#updateColl(com.hikvision.finance.fms.modules.device.dto.DeviceInfo, java.lang.String)
     */
    @Override
    public AjaxData updateColl(DeviceInfo deviceInfo, String jsonStr) {
    	AjaxData ajaxData = new AjaxData(false);
		DeviceDto deviceDto = JsonUtils.json2Object(jsonStr, DeviceDto.class);
		List<DeviceChannelInfo> channelInfos = deviceDto.getDeviceChannelList();
		// 1、保存采集设备基本信息
		Device device = deviceDao.get(deviceInfo.getId());
		
		List<Device> devices =  deviceDao.findBy("strName", deviceInfo.getStrName().trim());
		if(devices != null && devices.size()>0){
			for (Device d : devices) {
				if (!d.getId().equals(device.getId())) {
					return ajaxData.setFailureMsg("已存在设备名称为" + deviceInfo.getStrName());
				}
			}			
		}
		
		List<Device> devices1 =  deviceDao.findBy("strIndexCode", deviceInfo.getStrIndexCode().trim());
		if(devices1 != null &&  devices1.size()>0){
			for (Device d : devices1) {
				if (!d.getId().equals(device.getId())) {
					return ajaxData.setFailureMsg("已存在设备编码为" + deviceInfo.getStrIndexCode());
				}
			}				
		}
		
		List<Device> devices2 = deviceDao.findBy(new String[]{"strIp","nport"}, new Object[]{deviceInfo.getStrIp().trim(),deviceInfo.getNport()});
		if(devices2 != null &&  devices2.size()>0){
			for (Device d : devices2) {
				if (!d.getId().equals(device.getId())) {
					return ajaxData.setFailureMsg("IP、端口组合有重复");															
				}
			}	
		}
		
		device.setNregionId(deviceInfo.getNregionId());// 区域id
		device.setNtype(deviceInfo.getNtype());// 设备类型
		device.setStrName(deviceInfo.getStrName());// 设备名称
		device.setStrIndexCode(deviceInfo.getStrIndexCode());// 设备编码
		device.setStrManufacture(deviceInfo.getStrManufacture());// 设备厂商
		device.setStrIp(deviceInfo.getStrIp());// 设备ip
		device.setNport(deviceInfo.getNport());// 设备端口
		device.setNstreamType(deviceInfo.getNstreamType());// nStreamType设备码流
		device.setStrUser(deviceInfo.getStrUser());// 设备用户名
		String pass = Base64Utils.getFromBase64(deviceInfo.getStrPassword());
		device.setStrPassword(SercurityUtil.encrypt(pass));// 设备密码
		device.setStrPasswordLevel(deviceInfo.getStrPasswordLevel());// 设备密码等级
		device.setTelChanNum(deviceInfo.getTelChanNum());// ip通道个数telChanNum
		device.setNchanNum(deviceInfo.getNchanNum());// 模拟通道个数nChanNum
		device.setNtpEnable(deviceInfo.getNtpEnable());
		deviceDao.update(device);
		configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.DEVICE.getCode(),GlobalMessageUtil.getMessage("log.colldev.update",new String[] {deviceInfo.getStrName()}));
		
		String strChoiceUsbIpc = FmsPropertyPlaceholder.getProperty("choiceUsbIpc");
		if (StringUtils.isEmpty(strChoiceUsbIpc)) {
			return ajaxData.setFailureMsg("未获取到通道方式");
		}
		String isNonCenterStorage = FmsPropertyPlaceholder.getProperty("nonCenterStorage");
		if (StringUtils.isEmpty(isNonCenterStorage)) {
			return ajaxData.setFailureMsg("未获取到中心存储的状态");
		}
		// 2、关联的存储设备CvrAssociateMapping表更新数据
		Cvrassociatemapping cvrassociatemapping = cvrassociatemappingDao.findUniqueBy("ncollId", deviceInfo.getId());
		if(cvrassociatemapping!=null && "ipc".equals(strChoiceUsbIpc) && !"true".equals(isNonCenterStorage)){
			cvrassociatemapping.setNcvrId(deviceInfo.getNdevStoId());// 存储设备id
			cvrassociatemapping.setNtype(deviceInfo.getNstreamType());// 码流类型
			if(deviceInfo.getGroupIndexId()==null){
				throw new ExpectedException("", "请关联分卷信息"); 
			}
			cvrassociatemapping.setGroupIndex(deviceInfo.getGroupIndexId());// 存储设备分券
			cvrassociatemappingDao.update(cvrassociatemapping);
		}else{
			if(deviceInfo.getNdevStoId()!=null && !"true".equals(isNonCenterStorage)){
				cvrassociatemapping = new Cvrassociatemapping();
				cvrassociatemapping.setNcollId(deviceInfo.getId());
				cvrassociatemapping.setNcvrId(deviceInfo.getNdevStoId());// 存储设备id
				cvrassociatemapping.setNtype(deviceInfo.getNstreamType());// 码流类型
				if(deviceInfo.getGroupIndexId()==null){
					throw new ExpectedException("", "请关联分卷信息"); 
				}
				cvrassociatemapping.setGroupIndex(deviceInfo.getGroupIndexId());// 存储设备分券
				cvrassociatemappingDao.save(cvrassociatemapping);
			}
		}
		
		// 3、更新通道信息
		updateCollChannel(channelInfos, device, deviceInfo.getRflag()==1?true:false);
		return ajaxData;
    }

    /**
     * 修改采集设备时候，更新通道信息，
     * @author fuqunqing 2016年9月23日 下午4:27:56
     * @param channelInfos
     * @param device
     * @param refresh 是否刷新通道信息
     */
    private void updateCollChannel(List<DeviceChannelInfo> channelInfos, Device device, boolean refresh){
    	
    	//未点击重新获取直接返回
    	if(refresh == false){
    		return;
    	}
    	
    	if (CollectionUtils.isNotEmpty(channelInfos)) {
			//判断通道是否超过限制，抛出异常处理
			int channelLimt = softdogDao.getAuthoriseDeviceChannel();//上限阈值
			int channelAll = deviceChannelDao.getAll().size()+channelInfos.size();
			if(channelLimt < channelAll){
				throw new ExpectedException("", "通道数量超过授权上限： "+channelLimt+" 条");
			}
			
			List<Devicechannel> channelolds = deviceChannelDao.findBy("ndeviceId", device.getId());
			Map<Integer, Devicechannel> channelmapOld = Maps.newHashMap();
			for(Devicechannel channelold: channelolds){
				channelmapOld.put(channelold.getNchannelNo(), channelold);
			}
			
			List<Devicechannel> channeladd = Lists.newArrayList();
			List<Devicechannel> channelupt = Lists.newArrayList();
			List<Devicechannel> channeldel = Lists.newArrayList();
			for (DeviceChannelInfo channelInfo : channelInfos) {
				if(channelmapOld.containsKey(channelInfo.getNchannelNo())){
					//update
					Devicechannel devicechannel = channelmapOld.get(channelInfo.getNchannelNo());
					devicechannel.setNchannelName(channelInfo.getNchannelName());
					channelupt.add(devicechannel);
					channelmapOld.remove(channelInfo.getNchannelNo());
				}else{
					//add
					Devicechannel devicechannel = new Devicechannel();
					devicechannel.setNchannelName(channelInfo.getNchannelName());
					devicechannel.setNchannelNo(channelInfo.getNchannelNo());
					devicechannel.setNdeviceId(device.getId());
					devicechannel.setNregionId(device.getNregionId());
					channeladd.add(devicechannel);
				}
			}
			channeldel = new ArrayList<Devicechannel>(channelmapOld.values());
			
			// 删除用户和通道的关联关系
			for(Devicechannel devicechannel :channeldel){
				userdevicemappingDao.deleteBy("devChannel", devicechannel.getId());
			}
			
			//更新信息
			deviceChannelDao.deleteAll(channeldel);
			deviceChannelDao.saveAll(channeladd);
			deviceChannelDao.updateAll(channelupt);
		}else{
			//被清空
			if(refresh){
				// 删除用户的关联关系
				userdevicemappingDao.deleteBy("devId", device.getId());
				// 删除通道
				deviceChannelDao.deleteBy("ndeviceId", device.getId());
			}
		}
    }
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.device.service.IDeviceService#deleteColls(java.util.List)
     */
    @Override
	public AjaxData deleteColls(List<Integer> ids) {
		AjaxData ajaxData = new AjaxData(false);
		List<String> names = new ArrayList<String>();
		for (Integer devId : ids) {
			//如果设备中有录像未上传
			List<Records> records = recordsDao.findBy("nrepairDevId",devId);
			for(Records record:records){
				if(record.getNrecordStatus()!=Constants.RecordStatus.UPLOADED){//4为已上传  其他为未上传
					throw new ExpectedException("", "选中的设备中,存在业务录像");
				}
			}
			Device device = deviceDao.get(devId);
			names.add(device.getStrName());
			// 删除用户和信道的关联关系
			userdevicemappingDao.deleteBy("devId", devId);
			// 删除信道
			deviceChannelDao.deleteBy("ndeviceId", devId);
			//集中存储和采集设备的关联表
			cvrassociatemappingDao.deleteBy("ncollId", devId);
			// 删除设备
			deviceDao.deleteById(devId);
		}
		if(ids.size()>5){
			configLogDao.setLogContent(OperStatus.DELETE, LogModuledEnum.DEVICE.getCode(),GlobalMessageUtil.getMessage("log.colldev.delete.batch"));
		}else{
			configLogDao.setLogContent(OperStatus.DELETE, LogModuledEnum.DEVICE.getCode(),GlobalMessageUtil.getMessage("log.colldev.delete",new String[] {ListUtil.listToStr(names)}));	
		}
		return ajaxData;
	}

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.device.service.IDeviceService#getDevChannelByIds(java.util.List)
     */
    @Override
	public List<DeviceChannelInfo> getDevChannelByIds(List<Integer> ids) {
		List<DeviceChannelInfo> channelInfos = new ArrayList<DeviceChannelInfo>();
		for (Integer devId : ids) {
			List<Devicechannel> list = deviceChannelDao.findBy("ndeviceId", devId);
			for (Devicechannel devicechannel : list) {
				DeviceChannelInfo channelInfo = new DeviceChannelInfo();
				try {
					BeanUtils.copyNotNullProperties(channelInfo, devicechannel);
				} catch (Exception e) {
					throw new ExpectedException("", "拷贝bean失败[getDevChannelByIds]");
				}
				channelInfos.add(channelInfo);
			}
		}
		return channelInfos;
	}
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.device.service.IDeviceService#getPoolsByDevId(java.lang.Integer)
     */
    @Override
	public List<CloudPoolInfo> getPoolsByDevId(DeviceInfo deviceInfo) {
    	//通过采集设备id去CvrAssociateMapping表中查询分券号
    	List<Cvrassociatemapping> cvrassociatemappings = null;
    	List<Cloudpoolmapping> cloudpoolmappings = new ArrayList<Cloudpoolmapping>();
    	if(deviceInfo!=null && deviceInfo.getId()!=null){ //编辑的时候，传了deviceInfo.ndevStoId、deviceInfo.id
    		cvrassociatemappings = cvrassociatemappingDao.findBy(new String[]{"ncollId","ncvrId"}, new Object[]{deviceInfo.getId(),deviceInfo.getNdevStoId()});	//"ncollId", deviceInfo.getId()
    	}
    	if(deviceInfo!=null) { //添加的时候，只传了deviceInfo.ndevStoId
    		cloudpoolmappings = cloudpoolmappingDao.findBy("ndeviceId", deviceInfo.getNdevStoId());
    	}
		List<CloudPoolInfo> cloudPoolInfos = new ArrayList<CloudPoolInfo>();
		for (Cloudpoolmapping cloudpoolmapping : cloudpoolmappings) {
			CloudPoolInfo cloudPoolInfo = new CloudPoolInfo();
			try {
				BeanUtils.copyNotNullProperties(cloudPoolInfo, cloudpoolmapping);
			} catch (Exception e) {
				throw new ExpectedException("", "拷贝bean失败[getPoolsByDevId]");
			}
			if(cvrassociatemappings!=null && cvrassociatemappings.size() > 0){
				cloudPoolInfo.setCheckedPoolId(cvrassociatemappings.get(0).getGroupIndex());	
			}
			cloudPoolInfos.add(cloudPoolInfo);
		}
		return cloudPoolInfos;
	}
    
    /**
     * 封装map信息
     * @author jinxindong 2016年4月6日 下午2:34:12
     * @param devicechannels
     * @return
     */
/*    private Map<Integer, DeviceChannelInfo> buildChannelMap(List<DeviceChannelInfo> devicechannels){
    	Map<Integer, DeviceChannelInfo> map = new HashMap<Integer, DeviceChannelInfo>();
    	for(DeviceChannelInfo devicechannel : devicechannels){
    		map.put(devicechannel.getNchannelNo(), devicechannel);
    	}
    	return map;
    }*/
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.device.service.IDeviceService#exportStoDevExcel(com.hikvision.finance.fms.modules.device.dto.DeviceInfo, java.lang.String[], java.lang.String[], java.lang.String)
     */
    @Override
	public boolean exportStoDevExcel(DeviceInfo deviceInfo, String[] titleName, String[] attrName, String path) {
		// 1，依次准备标题、标题映射属性、Sheet以及对应内容
		List<String[]> titleNames = new ArrayList<String[]>();
		List<String[]> attrNames = new ArrayList<String[]>();
		LinkedHashMap<String, List<?>> map = new LinkedHashMap<String, List<?>>();
		titleNames.add(titleName);
		attrNames.add(attrName);
		List<DeviceInfo> clientdeletelogInfos = deviceDao.exportStoDevExcel(deviceInfo);
		map.put("Sheet1", clientdeletelogInfos);
		// 2，创建Excel的数据对象并导出表格
		ExcelUtil.ExcelExportData setInfo = new ExcelUtil.ExcelExportData();
		setInfo.setColumnNames(titleNames);
		setInfo.setFieldNames(attrNames);
		setInfo.setDataMap(map);
		boolean exportResult = false;
		try {
			exportResult = ExcelUtil.export2File(setInfo, path);
			configLogDao.setLogContent(OperStatus.EXPORT, LogModuledEnum.DEVICE.getCode(),GlobalMessageUtil.getMessage("log.stodev.export"));
		} catch (Exception e) {
			logger.error("",e);
		}
		return exportResult;
	}


	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.device.service.IDeviceService#exportCollDevExcel(com.hikvision.finance.fms.modules.device.dto.DeviceInfo, java.lang.String[], java.lang.String[], java.lang.String)
     */
    @Override
    public boolean exportCollDevExcel(DeviceInfo deviceInfo, String[] titleName, String[] attrName, String path) {
		// 1，依次准备标题、标题映射属性、Sheet以及对应内容
		List<String[]> titleNames = new ArrayList<String[]>();
		List<String[]> attrNames = new ArrayList<String[]>();
		LinkedHashMap<String, List<?>> map = new LinkedHashMap<String, List<?>>();
		titleNames.add(titleName);
		attrNames.add(attrName);
		List<DeviceInfo> clientdeletelogInfos = deviceDao.exportCollDevExcel(deviceInfo);
		map.put("Sheet1", clientdeletelogInfos);
		// 2，创建Excel的数据对象并导出表格
		ExcelUtil.ExcelExportData setInfo = new ExcelUtil.ExcelExportData();
		setInfo.setColumnNames(titleNames);
		setInfo.setFieldNames(attrNames);
		setInfo.setDataMap(map);
		boolean exportResult = false;
		try {
			exportResult = ExcelUtil.export2File(setInfo, path);
			configLogDao.setLogContent(OperStatus.EXPORT, LogModuledEnum.DEVICE.getCode(),GlobalMessageUtil.getMessage("log.colldev.export"));
		} catch (Exception e) {
			logger.error("",e);
		} 
		return exportResult;
    }
	
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.device.service.IDeviceService#downLoadStoTemplate(java.lang.String)
     */
    @Override
	public boolean downLoadStoTemplate(String path) {
		// 输入输出流
		InputStream is = null;
		FileOutputStream out = null;
		try {
			is = new FileInputStream(path);
			// 创建工作空间
			Workbook wb = WorkbookFactory.create(is);
			Sheet sheet = wb.getSheetAt(0);
			String mode = FmsPropertyPlaceholder.getProperty("choiceUsbIpc");
			if (StringUtils.isEmpty(mode)) {
				throw new ExpectedException("", "未获取到通道方式");
			}
			// 设备类型 下拉框内容
			if("ipc".equals(mode)){
				String[] arrayDevType = {"CVR", "云存储"};
				sheet = setHSSFValidation(sheet, arrayDevType, 1, 2000, 2, 2);
			}else if("usb".equals(mode)){
				String[] arrayDevType = {"KMS", "云存储-对象存储"};
				sheet = setHSSFValidation(sheet, arrayDevType, 1, 2000, 2, 2);
			}else{
				String[] arrayDevType = {"CVR", "云存储", "KMS", "云存储-对象存储"};
				sheet = setHSSFValidation(sheet, arrayDevType, 1, 2000, 2, 2);
			}
			// 创建文件（输出到目标地址）
			String rootpath = ServletActionContext.getServletContext().getRealPath("/");
			String path1 = Constants.FILEUPLOAD + "excel/" + "storageDeviceTemplate.xls";
			File saveFile = new File(rootpath+Constants.FILEUPLOAD + "excel/");
			if(!saveFile.exists()) {
				saveFile.mkdirs();
			}
			out = new FileOutputStream(rootpath + path1);
			// 写出文件
			wb.write(out);
			out.flush();
		} catch (Exception e) {
			logger.error("",e);
			throw new ExpectedException("", "下载模板出错:" + e.getMessage());
		}finally {
			try {
				if (null != is) {
					is.close();
				}
				if (null != out) {
					out.close();
				}
            } catch (IOException e) {
	            // #ignore
            	logger.error("",e);
            }
		}
		return true;
	}
       
    /**
     * 设置下拉
     * @author jinxindong 2016年4月22日 上午11:38:21
     * @param sheet
     * @param textlist
     * @param firstRow
     * @param endRow
     * @param firstCol
     * @param endCol
     * @return
     */
    public Sheet setHSSFValidation(Sheet sheet, String[] textlist, int firstRow, int endRow, int firstCol, int endCol) {
		// 加载下拉列表内容
		DVConstraint constraint = DVConstraint.createExplicitListConstraint(textlist);
		// 设置数据有效性加载在哪个单元格上,四个参数分别是：起始行、终止行、起始列、终止列
		CellRangeAddressList regions = new CellRangeAddressList(firstRow, endRow, firstCol, endCol);
		// 数据有效性对象
		HSSFDataValidation data_validation_list = new HSSFDataValidation(regions, constraint);
		sheet.addValidationData(data_validation_list);
		return sheet;
	}
	

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.device.service.IDeviceService#downLoadCollTemplate(java.lang.String)
     */
    @Override
	public boolean downLoadCollTemplate(String path) {
		// 输入输出流
		InputStream is = null;
		FileOutputStream out = null;
		try {
			is = new FileInputStream(path);
			// 创建工作空间
			Workbook wb = WorkbookFactory.create(is);
			Sheet sheet = wb.getSheetAt(0);
			
			// 设备类型下拉
			String[] arrayDevType = {"DVR","NVR"};
			sheet = setHSSFValidation(sheet, arrayDevType, 1, 2000, 2, 2);
			
			// 设备码流下拉
			String[] arrayDevStream = {"主码流","子码流"};
			sheet = setHSSFValidation(sheet, arrayDevStream, 1, 2000, 7, 7);
			// 创建文件（输出到目标地址）
			String rootpath = ServletActionContext.getServletContext().getRealPath("/");
			String path1 = Constants.FILEUPLOAD + "excel/" + "collectDeviceTemplate.xls";
			File saveFile = new File(rootpath+Constants.FILEUPLOAD + "excel/");
			if(!saveFile.exists()) {
				saveFile.mkdirs();
			}
			out = new FileOutputStream(rootpath + path1);
			// 写出文件
			wb.write(out);
			out.flush();
		} catch (Exception e) {
			logger.error("",e);
			throw new ExpectedException("", "下载模板出错:" + e.getMessage());
		}finally {
			try {
				if (null != is) {
					is.close();
				}
				if (null != out) {
					out.close();
				}
            } catch (IOException e) {
	            // #ignore
            	logger.error("",e);
            }
		}
		return true;
	}

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.device.service.IDeviceService#addCollDevFromExcel(java.io.File, java.lang.String, com.hikvision.finance.fms.common.excel.model.ExcelHead)
     */
    @Override
    public Map<String,Object> addCollDevFromExcel(File devFile, String devFileFileName, ExcelHead excelHead) {
    	Map<String,Object> prosMap = Maps.newHashMap();
    	prosMap.put("errorNo", true);
    	List<DeviceInfo> pros = Lists.newArrayList();
 	    ExcelHelper<DeviceInfo> excelHelper = ExcelHelperFactory.createExcelHelper(devFileFileName);
 	    pros = excelHelper.importToObjectList(excelHead, devFile, DeviceInfo.class);
 	    prosMap.put("prosList", pros);
     	return prosMap;
    }
    
	/* 
	 * 验证导入的存储设备 TODO
     * @see com.hikvision.finance.fms.modules.device.service.IDeviceService#addAndValidateStoExcel(java.io.File, java.lang.String, com.hikvision.finance.fms.common.excel.model.ExcelHead)
     */
    @Override
	public AjaxData addAndValidateStoExcel(File devFile, String devFileFileName, ExcelHead excelHead) {
		AjaxData ajaxData = new AjaxData(false);
		Map<String, Object> prosMap = Maps.newHashMap();
		List<DeviceInfo> pros = Lists.newArrayList();
		try {
			ExcelHelper<DeviceInfo> excelHelper = ExcelHelperFactory.createExcelHelper(devFileFileName);
			pros = excelHelper.importToObjectList(excelHead, devFile, DeviceInfo.class);
			List<Object> mapNameList = Lists.newArrayList();// 存放表格内设备名称
			List<Object> mapCodeList = Lists.newArrayList();// 存放表格内设备编码
			List<Object> mapIpPortList = Lists.newArrayList();// 存放表格内ip+端口
			if (pros == null || pros.size() < 1) {
				ajaxData.setFailureMsg("导入的表格内关键数据缺失或无数据");
				return ajaxData;
			}
			for (DeviceInfo info : pros) {
				if(info.getNtype()==0 || info.getNtype()==null){
					ajaxData.setFailureMsg("导入文件的内容格式有误,请检查文件内容是否合规");
					return ajaxData;
				}
				String strChoiceUsbIpc = FmsPropertyPlaceholder.getProperty("choiceUsbIpc");
				if (StringUtils.isEmpty(strChoiceUsbIpc)) {
					throw new ExpectedException("", "未获取到通道方式");
				}
				if ("usb".equals(strChoiceUsbIpc)) { //KMS, 对象存储
					if (info.getNtype() == Constants.DeviceType.DEV_TYPE_CLOUD_STORAGE_OBJECT_STORAGE || 
							info.getNtype() == Constants.DeviceType.DEV_TYPE_KMS) {
					}else {
						ajaxData.setFailureMsg("USB模式下只能导入KMS或者云存储-对象存储");
						return ajaxData;
					}
				}else if("ipc".equals(strChoiceUsbIpc)){
					if (info.getNtype() == Constants.DeviceType.DEV_TYPE_CVR || 
							info.getNtype() == Constants.DeviceType.DEV_TYPE_CLOUD_STORAGE) {
					}else {
						ajaxData.setFailureMsg("IPC模式下只能导入CVR或者云存储");
						return ajaxData;
					}
				}
				// 判断必填项是否为空
				if (StringUtils.isBlank(info.getRegionName())) {
					ajaxData.setFailureMsg("所在区域列数据不全");
					return ajaxData;
				}
				if (StringUtils.isBlank(info.getStrName())) {
					ajaxData.setFailureMsg("设备名称列数据不全");
					return ajaxData;
				}
				if (StringUtils.isBlank(info.getStrIp())) {
					ajaxData.setFailureMsg("设备IP列数据不全");
					return ajaxData;
				}
				if (info.getNport() == null || info.getNport() == 0) {
					ajaxData.setFailureMsg("设备端口列数据不全或为0");
					return ajaxData;
				}
				if (StringUtils.isBlank(info.getStrIndexCode())) {
					ajaxData.setFailureMsg("设备编码列数据不全");
					return ajaxData;
				}
				if (StringUtils.isBlank(info.getStrManufacture())) {
					ajaxData.setFailureMsg("设备厂商列数据不全");
					return ajaxData;
				}
				if (StringUtils.isBlank(info.getStrUser())) {
					ajaxData.setFailureMsg("设备用户名列数据不全");
					return ajaxData;
				}
				// 非对象存储
				if (!Integer.valueOf(Constants.DeviceType.DEV_TYPE_CLOUD_STORAGE_OBJECT_STORAGE).equals(info.getNtype())) {
					if (StringUtils.isBlank(info.getStrPassword())) {
						ajaxData.setFailureMsg("设备密码列数据不全");
						return ajaxData;
					}
				} else {// 对象存储
					if (StringUtils.isBlank(info.getAccessKey())) {
						ajaxData.setFailureMsg("访问Key列数据不全");
						return ajaxData;
					}
					if (StringUtils.isBlank(info.getSecretKey())) {
						ajaxData.setFailureMsg("加密Key列数据不全");
						return ajaxData;
					}
				}
				
				// 校验表格内设备名称和设备编码 设备ip+端口是否重复
				if (mapNameList.contains(info.getStrName())) {
					ajaxData.setFailureMsg("设备名称列表中存在重复数据：" + info.getStrName());
					return ajaxData;
				}
				
				if (mapCodeList.contains(info.getStrIndexCode())) {
					ajaxData.setFailureMsg("设备编码列表中存在重复数据：" + info.getStrIndexCode());
					return ajaxData;
				}
				
				if (mapIpPortList.contains(info.getStrIp() + info.getNport())) {
					ajaxData.setFailureMsg("设备IP和端口组合存在重复数据：" + info.getStrIp() + " : " + info.getNport());
					return ajaxData;
				}
				mapNameList.add(info.getStrName());
				mapCodeList.add(info.getStrIndexCode());
				mapIpPortList.add(info.getStrIp() + info.getNport());
			}
		} catch (Exception e) {
			ajaxData.setFailureMsg("导入文件的内容格式有误,请检查文件内容是否合规");
			return ajaxData;
		}
		prosMap.put("prosList", pros);
		ajaxData.setData(prosMap);
		return ajaxData;
	}

	/* 
	 * TODO
     * @see com.hikvision.finance.fms.modules.device.service.IDeviceService#addAndValidateCollExcel(java.io.File, java.lang.String, com.hikvision.finance.fms.common.excel.model.ExcelHead)
     */
    @Override
	public AjaxData addAndValidateCollExcel(File devFile, String devFileFileName, ExcelHead excelHead) {
		AjaxData ajaxData = new AjaxData(false);
		Map<String, Object> prosMap = Maps.newHashMap();
		List<DeviceInfo> pros = Lists.newArrayList();
		try {
			ExcelHelper<DeviceInfo> excelHelper = ExcelHelperFactory.createExcelHelper(devFileFileName);
			pros = excelHelper.importToObjectList(excelHead, devFile, DeviceInfo.class);
			List<Object> mapNameList = Lists.newArrayList();// 存放表格内设备名称
			List<Object> mapCodeList = Lists.newArrayList();// 存放表格内设备编码
			List<Object> mapIpPortList = Lists.newArrayList();// 存放表格内ip+端口
			if (pros == null || pros.size() < 1) {
				ajaxData.setFailureMsg("导入的表格内关键数据缺失或无数据");
				return ajaxData;
			}
			for (DeviceInfo info : pros) {
				if (StringUtils.isBlank(info.getRegionName())) {
					ajaxData.setFailureMsg("所在区域列数据不全");
					return ajaxData;
				}
				if (StringUtils.isBlank(info.getStrName())) {
					ajaxData.setFailureMsg("设备名称列数据不全");
					return ajaxData;
				}
				if (StringUtils.isBlank(info.getStrIp())) {
					ajaxData.setFailureMsg("设备IP列数据不全");
					return ajaxData;
				}
				if (info.getNport() == null) {
					ajaxData.setFailureMsg("设备端口列数据不全");
					return ajaxData;
				}
				if (StringUtils.isBlank(info.getStrIndexCode())) {
					ajaxData.setFailureMsg("设备编码列数据不全");
					return ajaxData;
				}
				if (StringUtils.isBlank(info.getStrManufacture())) {
					ajaxData.setFailureMsg("设备厂商列数据不全");
					return ajaxData;
				}
				if (info.getNstreamType() == null) {
					ajaxData.setFailureMsg("码流类型列数据不全");
					return ajaxData;
				}
				if (StringUtils.isBlank(info.getStrUser())) {
					ajaxData.setFailureMsg("设备用户名列数据不全");
					return ajaxData;
				}
				if (StringUtils.isBlank(info.getStrPassword())) {
					ajaxData.setFailureMsg("设备密码列数据不全");
					return ajaxData;
				}
				
				// 校验表格内设备名称和设备编码 设备ip+端口是否重复
				if (mapNameList.contains(info.getStrName())) {
					ajaxData.setFailureMsg("设备名称列表中存在重复数据：" + info.getStrName());
					return ajaxData;
				}
				
				if (mapCodeList.contains(info.getStrIndexCode())) {
					ajaxData.setFailureMsg("设备编码列表中存在重复数据：" + info.getStrIndexCode());
					return ajaxData;
				}
				
				if (mapIpPortList.contains(info.getStrIp() + info.getNport())) {
					ajaxData.setFailureMsg("设备IP和端口组合存在重复数据：" + info.getStrIp() + " : " + info.getNport());
					return ajaxData;
				}
				mapNameList.add(info.getStrName());
				mapCodeList.add(info.getStrIndexCode());
				mapIpPortList.add(info.getStrIp() + info.getNport());
			}
		} catch (Exception e) {
			ajaxData.setFailureMsg("导入文件的内容格式有误,请检查文件内容是否合规");
			return ajaxData;
		}
		prosMap.put("prosList", pros);
		ajaxData.setData(prosMap);
		return ajaxData;
	}
	/* 
	 * 导入时存储设备信息（所有设备导入都使用该方法） TODO
     * @see com.hikvision.finance.fms.modules.device.service.IDeviceService#addAllStoDevs(java.util.List)
     */
    @Override
    public AjaxData addAllDevs(List<DeviceInfo> deviceInfos) {
    	AjaxData ajaxData = new AjaxData(false);
    	StringBuilder builderStr = new StringBuilder();
    	//查询区域 组装map<区域名,区域> 用于和区域关系的保存
    	Map<String, Region> regMap = new HashMap<String, Region>();
    	List<Region> regions = regionDao.getAll();
    	for(Region region: regions){
    		regMap.put(region.getStrName(), region);
    	}
    	  	
    	//设备信息组装map<ip，设备> 用户和数据库对比IP重复
    	Map<String, Device> strIpMap = Maps.newHashMap();
    	Map<String, Device> nameMap = Maps.newHashMap();
    	Map<String, Device> codeMap = Maps.newHashMap();
    	
    	List<Device> devices = deviceDao.getAll();
    	for(Device device : devices){
    		strIpMap.put(device.getStrIp()+device.getNport(), device);
    		nameMap.put(device.getStrName(), device);
    		codeMap.put(device.getStrIndexCode(), device);
    	}
    	Map<String, Object> data = Maps.newHashMap();
        int scucessNum = 0;
        int faileNum = 0;
    	//数据有效性校验
    	for(DeviceInfo  deviceInfo:deviceInfos){
    		if(!validateCoulm(deviceInfo)){
    			ajaxData.setFailureMsg("文件内容含非法数据或有特殊字符\\/:*?\"<|'%>&");
    			data.put("scucessNum", scucessNum);
    			ajaxData.setData(data);
    			return ajaxData;
    		}	
    	}
    	for(DeviceInfo  deviceInfo:deviceInfos){
    		Device dev = nameMap.get(deviceInfo.getStrName());
    		if(dev != null){
    			faileNum++;
    			builderStr.append(deviceInfo.getStrName()+",设备名称:"+deviceInfo.getStrName()+" 已存在&");
    			continue;
    		}
    		Device dev1 = codeMap.get(deviceInfo.getStrIndexCode());
    		if(dev1 != null){
    			faileNum++;
    			builderStr.append(deviceInfo.getStrName()+",设备编码:"+deviceInfo.getStrIndexCode()+" 已存在&");
    			continue;
    		}
    		
    		//判断设备名称是否和数据库重复
    		Device dev2 = strIpMap.get(deviceInfo.getStrIp()+deviceInfo.getNport());
    		if(dev2 != null){
    			faileNum++;
    			builderStr.append(deviceInfo.getStrName()+",设备IP、端口组合:"+deviceInfo.getStrIp()+" : "+deviceInfo.getNport()+" 已存在&");
    			continue;
    		}
    		Region region = regMap.get(deviceInfo.getRegionName());
    		if(region == null){
    			faileNum++;
    			builderStr.append(deviceInfo.getStrName()+",所属区域:"+deviceInfo.getRegionName()+" 不存在&");
    			continue;
    		}

    	    //添加设备
    		Device device = new Device();
    		device.setNregionId(regMap.get(deviceInfo.getRegionName()).getId());// 区域id    		
    		device.setNtype(deviceInfo.getNtype());// 设备类型
    		device.setStrName(deviceInfo.getStrName());// 设备名称
    		device.setStrIndexCode(deviceInfo.getStrIndexCode());// 设备编码
    		device.setStrManufacture(deviceInfo.getStrManufacture());// 设备厂商
    		device.setStrIp(deviceInfo.getStrIp());// 设备ip
    		device.setNport(deviceInfo.getNport());// 设备端口
    		device.setOnline(Constants.CONS_0);
			// 非对象存储
    		device.setStrUser(deviceInfo.getStrUser());// 设备用户名
			if (!Integer.valueOf(Constants.DeviceType.DEV_TYPE_CLOUD_STORAGE_OBJECT_STORAGE).equals(deviceInfo.getNtype())) {
				device.setStrPassword(SercurityUtil.encrypt(deviceInfo.getStrPassword()));// 设备密码
	    		//对密码进行登记判断？TODO  
	    		int a = ValidateUtil.getPwdRank(deviceInfo.getStrPassword(), device.getStrUser());
	    		device.setStrPasswordLevel(String.valueOf(a));// 设备密码等级
			} else {// 对象存储
				device.setStrPassword("");
				device.setStrPasswordLevel(String.valueOf(Constants.PasswordSecurityLevel.NO_PWD));
				device.setStrAccessKey(deviceInfo.getAccessKey());
				device.setStrSecretKey(deviceInfo.getSecretKey());
			}
    		
    		// DVR和NVR要设置码流类型
			if (Integer.valueOf(Constants.DeviceType.DEV_TYPE_DVR).equals(deviceInfo.getNtype())
			        || Integer.valueOf(Constants.DeviceType.DEV_TYPE_NVR).equals(deviceInfo.getNtype())) {
				device.setNstreamType(deviceInfo.getNstreamType());// nStreamType设备码流
			}		
    		deviceDao.save(device);
    		scucessNum++;
    	}
    	if(faileNum>0){
    		ajaxData.setSuccess(true);
    		ajaxData.setMsg("导入失败设备：失败个数( "+faileNum+" )个。&"+builderStr.toString());
    	}
    	data.put("scucessNum", scucessNum);
    	ajaxData.setData(data);
		return ajaxData;
    }

    /**
     * 校验导入的数据
     * @author jinxindong 2016年6月15日 下午5:02:37
     * @param deviceInfo
     * @return
     */
	private Boolean validateCoulm(DeviceInfo deviceInfo) {

		if(ValidateUtil.validateChar(deviceInfo.getRegionName())){
			return false;
		}
		if(ValidateUtil.validateChar(deviceInfo.getStrName())){
			return false;
		}
		if(ValidateUtil.validateChar(deviceInfo.getStrIndexCode())){
			return false;
		}
		if(ValidateUtil.validateChar(deviceInfo.getStrManufacture())){
			return false;
		}
		// 非对象存储
		if (!Integer.valueOf(Constants.DeviceType.DEV_TYPE_CLOUD_STORAGE_OBJECT_STORAGE).equals(deviceInfo.getNtype())) {
			if (ValidateUtil.validateChar(deviceInfo.getStrUser())) {
				return false;
			}
			if (ValidateUtil.validateChar(deviceInfo.getStrPassword())) {
				return false;
			}
			if (ValidateUtil.isZHCase(deviceInfo.getStrPassword())) {
				return false;
			}
		}
		if(!ValidateUtil.validateIp(deviceInfo.getStrIp())){
			return false;
		}

		return true;
	}
	
	
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.device.service.IDeviceService#getChannelsDataPage(int, int, com.hikvision.finance.fms.modules.device.dto.DeviceChannelInfo)
     */
    @Override
    public Page getChannelsDataPage(int limit, int start, DeviceChannelInfo deviceChannelInfo) {
    	return deviceChannelDao.getChannelsDataPage(limit, start, deviceChannelInfo);
    }
	
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.device.service.IDeviceService#updateChannel(com.hikvision.finance.fms.modules.device.dto.DeviceChannelInfo)
     */
    @Override
    public AjaxData updateChannel(DeviceChannelInfo deviceChannelInfo) {
    	AjaxData ajaxData = new AjaxData(false);
    	Devicechannel devicechannel =  deviceChannelDao.get(deviceChannelInfo.getId());

    	devicechannel.setUserIp(deviceChannelInfo.getUserIp());
    	devicechannel.setNchannelName(deviceChannelInfo.getNchannelName());
    	deviceChannelDao.update(devicechannel);

    	configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.DEVICE.getCode(),GlobalMessageUtil.getMessage("log.devchannel.update",new String[] {deviceChannelInfo.getNchannelName()}));
    	return ajaxData;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.device.service.IDeviceService#deleteChannel(java.util.List)
     */
    @Override
	public AjaxData deleteChannel(List<Integer> ids) {
		AjaxData ajaxData = new AjaxData(false);
		List<String> names = new ArrayList<String>();
		if (ids != null && ids.size() > 0) {
			for (Integer id : ids) {
				names.add(deviceChannelDao.get(id).getNchannelName());
				// 删除用户通道的关联关系
				userdevicemappingDao.deleteBy("devChannel", id);
				// 删除通道
				deviceChannelDao.deleteBy("id", id);
			}
			if (ids.size() > 5) {
				configLogDao.setLogContent(OperStatus.DELETE, LogModuledEnum.DEVICE.getCode(),
				        GlobalMessageUtil.getMessage("log.devchannel.delete.batch"));
			} else {
				configLogDao.setLogContent(OperStatus.DELETE, LogModuledEnum.DEVICE.getCode(),
				        GlobalMessageUtil.getMessage("log.devchannel.delete", new String[] {ListUtil.listToStr(names)}));
			}
		}
		return ajaxData;
	}

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.device.service.IDeviceService#getChannelInfoById(java.lang.Integer)
     */
    @Override
	public DeviceChannelInfo getChannelInfoById(Integer id) {
		
		DeviceChannelInfo deviceChannelInfo = new DeviceChannelInfo();
		Devicechannel devicechannel = deviceChannelDao.get(id);
		Device device = deviceDao.get(devicechannel.getNdeviceId());
		Region region = null;
		if (null != device) {
			region = regionDao.get(device.getNregionId());
		}
		try {
			BeanUtils.copyNotNullProperties(deviceChannelInfo, devicechannel);
		} catch (Exception e) {
			throw new ExpectedException("", "拷贝bean失败[getChannelInfoById]");
		}
		deviceChannelInfo.setNdeviceName(device == null?null:device.getStrName());
		deviceChannelInfo.setRegionName(region == null?null:region.getStrName());
		return deviceChannelInfo;
	}
    
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.device.service.IDeviceService#getDeviceInfo(java.lang.Integer)
     */
    @Override
    public DeviceInfo getDeviceInfoById(Integer id) {
    	DeviceInfo deviceInfo = new DeviceInfo();
		Device device = deviceDao.get(id);
		try {
			BeanUtils.copyNotNullProperties(deviceInfo, device);
		} catch (Exception e) {
			throw new ExpectedException("", "拷贝bean失败[getStoDevById]");
		}
	    return deviceInfo;
    }
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.device.service.IDeviceService#updateDeviceNtp(java.lang.Integer)
     */
    @Override
    public void updateDeviceNtp(Integer id) {
    	Device device = deviceDao.get(id);
    	if(device.getNtpEnable() == null || device.getNtpEnable() != Constants.SysDictionary.enable){
    		device.setNtpEnable(Constants.SysDictionary.enable);
    		deviceDao.update(device);
    		configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.DEVICE.getCode(),GlobalMessageUtil.getMessage("log.ntp.timing", new String[] {device.getStrName()}));
    	}
    }
    
    /* (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.device.service.IDeviceService#getDevChannelInfo(java.lang.Integer)
	 */
	@Override
	public RecordPojo getDevChannelInfo(Integer channelId) {
		RecordPojo recordPojo = new RecordPojo();
	    Devicechannel devicechannel = deviceChannelDao.get(channelId);
	    recordPojo.setChannelId(channelId);
	    recordPojo.setChannel(devicechannel.getNchannelNo());
	    recordPojo.setCamName(devicechannel.getNchannelName());
	    recordPojo.setRegionId(devicechannel.getNregionId());
	    recordPojo.setDevType(Constants.CONS_0);
	    Device device =  deviceDao.get(devicechannel.getNdeviceId());
		recordPojo.setDevIp(device.getStrIp());
		recordPojo.setDevPort(device.getNport());
		recordPojo.setUser(device.getStrUser());
		String pass = SercurityUtil.decrypt(device.getStrPassword());
		recordPojo.setPwd(Base64Utils.getBase64(pass));
		recordPojo.setRealPlayType(device.getNstreamType());
		return recordPojo;
	}
    
	// ==========================getter/setter====================================================
	
	public IDeviceDao getDeviceDao() {
		return deviceDao;
	}
	
	public void setDeviceDao(IDeviceDao deviceDao) {
		this.deviceDao = deviceDao;
	}
	
	public IRegionDao getRegionDao() {
		return regionDao;
	}
	
	public void setRegionDao(IRegionDao regionDao) {
		this.regionDao = regionDao;
	}
	
	public IDeviceChannelDao getDeviceChannelDao() {
		return deviceChannelDao;
	}
	
	public void setDeviceChannelDao(IDeviceChannelDao deviceChannelDao) {
		this.deviceChannelDao = deviceChannelDao;
	}
	
	public ICloudpoolmappingDao getCloudpoolmappingDao() {
		return cloudpoolmappingDao;
	}
	
	public void setCloudpoolmappingDao(ICloudpoolmappingDao cloudpoolmappingDao) {
		this.cloudpoolmappingDao = cloudpoolmappingDao;
	}
	
	public ICvrassociatemappingDao getCvrassociatemappingDao() {
		return cvrassociatemappingDao;
	}
	
	public void setCvrassociatemappingDao(ICvrassociatemappingDao cvrassociatemappingDao) {
		this.cvrassociatemappingDao = cvrassociatemappingDao;
	}
	
	public IUserdevicemappingDao getUserdevicemappingDao() {
		return userdevicemappingDao;
	}
	
	public void setUserdevicemappingDao(IUserdevicemappingDao userdevicemappingDao) {
		this.userdevicemappingDao = userdevicemappingDao;
	}
	
	public ISoftdogDao getSoftdogDao() {
		return softdogDao;
	}
	
	public void setSoftdogDao(ISoftdogDao softdogDao) {
		this.softdogDao = softdogDao;
	}
	
	public IConfigLogDao getConfigLogDao() {
		return configLogDao;
	}
	
	public void setConfigLogDao(IConfigLogDao configLogDao) {
		this.configLogDao = configLogDao;
	}
	
	public DataManager getDataManager() {
		return dataManager;
	}
	
	public void setDataManager(DataManager dataManager) {
		this.dataManager = dataManager;
	}
	
	public IRecordsDao getRecordsDao() {
		return recordsDao;
	}
	
	public void setRecordsDao(IRecordsDao recordsDao) {
		this.recordsDao = recordsDao;
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
