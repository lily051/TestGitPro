/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-3-18 上午10:31:42
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.cms.hppservice.verify;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.google.protobuf.RpcCallback;
import com.google.protobuf.RpcController;
import com.hikvision.finance.core.util.StringUtils;
import com.hikvision.finance.fms.cms.hpp.type.CmdProto.CMD;
import com.hikvision.finance.fms.cms.hpp.verify.ReqServerLoginProto.ReqServerLogin;
import com.hikvision.finance.fms.cms.hpp.verify.ReqServerLoginProto.ServerLoginService;
import com.hikvision.finance.fms.cms.hpp.verify.RspServerLoginProto.RspServerLogin;
import com.hikvision.finance.fms.common.exception.CMSException;
import com.hikvision.finance.fms.common.init.FmsPropertyPlaceholder;
import com.hikvision.finance.fms.common.util.Constants.ResultStatus;
import com.hikvision.finance.fms.license.util.RununingStateUtil;
import com.hikvision.finance.fms.model.Server;
import com.hikvision.finance.fms.modules.server.dao.IServerDao;
import com.hikvision.finance.fms.modules.sysconfig.service.ISysconfigService;
import com.hikvision.finance.fwork.exception.ExpectedException;

/**
 * <p></p>
 * @author xujiangfei 2016-3-18 上午10:31:42
 * @version V1.0 
 */
public class HServerLoginServiceImpl extends ServerLoginService{
	private static Logger logger = LoggerFactory.getLogger(HServerLoginServiceImpl.class);
	private IServerDao serverDao;
	private ISysconfigService sysconfigService;
	/* (non-Javadoc)
	 * @see com.hikvision.finance.fms.cms.hpp.verify.ReqServerLoginProto.ServerLoginService#serverLogin(com.google.protobuf.RpcController, com.hikvision.finance.fms.cms.hpp.verify.ReqServerLoginProto.ReqServerLogin, com.google.protobuf.RpcCallback)
	 */
	@Override
	public void serverLogin(RpcController controller, ReqServerLogin request,RpcCallback<RspServerLogin> done) {
		
		logger.info("开始 服务器端登录请求处理:cmd: 【" + CMD.CMD_REQ_SERVER_LOGIN_VALUE + "】; request:【" + request.toString() + "】");
		RspServerLogin.Builder rsp = RspServerLogin.newBuilder();
		logger.info("服务器类型："+request.getMoudleType()+",服务器ip："+request.getIp());
		try{
		//1.设置返回参数
		rsp.setCmd(CMD.CMD_RSP_SERVER_LOGIN_VALUE);
		rsp.setMoudleType(request.getMoudleType());
		
		String ip = request.getIp();//登录机器的ip
		String dcIp = request.getDcIp();//服务的ip
		Integer MoudleType = request.getMoudleType();
		
		//2.检查
		verifyBasic();
		//verifyAuthorize(rspServerLogin);//检查授权--暂时没有验证
		if (StringUtils.isBlank(request.getIp())|| StringUtils.isBlank(dcIp)) {
			throw new CMSException(CMSException.SERVER_REQ_REQUIRED, "登录的机器IP或 数据中心服务IP不能为空");
		}
		if(!request.hasMoudleType()){
			throw new CMSException(CMSException.SERVER_REQ_REQUIRED, "服务器类型不能为空");
		}
		
		//3.网域处理
		//4.查询服务
		//用来判断系统的模式   "ipc"：ipc模式，"usb"：usb模式
		String Modelusb = FmsPropertyPlaceholder.getProperty("choiceUsbIpc");
		if (StringUtils.isEmpty(Modelusb)) {
			throw new ExpectedException("", "未获取到通道方式");
		}
		Server server = serverDao.findServerByip(ip,MoudleType,Modelusb);
		if(server!=null){
			//结果处理
			rsp.setConfigInfo(server.getStrConfig());//返回配置信息
			if(server.getNctrlPort()!=null){
				rsp.setCtrlPort(server.getNctrlPort());////返回控制端口--这个只有系统巡检服务有其他的没有
			}
			rsp.setResult(ResultStatus.RESULT_SUCCESS);
			rsp.setResultInfo("Successed to login");
		}else{
			throw new CMSException(CMSException.SERVER_NOT_EXITS, "无此类型服务！");
		}
		done.run(rsp.build());
		}catch (CMSException e) {
			rsp.setResult(ResultStatus.RESULT_FAILE);
			rsp.setResultInfo(e.getMessage());
			done.run(rsp.build());
			logger.error("结束 服务器端登录请求处理: 异常: 【" + e.getMessage() + "】",e);
			return;
		} 
		
		logger.info("结束 服务器端登录请求处理:cmd: 【" + CMD.CMD_RSP_SERVER_LOGIN.getNumber() + "】; resultInfo:【"
		        + rsp.build().toString() + "】");
	}
	
	
	/**
	 * @author xujiangfei 2016-3-18 上午10:45:39
	 */
	private void verifyBasic() {
		// 1. 检查数据库连接
		if (!RununingStateUtil.isDBConnected()) {
				throw new CMSException(CMSException.SERVER_DB_DIS_CONNECT, "数据库连接异常！");
			}
			// 2. 检查mina服务
		if (!RununingStateUtil.isMINAServer()) {
				throw new CMSException(CMSException.SERVER_DB_DIS_CONNECT, "MINA服务未启动！");
			}
		
	}

	//setter  getter
	public IServerDao getServerDao() {
		return serverDao;
	}
	public void setServerDao(IServerDao serverDao) {
		this.serverDao = serverDao;
	}

	public ISysconfigService getSysconfigService() {
		return sysconfigService;
	}
	public void setSysconfigService(ISysconfigService sysconfigService) {
		this.sysconfigService = sysconfigService;
	}
	
	
	
}
