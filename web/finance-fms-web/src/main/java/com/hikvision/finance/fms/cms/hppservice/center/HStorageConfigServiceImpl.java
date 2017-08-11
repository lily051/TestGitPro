/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-3-15 下午6:39:16
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.cms.hppservice.center;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.google.protobuf.RpcCallback;
import com.google.protobuf.RpcController;
import com.hikvision.finance.fms.cms.dto.DeviceInfo;
import com.hikvision.finance.fms.cms.hpp.center.StorageConfigProto.ModifyStorageConfigService;
import com.hikvision.finance.fms.cms.hpp.center.StorageConfigProto.ReqStorageConfig;
import com.hikvision.finance.fms.cms.hpp.center.StorageConfigProto.RspStorageConfig;
import com.hikvision.finance.fms.cms.hpp.type.CmdProto.CMD;
import com.hikvision.finance.fms.common.exception.CMSException;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.modules.device.dao.IDeviceDao;

/**
 * <p></p>
 * @author xujiangfei 2016-3-15 下午6:39:16
 * @version V1.0 
 */
public class HStorageConfigServiceImpl extends ModifyStorageConfigService{
	private static Logger logger = LoggerFactory.getLogger(HStorageConfigServiceImpl.class);
	private IDeviceDao deviceDao;
	//private Device device;
	/* CMD_DT_STORAGE_INFO--获取存储设备参数;(根据采集设备的ip和端口来获得存储设备的参数)
	 * @see com.hikvision.finance.fms.cms.hpp.center.StorageConfigProto.ModifyStorageConfigService#qyeryStorageConfiginfo(com.google.protobuf.RpcController, com.hikvision.finance.fms.cms.hpp.center.StorageConfigProto.ReqStorageConfig, com.google.protobuf.RpcCallback)
	 */
	@Override
	public void queryStorageConfiginfo(RpcController controller,ReqStorageConfig request, RpcCallback<RspStorageConfig> done) {
		
		logger.info("【上传服务】获取存储设备参数：cmd： 【"+CMD.CMD_DT_STORAGE_INFO_VALUE+"】; request:【"+request.toString()+"】");
		RspStorageConfig.Builder rsp = RspStorageConfig.newBuilder();
		try{
		//1.设置返回参数
		rsp.setCmd(CMD.CMD_DT_STORAGE_INFO_VALUE);
		
		//2.根据请求参数查询是否存在该设备
		String ip = request.getIp();
		int port = request.getPort();
		DeviceInfo device = new DeviceInfo();
		device = deviceDao.getStorageInfo(ip,port);
		
	    if(device!=null&&device.getNtype() !=null){
	    	if(device!=null){
	    		rsp.setType(device.getNtype());//设备类型
	    	}
	    	if(device.getDvrType()!=null){
	    		rsp.setDevicetype(device.getDvrType());//产品类型，设备型号
	    	}
			if(device.getStrIp()!=null){
				rsp.setStorageIp(device.getStrIp());
			}
			if(device.getNport()!=null){
				rsp.setStoragePort(device.getNport());
			}
			if(device.getStrUser()!=null){
				rsp.setStorageUserName(device.getStrUser());
			}
			if(device.getStrPassword()!=null){
				rsp.setStoragePwd(device.getStrPassword());
			}
			if(device.getGroupIndex()!=null){
				rsp.setPoolid(device.getGroupIndex());
			}
			rsp.setResult(Constants.ResultStatus.RESULT_SUCCESS);
		}else{
			rsp.setResult(Constants.ResultStatus.RESULT_FAILE);
			throw new CMSException(CMSException.DEVICE_NO_EXITS, "没有对应的设备！");
		}
		//3.结果处理
	    done.run(rsp.build());
		
		}catch(CMSException e){
			done.run(rsp.build());
			logger.error("结束 获取存储设备参数请求处理: 异常: 【" + e.getMessage() + "】",e);
			return;
		}
		logger.info("结束 获取存储设备参数请求处理:cmd: 【" + CMD.CMD_DT_STORAGE_INFO.getNumber() + "】; resultInfo:【"
		        + rsp.build().toString() + "】");
	}
	
	
	
	//getter*************setter 
	
	public IDeviceDao getDeviceDao() {
		return deviceDao;
	}
	public void setDeviceDao(IDeviceDao deviceDao) {
		this.deviceDao = deviceDao;
	}
	
}
