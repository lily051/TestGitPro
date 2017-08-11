/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年10月10日 下午8:45:43
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.cms.hppservice.verify;

import java.net.InetSocketAddress;
import java.util.Calendar;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.google.common.collect.Lists;
import com.google.protobuf.RpcCallback;
import com.google.protobuf.RpcController;
import com.hikvision.finance.core.util.DateUtils;
import com.hikvision.finance.core.util.StringUtils;
import com.hikvision.finance.fms.cms.hpp.type.CmdProto.CMD;
import com.hikvision.finance.fms.cms.hpp.verify.ClientVerifyProto;
import com.hikvision.finance.fms.cms.hpp.verify.ClientVerifyProto.ClientVerifyService;
import com.hikvision.finance.fms.cms.hpp.verify.ClientVerifyProto.ReqChangePassword;
import com.hikvision.finance.fms.cms.hpp.verify.ClientVerifyProto.ReqClientInfo;
import com.hikvision.finance.fms.cms.hpp.verify.ClientVerifyProto.ReqClientLogin;
import com.hikvision.finance.fms.cms.hpp.verify.ClientVerifyProto.RspClientInfo;
import com.hikvision.finance.fms.cms.hpp.verify.ClientVerifyProto.RspClientLogin;
import com.hikvision.finance.fms.cms.hppservice.model.CheckDateVO;
import com.hikvision.finance.fms.common.cache.HeartbeatCache;
import com.hikvision.finance.fms.common.enums.SysRoleEnum;
import com.hikvision.finance.fms.common.exception.CMSException;
import com.hikvision.finance.fms.common.init.FmsPropertyPlaceholder;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.common.util.Constants.LoginStatus;
import com.hikvision.finance.fms.common.util.Constants.ModuleType;
import com.hikvision.finance.fms.common.util.Constants.PasswordType;
import com.hikvision.finance.fms.common.util.Constants.SysConfigType;
import com.hikvision.finance.fms.common.util.Constants.UserStatus;
import com.hikvision.finance.fms.license.dao.ISoftdogDao;
import com.hikvision.finance.fms.license.util.LicenseConstant.DOGTYPE;
import com.hikvision.finance.fms.model.Softdongle;
import com.hikvision.finance.fms.model.Sysconfig;
import com.hikvision.finance.fms.model.Userlock;
import com.hikvision.finance.fms.model.Users;
import com.hikvision.finance.fms.modules.log.dao.ILogsysDao;
import com.hikvision.finance.fms.modules.region.dao.IRegionDao;
import com.hikvision.finance.fms.modules.sysconfig.dao.ISysconfigDao;
import com.hikvision.finance.fms.modules.sysconfig.qo.SysconfigQo;
import com.hikvision.finance.fms.modules.user.dao.IUserlockDao;
import com.hikvision.finance.fms.modules.user.dao.IUsersDao;
import com.hikvision.finance.fms.modules.user.qo.UserlockQo;
import com.hikvision.finance.fms.modules.user.qo.UsersQo;
import com.hikvision.finance.fwork.exception.ExpectedException;
import com.hikvision.finance.fwork.util.GlobalMessageUtil;
import com.hikvision.license.constant.LicenseConstant.DOGVALUE;
import com.ivms6.core.hpp.HppChannel;
import com.ivms6.core.hpp.codec.HppConstants;
import com.ivms6.core.hpp.service.RpcControllerImpl;
import com.ivms6.core.hpp.util.ContextUtil;

/**
 * <p>CS客户端登录登出服务</p>
 * @author jinchenwu 2016年10月10日 下午8:45:43
 * @version V1.0 
 */
public class HClientVerifyServiceImpl extends ClientVerifyService {
	
	private static Logger logger = LoggerFactory.getLogger(HClientVerifyServiceImpl.class);
	
	private ISoftdogDao softdogDao;
	private IUsersDao usersDao;
	private ISysconfigDao sysconfigDao;
	private ILogsysDao logsysDao;
	private IUserlockDao userlockDao;
	private IRegionDao regionDao;
	
