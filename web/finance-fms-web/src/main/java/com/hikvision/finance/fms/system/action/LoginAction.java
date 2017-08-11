/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月23日 上午9:54:21
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.system.action;

import java.util.Calendar;
import java.util.Date;
import java.util.List;

import javax.servlet.http.HttpServletRequest;

import org.apache.struts2.ServletActionContext;

import com.hikvision.cms.cache.manager.DataManager;
import com.hikvision.finance.core.util.DateUtils;
import com.hikvision.finance.core.util.StringUtils;
import com.hikvision.finance.core.util.UUIDUtils;
import com.hikvision.finance.core.util.cm.ConfigManager;
import com.hikvision.finance.fms.cms.hppservice.model.CheckDateVO;
import com.hikvision.finance.fms.common.cache.HeartbeatCache;
import com.hikvision.finance.fms.common.enums.LogModuledEnum;
import com.hikvision.finance.fms.common.enums.SysRoleEnum;
import com.hikvision.finance.fms.common.model.MenuBean;
import com.hikvision.finance.fms.common.model.UserSession;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.common.util.Constants.FailureType;
import com.hikvision.finance.fms.common.util.Constants.OperStatus;
import com.hikvision.finance.fms.common.util.Constants.SessionAttrbutes;
import com.hikvision.finance.fms.common.util.Constants.UserStatus;
import com.hikvision.finance.fms.common.util.SessionUtil;
import com.hikvision.finance.fms.common.util.ValidateUtil;
import com.hikvision.finance.fms.license.dao.ISoftdogDao;
import com.hikvision.finance.fms.model.Businessorder;
import com.hikvision.finance.fms.model.Privilege;
import com.hikvision.finance.fms.model.Userlock;
import com.hikvision.finance.fms.model.Users;
import com.hikvision.finance.fms.modules.alarm.dto.AlarmlogInfo;
import com.hikvision.finance.fms.modules.alarm.service.IAlarmlogService;
import com.hikvision.finance.fms.modules.analysis.dto.AnalysisInfo;
import com.hikvision.finance.fms.modules.analysis.service.IAnalysisService;
import com.hikvision.finance.fms.modules.duerecord.dto.DueRecordInfo;
import com.hikvision.finance.fms.modules.duerecord.service.IDueRecordService;
import com.hikvision.finance.fms.modules.log.dao.IConfigLogDao;
import com.hikvision.finance.fms.modules.log.service.ILogService;
import com.hikvision.finance.fms.modules.role.dao.IPrivilegeDao;
import com.hikvision.finance.fms.modules.sysconfig.dto.SysconfigDto;
import com.hikvision.finance.fms.modules.sysconfig.service.ISysconfigService;
import com.hikvision.finance.fms.modules.user.dao.IUserlockDao;
import com.hikvision.finance.fms.modules.user.dao.IUsersDao;
import com.hikvision.finance.fms.modules.user.qo.UserlockQo;
import com.hikvision.finance.fms.modules.user.service.IUserService;
import com.hikvision.finance.fms.system.service.ILoginService;
import com.hikvision.finance.fms.webdevice.contact.service.IContactService;
import com.hikvision.finance.fwork.exception.ExpectedException;
import com.hikvision.finance.fwork.util.AjaxData;
import com.hikvision.finance.fwork.util.CookieUtil;
import com.hikvision.finance.fwork.util.GlobalMessageUtil;
import com.hikvision.finance.fwork.util.ServletUtil;
import com.hikvision.finance.fwork.www.action.BaseAction;
import com.hikvision.finance.fwork.www.session.HikSession;
import com.hikvision.finance.fwork.www.session.HikWebSession;

/**
 * 登录、登出相关Action类
 * @author jinxindong 2016年2月23日 上午9:54:21
 * @version V1.0
 */
public class LoginAction extends BaseAction<Users> {
	
