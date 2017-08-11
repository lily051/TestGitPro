/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月5日 下午2:41:59
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.device.action;

import java.io.File;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.struts2.ServletActionContext;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.google.common.collect.Lists;
import com.google.common.collect.Maps;
import com.hikvision.finance.core.util.JsonUtils;
import com.hikvision.finance.core.util.SercurityUtil;
import com.hikvision.finance.fms.common.enums.LogModuledEnum;
import com.hikvision.finance.fms.common.excel.model.ExcelColumn;
import com.hikvision.finance.fms.common.excel.model.ExcelHead;
import com.hikvision.finance.fms.common.util.Base64Utils;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.common.util.Constants.OperStatus;
import com.hikvision.finance.fms.common.util.http.HttpPostUtil;
import com.hikvision.finance.fms.model.Device;
import com.hikvision.finance.fms.model.Region;
import com.hikvision.finance.fms.modules.device.dto.CloudPoolInfo;
import com.hikvision.finance.fms.modules.device.dto.DeviceChannelInfo;
import com.hikvision.finance.fms.modules.device.dto.DeviceInfo;
import com.hikvision.finance.fms.modules.device.dto.KmsInfo;
import com.hikvision.finance.fms.modules.device.service.IDeviceService;
import com.hikvision.finance.fms.modules.log.dao.IConfigLogDao;
import com.hikvision.finance.fms.modules.region.service.IRegionService;
import com.hikvision.finance.fms.modules.sysconfig.dto.NtpParamInfo;
import com.hikvision.finance.fms.modules.sysconfig.service.ISysconfigService;
import com.hikvision.finance.fms.modules.user.model.RecordPojo;
import com.hikvision.finance.fwork.exception.ExpectedException;
import com.hikvision.finance.fwork.util.GlobalMessageUtil;
import com.hikvision.finance.fwork.www.action.BaseAction;
import com.hikvision.ivms.hikcstor.service.Bucket;
import com.hikvision.ivms.hikcstor.service.JBucketService;

/**
 * <p>资源管理Action</p>
 * @author jinxindong 2016年4月5日 下午2:41:59
 * @version V1.0 
 */