	/*
	 * 客户端登录
	 * @see com.hikvision.finance.fms.cms.hpp.verify.ReqClientLoginProto.ClientVerifyService#clientLogin(com.google.protobuf.RpcController,
	 * com.hikvision.finance.fms.cms.hpp.verify.ReqClientLoginProto.ReqClientLogin, com.google.protobuf.RpcCallback)
	 */
	@Override
	public void clientLogin(RpcController controller, ReqClientLogin request, RpcCallback<RspClientLogin> done) {
//		logger.info("开始客户端登录，Cmd：【" + CMD.CMD_DT_CLIENT_LOGIN_VALUE + "】 request：【" + request.toString() + "】 ");
		logger.info("开始客户端登录，Cmd：【" + CMD.CMD_DT_CLIENT_LOGIN_VALUE + "】  "); //打印出的日志中偶现出明文密码,后期再改
		ClientVerifyProto.RspClientLogin.Builder rspClientLogin = ClientVerifyProto.RspClientLogin.newBuilder();
		rspClientLogin.setCmd(CMD.CMD_DT_CLIENT_LOGIN_VALUE);// cmd
		try {
			// 获取客户端ip地址
			RpcControllerImpl rpcControllerImpl = (RpcControllerImpl)controller;
			long sessionId = rpcControllerImpl.getSessionId();
			HppChannel hppChannel = ContextUtil.getHppChannel(HppConstants.TYPE_SERVER + sessionId);
	    	if(null == hppChannel){// Coverity提示需要加上非空判断
				rspClientLogin.setResult(HppConstants.RESULT_ERROR);
				rspClientLogin.setResultInfo("系统错误");
				done.run(rspClientLogin.build());
				logger.error("结束 根据录像状态获取录像信息  请求处理异常: hppChannel为空");
				return;
	    	}
			String clientIP = ((InetSocketAddress)hppChannel.getSession().getRemoteAddress()).getAddress().getHostAddress();
			String serverIP = ((InetSocketAddress)hppChannel.getSession().getLocalAddress()).getAddress().getHostAddress();
			logger.info("服务端ip地址为: " + serverIP +" 客户端登录ip地址为：" + clientIP + "，客户端登录用户：" + request.getUserName());
			
			// 验证授权信息
			String errorInfo = getAuthorizeInfo();
			if (null != errorInfo) {
				rspClientLogin.setResult(HppConstants.RESULT_ERROR);
				rspClientLogin.setResultInfo(errorInfo);
				done.run(rspClientLogin.build());
				return;
			}
			
			// 验证用户信息
			rspClientLogin = verifyLoginUser(request, rspClientLogin, serverIP);
			if (HppConstants.RESULT_ERROR == rspClientLogin.getResult()) {
				done.run(rspClientLogin.build());
				return;
			}
			rspClientLogin.setClientIp(clientIP);
			com.hikvision.finance.fms.model.Logsys logsys = new com.hikvision.finance.fms.model.Logsys();
	    	logsys.setNmoduleType(ModuleType.COLLECT);//采集还是中心登录
	    	logsys.setStrUser(request.getUserName());
	    	logsys.setNtype(LoginStatus.LOGIN_IN);
	    	logsys.setStrDate(new Date());
	    	logsys.setStrIp(clientIP);
			logsysDao.save(logsys);
			rspClientLogin.setResult(HppConstants.RESULT_OK);
			rspClientLogin.setResultInfo("操作成功");
			done.run(rspClientLogin.build());
		} catch (Exception e) {
			logger.error("客户端登录异常：" + e);
			rspClientLogin.setResult(HppConstants.RESULT_ERROR);
			rspClientLogin.setResultInfo("登录异常");
			done.run(rspClientLogin.build());
			throw new ExpectedException("", e);
		}
	}
	
