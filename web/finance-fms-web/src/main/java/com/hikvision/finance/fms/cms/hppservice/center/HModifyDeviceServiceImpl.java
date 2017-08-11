/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-3-21 下午7:46:25
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.cms.hppservice.center;

import java.util.List;

import org.apache.commons.lang3.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.google.protobuf.RpcCallback;
import com.google.protobuf.RpcController;
import com.hikvision.finance.fms.cms.hpp.center.DeviceProto.DeviceList;
import com.hikvision.finance.fms.cms.hpp.center.DeviceProto.ModifyDeviceService;
import com.hikvision.finance.fms.cms.hpp.center.DeviceProto.ReqDevice;
import com.hikvision.finance.fms.cms.hpp.server.RspServerDataProto.RspServerData;
import com.hikvision.finance.fms.cms.hpp.type.CmdProto.CMD;
import com.hikvision.finance.fms.common.exception.CMSException;
import com.hikvision.finance.fms.common.util.Constants.ResultStatus;
import com.hikvision.finance.fms.common.util.StringUtilExpand;
import com.hikvision.finance.fms.model.Device;
import com.hikvision.finance.fms.modules.device.dao.IDeviceDao;

/**
 * <p></p>
 * @author xujiangfei 2016-3-21 下午7:46:25
 * @version V1.0 
 */
public class HModifyDeviceServiceImpl extends ModifyDeviceService{
	private static Logger logger = LoggerFactory.getLogger(HModifyDeviceServiceImpl.class);
	private IDeviceDao deviceDao ;
	/* (non-Javadoc)--// CMD_DT_STORAGEDEVICE_INFO(获取存储设备信息)
	 * @see com.hikvision.finance.fms.cms.hpp.center.DeviceProto.ModifyDeviceService#queryStorageDeviceInfo(com.google.protobuf.RpcController, com.hikvision.finance.fms.cms.hpp.center.DeviceProto.ReqDevice, com.google.protobuf.RpcCallback)
	 */
	@Override
	public void queryStorageDeviceInfo(RpcController controller,ReqDevice request, RpcCallback<DeviceList> done) {
		logger.info("开始【获取存储设备信息】处理:cmd: 【" + CMD.CMD_DT_STORAGEDEVICE_INFO_VALUE + "】; request:【" + request.toString() + "】");
		
		DeviceList.Builder rspDeviceList = DeviceList.newBuilder();
		//设置返回参数
		rspDeviceList.setCmd(CMD.CMD_DT_STORAGEDEVICE_INFO_VALUE);
		
		List<Device> deviceList = deviceDao.getDCServiceInfobyCvrid(request.getId());
		try{
			if(deviceList == null){
				throw new CMSException(CMSException.DEVICE_NO_EXITS, "没有查到对应的录像设备！");
			}
			for(int i=0;i<deviceList.size();i++){
				com.hikvision.finance.fms.cms.hpp.center.DeviceProto.Device.Builder deviceRsp = com.hikvision.finance.fms.cms.hpp.center.DeviceProto.Device.newBuilder();
				// 在数据库中的流水号
				if(deviceList.get(i).getId()!=null){
					deviceRsp.setId(deviceList.get(i).getId());
				}
				// 设备名称
				if(StringUtils.isNotBlank(deviceList.get(i).getStrName())){
					deviceRsp.setStrName(deviceList.get(i).getStrName());
				}
				// 设备IP地址
				if(StringUtils.isNotBlank(deviceList.get(i).getStrIp())){
					deviceRsp.setStrIp(deviceList.get(i).getStrIp());
				}
				// 设备端口号
				if(deviceList.get(i).getNport()!=null){
					deviceRsp.setNPort(deviceList.get(i).getNport());
				}
				// 设备用户名
				if(StringUtils.isNotBlank(deviceList.get(i).getStrUser())){
					deviceRsp.setStrUser(deviceList.get(i).getStrUser());
				}
				// 设备密码
				if(StringUtils.isNotBlank(deviceList.get(i).getStrPassword())){
					deviceRsp.setStrPassword(deviceList.get(i).getStrPassword());
				}
				//所属区域流水号
				if(deviceList.get(i).getNregionId()!=null){
					deviceRsp.setNRegionId(deviceList.get(i).getNregionId());
				}
				// 设备类型
				if(deviceList.get(i).getNtype()!=null){
					deviceRsp.setNType(deviceList.get(i).getNtype());
				}
				//云存储-对象存储AccessKey
				if(deviceList.get(i).getStrAccessKey() != null){
					deviceRsp.setStrAccessKey(deviceList.get(i).getStrAccessKey());
				}
				//云存储-对象存储SecretKey
				if(deviceList.get(i).getStrSecretKey() != null){
					deviceRsp.setStrSecretKey(deviceList.get(i).getStrSecretKey());
				}
				rspDeviceList.addDevice(i, deviceRsp);
			}
			
			//处理结果
			done.run(rspDeviceList.build());
		}catch(CMSException e){
			done.run(rspDeviceList.build());
			logger.error("结束 获取存储设备信息 请求处理: 异常: 【" + e.getMessage() + "】",e);
			return;
		}
		
		logger.info("结束 获取存储设备信息  请求处理:cmd: 【" + CMD.CMD_DT_STORAGEDEVICE_INFO.getNumber() + "】; resultInfo:【"
		        + rspDeviceList.build().toString() + "】");
	}

