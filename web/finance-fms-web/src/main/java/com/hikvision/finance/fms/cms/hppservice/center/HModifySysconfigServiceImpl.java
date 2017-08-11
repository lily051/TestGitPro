/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-3-22 下午4:50:07
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.cms.hppservice.center;

import java.util.List;

import org.apache.commons.lang3.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.google.protobuf.RpcCallback;
import com.google.protobuf.RpcController;
import com.hikvision.finance.fms.cms.hpp.center.SysconfigProto.ModifySysconfigService;
import com.hikvision.finance.fms.cms.hpp.center.SysconfigProto.Sysconfig;
import com.hikvision.finance.fms.cms.hpp.center.SysconfigProto.SysconfigList;
import com.hikvision.finance.fms.cms.hpp.type.CmdProto.CMD;
import com.hikvision.finance.fms.common.exception.CMSException;
import com.hikvision.finance.fms.modules.record.dao.IProSaveRecordDao;

/**
 * <p></p>
 * @author xujiangfei 2016-3-22 下午4:50:07
 * @version V1.0 
 */
public class HModifySysconfigServiceImpl extends ModifySysconfigService{

	private static Logger logger = LoggerFactory.getLogger(HModifySysconfigServiceImpl.class);
	private IProSaveRecordDao proSaveRecordDao;
	
	/* (non-Javadoc)--// CMD_DT_SYSCONFIG_LIST--获取录像守卫服务的工作时间
	 * @see com.hikvision.finance.fms.cms.hpp.center.SysconfigProto.ModifySysconfigService#qyerySysconfigServicetime(com.google.protobuf.RpcController, com.hikvision.finance.fms.cms.hpp.center.SysconfigProto.Sysconfig, com.google.protobuf.RpcCallback)
	 */
	@Override
	public void qyerySysconfigServicetime(RpcController controller,Sysconfig request, RpcCallback<SysconfigList> done) {
		
		logger.info("开始【获取录像守卫服务的工作时间】处理:cmd: 【" + CMD.CMD_DT_SYSCONFIG_LIST_VALUE + "】; request:【" + request.toString() + "】");
		SysconfigList.Builder listRsp = SysconfigList.newBuilder();
		
		try{
			//1.设置返回参数
			listRsp.setCmd(CMD.CMD_DT_SYSCONFIG_LIST_VALUE);
			
			List<com.hikvision.finance.fms.model.Sysconfig> listSysconfig = proSaveRecordDao.getProsaverecoreTime();
			if(listSysconfig==null){
				throw new CMSException(CMSException.DEVICE_NO_EXITS, "没有对应录像设备！");
			}
			
			for(int i=0;i<listSysconfig.size();i++){
				Sysconfig.Builder rsp = Sysconfig.newBuilder();
				// 参数key值
				if(listSysconfig.get(i).getNkey()!=null){
					rsp.setNKey(listSysconfig.get(i).getNkey());
				}
				// 参数值
				if(StringUtils.isNotBlank(listSysconfig.get(i).getStrValue())){
					rsp.setStrValue(listSysconfig.get(i).getStrValue());
				}
				listRsp.addSysconfig(i, rsp);
			}
			
			//结果处理
			done.run(listRsp.build());
		}catch(CMSException e){
			done.run(listRsp.build());
			logger.error("结束 获取录像守卫服务的工作时间  请求处理: 异常: 【" + e.getMessage() + "】",e);
			return;
		}
		logger.info("结束 获取录像守卫服务的工作时间  请求处理:cmd: 【" + CMD.CMD_DT_STORAGEDEVICE_LIST.getNumber() + "】; resultInfo:【"
		        + listRsp.build().toString() + "】");
	}
	
	// setter--getter
	
	public IProSaveRecordDao getProSaveRecordDao() {
		return proSaveRecordDao;
	}
	
	public void setProSaveRecordDao(IProSaveRecordDao proSaveRecordDao) {
		this.proSaveRecordDao = proSaveRecordDao;
	}
	
}