	/**
	 * 客户端登出
	 */
	@Override
	public void clientLogout(RpcController controller, ReqClientInfo request, RpcCallback<RspClientInfo> done) {
		logger.info("开始客户端登出，Cmd：【" + CMD.CMD_MSG_CLIENT_LOGOUT_VALUE + "】 request：【" + request.toString() + "】 ");
		ClientVerifyProto.RspClientInfo.Builder rspClientLogout = ClientVerifyProto.RspClientInfo.newBuilder();
		rspClientLogout.setCmd(CMD.CMD_MSG_CLIENT_LOGOUT_VALUE);// cmd
		try {
			// 获取客户端ip地址
			RpcControllerImpl rpcControllerImpl = (RpcControllerImpl)controller;
			long sessionId = rpcControllerImpl.getSessionId();
			HppChannel hppChannel = ContextUtil.getHppChannel(HppConstants.TYPE_SERVER + sessionId);
			if(null == hppChannel){// Coverity提示需要加上非空判断
				rspClientLogout.setResult(HppConstants.RESULT_ERROR);
				rspClientLogout.setResultInfo("系统错误");
				done.run(rspClientLogout.build());
				logger.error("结束 根据录像状态获取录像信息  请求处理异常: hppChannel为空");
				return;
			}
			String clientIP = ((InetSocketAddress)hppChannel.getSession().getRemoteAddress()).getAddress().getHostAddress();
			HeartbeatCache.removeHeartBeat(request.getUserId(), null);
			com.hikvision.finance.fms.model.Users user = usersDao.get(request.getUserId());
			com.hikvision.finance.fms.model.Logsys logsys = new com.hikvision.finance.fms.model.Logsys();
			logsys.setNmoduleType(ModuleType.COLLECT);//采集还是中心登录
			if (user != null) {
				logsys.setStrUser(user.getStrName());
			}
			logsys.setNtype(LoginStatus.LOGIN_OUT);
			logsys.setStrDate(new Date());
			logsys.setStrIp(clientIP);
			logsysDao.save(logsys);
			rspClientLogout.setResult(HppConstants.RESULT_OK);
			rspClientLogout.setResultInfo("登出成功");
			done.run(rspClientLogout.build());
		} catch (Exception e) {
			logger.error("客户端登出异常：" + e);
			rspClientLogout.setResult(HppConstants.RESULT_ERROR);
			rspClientLogout.setResultInfo("登出异常");
			done.run(rspClientLogout.build());
			throw new CMSException(CMSException.UNKNOWN_ERR, e);
		}	
	}
	
	/**
	 * 心跳
	 */
	@Override
	public void workState(RpcController controller, ReqClientInfo request, RpcCallback<RspClientInfo> done) {
		
		logger.info("开始客户端心跳，Cmd：【" + CMD.CMD_WRITE_WORK_STATE_VALUE + "】 request：【" + request.toString() + "】 ");
		ClientVerifyProto.RspClientInfo.Builder rspClientWorkState = ClientVerifyProto.RspClientInfo.newBuilder();
		rspClientWorkState.setCmd(CMD.CMD_WRITE_WORK_STATE_VALUE);// cmd
		try {

			//如何有去判断修改密码和断网的情况? 心跳中是否存在userId
			if(HeartbeatCache.isExistUser(request.getUserId())) {
				HeartbeatCache.setHeartBeatTime(request.getUserId(), null);
			}else{//修改密码后, 会清空缓存
				rspClientWorkState.setIsModifyPwd(1); //修改过密码, CS客户端注销
			}
			rspClientWorkState.setResult(HppConstants.RESULT_OK);
			rspClientWorkState.setResultInfo("心跳成功");
			done.run(rspClientWorkState.build());
		} catch (Exception e) {
			logger.error("客户端心跳异常：" + e);
			rspClientWorkState.setResult(HppConstants.RESULT_ERROR);
			rspClientWorkState.setResultInfo("心跳异常");
			done.run(rspClientWorkState.build());
		}	
	}