	/* (non-Javadoc)--CMD_DT_STORAGEDEVICE_LIST(根据设备类型获取存储设备信息)
	 * @see com.hikvision.finance.fms.cms.hpp.center.DeviceProto.ModifyDeviceService#queryStorageDeviceInfoBytype(com.google.protobuf.RpcController, com.hikvision.finance.fms.cms.hpp.center.DeviceProto.ReqDevice, com.google.protobuf.RpcCallback)
	 */
	@Override
	public void queryStorageDeviceInfoBytype(RpcController controller,ReqDevice request, RpcCallback<DeviceList> done) {
		logger.info("开始【根据设备类型获取存储设备信息】处理:cmd: 【" + CMD.CMD_DT_STORAGEDEVICE_LIST_VALUE + "】; request:【" + request.toString() + "】");
		DeviceList.Builder deviceListRsp = DeviceList.newBuilder();
		deviceListRsp.setCmd(CMD.CMD_DT_STORAGEDEVICE_LIST_VALUE);
		try{
			List<Device> listDevice = deviceDao.getDeviceinfobyntype();
			if(listDevice==null){
				throw new CMSException(CMSException.DEVICE_NO_EXITS, "没有对应的DVR设备");
			}
			for(int i = 0;i<listDevice.size();i++){
				com.hikvision.finance.fms.cms.hpp.center.DeviceProto.Device.Builder deviceRsp = com.hikvision.finance.fms.cms.hpp.center.DeviceProto.Device.newBuilder();
				// 在数据库中的流水号
				if(StringUtilExpand.integerIsNotNull(listDevice.get(i).getId())){
					deviceRsp.setId(listDevice.get(i).getId());
				}
				// 设备名称
				if(StringUtils.isNotBlank(listDevice.get(i).getStrName())){
					deviceRsp.setStrName(listDevice.get(i).getStrName());
				}
				// 设备类型
				if(StringUtilExpand.integerIsNotNull(listDevice.get(i).getNtype())){
					deviceRsp.setNType(listDevice.get(i).getNtype());
				}
				//设备ip
				if(StringUtils.isNotBlank(listDevice.get(i).getStrIp())){
					deviceRsp.setStrIp(listDevice.get(i).getStrIp());
				}
				//设备端口
				if(StringUtilExpand.integerIsNotNull(listDevice.get(i).getNport())){
					deviceRsp.setNPort(listDevice.get(i).getNport());
				}
				//用户名
				if(StringUtils.isNotBlank(listDevice.get(i).getStrUser())){
					deviceRsp.setStrUser(listDevice.get(i).getStrUser());
				}
				//密码
				if(StringUtils.isNotBlank(listDevice.get(i).getStrPassword())){
					deviceRsp.setStrPassword(listDevice.get(i).getStrPassword());
				}
				//存储设备的区域号
				if(StringUtilExpand.integerIsNotNull(listDevice.get(i).getNregionId())){
					deviceRsp.setNRegionId(listDevice.get(i).getNregionId());
				}
				//云存储-对象存储AccessKey
				if(StringUtils.isNotBlank(listDevice.get(i).getStrAccessKey())){
					deviceRsp.setStrAccessKey(listDevice.get(i).getStrAccessKey());
				}
				//云存储-对象存储SecretKey
				if(StringUtils.isNotBlank(listDevice.get(i).getStrSecretKey())){
					deviceRsp.setStrSecretKey(listDevice.get(i).getStrSecretKey());
				}
				deviceListRsp.addDevice(i, deviceRsp);//.setDevice(i, deviceRsp);
			}
			
			//结果处理
			done.run(deviceListRsp.build());
		}catch(CMSException e){
			done.run(deviceListRsp.build());
			logger.error("结束 根据设备类型获取存储设备信息 请求处理: 异常: 【" + e.getMessage() + "】",e);
			return;
		}
		logger.info("结束 根据设备类型获取存储设备信息  请求处理:cmd: 【" + CMD.CMD_DT_STORAGEDEVICE_LIST.getNumber() + "】; resultInfo:【"
		        + deviceListRsp.build().toString() + "】");
	}

