/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月24日 下午5:42:17
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.system.service.impl;

import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.google.common.base.Splitter;
import com.hikvision.finance.core.util.DateUtils;
import com.hikvision.finance.core.util.StringUtils;
import com.hikvision.finance.fms.common.cache.HeartbeatCache;
import com.hikvision.finance.fms.common.model.MenuBean;
import com.hikvision.finance.fms.common.model.UserSession;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.common.util.Constants.RoleType;
import com.hikvision.finance.fms.common.util.Constants.SessionAttrbutes;
import com.hikvision.finance.fms.license.dao.ISoftdogDao;
import com.hikvision.finance.fms.license.util.LicenseConstant.DOGTYPE;
import com.hikvision.finance.fms.model.Privilege;
import com.hikvision.finance.fms.model.Region;
import com.hikvision.finance.fms.model.Role;
import com.hikvision.finance.fms.model.Softdongle;
import com.hikvision.finance.fms.model.Sysconfig;
import com.hikvision.finance.fms.model.Users;
import com.hikvision.finance.fms.modules.region.dao.IRegionDao;
import com.hikvision.finance.fms.modules.role.dao.IPrivilegeDao;
import com.hikvision.finance.fms.modules.role.dao.IRoleDao;
import com.hikvision.finance.fms.modules.sysconfig.dao.ISysconfigDao;
import com.hikvision.finance.fms.modules.sysconfig.service.ISysconfigService;
import com.hikvision.finance.fms.modules.user.dao.IUsersDao;
import com.hikvision.finance.fms.system.service.ILoginService;
import com.hikvision.finance.fms.webdevice.datacollect.dao.IRecordsDao;
import com.hikvision.finance.fwork.util.AjaxData;
import com.hikvision.finance.fwork.util.GlobalMessageUtil;
import com.hikvision.license.constant.LicenseConstant.DOGVALUE;

/**
 * <p>
 * </p>
 * @author jinxindong 2016年2月24日 下午5:42:17
 * @version V1.0
 */
public class LoginServiceImpl implements ILoginService {
	
	private static Logger logger = LoggerFactory.getLogger(LoginServiceImpl.class);
	
	private IRecordsDao recordsDao;
	private IUsersDao usersDao;
	private IRoleDao  roleDao;
	private IPrivilegeDao  privilegeDao;
	private IRegionDao regionDao;
	private ISysconfigService sysconfigService; 
	private ISoftdogDao softdogDao;
	private ISysconfigDao sysconfigDao;
	