	/**
	 * 验证授权信息
	 * @author jinchenwu 2016年10月17日 上午10:33:14
	 * @return
	 */
	private String getAuthorizeInfo() {
		Softdongle softdog = softdogDao.findUniqueBy("ntype", DOGTYPE.DOG_VERSION);
		// 1，验证数据库授权是否存在
		if (null == softdog) {
			return GlobalMessageUtil.getMessage("login.auth.notexist");
		}
		// 2，验证服务狗授权文件是否存在，若不存在，则数据库不会维护"更新时间"字段
		Date now = softdogDao.getSysDate();
		long diffTime = (now.getTime() - softdog.getDtUpdateTime().getTime()) / 1000;
		if (diffTime > Constants.SOFT_TIME) {
			logger.error("version check, 正常时间间隔为" + Constants.SOFT_TIME + "s,加密狗没有更新时间间隔为" + diffTime + "s");
			return GlobalMessageUtil.getMessage("login.auth.stop");
		}
		// 3，非正式版检查是否过期
		if (softdog.getNvalue() != DOGVALUE.AUTH_VAIL) {
			if (null != softdog.getStrDescribe()) {
				Date expiryDate = DateUtils.getDateTime(DateUtils.yyyy_MM_dd_HH_mm_ss, softdog.getStrDescribe());
				if (!expiryDate.after(now)) {
					// "试用版本，授权过期";
					return GlobalMessageUtil.getMessage("login.auth.expire");
				}
			} else {
				// "试用版本，试用时间不存在";
				return GlobalMessageUtil.getMessage("login.auth.expire");
			}
		}
		return null;
	}
	