	/* (non-Javadoc)--CMD_DT_DVRDEVICE_LIST(获取所有DVR设备信息)
	 * @see com.hikvision.finance.fms.cms.hpp.center.DeviceProto.ModifyDeviceService#queryAllDvrInfo(com.google.protobuf.RpcController, com.hikvision.finance.fms.cms.hpp.center.DeviceProto.ReqDevice, com.google.protobuf.RpcCallback)
	 */
	@Override
	public void queryAllDvrInfo(RpcController controller, ReqDevice request,RpcCallback<DeviceList> done) {
		
		logger.info("开始【获取所有DVR设备信息】处理:cmd: 【" + CMD.CMD_DT_DVRDEVICE_LIST_VALUE + "】; request:【" + request.toString() + "】");
		DeviceList.Builder deviceListRsp = DeviceList.newBuilder();
		deviceListRsp.setCmd(CMD.CMD_DT_DVRDEVICE_LIST_VALUE);
		try{
			List<Device> deviceList= deviceDao.getAllDvrServiceInfo();
			
			if(deviceList==null||deviceList.size()==0){
				throw new CMSException(CMSException.DEVICE_NO_EXITS, "没有对应的DVR设备！");
			}
			for(int i = 0;i<deviceList.size();i++){
				com.hikvision.finance.fms.cms.hpp.center.DeviceProto.Device.Builder deviceRsp = com.hikvision.finance.fms.cms.hpp.center.DeviceProto.Device.newBuilder();
				// 在数据库中的流水号
				if(StringUtilExpand.integerIsNotNull(deviceList.get(i).getId())){
					deviceRsp.setId(deviceList.get(i).getId());
				}
				// 设备类型
				if(StringUtilExpand.integerIsNotNull(deviceList.get(i).getNtype())){
					deviceRsp.setNType(deviceList.get(i).getNtype());
				}
				// 设备名称
				if(StringUtils.isNotBlank(deviceList.get(i).getStrName()) ){
					deviceRsp.setStrName(deviceList.get(i).getStrName());
				}
				// 设备IP地址
				if(StringUtils.isNotBlank(deviceList.get(i).getStrIp())){
					deviceRsp.setStrIp(deviceList.get(i).getStrIp());
				}
				// 设备端口号
				if(StringUtilExpand.integerIsNotNull(deviceList.get(i).getNport())){
					deviceRsp.setNPort(deviceList.get(i).getNport());
				}
				// 设备用户名
				if(StringUtils.isNotBlank(deviceList.get(i).getStrUser())){
					deviceRsp.setStrUser(deviceList.get(i).getStrUser());
				}
				// 设备密码
				if(StringUtils.isNotBlank(deviceList.get(i).getStrPassword())){
					deviceRsp.setStrPassword(deviceList.get(i).getStrPassword());
				}
				// 设备密码等级
				if(StringUtils.isNotBlank(deviceList.get(i).getStrPasswordLevel())){
					deviceRsp.setStrPasswordLevel(deviceList.get(i).getStrPasswordLevel());
				}
				// 模拟通道个数
				if(StringUtilExpand.integerIsNotNull(deviceList.get(i).getNchanNum())){
					deviceRsp.setNChanNum(deviceList.get(i).getNchanNum());
				}
				// ip通道个数
				if(StringUtilExpand.integerIsNotNull(deviceList.get(i).getTelChanNum())){
					deviceRsp.setTelChanNum(deviceList.get(i).getTelChanNum());
				}
				// 所属区域流水号
				if(StringUtilExpand.integerIsNotNull(deviceList.get(i).getNregionId())){
					deviceRsp.setNRegionId(deviceList.get(i).getNregionId());
				}
				// 主子码流类型
				if(StringUtilExpand.integerIsNotNull(deviceList.get(i).getNstreamType())){
					deviceRsp.setNStreamType(deviceList.get(i).getNstreamType());
				}
				// 在线状态
				if(StringUtilExpand.integerIsNotNull(deviceList.get(i).getOnline())){
					deviceRsp.setOnline(deviceList.get(i).getOnline());
				}
				// 设备编号
				if(StringUtils.isNotBlank(deviceList.get(i).getStrIndexCode())){
					deviceRsp.setStrIndexCode(deviceList.get(i).getStrIndexCode());
				}
				// 厂商
				if(StringUtils.isNotBlank(deviceList.get(i).getStrManufacture())){
					deviceRsp.setStrManufacture(deviceList.get(i).getStrManufacture());
				}
				deviceListRsp.addDevice(i, deviceRsp);//.setDevice(i, deviceRsp);
			}
			
			//结果处理
			done.run(deviceListRsp.build());
			
		}catch(CMSException e){
			done.run(deviceListRsp.build());
			logger.error("结束 获取所有DVR设备信息 请求处理: 异常: 【" + e.getMessage() + "】",e);
			return;
		}
		
		logger.info("结束 获取所有DVR设备信息  请求处理:cmd: 【" + CMD.CMD_DT_DVRDEVICE_LIST.getNumber() + "】; resultInfo:【"
		        + deviceListRsp.build().toString() + "】");
	}

	
	/* (non-Javadoc)--CMD_MDT_DEVIDE_STATUS(更新设备在线状态)
	 * @see com.hikvision.finance.fms.cms.hpp.center.DeviceProto.ModifyDeviceService#updateDeviceOnline(com.google.protobuf.RpcController, com.hikvision.finance.fms.cms.hpp.center.DeviceProto.ReqDevice, com.google.protobuf.RpcCallback)
	 */
	@Override
	public void updateDeviceOnline(RpcController controller, ReqDevice request,
			RpcCallback<RspServerData> done) {
		logger.info("开始【更新设备在线状态】处理:cmd: 【" + CMD.CMD_MDT_DEVIDE_STATUS_VALUE + "】; request:【" + request.toString() + "】");
		//1.设置返回参数
		RspServerData.Builder rspDate = RspServerData.newBuilder();
		rspDate.setCmd(CMD.CMD_MDT_DEVIDE_STATUS_VALUE);
		try{
			String strIp = request.getStrIp();
			int port = request.getNPort();
			int online = request.getOnline();
			
			deviceDao.updateDeviceOnline(strIp,port,online);
			
			rspDate.setResult(ResultStatus.RESULT_SUCCESS);
			
			//结果处理
			done.run(rspDate.build());
		}catch(CMSException e){
			done.run(rspDate.build());
			logger.error("结束 更新设备在线状态 请求处理: 异常: 【" + e.getMessage() + "】",e);
			return;
		}
		
		logger.info("结束 更新设备在线状态  请求处理:cmd: 【" + CMD.CMD_MDT_DEVIDE_STATUS.getNumber() + "】; resultInfo:【"
		        + rspDate.build().toString() + "】");
	}

