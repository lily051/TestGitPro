/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-3-18 下午2:55:16
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.cms.hppservice.verify;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.google.protobuf.RpcCallback;
import com.google.protobuf.RpcController;
import com.hikvision.finance.fms.cms.hpp.server.RspServerDataProto.RspServerData;
import com.hikvision.finance.fms.cms.hpp.type.CmdProto.CMD;
import com.hikvision.finance.fms.cms.hpp.verify.MsgServerLogoutProto.MsgServerLogout;
import com.hikvision.finance.fms.cms.hpp.verify.MsgServerLogoutProto.ServerLogoutService;
import com.hikvision.finance.fms.modules.server.dao.IServerDao;

/**
 * <p></p>
 * @author xujiangfei 2016-3-18 下午2:55:16
 * @version V1.0 
 */
public class HServerLogoutServiceImpl extends ServerLogoutService{
	
	private static Logger logger = LoggerFactory.getLogger(HServerLogoutServiceImpl.class);
	private IServerDao serverDao;
	
	/* (non-Javadoc)
	 * @see com.hikvision.finance.fms.cms.hpp.verify.MsgServerLogoutProto.ServerLogoutService#serverLogout(com.google.protobuf.RpcController, com.hikvision.finance.fms.cms.hpp.verify.MsgServerLogoutProto.MsgServerLogout, com.google.protobuf.RpcCallback)
	 */
	@Override
	public void serverLogout(RpcController controller, MsgServerLogout request,RpcCallback<RspServerData> done) {
		logger.info("服务器端登出 :cmd: 【" + CMD.CMD_MSG_SERVER_LOGOUT.getNumber() + "】; request:【" + request.toString() + "】");
		
		logger.info("服务类型：" + request.getMoudleType() + "服务IP地址：" + request.getIp());
		
	}
	
	//setter--getter
	public IServerDao getServerDao() {
		return this.serverDao;
	}
	public void setServerDao(IServerDao serverDao) {
		this.serverDao = serverDao;
	}

}