	/**
	 * 验证用户信息
	 * @author jinchenwu 2016年10月11日 下午5:44:28
	 * @param request
	 * @param rspClientLogin
	 * @return
	 */
	private ClientVerifyProto.RspClientLogin.Builder verifyLoginUser(ReqClientLogin request,
			ClientVerifyProto.RspClientLogin.Builder rspClientLogin, String serverIP) {

		//c/s客户端日志上报功能是否启用，若启用，再赋值日志上报的方式
		String strChoiceUsbIpc = FmsPropertyPlaceholder.getProperty("choiceUsbIpc");
		if (StringUtils.isEmpty(strChoiceUsbIpc)) {
			throw new ExpectedException("", "未获取到通道方式");
		}
		if ("usb".equals(strChoiceUsbIpc)) {
			String isLogReport = FmsPropertyPlaceholder.getProperty("logReport");
			if (StringUtils.isEmpty(isLogReport)) {
				throw new ExpectedException("", "未获取到c/s客户端日志上报功能的状态");
			}
			if ("true".equals(isLogReport)) {
				rspClientLogin.setLogReport(Constants.LogReportStatus.ENABLED);
				
				String strLogReportMethod = FmsPropertyPlaceholder.getProperty("logReportMethod");
				if (StringUtils.isEmpty(strLogReportMethod)) {
					throw new ExpectedException("", "未获取到c/s客户端日志上报的方式");
				}
				if (strLogReportMethod.equals("ftp")) {
					rspClientLogin.setLogReportMethod(Constants.LogReportMethodStatus.FTP);
				} else {
					rspClientLogin.setLogReportMethod(Constants.LogReportMethodStatus.HTTP);
				}
			} else {
				rspClientLogin.setLogReport(Constants.LogReportStatus.FORBIDDEN);
			}
		}else{
			rspClientLogin.setResult(HppConstants.RESULT_ERROR);
			rspClientLogin.setResultInfo("IPC模式下, CS客户端不允许登录");
			return rspClientLogin;
		}
		// 获取系统配置信息
		List<Integer> nkeys = Lists.newArrayList(SysConfigType.PWD_FRESH_DAYS, SysConfigType.DEFAULT_PASSWORD,
				SysConfigType.PASSWORD_SECRUITY_LEVEL, SysConfigType.PLATFORM_DISCRIBE, SysConfigType.PLATFORM_COPYRIGHT,
				SysConfigType.VERSION_URL, SysConfigType.LOGUPLOAD_URL, SysConfigType.FAIL_COUNT, SysConfigType.DISTANCE_MINUTES, SysConfigType.VERSION_NO,
				SysConfigType.FORCE_UPDATE);
		SysconfigQo sysconfigQo = new SysconfigQo();
		sysconfigQo.setNkeys(nkeys);
		List<Sysconfig> sysconfigList = sysconfigDao.queryList(sysconfigQo);
		Map<Integer, String> sysconfigMap = new HashMap<Integer, String>();
		for (Sysconfig sysconfig : sysconfigList) {
			sysconfigMap.put(sysconfig.getNkey(), sysconfig.getStrValue());
		}
		
		//平台描述文字
		if (sysconfigMap.get(SysConfigType.PLATFORM_DISCRIBE) != null){
			rspClientLogin.setCustomTitle(sysconfigMap.get(SysConfigType.PLATFORM_DISCRIBE));
		}
		//版权描述文字
		if (sysconfigMap.get(SysConfigType.PLATFORM_COPYRIGHT) != null){
			rspClientLogin.setCopyright(sysconfigMap.get(SysConfigType.PLATFORM_COPYRIGHT));
		}
		//CSClient日志上传路径
		if (sysconfigMap.get(SysConfigType.VERSION_URL) != null){
			rspClientLogin.setVersionUrl("http://" + serverIP + sysconfigMap.get(SysConfigType.VERSION_URL));
		}
		//CSClient版本更新路径
		if (sysconfigMap.get(SysConfigType.LOGUPLOAD_URL) != null){
			rspClientLogin.setLogUploadUrl("http://" + serverIP + sysconfigMap.get(SysConfigType.LOGUPLOAD_URL));
		}
		//CSClient最新版本号
		if (sysconfigMap.get(SysConfigType.VERSION_NO) != null){
			rspClientLogin.setVersionNo(sysconfigMap.get(SysConfigType.VERSION_NO));
		}
		//CSClient强制升级(1:启用 0:禁用)
		if (sysconfigMap.get(SysConfigType.FORCE_UPDATE) != null){
			rspClientLogin.setIsForceUpdate(sysconfigMap.get(SysConfigType.FORCE_UPDATE));
		}

		// 该类型是基本数据类型, 默认值就是0, 0表示修改过密码, 要设置为-1, 做区分.
		rspClientLogin.setIsInitalPwd(-1);
		// 0.进行账户锁定的相关操作
		String failCount = sysconfigMap.get(SysConfigType.FAIL_COUNT);
		String distanceMinutes = sysconfigMap.get(SysConfigType.DISTANCE_MINUTES);
		if (failCount == null  || distanceMinutes == null) {
			rspClientLogin.setResult(HppConstants.RESULT_ERROR);
			rspClientLogin.setResultInfo("获取系统参数异常");
			return rspClientLogin;
		}
		
		//验证用户锁定
		Userlock userlock = checkUserLock(request.getUserName(), failCount, distanceMinutes, rspClientLogin);
		if (HppConstants.RESULT_ERROR == rspClientLogin.getResult()) {
			return rspClientLogin;
		}
		
		UsersQo userqo = new UsersQo();
		userqo.setStrName(request.getUserName());
		Users user = usersDao.queryUnique(userqo);
		
		// 1.用户名不存在
		if (null == user) {
			rspClientLogin.setResult(HppConstants.RESULT_ERROR);
			rspClientLogin.setResultInfo("用户名不存在");
			return rspClientLogin;
		}
		
		// 2.中心管理员账户不能登录
		if (SysRoleEnum.CENTRY_ADMIN_0.getCode().equals(user.getSysRole().toString())) {
			rspClientLogin.setResult(HppConstants.RESULT_ERROR);
			rspClientLogin.setResultInfo("中心管理员无法登录理财客户端");
			return rspClientLogin;
		}
		// 3.用户已经禁用
		if (UserStatus.DISABLE == user.getNstate()) {
			rspClientLogin.setResult(HppConstants.RESULT_ERROR);
			rspClientLogin.setResultInfo("该用户已禁用");
			return rspClientLogin;
		}
		// 4.用户已经到期
		Date userEndDate = DateUtils.addDays(user.getStrDate(), 1);
		if (userEndDate.before(new Date())) {// 到期日当天不算过期
			rspClientLogin.setResult(HppConstants.RESULT_ERROR);
			rspClientLogin.setResultInfo("该用户已到期");
			return rspClientLogin;
		}
		if (user.getStrPassword() != null && !(user.getStrPassword().equals(request.getUserPwd()))) {//密码不正确
			//保存或更新账户检查信息
			userlock = saveOrUpdateUserCheck(userlock, user);
			rspClientLogin.setResult(HppConstants.RESULT_ERROR);
			Integer leftCount = Integer.valueOf(failCount) - userlock.getNfailCount();
			String errorMsg = "";
			if (leftCount == 0) {
				errorMsg = Integer.valueOf(failCount) + "次输入错误密码, 请"+ distanceMinutes +"分钟后再试";
			}else {
				errorMsg = "输入错误，还有"+ leftCount +"次机会";
			}
			rspClientLogin.setResultInfo(errorMsg);
			return rspClientLogin;
		}else {
			if (userlock != null){
				userlockDao.deleteById(userlock.getId());
			}
		}
		rspClientLogin.setUserId(user.getId());
		rspClientLogin.setUserNo(user.getUserNo());
		rspClientLogin.setStrName(user.getStrCode());
		com.hikvision.finance.fms.model.Region dbRegion = regionDao.get(user.getNetId());
		if (dbRegion == null) {
			rspClientLogin.setResult(HppConstants.RESULT_ERROR);
			rspClientLogin.setResultInfo("用户区域信息不存在");
			return rspClientLogin;
		}
		rspClientLogin.setNetId(user.getNetId());
		rspClientLogin.setNet(dbRegion.getStrName());
		// 返回系统密码等级，修改密码时使用
		rspClientLogin.setUserLevel(Integer.valueOf(sysconfigMap.get(SysConfigType.PASSWORD_SECRUITY_LEVEL)));
		
		// 5.判断首次登录，首次登录是初始密码，需要修改密码
		if (null == user.getLastTimeOnline()) {
			rspClientLogin.setResult(HppConstants.RESULT_ERROR);
			rspClientLogin.setResultInfo("首次登陆平台");
			rspClientLogin.setIsInitalPwd(PasswordType.INITAL_PASSWORD);
			return rspClientLogin;
		}
		
		// 6.判断密码是否为初始密码，初始密码需要修改密码
		if (user.getStrPassword().equals(sysconfigMap.get(SysConfigType.DEFAULT_PASSWORD))) {
			rspClientLogin.setResult(HppConstants.RESULT_ERROR);
			rspClientLogin.setResultInfo("密码为初始密码");
			rspClientLogin.setIsInitalPwd(PasswordType.INITAL_PASSWORD);
			return rspClientLogin;
		}
		
		// 7.判断密码是否到期，到期需要修改密码
		Integer passwordFresh = Integer.parseInt(sysconfigMap.get(SysConfigType.PWD_FRESH_DAYS));// 密码保险天数
		if (null != passwordFresh && passwordFresh != 0) {
			Date date = DateUtils.addDays(new Date(), -passwordFresh);// 当前时间-保鲜天数
			if (user.getStrUpdatePwdTime().before(date)) {
				rspClientLogin.setResult(HppConstants.RESULT_ERROR);
				rspClientLogin.setResultInfo("密码超过保鲜期");
				return rspClientLogin;
			}
		}
		
		// 8.判断用户是否重复登录
		if(HeartbeatCache.isOnline(user.getId())){
			rspClientLogin.setResult(HppConstants.RESULT_ERROR);
			rspClientLogin.setResultInfo("用户已经登录，不能重复登录！");
			return rspClientLogin;
		}
		return rspClientLogin;
	}
	