	/**
	 * 序列化ID
	 */
	private static final long serialVersionUID = -7621908261093609781L;
	private ILoginService loginService;
	private String menuCode;
	private IUserService userService;
	private IPrivilegeDao privilegeDao;
	private ISysconfigService sysconfigService;
	private ISoftdogDao softdogDao;
    private ILogService logService;
    private IConfigLogDao configLogDao;
	private String oldPwd;
	private String newPwd;
	private String username;
	private String passwordSecurityLevel;
	private DataManager dataManager;
	private Integer flag;
	private Privilege privilege;
	private List<AlarmlogInfo> alarmlogList;
	private IAlarmlogService alarmlogService;
	private AnalysisInfo analysisInfo;
	private IAnalysisService analysisService;
	private IDueRecordService dueRecordService;
	private IUsersDao usersDao;
	private String randomCode;
	private String osdenabled;//osd是否启用--返回到采集端
	private IContactService contactService;
	private Businessorder businessorder;
	private IUserlockDao userlockDao;
	/**
	 * 访问首页
	 */
	public String index() {		
		UserSession userSession = SessionUtil.getUserSession();
		if (1==userSession.getSysRole()) {
			return "home";
		} else {
			return "index";
		}
	}
	
	
	/**
	 * 登录方法 1、验证 2、设置session
	 * @author jinxindong 2016年2月24日 下午5:32:52
	 * @return
	 */
	public String login() {
		String method = ServletActionContext.getRequest().getMethod();
		if (!"POST".equals(method)) {// AppScan扫描出“查询中接受的主体参数”缺陷，需要禁用GET方法
			throw new ExpectedException("", "请求方法错误");
		}
		//前置检查： 三次登录失败  就不会传yzm,就需要去校验验证码
		if (!"yzm".equals(randomCode)) {
			if (ValidateUtil.isValidateCode(getRequest(), randomCode)) {
				ajaxData.setAutoSet(false);
				ajaxData.setFailureMsg(GlobalMessageUtil.getMessage("login.validate.code"));
				return AJAX;
			}
		}
		
		// 前置检查：授权检查
		String authorizeInfo = loginService.getAuthorizeInfo();
		if (authorizeInfo != null) {
			ajaxData.setAutoSet(false);
			ajaxData.setFailureMsg(authorizeInfo);
			return AJAX;
		}
		SysconfigDto sysconfigDto  = sysconfigService.getSysconfig();
		//验证用户锁定
		Userlock userlock = checkUserLock(entity.getStrName(), sysconfigDto.getFailCount(), sysconfigDto.getDistanceMinutes());
		if (!ajaxData.getSuccess()) {
			return AJAX;
		}
		// 1：检查用户身份
		List<Users> list = usersDao.findBy(new String[] {"strName"}, new Object[] {entity.getStrName()});
		 //1.1,用户名不存在
		if (list == null || list.size() == 0) {
			ajaxData.setAutoSet(false);
			ajaxData.setFailureMsg(GlobalMessageUtil.getMessage("login.usernameerror"));
			return AJAX;
		}
		//保留上次登录的用户名
		CookieUtil.setCookie(getResponse(), "HIK_COOKIE_NAME", entity.getStrName(), 3600 * 24 * 30, null, null);// 30天
		
		Users user = list.get(0); 	
		// 2.1：密码不匹配
		if (!entity.getStrPassword().equals(user.getStrPassword())) {
			//保存或更新账户检查信息
			userlock = saveOrUpdateUserCheck(userlock, user);
			ajaxData.setAutoSet(false);
			Integer leftCount = Integer.valueOf(sysconfigDto.getFailCount()) - userlock.getNfailCount();
			String errorMsg = "";
			if (leftCount == 0) {
				errorMsg = Integer.valueOf(sysconfigDto.getFailCount()) + "次输入错误密码, 请"+ sysconfigDto.getDistanceMinutes() +"分钟后再试";
			}else {
				errorMsg = "输入错误，还有"+ leftCount +"次机会";
			}
			ajaxData.setAutoSet(false);
			ajaxData.setFailureMsg(errorMsg);
			return AJAX;
		}else{
			if (userlock != null){
				userlockDao.deleteById(userlock.getId());
			}
		}
		
		//2.2，判断密码是否为初始密码
		String passwordDefault = sysconfigDto.getPasswordDefault();
		if (user.getStrPassword().equals(passwordDefault)) {	
			ajaxData.setAutoSet(false);
			ajaxData.put("type", FailureType.PASSWORD_DEFAULT);
			ajaxData.setFailureMsg(GlobalMessageUtil.getMessage("login.pwd.default"));
			return AJAX;
		}
		
		//2.3，判断首次登录，记录是否首次登录
		if(user.getLastTimeOnline()==null){
			ajaxData.setAutoSet(false);
			ajaxData.put("type", FailureType.PASSWORD_DEFAULT);
			ajaxData.setFailureMsg(GlobalMessageUtil.getMessage("login.login.first"));
			return AJAX;
		}
		//3：判断是否是系统管理员用户
		if (!"admin".equals(user.getStrName())) {
			//3.1，用户已经过期
			Date userEndDate = DateUtils.addDays(user.getStrDate(), 1);
			if (userEndDate.before(new Date())) {// 到期日当天不算过期
				ajaxData.setAutoSet(false);
				ajaxData.setFailureMsg(GlobalMessageUtil.getMessage("login.expire"));
				return AJAX;
			}
			//3.2，用户已经禁用
			if (user.getNstate() == UserStatus.DISABLE) {
				ajaxData.setAutoSet(false);
				ajaxData.setFailureMsg(GlobalMessageUtil.getMessage("login.disable"));
				return AJAX;
			}
			//3.3，验证密码保险期
			Integer passwordFresh = Integer.parseInt(sysconfigDto.getPasswordFresh());
			if (passwordFresh != null && passwordFresh != 0) {
				Calendar calendar = Calendar.getInstance();
				calendar.add(Calendar.DAY_OF_MONTH, -passwordFresh);
				Date date = calendar.getTime();
				if (user.getStrUpdatePwdTime().before(date)) {// 当天时间-保鲜天数
					ajaxData.setAutoSet(false);
					ajaxData.put("type", FailureType.PASSWORD_UNFRESH);
					ajaxData.setFailureMsg(GlobalMessageUtil.getMessage("login.pwd.unfresh"));
					return AJAX;
				}
			}
		}
		
		//4：判断中心管理员还是理财经理
		if (SysRoleEnum.FINANC_MANAGER_1.getCode().equals(user.getSysRole().toString())) {
			// 4.1.采集端登录的常规验证
			ajaxData = loginService.enableLogin(entity,user);
		} else {
			// 4.2、中心管理员 验证登录信息
			ajaxData = loginService.validateLoginUser(entity,user);
		}
		
		// 5：验证成功后构建session，给前段拦截器设置登录成功的信息头
		if (ajaxData.getSuccess()) { // 登录成功，设置session
			//5.1，构建session信息
			HikWebSession hikSession = new HikWebSession();
			hikSession = this.buildSession(ajaxData);
			UserSession userSession = (UserSession)hikSession.getAttribute(SessionAttrbutes.USER_SESSION);
			//5.2， 设置session到缓存中
			this.setSession(hikSession);
			userSession.setLoginIp((String)hikSession.getAttribute("ip"));
			//5.3，设置用户在线
			Users user1 = userService.get(userSession.getUserId());
			userSession.setUserRegionId(user1.getNetId());
			user1.setNonline(UserStatus.ONLINE);
			user1.setUserSessionId(hikSession.getAttribute("session").toString());
			userService.update(user1);
			//5.4，记录登录日志
			logService.saveSyslog(user.getStrName(), Constants.LoginStatus.LOGIN_IN,user.getSysRole());// 暂时先放在这里
		}
		ajaxData.getData().remove("user");
		ajaxData.getData().remove("role");
		ajaxData.getData().remove("menus");
		return AJAX;
		
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
	private Userlock checkUserLock(String userName, String failCount, String distanceMinutes) {
		
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
					ajaxData.setAutoSet(false);
					ajaxData.setFailureMsg(Integer.valueOf(failCount) + "次输入错误密码, 请"+ checkDateVO.getLeftMintes() +"分钟后再试");
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
	
	
	/**
	 * 系统登出
	 * @author jinxindong 2016年3月25日 下午6:05:52
	 * @return
	 */
	public String logout() {
		try {
			UserSession userSession = SessionUtil.getUserSession();
			Integer userId = userSession.getUserId();
			Integer channelId = userSession.getDeviceChannelId();
			// 移除session
			// 记录登出日志
			logService.saveSyslog(userSession.getUserName(), Constants.LoginStatus.LOGIN_OUT,userSession.getSysRole());
			sessionProcessor.removeSession(getResponse(), getRequest());
			// 最后移除心跳信息
			HeartbeatCache.removeHeartBeat(userId, channelId);
		} catch (Exception e) {
			ajaxData.setFailureMsg("数据库异常");
			ajaxData.setAutoSet(false);
		}
		return AJAX;
	}
	
	
	
	/**
	 * 跳转修改密码页
	 * @author jinxindong 2016年4月1日 下午5:29:48
	 * @return
	 */
	public String toChangePassword(){
		username = sessionProcessor.getSession(getRequest()).getUserName();
		passwordSecurityLevel = sysconfigService.getSysconfig().getPasswordSecruityLevel();
		operPage = "/web/changePassword.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 跳转修改保鲜密码或初始密码页
	 * @author jinxindong 2016年4月1日 下午5:30:10
	 * @return
	 */
	public String toPasswordUnfresh() {
		passwordSecurityLevel = sysconfigService.getSysconfig().getPasswordSecruityLevel();
		operPage = "/web/pwdUnfresh.jsp";
		return DISPATCHER;
	}
	
	/**
	 * cs客户端跳转修改密码页面
	 * @author zhoujiajun 2016年11月26日 上午11:57:30
	 * @return
	 */
	public String csToChangePwd(){
		passwordSecurityLevel = sysconfigService.getSysconfig().getPasswordSecruityLevel();
		return "pwd";
	}
	
	/**
	 * cs客户端修改密码
	 * @author zhoujiajun 2016年11月26日 上午11:57:30
	 * @return
	 */
	public String csChangePwd(){
		ajaxData = userService.updatePassword(username, oldPwd, newPwd);
		if(ajaxData.getSuccess()){
			Users user = userService.getUserByName(username);
			configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.AUTH.getCode(),GlobalMessageUtil.getMessage("log.users.uppwd",new String[] {user.getStrName()}), username);
			// 记录登出日志
			logService.saveSyslog(user.getStrCode(), Constants.LoginStatus.LOGIN_OUT,user.getSysRole());
			HeartbeatCache.removeHeartBeat(user.getId(), null); //移除缓存中的用户登陆信息
		}
		return AJAX;
	}
	
	/**
	 * 修改密码
	 * @author jinxindong 2016年4月1日 下午5:30:23
	 * @return
	 */
	public String changePassword() {
		HikSession webSession = sessionProcessor.getSession(getRequest());
		if(webSession == null) {
			success = false;
			msg = "用户登录已过期";
			return AJAX;
		}
		Integer userId = Integer.valueOf(webSession.getUserId());
		ajaxData = userService.updatePassword(userId, oldPwd, newPwd);
		if(ajaxData.getSuccess()){
			Users user = userService.get(userId);
			configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.AUTH.getCode(),GlobalMessageUtil.getMessage("log.users.uppwd",new String[] {user.getStrName()}));
			// 记录登出日志
			logService.saveSyslog(user.getStrCode(), Constants.LoginStatus.LOGIN_OUT,user.getSysRole());
			HeartbeatCache.removeHeartBeat(userId, null); //移除缓存中的用户登陆信息
			sessionProcessor.removeSession(getResponse(), getRequest());
		}
		return AJAX;
	}
	
	/**
	 * 修改保鲜或者初始密码
	 * @author jinxindong 2016年4月1日 下午5:31:15
	 * @return
	 */
	public String changePasswordNoLogin() {
		ajaxData = userService.updatePassword(username, oldPwd, newPwd);
		configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.AUTH.getCode(),GlobalMessageUtil.getMessage("log.users.uppwd",new String[] {username}),username);
		return AJAX;
	}
	