	/* (non-Javadoc)--CMD_MDT_DEVIDE_SPACE(更新设备可用空间)
	 * @see com.hikvision.finance.fms.cms.hpp.center.DeviceProto.ModifyDeviceService#updateDeviceSpace(com.google.protobuf.RpcController, com.hikvision.finance.fms.cms.hpp.center.DeviceProto.ReqDevice, com.google.protobuf.RpcCallback)
	 */
	@Override
	public void updateDeviceSpace(RpcController controller, ReqDevice request,
			RpcCallback<RspServerData> done) {
		logger.info("开始【更新设备可用空间】处理:cmd: 【" + CMD.CMD_MDT_DEVIDE_SPACE_VALUE + "】; request:【" + request.toString() + "】");
		
		//1.设置返回参数
		RspServerData.Builder rspDate = RspServerData.newBuilder();
		rspDate.setCmd(CMD.CMD_MDT_DEVIDE_STATUS_VALUE);
		try{
			String strIp = request.getStrIp();
			int port = request.getNPort();
			int uiTotalSpace = request.getUiTotalSpace();//硬盘总容量
			int uiFreeSpace = request.getUiFreeSpace();//硬盘空闲容量
			
			deviceDao.updateDeviceSpace(strIp,port,uiTotalSpace,uiFreeSpace);
			rspDate.setResult(ResultStatus.RESULT_SUCCESS);
			
			//结果处理
			done.run(rspDate.build());
		}catch(CMSException e){
			done.run(rspDate.build());
			logger.error("结束 更新设备可用空间 请求处理: 异常: 【" + e.getMessage() + "】",e);
			return;
		}
		
		logger.info("结束 更新设备可用空间  请求处理:cmd: 【" + CMD.CMD_MDT_DEVIDE_SPACE.getNumber() + "】; resultInfo:【"
		        + rspDate.build().toString() + "】");
	}


	
	
	//setter -- getter

	public IDeviceDao getDeviceDao() {
		return deviceDao;
	}
	public void setDeviceDao(IDeviceDao deviceDao) {
		this.deviceDao = deviceDao;
	}
	

}