	/**
	 * 保存或更新账户检查信息
	 * @author zhoujiajun 2016年11月7日 下午1:49:57
	 * @param userlock
	 * @param user
	 */
	private Userlock saveOrUpdateUserCheck(Userlock userlock, Users user) {
		
		if (userlock != null) { 
			userlock.setNfailCount(userlock.getNfailCount() + 1);
			userlock.setDtLoginTime(new Date());
			userlockDao.update(userlock);
		}else {
			userlock = new Userlock();
			userlock.setStrName(user.getStrName());
			userlock.setUserId(user.getId());
			userlock.setNfailCount(1);
			userlock.setDtLoginTime(new Date());
			userlockDao.save(userlock);
		}
		return userlock;
	}
	
	/**
	 * 验证用户锁定
	 * @author zhoujiajun 2016年11月7日 下午1:34:41
	 * @param userName
	 * @param failCount
	 * @param distanceMinutes
	 * @param rspClientLogin
	 * @return
	 */
	private Userlock checkUserLock(String userName, String failCount, String distanceMinutes, ClientVerifyProto.RspClientLogin.Builder rspClientLogin) {
		
		UserlockQo userlockQo = new UserlockQo();
		userlockQo.setStrName(userName);
		Userlock userlock = userlockDao.queryUnique(userlockQo);
		if (userlock != null) {
			Integer dbfailCount = userlock.getNfailCount();
			Date lastLoginTime = userlock.getDtLoginTime();
			//在规定的输入密码错误的次数内, 如果有该用户的锁定时间超过给定的分钟数, 那么要删除该数据
			CheckDateVO checkDateVO = checkDate(lastLoginTime, Integer.valueOf(distanceMinutes));
			if (checkDateVO.getCanLogin()) {
				userlockDao.deleteById(userlock.getId());
				//清除内存中的用户锁定信息
				userlock = null; 
				dbfailCount = 0;
			}
			if (dbfailCount >= Integer.valueOf(failCount)) {  //登录次数超过限制
				// 最后一次登录时间 + 系统规定的相隔分钟数 与 当前时间进行比较, 如果< 就能进行登录
				if (!checkDateVO.getCanLogin()){ //不能进行登录
					rspClientLogin.setResult(HppConstants.RESULT_ERROR);
					rspClientLogin.setResultInfo(Integer.valueOf(failCount) + "次输入错误密码, 请"+ checkDateVO.getLeftMintes() +"分钟后再试");
					return null;
				}
			}
			return userlock;
		}
		return null;
	}
	