	/**
	 * 安装说明页面跳转
	 * @author jinxindong 2016年4月19日 下午2:51:35
	 * @return
	 */
	public String toUploadDesc(){
		operPage = "/web/module/setupState.jsp";
		return DISPATCHER;
	}
	
	
	/**
	 * 菜单导航
	 * @author jinxindong 2016年2月25日 下午12:29:46
	 * @return
	 */
	@SuppressWarnings("unchecked")
    public String menuNavigation(){
		List<MenuBean> menus = (List<MenuBean>)getSession().getAttribute(SessionAttrbutes.MENUS);
		String requestUrl = getRequestUrl();
		menuCode = loginService.getRequestMenuCode(menus, requestUrl);
		if(menuCode != null) {
			return SUCCESS;
		} else {
			return ERROR;
		}
	}
	
	/**
	 * 查询有权限菜单的第一个菜单
	 * @author jinxindong 2016年4月12日 上午9:46:09
	 * @return
	 */
	public String getfirstmenu() {
		String method = ServletActionContext.getRequest().getMethod();
		if ("POST".equals(method)) {// AppScan扫描出“查询中接受的主体参数”缺陷，需要禁用GET方法
			privilege = privilegeDao.getFristByPower(flag);
			ajaxData.put("privilege", privilege);
		} else {
			throw new ExpectedException("", "请求方法错误");
		}
		return AJAX;
	}
	