public class DeviceAction extends BaseAction<Device>{

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = -2331281388861027706L;
    private static final Logger logger = LoggerFactory.getLogger(DeviceAction.class);
    private String passwordSecurityLevel;
    private IDeviceService deviceService;
    private IRegionService regionService;
    private IConfigLogDao configLogDao;
    private List<Integer> ids;
    private List<DeviceInfo> deviceList;
    private List<DeviceChannelInfo> deviceChannelList;
    private DeviceInfo deviceInfo;
    private DeviceChannelInfo deviceChannelInfo;
    private List<CloudPoolInfo> cloudpoolInfoList;
    private ISysconfigService sysconfigService;
    private static String KmsToken = "/kms/services/rest/operationService/getTokenInfo"; 
    private static String KmsSpace = "/kms/services/rest/CRL/getSpaceInfo"; 
	private File devFile;// 导入设备的文件
	private String devFileFileName;// 导入设备的文件名
	private Map<String,Object> resMap; 
	private NtpParamInfo ntpParamInfo;
	private RecordPojo recordPojo;
	
	
    public Map<String, Object> getResMap() {
    	return resMap;
    }

	
    public void setResMap(Map<String, Object> resMap) {
    	this.resMap = resMap;
    }
    /**
     * 跳到存储设备页面
     * @author jinxindong 2016年4月12日 下午3:52:54
     * @return
     */
	public String toStoPage() {
		operPage = "/modules/device/storage/storageDevice.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 跳到采集设备页面
	 * @author jinxindong 2016年4月12日 下午3:52:57
	 * @return
	 */
	public String toCollPage() {
		operPage = "/modules/device/collect/collectDevice.jsp";
		return DISPATCHER;
	}
	
	
    /**
     * 进入存储设备数据获取页面
     * @author jinxindong 2016年4月5日 下午3:22:34
     * @return
     */
    public String getStoDataPage(){
    	page = deviceService.getStoDataPage(limit, start, deviceInfo);
    	return PAGE;
    }
    
    /**
     * 跳到存储设备添加页面
     * @author jinxindong 2016年4月5日 下午3:22:39
     * @return
     */
	public String toAddStoPage() {
		if (deviceInfo == null || deviceInfo.getNregionId() == null) {
			throw new ExpectedException("", "未获取到区域的ID");
		}
		Region region = regionService.get(deviceInfo.getNregionId());
		deviceInfo.setRegionName(region.getStrName());
		passwordSecurityLevel = sysconfigService.getSysconfig().getPasswordSecruityLevel();
		ntpParamInfo = sysconfigService.getNtpParam();
		operPage = "/modules/device/storage/dialogs/addStorageDevice.jsp";
		return DISPATCHER;
	}
	
    /**
     * 添加存储设备
     * @author jinxindong 2016年4月5日 下午3:23:17
     * @return
     */
	public String addSto() {
		ajaxData = deviceService.addSto(deviceInfo, jsonStr);
		return AJAX;
	}
	
    /**
     * 跳到存储设备修改页面
     * @author jinxindong 2016年4月5日 下午3:23:38
     * @return
     */
	public String toUpdateStoPage() {
		if (deviceInfo == null || deviceInfo.getId() == null) {
			throw new ExpectedException("", "未获取到该存储设备的ID");
		}
		passwordSecurityLevel = sysconfigService.getSysconfig().getPasswordSecruityLevel();
		deviceInfo = deviceService.getStoDevById(deviceInfo.getId());
		ntpParamInfo = sysconfigService.getNtpParam();
		operPage = "/modules/device/storage/dialogs/editStorageDevice.jsp";
		return DISPATCHER;
	}
	
    /**
     * 修改存储设备
     * @author jinxindong 2016年4月5日 下午3:24:06
     * @return
     */
	public String updateSto() {
		ajaxData = deviceService.updateSto(deviceInfo, jsonStr);
		return AJAX;
	}
	
    /**
     * 删除存储设备
     * @author jinxindong 2016年4月5日 下午3:24:50
     * @return
     */
	public String deleteStos() {
		ajaxData = deviceService.deleteStos(ids);
		return AJAX;
	}
    
	/**
	 * KMS分券信息获取
	 * @author jinxindong 2016年4月8日 下午6:40:54
	 * @return
	 */
	public String getKmsPoolsInfo(){
		if (deviceInfo == null || deviceInfo.getStrIp() == null) {
			throw new ExpectedException("", "未获取到该设备ip");
		}else if(deviceInfo.getNport() == null){
			throw new ExpectedException("", "未获取到该设备端口");
		}else if(deviceInfo.getStrUser() == null){
			throw new ExpectedException("", "未获取到该设备用户名");
		}else if(deviceInfo.getStrPassword() == null){
			throw new ExpectedException("", "未获取到该设备密码");
		}
		String strIp = deviceInfo.getStrIp();
		Integer nport = deviceInfo.getNport();
		String userName = deviceInfo.getStrUser();
		String password = Base64Utils.getFromBase64(deviceInfo.getStrPassword());
		
		//String s1 = HttpPostUtil.sendGet("http://10.10.48.101:8080/kms/services/rest/operationService/getTokenInfo", "username=admin&password=hik12345_");
		//String s1 = HttpPostUtil.sendGet("http://10.10.48.101:8080/kms/services/rest/CRL/getSpaceInfo", "token=7a57a5a70000015360ecfe43f56d21d4d9");
		
		String tokenString = HttpPostUtil.sendGet(" http://"+strIp+":"+nport+KmsToken, "username="+userName+"&password="+password+" ");
		KmsInfo tokenInfo = JsonUtils.json2Object(tokenString, KmsInfo.class);
		
		if(tokenInfo.getRet()==Constants.CONS_0){//等于0的时候是操作成功
			String spaceString = HttpPostUtil.sendGet(" http://"+strIp+":"+nport+KmsSpace, "token="+tokenInfo.getToken());
			KmsInfo spaceInfo = JsonUtils.json2Object(tokenString, KmsInfo.class);
			if(spaceInfo.getRet()!=Constants.CONS_0){
				throw new ExpectedException("", "获取kms的磁盘信息失败["+tokenInfo.getMsg()+"]");
			}
			ajaxData.put("kmsPool",spaceString);
		}else{
			throw new ExpectedException("", "获取kms的Token值失败["+tokenInfo.getMsg()+"]");
		}
		return AJAX;
	}
	
	/**
	 * 获取云存储-对象存储的容量
	 * @author zhengquanwu 2016年10月10日 下午8:23:12
	 * @return
	 */
	public String getCloudStorageObjectStorage() {
		if (null == deviceInfo || null == deviceInfo.getStrIp()) {
			throw new ExpectedException("", "未获取到该设备ip");
		}else if(null == deviceInfo.getNport()){
			throw new ExpectedException("", "未获取到该设备端口");
		}else if(null == deviceInfo.getAccessKey()){
			throw new ExpectedException("", "未获取到该设备用户名");
		}else if(null == deviceInfo.getSecretKey()){
			throw new ExpectedException("", "未获取到该设备密码");
		}
		
		String strIp = deviceInfo.getStrIp();
		Integer nport = deviceInfo.getNport();
		String accessKey = Base64Utils.getFromBase64(deviceInfo.getAccessKey());
		String secretKey = Base64Utils.getFromBase64(deviceInfo.getSecretKey());
		JBucketService jBucketService = new JBucketService("http://"+strIp+":"+nport,accessKey,secretKey);
		try {
			List<Bucket> buckets = jBucketService.listBuckets();
			if(buckets.size()>0){
				Bucket bucket = buckets.get(0);
				CloudPoolInfo cloudPoolInfo = new CloudPoolInfo();
				int freeSize = new Double(bucket.getFreeSize()).intValue();
				int size = new Double(bucket.getSize()).intValue();
				cloudPoolInfo.setPoolFreeSize(freeSize);
				cloudPoolInfo.setPoolSize(size);
				cloudPoolInfo.setPoolName(bucket.getName());
				ajaxData.put("CloudPoolInfo",cloudPoolInfo);
			}else{
				throw new ExpectedException("", "Bucket信息为空");
			}
			
		} catch (Exception e) {
			throw new ExpectedException("", "获取Bucket失败");
		}
		return AJAX;
	}
	
	
	/**
	 * 中心存储导出表格
	 * @author jinxindong 2016年4月12日 下午6:48:31
	 * @return
	 */
	public String exportStoDevExcel() {
		String[] titleNames = new String[] {"设备名称", "设备类型",  "设备IP", "设备端口", "密码等级", "总容量(G)","可用容量(G)","在线状态","所属区域"};
		String[] attrNames = new String[] {"strName", "ntypeName",  "strIp", "nport", "strPasswordLevelName", "uiTotalSpace", "uiFreeSpace", "onlineName", "regionName"};
		String rootpath = ServletActionContext.getServletContext().getRealPath("/");
		String path = Constants.FILEUPLOAD + "excel/" + "storageDeviceQuery.xls";
		if(deviceService.exportStoDevExcel(deviceInfo, titleNames, attrNames, rootpath + path)) {
			ajaxData.put("url", "/" + path);
    		success = true;
		} else {
			msg = "导出失败";
    		success = false;
		}
		return AJAX;
	}
	
	/**
	 * 跳转到中心存储下载模板界面
	 * @author jinxindong 2016年4月25日 下午3:58:25
	 * @return
	 */
	public String toCheckOutStoPage() {
		operPage = "/modules/device/storage/dialogs/import.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 下载模板
	 * @author jinxindong 2016年4月25日 上午9:44:38
	 * @return
	 */
	public String downLoadStoTemplate() {
		String rootpath = ServletActionContext.getServletContext().getRealPath("/");
		String path ="modules/device/doc/" + "storageDeviceTemplate.xls";
		String path1 = Constants.FILEUPLOAD + "excel/" + "storageDeviceTemplate.xls";
		if(deviceService.downLoadStoTemplate(rootpath + path)) {
			ajaxData.put("url", "/" + path1);
    		success = true;
		} else {
			msg = "下载失败";
    		success = false;
		}
		return AJAX;
	}
	
	/**
	 * 导入存储设备Excel TODO
	 * @author jinxindong 2016年4月25日 上午9:44:33
	 */
	@SuppressWarnings({"serial", "unchecked"})
	public String stoUploadExcel() {
		resMap = new HashMap<String, Object>();
		if (this.devFile != null) {
			// 1、excel的每行显示名称-数据库名称
			List<ExcelColumn> excelColumns = Lists.newArrayList();
			excelColumns.add(new ExcelColumn(0, "regionName", "所属区域*"));
			excelColumns.add(new ExcelColumn(1, "strName", "设备名称*"));
			excelColumns.add(new ExcelColumn(2, "ntype", "设备类型*"));
			excelColumns.add(new ExcelColumn(3, "strIp", "设备IP*"));
			excelColumns.add(new ExcelColumn(4, "nport", "设备端口*"));
			excelColumns.add(new ExcelColumn(5, "strIndexCode", "设备编码*"));
			excelColumns.add(new ExcelColumn(6, "strManufacture", "设备厂商*"));
			excelColumns.add(new ExcelColumn(7, "strUser", "设备用户名"));
			excelColumns.add(new ExcelColumn(8, "strPassword", "设备密码"));
			excelColumns.add(new ExcelColumn(9, "accessKey", "访问Key(对象存储)"));
			excelColumns.add(new ExcelColumn(10, "secretKey", "加密Key(对象存储)"));
			
			// 2、机构级别中显示的名称-数据库中整数
			Map<String, Integer> devTypeMap = new HashMap<String, Integer>() {
				
				{
					put("CVR", Constants.DeviceType.DEV_TYPE_CVR);
					put("云存储", Constants.DeviceType.DEV_TYPE_CLOUD_STORAGE);
					put("KMS", Constants.DeviceType.DEV_TYPE_KMS);
					put("云存储-对象存储", Constants.DeviceType.DEV_TYPE_CLOUD_STORAGE_OBJECT_STORAGE);
				}
			};
			// 3、excel中显示的信息转换为数据库中的值
			Map<String, Map<?, ?>> excelColumnsConvertMap = Maps.newHashMap();
			excelColumnsConvertMap.put("ntype", devTypeMap);
			// 4、组装excel信息
			ExcelHead excelHead = new ExcelHead();
			excelHead.setRowCount(1);// 模板头部所占行数
			excelHead.setColumnCount(11);
			excelHead.setColumns(excelColumns);// 列的定义
			excelHead.setColumnsConvertMap(excelColumnsConvertMap);// 列的转换
			ajaxData = deviceService.addAndValidateStoExcel(devFile, devFileFileName, excelHead);
			if (ajaxData.getSuccess()) {
				// 批量添加prosList
				List<DeviceInfo> deviceInfos = (List<DeviceInfo>)ajaxData.getData().get("prosList");
				for (DeviceInfo deviceInfo : deviceInfos) {
					if (deviceInfo.getNtype() == Constants.DeviceType.DEV_TYPE_CLOUD_STORAGE_OBJECT_STORAGE) {
						resMap.put("isObjectStorage", "yes");//
						break;
					}
				}
				ajaxData = deviceService.addAllDevs(deviceInfos);
				if ((Integer)ajaxData.getData().get("scucessNum") > 0) {
					configLogDao.setLogContent(OperStatus.IMPORT, LogModuledEnum.DEVICE.getCode(),
					        GlobalMessageUtil.getMessage("log.stodev.import"));
				}
			}
			resMap.put("errorNo", ajaxData.getSuccess());
			resMap.put("errorInfo", ajaxData.getMsg());
		}
		return "upload";
	}
	
	/**
	 * 导入的设备IP有重复，则跳转到该页面展示
	 * @author jinxindong 2016年4月25日 下午12:49:07
	 * @return
	 */
	public String toAlreadyStoImport() {
		operPage = "/modules/device/storage/dialogs/alreadyImport.jsp";
		return DISPATCHER;
	}
	//************************************采集设备相关*************************************************
	
    /**
     * 进入采集设备数据获取页面
     * @author jinxindong 2016年4月6日 上午9:19:38
     * @return
     */
    public String getCollDataPage(){
    	page = deviceService.getCollDataPage(limit, start, deviceInfo);
    	return PAGE;
    }
    
    /**
     * 
     * @author fuqunqing 2016年9月23日 下午5:56:26
     * @return
     */
    public String getDeviceJson(){
    	//手动捕获全部异常
    	try {
    		deviceInfo = deviceService.getDeviceInfoById(id.intValue());
        	deviceInfo.setStrPassword(Base64Utils.getBase64(SercurityUtil.decrypt(deviceInfo.getStrPassword())));
        	ajaxData.put("deviceInfo", deviceInfo);
        } catch (Exception e) {
	        success = false;
	        msg = "获取设备详情失败";
	        logger.error(msg, e);
        }
    	return AJAX;
    }
    
    /**
     * 更新设备NTP校时状态
     * @author fuqunqing 2016年9月23日 下午6:02:26
     * @return
     */
    public String updateDeviceNtp(){
    	//手动捕获全部异常
    	try {
    		deviceService.updateDeviceNtp(id.intValue());
        } catch (Exception e) {
        	success = false;
	        msg = "更新设备NTP校时状态失败";
	        logger.error(msg, e);
        }
    	return AJAX;
    }
    
    /**
     * 跳到采集设备添加页面
     * @author jinxindong 2016年4月6日 上午9:19:46
     * @return
     */
	public String toAddCollPage() {
		if (deviceInfo == null || deviceInfo.getNregionId() == null) {
			throw new ExpectedException("", "未获取到区域的ID");
		}
		passwordSecurityLevel = sysconfigService.getSysconfig().getPasswordSecruityLevel();
		Region region = regionService.get(deviceInfo.getNregionId());
		deviceInfo.setRegionName(region.getStrName());
		ntpParamInfo = sysconfigService.getNtpParam();
		operPage = "/modules/device/collect/dialogs/addCollectDevice.jsp";
		return DISPATCHER;
	}
	
    /**
     * 添加采集设备
     * @author jinxindong 2016年4月6日 上午9:22:02
     * @return
     */
	public String addColl() {
		ajaxData = deviceService.addColl(deviceInfo, jsonStr);
		return AJAX;
	}
	
    /**
     * 跳到采集设备修改页面
     * @author jinxindong 2016年4月6日 上午9:24:30
     * @return
     */
	public String toUpdateCollPage() {
		if (deviceInfo == null || deviceInfo.getId() == null) {
			throw new ExpectedException("", "未获取到该存储设备的ID");
		}
		passwordSecurityLevel = sysconfigService.getSysconfig().getPasswordSecruityLevel();
		deviceInfo = deviceService.getCollDevById(deviceInfo.getId());
		ntpParamInfo = sysconfigService.getNtpParam();
		operPage = "/modules/device/collect/dialogs/editCollectDevice.jsp";
		return DISPATCHER;
	}
	
    /**
     * 修改采集设备（设备和信道的dto）
     * @author jinxindong 2016年4月6日 上午9:25:40
     * @return
     */
	public String updateColl() {
		ajaxData = deviceService.updateColl(deviceInfo, jsonStr);
		return AJAX;
	}
	
    /**
     * 删除采集设备
     * @author jinxindong 2016年4月6日 上午9:27:01
     * @return
     */
	public String deleteColls() {
		ajaxData = deviceService.deleteColls(ids);
		return AJAX;
	}
	
    /**
     * 根据设备的ids获取信道信息表
     * @author jinxindong 2016年4月6日 上午9:29:15
     * @return
     */
	public String getDevChannelByIds(){
		deviceChannelList = deviceService.getDevChannelByIds(ids);
		ajaxData.put("deviceChannelList",deviceChannelList);
		return AJAX;
	}
	
	/**
	 * 通过存储设备id获取采集设备分券信息和已选分券
	 * @author jinxindong 2016年4月6日 下午6:16:30
	 * @return
	 */
	public String getPoolsByDevId(){
		if (deviceInfo == null || deviceInfo.getNdevStoId() == null) {
			throw new ExpectedException("", "未获取到该存储设备的ID");
		}
		cloudpoolInfoList = deviceService.getPoolsByDevId(deviceInfo);//ndevStoId
		ajaxData.put("cloudpoolInfoList",cloudpoolInfoList);
		return AJAX;
	}
	
	/**
	 * 采集设备导出
	 * @author jinxindong 2016年4月12日 下午6:49:16
	 * @return
	 */
	public String exportCollDevExcel() {
		String[] titleNames = new String[] {"设备名称", "设备类型",  "设备IP", "设备端口", "密码等级", "模拟通道数","IP通道数","在线状态","所属区域"};
		String[] attrNames = new String[] {"strName", "ntypeName",  "strIp", "nport", "strPasswordLevelName", "nchanNum", "telChanNum", "onlineName", "regionName"};
		String rootpath = ServletActionContext.getServletContext().getRealPath("/");
		String path = Constants.FILEUPLOAD + "excel/" + "collectDeviceQuery.xls";
		if(deviceService.exportCollDevExcel(deviceInfo, titleNames, attrNames, rootpath + path)) {
			ajaxData.put("url", "/" + path);
    		success = true;
		} else {
			msg = "导出失败";
    		success = false;
		}
		return AJAX;
	}
	
	/**
	 * 跳转到采集设备下载模板界面
	 * @author jinxindong 2016年4月25日 下午3:58:25
	 * @return
	 */
	public String toCheckOutCollPage() {
		operPage = "/modules/device/collect/dialogs/import.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 下载模板
	 * @author jinxindong 2016年4月25日 上午9:44:38
	 * @return
	 */
	public String downLoadCollTemplate() {
		String rootpath = ServletActionContext.getServletContext().getRealPath("/");
		String path ="modules/device/doc/" + "collectDeviceTemplate.xls";
		String path1 = Constants.FILEUPLOAD + "excel/" + "collectDeviceTemplate.xls";
		if(deviceService.downLoadCollTemplate(rootpath + path)) {
			ajaxData.put("url", "/" + path1);
    		success = true;
		} else {
			msg = "下载失败";
    		success = false;
		}
		return AJAX;
	}
	
	/**
	 * 导入采集设备Excel TODO
	 * @author jinxindong 2016年4月25日 上午9:44:33
	 * @return
	 */
	@SuppressWarnings({"serial", "unchecked"})
	public String collUploadExcel() {
		resMap = new HashMap<String, Object>();
		if (this.devFile != null) {
			// 1、excel的每行显示名称-数据库名称
			List<ExcelColumn> excelColumns = Lists.newArrayList();
			excelColumns.add(new ExcelColumn(0, "regionName", "所属区域*"));
			excelColumns.add(new ExcelColumn(1, "strName", "设备名称*"));
			excelColumns.add(new ExcelColumn(2, "ntype", "设备类型*"));
			excelColumns.add(new ExcelColumn(3, "strIp", "设备IP*"));
			excelColumns.add(new ExcelColumn(4, "nport", "设备端口*"));
			excelColumns.add(new ExcelColumn(5, "strIndexCode", "设备编码*"));
			excelColumns.add(new ExcelColumn(6, "strManufacture", "设备厂商*"));
			excelColumns.add(new ExcelColumn(7, "nstreamType", "码流类型*"));
			excelColumns.add(new ExcelColumn(8, "strUser", "设备用户名*"));
			excelColumns.add(new ExcelColumn(9, "strPassword", "设备密码*"));
			
			// 2、机构级别中显示的名称-数据库中整数
			Map<String, Integer> devTypeMap = new HashMap<String, Integer>() {
				
				{
					put("DVR", Constants.DeviceType.DEV_TYPE_DVR);
					put("NVR", Constants.DeviceType.DEV_TYPE_NVR);
				}
			};
			Map<String, Integer> streamTypeMap = new HashMap<String, Integer>() {
				
				{
					put("主码流", Constants.StreamType.STREAMTYPE_MAINSTREAM);
					put("子码流", Constants.StreamType.STREAMTYPE_SUBSTREAM);
				}
			};
			// 3、excel中显示的信息转换为数据库中的值
			Map<String, Map<?, ?>> excelColumnsConvertMap = Maps.newHashMap();
			excelColumnsConvertMap.put("ntype", devTypeMap);
			excelColumnsConvertMap.put("nstreamType", streamTypeMap);
			// 4、组装excel信息
			ExcelHead excelHead = new ExcelHead();
			excelHead.setRowCount(1);// 模板头部所占行数
			excelHead.setColumnCount(11);
			excelHead.setColumns(excelColumns);// 列的定义
			excelHead.setColumnsConvertMap(excelColumnsConvertMap);// 列的转换
			ajaxData = deviceService.addAndValidateCollExcel(devFile, devFileFileName, excelHead);
			if (ajaxData.getSuccess()) {
				// 批量添加prosList
				List<DeviceInfo> deviceInfos = (List<DeviceInfo>)ajaxData.getData().get("prosList");
				ajaxData = deviceService.addAllDevs(deviceInfos);
				if ((Integer)ajaxData.getData().get("scucessNum") > 0) {
					configLogDao.setLogContent(OperStatus.IMPORT, LogModuledEnum.DEVICE.getCode(),
					        GlobalMessageUtil.getMessage("log.colldev.import"));
				}
			}
			resMap.put("errorNo", ajaxData.getSuccess());
			resMap.put("errorInfo", ajaxData.getMsg());
		}
		return "upload";
	}
	
	/**
	 * 调整通道管理页面 
	 * @author fuqunqing 2016年9月23日 上午9:56:47
	 * @return
	 */
	public String toChannelMngPage() {
		operPage = "/modules/device/channelMng/channelMng.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 获取通道分页数据
	 * @author fuqunqing 2016年9月23日 上午9:56:59
	 * @return
	 */
    public String getChannelsDataPage(){
    	page = deviceService.getChannelsDataPage(limit, start, deviceChannelInfo);
    	return PAGE;
    }
    
    /**
     * 加载详细通道信息
     * @author fuqunqing 2016年9月23日 上午10:41:34
     * @return
     */
    public String toUpdateChannel(){
    	deviceChannelInfo = deviceService.getChannelInfoById(id.intValue());
    	operPage = "/modules/device/channelMng/dialogs/editChannel.jsp";
		return DISPATCHER;
    }
    /**
     * 更新通道信息
     * @author fuqunqing 2016年9月23日 上午9:57:07
     * @return
     */
	public String updateChannel() {
		ajaxData = deviceService.updateChannel(deviceChannelInfo);
		return AJAX;
	}
    
	/**
	 * 删除通道，支持批量
	 * @author fuqunqing 2016年9月23日 上午9:57:49
	 * @return
	 */
	public String deleteChannel(){
		ajaxData = deviceService.deleteChannel(ids);
		return AJAX;
	}
	
	/**
	 * 预览通道信息
	 * @author zhengquanwu 2016年11月5日 上午11:40:02
	 * @return
	 */
	public String getDevChannelInfo() {
		recordPojo = deviceService.getDevChannelInfo(id.intValue());
		operPage = "/modules/device/channelMng/dialogs/previewChannel.jsp";
		return DISPATCHER;
	}
    //=======================getter/setter======================================================================
    public String getPasswordSecurityLevel() {
    	return passwordSecurityLevel;
    }
	
    public void setPasswordSecurityLevel(String passwordSecurityLevel) {
    	this.passwordSecurityLevel = passwordSecurityLevel;
    }
	
    public IDeviceService getDeviceService() {
    	return deviceService;
    }
	
    public void setDeviceService(IDeviceService deviceService) {
    	this.deviceService = deviceService;
    }
	
    public List<Integer> getIds() {
    	return ids;
    }
	
    public void setIds(List<Integer> ids) {
    	this.ids = ids;
    }
	
    public List<DeviceInfo> getDeviceList() {
    	return deviceList;
    }
	
    public void setDeviceList(List<DeviceInfo> deviceList) {
    	this.deviceList = deviceList;
    }
	
    public List<DeviceChannelInfo> getDeviceChannelList() {
    	return deviceChannelList;
    }
	
    public void setDeviceChannelList(List<DeviceChannelInfo> deviceChannelList) {
    	this.deviceChannelList = deviceChannelList;
    }
	
    public DeviceInfo getDeviceInfo() {
    	return deviceInfo;
    }
	
    public void setDeviceInfo(DeviceInfo deviceInfo) {
    	this.deviceInfo = deviceInfo;
    }

    public IRegionService getRegionService() {
    	return regionService;
    }
	
    public void setRegionService(IRegionService regionService) {
    	this.regionService = regionService;
    }

	
    public List<CloudPoolInfo> getCloudpoolInfoList() {
    	return cloudpoolInfoList;
    }

	
    public void setCloudpoolInfoList(List<CloudPoolInfo> cloudpoolInfoList) {
    	this.cloudpoolInfoList = cloudpoolInfoList;
    }
	
    public ISysconfigService getSysconfigService() {
    	return sysconfigService;
    }

    public void setSysconfigService(ISysconfigService sysconfigService) {
    	this.sysconfigService = sysconfigService;
    }
	
    public File getDevFile() {
    	return devFile;
    }

	
    public void setDevFile(File devFile) {
    	this.devFile = devFile;
    }

    public String getDevFileFileName() {
    	return devFileFileName;
    }
	
    public void setDevFileFileName(String devFileFileName) {
    	this.devFileFileName = devFileFileName;
    }
	
    public IConfigLogDao getConfigLogDao() {
    	return configLogDao;
    }

	
    public void setConfigLogDao(IConfigLogDao configLogDao) {
    	this.configLogDao = configLogDao;
    }

	public NtpParamInfo getNtpParamInfo() {
	    return ntpParamInfo;
    }

	public void setNtpParamInfo(NtpParamInfo ntpParamInfo) {
	    this.ntpParamInfo = ntpParamInfo;
    }

    public DeviceChannelInfo getDeviceChannelInfo() {
    	return this.deviceChannelInfo;
    }

    public void setDeviceChannelInfo(DeviceChannelInfo deviceChannelInfo) {
    	this.deviceChannelInfo = deviceChannelInfo;
    }
    
    public RecordPojo getRecordPojo() {
		return recordPojo;
	}
    
    public void setRecordPojo(RecordPojo recordPojo) {
		this.recordPojo = recordPojo;
	}

}