	/**
	 * 检测最后一次登录+限制时间 是否大于当前时间, 大于当前时间(不能登录), 小于等于当前时间(可以登录)
	 * @author zhoujiajun 2016年11月4日 下午2:14:21
	 * @param lastLoginTime
	 * @param distanceMinutes
	 * @return
	 */
	private CheckDateVO checkDate(Date lastLoginTime, Integer distanceMinutes) {
		
		CheckDateVO checkDateVO = new CheckDateVO();
		Calendar calendar = Calendar.getInstance();
		calendar.setTime(lastLoginTime);
		calendar.add(Calendar.MINUTE, distanceMinutes);
		lastLoginTime = calendar.getTime();
		Date currentDate = softdogDao.getSysDate();
		if (lastLoginTime.compareTo(currentDate) > 0) { // 表示大于当前时间(不能登录)
			Long timeOne=calendar.getTimeInMillis();
			calendar.setTime(currentDate);
			Long timeTwo=calendar.getTimeInMillis();
			Long leftMintes = (timeOne-timeTwo) / (1000 * 60) + 1;//转化minute
			checkDateVO.setCanLogin(false);
			checkDateVO.setLeftMintes(leftMintes);
		}else { // 表示小于当前时间(可以登录)
			checkDateVO.setCanLogin(true);
		}
		return checkDateVO;
	}
	