	/**
	 * 首页获取报警数据
	 * @author jinxindong 2016年4月14日 下午2:27:35
	 * @return
	 */
	public String getIndexAlarm() {
		alarmlogList = alarmlogService.getIndexAlarm();
		ajaxData.put("alarmlogList", alarmlogList);
		return AJAX;
	}
	
	/**
	 * 获取首页合规统计饼图
	 * @author jinxindong 2016年4月19日 上午9:42:43
	 * @return
	 */
	public String getIndexRateNum() {
		AnalysisInfo info = new AnalysisInfo();
		info.setNregionId(SessionUtil.getUserSession().getRootOrgId());
		analysisInfo = analysisService.getIndexRateNum(info);
		ajaxData.put("analysisInfo", analysisInfo);
		return AJAX;
	}
	
	/**
	 * 首页获取到期信息
	 * @author jinchenwu 2016年4月19日 上午10:37:51
	 * @return
	 */
	public String getIndexDueRecord(){
		List<DueRecordInfo> dueRecordInfoList = dueRecordService.getIndexDueRecord(6);
		ajaxData.put("dueRecordInfoList", dueRecordInfoList);
		return AJAX;
	}
	
	/**
	 * 登录页获取授权信息
	 * @author jinchenwu 2016年11月1日 下午3:54:24
	 * @return
	 */
	public String getLicense(){
		ajaxData = loginService.getLicense();
		return AJAX;
	}
	
