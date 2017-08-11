/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-5-5 下午6:36:36
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.cms.hppservice.center;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.google.protobuf.RpcCallback;
import com.google.protobuf.RpcController;
import com.hikvision.finance.fms.cms.hpp.center.ServerProto.ModifyServerService;
import com.hikvision.finance.fms.cms.hpp.center.ServerProto.ReqServer;
import com.hikvision.finance.fms.cms.hpp.server.RspServerDataProto.RspServerData;
import com.hikvision.finance.fms.cms.hpp.type.CmdProto.CMD;
import com.hikvision.finance.fms.modules.server.dao.IServerDao;

/**
 * <p></p>
 * @author xujiangfei 2016-5-5 下午6:36:36
 * @version V1.0 
 */
public class HModifyServerServiceImpl extends ModifyServerService{
	private static Logger logger = LoggerFactory.getLogger(HModifyServerServiceImpl.class);
	private IServerDao serverDao;
	/* (non-Javadoc)--更新服务心跳时间(CMD_MDT_SERVER_STATUS)
	 * @see com.hikvision.finance.fms.cms.hpp.center.ServerProto.ModifyServerService#updateServerOnline(com.google.protobuf.RpcController, com.hikvision.finance.fms.cms.hpp.center.ServerProto.ReqServer, com.google.protobuf.RpcCallback)
	 */
	@Override
	public void updateServerOnline(RpcController controller, ReqServer request,
			RpcCallback<RspServerData> done) {
		logger.info("开始【更新服务心跳时间】处理:cmd: 【" + CMD.CMD_MDT_SERVER_STATUS_VALUE + "】; request:【" + request.toString() + "】");
		//1.更新对应服务的心跳时间，不做接口的返回
		String ip = request.getStrIP();
		int ntype = request.getNType();
			
		serverDao.setHeartbeatsTime(ip,ntype);//更新服务的心跳时间
	}
	
	
	//setter--getter
	public IServerDao getServerDao() {
		return serverDao;
	}
	public void setServerDao(IServerDao serverDao) {
		this.serverDao = serverDao;
	}
}