	/*
	 * 修改密码
	 * @see com.hikvision.finance.fms.cms.hpp.verify.ClientVerifyProto.ClientVerifyService#changePassword(com.google.protobuf.RpcController,
	 * com.hikvision.finance.fms.cms.hpp.verify.ClientVerifyProto.ReqChangePassword, com.google.protobuf.RpcCallback)
	 */
	@Override
	public void changePassword(RpcController controller, ReqChangePassword request, RpcCallback<RspClientLogin> done) {
		logger.info("开始修改密码，Cmd：【" + CMD.CMD_DT_CHANGE_PASSWORD_VALUE + "】 request：【" + request.toString() + "】 ");
		ClientVerifyProto.RspClientLogin.Builder rspClientLogin = ClientVerifyProto.RspClientLogin.newBuilder();
		rspClientLogin.setCmd(CMD.CMD_DT_CHANGE_PASSWORD_VALUE);// cmd
		try {
			Users user = usersDao.get(request.getUserId());
			if (user == null) {
				rspClientLogin.setResult(HppConstants.RESULT_ERROR);
				rspClientLogin.setResultInfo("用户不存在");
				done.run(rspClientLogin.build());
				return;
			}
			if (!user.getStrPassword().equals(request.getOldPwd())) {
				rspClientLogin.setResult(HppConstants.RESULT_ERROR);
				rspClientLogin.setResultInfo("原密码不正确");
				done.run(rspClientLogin.build());
				return;
			}
			
			if (request.getNewPwd().equals(request.getOldPwd())) {
				rspClientLogin.setResult(HppConstants.RESULT_ERROR);
				rspClientLogin.setResultInfo("原密码与新密码相同");
				done.run(rspClientLogin.build());
				return;
			}
			
			// 获取系统配置信息
			List<Integer> nkeys = Lists.newArrayList(SysConfigType.DEFAULT_PASSWORD);
			SysconfigQo sysconfigQo = new SysconfigQo();
			sysconfigQo.setNkeys(nkeys);
			List<Sysconfig> sysconfigList = sysconfigDao.queryList(sysconfigQo);
			if (sysconfigList == null) {
				rspClientLogin.setResult(HppConstants.RESULT_ERROR);
				rspClientLogin.setResultInfo("获取系统配置信息错误");
				done.run(rspClientLogin.build());
				return;
			}
			if (request.getNewPwd().equals(sysconfigList.get(0).getStrValue())) {
				rspClientLogin.setResult(HppConstants.RESULT_ERROR);
				rspClientLogin.setResultInfo("新密码与平台初始密码一致");
				done.run(rspClientLogin.build());
				return;
			}
			
			// 更新密码、修改密码时间
			user.setStrPassword(request.getNewPwd());
			user.setStrUpdatePwdTime(new Date());
			user.setLastTimeOnline(new Date());
			usersDao.update(user);
			rspClientLogin.setResult(HppConstants.RESULT_OK);
			rspClientLogin.setResultInfo("操作成功");
			done.run(rspClientLogin.build());
		} catch (Exception e) {
			logger.error("修改密码异常：" + e);
			rspClientLogin.setResult(HppConstants.RESULT_ERROR);
			rspClientLogin.setResultInfo("修改密码异常");
			done.run(rspClientLogin.build());
		}
	}
	
	public IUsersDao getUsersDao() {
		return usersDao;
	}
	
	public void setUsersDao(IUsersDao usersDao) {
		this.usersDao = usersDao;
	}
	
	public ISysconfigDao getSysconfigDao() {
		return sysconfigDao;
	}
	
	public void setSysconfigDao(ISysconfigDao sysconfigDao) {
		this.sysconfigDao = sysconfigDao;
	}
	
	public ISoftdogDao getSoftdogDao() {
		return softdogDao;
	}
	
	public void setSoftdogDao(ISoftdogDao softdogDao) {
		this.softdogDao = softdogDao;
	}

	public ILogsysDao getLogsysDao() {
		return logsysDao;
	}

	public void setLogsysDao(ILogsysDao logsysDao) {
		this.logsysDao = logsysDao;
	}

	public IUserlockDao getUserlockDao() {
		return userlockDao;
	}

	public void setUserlockDao(IUserlockDao userlockDao) {
		this.userlockDao = userlockDao;
	}

	public IRegionDao getRegionDao() {
		return regionDao;
	}

	public void setRegionDao(IRegionDao regionDao) {
		this.regionDao = regionDao;
	}

}