	//==================私有方法==================================================
	/**
	 * 设置session信息到缓存
	 * 前段拦截器根据session内容（设置头信息）来判断是否登录
	 * @author jinxindong 2016年2月25日 下午3:04:36
	 * @param hikSession
	 * @return
	 */
	private boolean setSession(HikWebSession hikSession) {
		String expire = ConfigManager.getConfiguration("web-session", "session.expire");
		if(StringUtils.isEmpty(expire)) {
			expire = "20";
		}
		CookieUtil.setCookie(getResponse(), "HIK_COOKIE_STYLE", StringUtils.defaultIfBlank("", "default"), 30 * 24 * 60 * 60, "/", null);
		boolean result = sessionProcessor.setSession(getRequest(), getResponse(), hikSession, Integer.parseInt(expire.trim()) * 60 * 1000);
		return result;
	}
	
	
	/**
	 * 构建session信息
	 * @author jinxindong 2016年2月25日 下午3:06:45
	 * @param ajaxData
	 * @return
	 */
	@SuppressWarnings("unchecked")
    private HikWebSession buildSession(AjaxData ajaxData) {
		HikWebSession hikSession = new HikWebSession();
		Users user = (Users)ajaxData.get("user");
		UserSession userSession = (UserSession)ajaxData.get(SessionAttrbutes.USER_SESSION);
		List<MenuBean> menus = (List<MenuBean>)ajaxData.get("menus");
		
		//hikSession.setAttribute(HikWebSession.KEY_SESSION_ID, SessionAttrbutes.SESSIONPRX+UUIDUtils.getUUID());
		hikSession.setAttribute(HikWebSession.KEY_SESSION_ID, UUIDUtils.getUUID());
		hikSession.setAttribute(HikWebSession.KEY_SESSION_USER, user.getId());
		hikSession.setAttribute(HikWebSession.KEY_SESSION_NAME, user.getStrName());
		hikSession.setAttribute(SessionAttrbutes.MENUS, menus);
		hikSession.setAttribute(SessionAttrbutes.USER_SESSION, userSession);
		
		// 客户端ip、
		HttpServletRequest request = ServletActionContext.getRequest();
		String ip = ServletUtil.getRemortIP(request);
		hikSession.setAttribute(HikWebSession.KEY_SESSION_IP, ip);
		return hikSession;
	}
	
	// ===================getter、setter===========================================
	
	public ILoginService getLoginService() {
		return loginService;
	}
	