	/**
	 * 授权检查 
	 * @author jinxindong 2016年4月19日 下午3:45:49
	 * @return
	 */
	@Override
	public String getAuthorizeInfo() {
		Softdongle softdog = softdogDao.findUniqueBy("ntype", DOGTYPE.DOG_VERSION);
		// 1，验证数据库授权是否存在
		if (softdog == null) {
			return GlobalMessageUtil.getMessage("login.auth.notexist");
		}
		// 2，验证服务狗授权文件是否存在，若不存在，则数据库不会维护"更新时间"字段
		Date now = softdogDao.getSysDate();
		long diffTime = (now.getTime() - softdog.getDtUpdateTime().getTime()) / 1000;
		if (diffTime > Constants.SOFT_TIME) {
			logger.error("version check, 正常时间间隔为" + Constants.SOFT_TIME + "s,加密狗没有更新时间间隔为" + diffTime+"s");
			return GlobalMessageUtil.getMessage("login.auth.stop");
		}
		// 3，非正式版检查是否过期
		if (softdog.getNvalue() != DOGVALUE.AUTH_VAIL) {
			if (softdog.getStrDescribe() != null) {
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
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.system.service.ILoginService#validateLoginUser(com.hikvision.finance.fms.model.Users)
	 */
	@Override
	public AjaxData validateLoginUser(Users entity, Users user) {
		AjaxData ajaxData = new AjaxData(false);
		ajaxData.put("user", user);
		
		// 2：绑定角色判断
		List<Role> roleList = roleDao.getRoleByUserId(user);
		if (roleList == null || roleList.size()<1) {
			return ajaxData.setFailureMsg(GlobalMessageUtil.getMessage("login.userroleerror",
			        new String[] {entity.getStrName()}));
		} else {
			Role role = roleList.get(0);
			ajaxData.put("role", role);// 当前系统只支持单角色绑定
			// 3：获取角色菜单权限
			List<Privilege> powers = null;
			if (role.getNtype() == RoleType.SYSTEM_MANAGER) {
				//powers = privilegeDao.findBy(new String[] {"nenabled", "nsysNo"}, new Object[] {Constants.CONS_1,Constants.CONS_0});
				powers = privilegeDao.getPowers(Constants.CONS_1, Constants.CONS_0);
			} else {
				// 4：根据用户获取根节点(非admin)
				Region org = regionDao.getRootOrg(user);
				if (org == null) {
					return ajaxData.setFailureMsg(GlobalMessageUtil.getMessage("login.region.root"));
				}
				powers = privilegeDao.getPowersByRoleId(role.getId());
			}
			List<MenuBean> menus = buildMenus(powers, "0"); // strSuper为0是一级菜单
			UserSession userSession = buildUserSession(user, role, powers);
			
			ajaxData.put("menus", menus);
			ajaxData.put(SessionAttrbutes.USER_SESSION, userSession);
		}
		
		return ajaxData;
	}
	
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.system.service.ILoginService#getRequestMenuCode(java.util.List, java.lang.String)
     */
    @Override
    public String getRequestMenuCode(List<MenuBean> menus, String requestUrl) {
		if (menus != null) {
			for (MenuBean menu : menus) {
				if (menu.getUrl().equals(requestUrl)) {
					return menu.getMenuId();
				}
			}
		}
		return null;
    }
    
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.system.service.ILoginService#getSysDate()
     */
    @Override
    public Date getSysDate() {
    	return softdogDao.getSysDate();
    }

    //*************************************私有方法start*************************************
	/**
	 * 构建菜单
	 * @author jinxindong 2016年3月5日 下午3:09:29
	 * @param powers
	 * @param parentCode
	 * @return
	 */
	private List<MenuBean> buildMenus(List<Privilege> powers, String parentCode) {
		List<MenuBean> subMenus = new ArrayList<MenuBean>();
		if (powers != null && powers.size() > 0) {
			for (Privilege power : powers) {
				if (power.getStrSuper().equals(parentCode)) {
					MenuBean menu = new MenuBean();
					menu.setMenuId(power.getStrCode());
					menu.setParentId(parentCode);
					menu.setName(power.getStrName());
					menu.setUrl(power.getStrHref());
					menu.setNbtype(power.getNbtype());
					menu.setChildren(buildMenus(powers, power.getStrCode()));
					subMenus.add(menu);
				}
			}
		}
		return subMenus;
	}
	
	/**
	 * 构建用户session最基本的信息
	 * 1、区域的信息 2、权限的信息
	 * @author jinxindong 2015年3月5日 下午8:01:39
	 * @param user
	 * @param role
	 * @return
	 */
	private UserSession buildUserSession(Users user, Role role, List<Privilege> powers) {
		UserSession userSession = new UserSession(user, role);
		//如果是管理员角色
		if (role.getNtype() == RoleType.SYSTEM_MANAGER) {
			userSession.setSystemRole(true);
			userSession.setRootOrgId(Constants.ROOT_REG_ID);
			userSession.setRootOrgPath(Constants.ROOT_REG_CODE);
			if(user.getStrName() == "admin") {
				userSession.setAdmin(true);
			}
		} else if (role.getNtype() == RoleType.CUSTOMIZE_ROLE) {//如果是自定义角色
			//根据用户获取根节点
			Region org = regionDao.getRootOrg(user);
			if (org != null) {
				userSession.setRootOrgId(org.getId());
				userSession.setRootOrgPath(org.getStrPath());
			}
		}
		//获取权限
		List<String> pris = new ArrayList<String>();
		for (Privilege power : powers) {
			if (StringUtils.isNotBlank(power.getStrHref()))
				pris.add(power.getStrHref());
			if (StringUtils.isNotBlank(power.getNameSpace()))
				pris.addAll(Splitter.on(";").splitToList(power.getNameSpace()));
		}
		userSession.setPrivileges(pris);
		return userSession;
	}
	
	
	/* (non-Javadoc)
	 * @see com.hikvision.finance.fms.system.service.ILoginService#simpleVerifyLogin(com.hikvision.finance.fms.model.Users)
	 */
	@Override
	public AjaxData enableLogin(Users entity, Users user) {
		AjaxData ajaxData = new AjaxData(false);
		ajaxData.put("user", user);
		
		//1.判断用户是否重复登录
		if(HeartbeatCache.isOnline(user.getId())){
			return ajaxData.setFailureMsg("用户已经登录，不能重复登录！");
		}
		
		//2.对没有结束的录像设置异常结束录像状态
		recordsDao.saveErrorRecordStatus(user.getId());
		
		//3.没有绑定角色  加入绑定了角色走else流程，为后续拓展采集权限管理预留
		List<Role> roleList = roleDao.getRoleByUserId(user);
		UserSession userSession = null;
		if (roleList == null || roleList.size()<1) {
			//List<Privilege> powers = privilegeDao.findBy(new String[] {"nenabled", "nsysNo"}, new Object[] {Constants.CONS_1,Constants.CONS_1});
			List<Privilege> powers = privilegeDao.getPowers(Constants.CONS_1, Constants.CONS_1);
			userSession = new UserSession(user);
			//获取权限
			List<String> pris = new ArrayList<String>();
			for (Privilege power : powers) {
				if (StringUtils.isNotBlank(power.getStrHref()))
					pris.add(power.getStrHref());
				if (StringUtils.isNotBlank(power.getNameSpace()))
					pris.addAll(Splitter.on(";").splitToList(power.getNameSpace()));
			}
			userSession.setPrivileges(pris);
		} else {
			// 当前系统只支持单角色绑定
			Role role = roleList.get(0);
			ajaxData.put("role", role);
			// 获取角色菜单权限
			List<Privilege> powers = null;
			powers = privilegeDao.getPowersByRoleId(role.getId());
			List<MenuBean> menus = buildMenus(powers, "5"); // strSuper为0是一级菜单？？
			userSession = buildUserSession(user, role, powers);
			ajaxData.put("menus", menus);
		}
		ajaxData.put(SessionAttrbutes.USER_SESSION, userSession);
		return ajaxData;
	}
	
	/* 
	 * 登录页获取授权信息
     * @see com.hikvision.finance.fms.system.service.ILoginService#getLicense()
     */
    @Override
	public AjaxData getLicense() {
		List<Integer> keys = new ArrayList<Integer>();
		keys.add(Constants.SysConfigType.DOG_NOTES_ID);
		keys.add(Constants.SysConfigType.DOG_LICENSE_NO);
		List<Sysconfig> sysconfigList = sysconfigDao.getSysconfigByKey(keys);
		
		AjaxData ajaxData = new AjaxData();
		for (Sysconfig sysconfig : sysconfigList) {
			if (Constants.SysConfigType.DOG_NOTES_ID == sysconfig.getNkey()) {
				if (StringUtils.isNotBlank(sysconfig.getStrValue())) {
					ajaxData.put("notesId", sysconfig.getStrValue());
				} else {
					ajaxData.put("notesId", "");
				}
			}
			if (Constants.SysConfigType.DOG_LICENSE_NO == sysconfig.getNkey()) {
				if (StringUtils.isNotBlank(sysconfig.getStrValue())) {
					ajaxData.put("licenseNo", sysconfig.getStrValue());
				} else {
					ajaxData.put("licenseNo", "");
				}
			}
		}
		ajaxData.put("success", true);
		ajaxData.put("msg", "操作成功");
		return ajaxData;
	}
	
	// ===================getter、setter===========================================
    
	public IUsersDao getUsersDao() {
		return usersDao;
	}
	
	public void setUsersDao(IUsersDao usersDao) {
		this.usersDao = usersDao;
	}
	
	public IRoleDao getRoleDao() {
		return roleDao;
	}
	
	public void setRoleDao(IRoleDao roleDao) {
		this.roleDao = roleDao;
	}
	
	public IPrivilegeDao getPrivilegeDao() {
		return privilegeDao;
	}
	
	public void setPrivilegeDao(IPrivilegeDao privilegeDao) {
		this.privilegeDao = privilegeDao;
	}
	
	public IRegionDao getRegionDao() {
		return regionDao;
	}
	
	public void setRegionDao(IRegionDao regionDao) {
		this.regionDao = regionDao;
	}
	
	public ISysconfigService getSysconfigService() {
		return sysconfigService;
	}
	
	public void setSysconfigService(ISysconfigService sysconfigService) {
		this.sysconfigService = sysconfigService;
	}
	
	public ISoftdogDao getSoftdogDao() {
		return softdogDao;
	}
	
	public void setSoftdogDao(ISoftdogDao softdogDao) {
		this.softdogDao = softdogDao;
	}
	
	public ISysconfigDao getSysconfigDao() {
		return sysconfigDao;
	}
	
	public void setSysconfigDao(ISysconfigDao sysconfigDao) {
		this.sysconfigDao = sysconfigDao;
	}
	
	public IRecordsDao getRecordsDao() {
		return recordsDao;
	}
	
	public void setRecordsDao(IRecordsDao recordsDao) {
		this.recordsDao = recordsDao;
	}
	
}