	public void setLoginService(ILoginService loginService) {
		this.loginService = loginService;
	}

	
    public String getMenuCode() {
    	return menuCode;
    }

	
    public void setMenuCode(String menuCode) {
    	this.menuCode = menuCode;
    }

	
    public IUserService getUserService() {
    	return userService;
    }

	
    public void setUserService(IUserService userService) {
    	this.userService = userService;
    }

	
    public String getOldPwd() {
    	return oldPwd;
    }

	
    public void setOldPwd(String oldPwd) {
    	this.oldPwd = oldPwd;
    }

	
    public String getNewPwd() {
    	return newPwd;
    }

	
    public void setNewPwd(String newPwd) {
    	this.newPwd = newPwd;
    }

	
    public String getUsername() {
    	return username;
    }

	
    public void setUsername(String username) {
    	this.username = username;
    }

	
    public String getPasswordSecurityLevel() {
    	return passwordSecurityLevel;
    }

	
    public void setPasswordSecurityLevel(String passwordSecurityLevel) {
    	this.passwordSecurityLevel = passwordSecurityLevel;
    }

	
    public ISysconfigService getSysconfigService() {
    	return sysconfigService;
    }

	
    public void setSysconfigService(ISysconfigService sysconfigService) {
    	this.sysconfigService = sysconfigService;
    }

	
    public DataManager getDataManager() {
    	return dataManager;
    }

	
    public void setDataManager(DataManager dataManager) {
    	this.dataManager = dataManager;
    }

	
    public IPrivilegeDao getPrivilegeDao() {
    	return privilegeDao;
    }

	
    public void setPrivilegeDao(IPrivilegeDao privilegeDao) {
    	this.privilegeDao = privilegeDao;
    }

	
    public Integer getFlag() {
    	return flag;
    }

	
    public void setFlag(Integer flag) {
    	this.flag = flag;
    }

	
    public Privilege getPrivilege() {
    	return privilege;
    }

	
    public void setPrivilege(Privilege privilege) {
    	this.privilege = privilege;
    }

    
    
    public List<AlarmlogInfo> getAlarmlogList() {
    	return alarmlogList;
    }

	
    public void setAlarmlogList(List<AlarmlogInfo> alarmlogList) {
    	this.alarmlogList = alarmlogList;
    }

	
    public IAlarmlogService getAlarmlogService() {
    	return alarmlogService;
    }

	
    public void setAlarmlogService(IAlarmlogService alarmlogService) {
    	this.alarmlogService = alarmlogService;
    }

	
    public AnalysisInfo getAnalysisInfo() {
    	return analysisInfo;
    }

	
    public void setAnalysisInfo(AnalysisInfo analysisInfo) {
    	this.analysisInfo = analysisInfo;
    }

	
    public IAnalysisService getAnalysisService() {
    	return analysisService;
    }

	
    public void setAnalysisService(IAnalysisService analysisService) {
    	this.analysisService = analysisService;
    }	
    public IUsersDao getUsersDao() {
    	return usersDao;
    }

	
    public void setUsersDao(IUsersDao usersDao) {
    	this.usersDao = usersDao;
    }

    public ISoftdogDao getSoftdogDao() {
    	return softdogDao;
    }

	
    public void setSoftdogDao(ISoftdogDao softdogDao) {
    	this.softdogDao = softdogDao;
    }
    
	//**************************采集端用到的--后面有添加的都拷到这个注释里面BEG*********************************************//

	public ILogService getLogService() {
		return logService;
	}
	
	public void setLogService(ILogService logService) {
		this.logService = logService;
	}
	
	public IDueRecordService getDueRecordService() {
		return dueRecordService;
	}
	
	public void setDueRecordService(IDueRecordService dueRecordService) {
		this.dueRecordService = dueRecordService;
	}

	
    public String getRandomCode() {
    	return randomCode;
    }

	
    public void setRandomCode(String randomCode) {
    	this.randomCode = randomCode;
    }


	public String getOsdenabled() {
		return osdenabled;
	}

	public void setOsdenabled(String osdenabled) {
		this.osdenabled = osdenabled;
	}

	public IContactService getContactService() {
		return contactService;
	}

	public void setContactService(IContactService contactService) {
		this.contactService = contactService;
	}

	public Businessorder getBusinessorder() {
		return businessorder;
	}

	public void setBusinessorder(Businessorder businessorder) {
		this.businessorder = businessorder;
	}


	
    public IConfigLogDao getConfigLogDao() {
    	return configLogDao;
    }


	
    public void setConfigLogDao(IConfigLogDao configLogDao) {
    	this.configLogDao = configLogDao;
    }


	public IUserlockDao getUserlockDao() {
		return userlockDao;
	}


	public void setUserlockDao(IUserlockDao userlockDao) {
		this.userlockDao = userlockDao;
	}

	//**************************************采集端用到的END***********************************************************//
	
}
