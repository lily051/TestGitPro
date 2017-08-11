/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月24日 下午6:04:40
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.user.dao.impl;


import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import org.hibernate.Criteria;
import org.hibernate.criterion.MatchMode;
import org.hibernate.criterion.Order;
import org.hibernate.criterion.Restrictions;

import com.google.common.collect.Maps;
import com.hikvision.finance.core.util.StringUtils;
import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.hibernate.dao.BaseDao;
import com.hikvision.finance.extend.hibernate.qo.SqlQuery;
import com.hikvision.finance.fms.common.enums.SysRoleEnum;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.common.util.Constants.UserStatus;
import com.hikvision.finance.fms.common.util.SessionUtil;
import com.hikvision.finance.fms.common.util.StringUtilExpand;
import com.hikvision.finance.fms.model.Role;
import com.hikvision.finance.fms.model.Userrolemapping;
import com.hikvision.finance.fms.model.Users;
import com.hikvision.finance.fms.modules.region.dao.IRegionDao;
import com.hikvision.finance.fms.modules.role.dao.IRoleDao;
import com.hikvision.finance.fms.modules.user.dao.IUserrolemappingDao;
import com.hikvision.finance.fms.modules.user.dao.IUsersDao;
import com.hikvision.finance.fms.modules.user.dto.UserInfo;
import com.hikvision.finance.fms.modules.user.qo.UsersQo;
import com.hikvision.finance.fwork.exception.ExpectedException;


/**
 * <p></p>
 * @author jinxindong 2016年2月24日 下午6:04:40
 * @version V1.0 
 */
public class UsersDaoImpl extends BaseDao<Users, UsersQo> implements IUsersDao {
	
	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 1893316185561091911L;
    private IRoleDao roleDao;
    private IUserrolemappingDao userrolemappingDao;
    private IRegionDao regionDao;
	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.hibernate.dao.BaseDao#buildCriteria(org.hibernate.Criteria, com.hikvision.finance.extend.hibernate.qo.BaseQo)
     */
    @Override
    protected Criteria buildCriteria(Criteria criteria, UsersQo qo) {
    	if(qo != null) {
    		if(qo.getStrName() != null) {
    			criteria.add(Restrictions.eq("strName", qo.getStrName()));
    		}
    		if(qo.getStrCode() != null) {
    			criteria.add(Restrictions.eq("strCode", qo.getStrCode()));
    		}
    		if(qo.getStrPassword() != null) {
    			criteria.add(Restrictions.eq("strPassword", qo.getStrPassword()));
    		}
    		if(qo.getIds() != null && qo.getIds().size() > 0) {
    			criteria.add(Restrictions.in("id", qo.getIds().toArray()));
    		}
    		if(qo.getNonline()!= null) {
    			criteria.add(Restrictions.eq("nonline", qo.getNonline()));
    		}
    		if(qo.getStrNameLike() != null) {
    			criteria.add(Restrictions.like("strName", qo.getStrNameLike(), MatchMode.ANYWHERE));
    		}
    		if(qo.getStrCodeLike() != null) {
    			criteria.add(Restrictions.like("strCode", qo.getStrCodeLike(), MatchMode.ANYWHERE));
    		}
    	}
    	criteria.addOrder(Order.desc("id"));
    	return criteria;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.user.dao.IUsersDao#resetPassword(java.util.List, java.lang.String)
     */
    @Override
    public void resetPassword(List<Integer> ids, String defaultPwd) {
	    for(Integer id : ids){
	    	Users user = get(id);
	    	user.setStrPassword(defaultPwd);
	    	this.saveOrUpdate(user);
	    }	    
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.user.dao.IUsersDao#enableUsers(java.util.List)
     */
    @Override
    public void enableUsers(List<Integer> ids) {
	    for(Integer id : ids){
	    	Users user = this.get(id);
	    	user.setNstate(UserStatus.NORMAL);
	    	this.update(user);
	    }	
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.user.dao.IUsersDao#disableUsers(java.util.List)
     */
    @Override
    public void disableUsers(List<Integer> ids) {
	    for(Integer id : ids){
	    	Users user = this.get(id);
	    	user.setNstate(UserStatus.DISABLE);
	    	this.update(user);
	    }	
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.user.dao.IUsersDao#getDataPage(java.lang.Integer, java.lang.Integer, java.lang.String, java.lang.String, java.lang.String, java.lang.Integer)
     */
    @Override
    public Page getDataPage(Integer limit, Integer start, UserInfo userInfo) {
    	//1.查询条件封装
    	SqlQuery sqlQuery = conditionQuery(userInfo);
    	
		// 2.设置page 属性
		Page page = new Page();
		page.setCondition(sqlQuery);
		page.setPageNo(start);
		page.setPageSize(limit);
	    page = this.queryPaginationBySql(page);
	    return page;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.user.dao.IUsersDao#getCheckUsers(com.hikvision.finance.fms.modules.user.dto.UserInfo)
     */
    @SuppressWarnings("unchecked")
    @Override
    public List<Users> getCheckUsers(String userName) {
    	String sql = "";
    	List<Users> list = null;
    	if (StringUtils.isNotBlank(userName)) {
    		sql = " select u.* from Users u where u.sysRole = 0 and u.strName like ?  escape '/' ";
    		String para =  "%"+StringUtilExpand.escapeSQLLike(userName)+"%";
    		list = (List<Users>)this.createSQLQuery(sql).addEntity(Users.class).setParameter(0, para).list();
    	}else{
    		sql = " select u.* from Users u where u.sysRole = 0 ";
    		list = (List<Users>)this.createSQLQuery(sql).addEntity(Users.class).list();
    	}
		return list;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.user.dao.IUsersDao#exportUsersExcel(com.hikvision.finance.fms.modules.user.dto.UserInfo)
     */
    @SuppressWarnings("unchecked")
    @Override
	public List<UserInfo> exportUsersExcel(UserInfo userInfo) {
		// 1.查询条件封装
		SqlQuery sqlQuery = conditionQuery(userInfo);
		
		// 2.限制查询数量
		List<UserInfo> infos = new ArrayList<UserInfo>();
		List<UserInfo> userInfos = (List<UserInfo>)this.queryListBySql(sqlQuery);
		Integer count = userInfos.size();
		if (count > Constants.EXCEL_MAX_NUM) {
			throw new ExpectedException("", "导出数据超过" + Constants.EXCEL_MAX_NUM + "条,无法导出");
		}
		// 3.获取userRoleMap<nuserId,Userrolemapping>
		List<Userrolemapping> userrolemappings = userrolemappingDao.getAll();
		Map<Integer, Userrolemapping> userRoleMap =  buildUserRoleMap(userrolemappings);
		
		// 4.获取roleMap<id,Role>
		List<Role> roles = roleDao.getAll();
		Map<Integer, Role> roleMap = buildRoleMap(roles);
		
		// 5.封装系统名称和角色名称到列表
		String typrName = "";
		Role role = null;
		for (UserInfo userOld : userInfos) {
			if (SysRoleEnum.CENTRY_ADMIN_0.getCode().equals(userOld.getSysRole().toString())) {
				Userrolemapping userrolemapping = userRoleMap.get(userOld.getId());
				if (userrolemapping == null) {
					throw new ExpectedException("", userOld.getStrName() + "用户未关联角色");
				}
				typrName = SysRoleEnum.CENTRY_ADMIN_0.getDescribe();
				role = roleMap.get(userrolemapping.getNroleId());
				userOld.setRoleName(role.getStrName());
			} else {
				typrName = SysRoleEnum.FINANC_MANAGER_1.getDescribe();
				userOld.setRoleName(Constants.ROLE_NAME_NULL);
			}
			
			if(0 == userOld.getNstate()){
				userOld.setNstateName("启用");
			}else if(1 == userOld.getNstate()){
				userOld.setNstateName("禁用");
			}else{
				userOld.setNstateName("未知");
			}
			
			userOld.setSysTypeName(typrName);
			infos.add(userOld);
		}
		return infos;
	}

    //************************************共用代码**********************************************
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.user.dao.IUsersDao#buildUserRoleMap(java.util.List)
     */
    @Override
    public Map<Integer, Userrolemapping> buildUserRoleMap(List<Userrolemapping> userrolemappings) {
    	Map<Integer, Userrolemapping> userRoleMap = Maps.newHashMap();
		for (Userrolemapping userrolemapping : userrolemappings) {
			userRoleMap.put(userrolemapping.getNuserId(), userrolemapping);
		}
		return userRoleMap;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.user.dao.IUsersDao#buildRoleMap(java.util.List)
     */
    @Override
    public Map<Integer, Role> buildRoleMap(List<Role> roles) {
		Map<Integer, Role> roleMap =  Maps.newHashMap();
		for (Role role : roles) {
			roleMap.put(role.getId(), role);
		}
		return roleMap;
    }
    
	/**
	 * 导出和分页查询的条件组装
	 * @author jinxindong 2016年5月19日 上午9:28:00
	 * @param regionInfo
	 * @return
	 */
	private SqlQuery conditionQuery(UserInfo userInfo) {
		String strName = "";
		String userNo = "";
		String net = "";
		Integer nonline = null;
		if (userInfo != null) {
			strName = null == userInfo.getStrName()?"":userInfo.getStrName();
			userNo = null == userInfo.getUserNo()?"":userInfo.getUserNo();
			net = null == userInfo.getNet()?"":userInfo.getNet();
			nonline = null == userInfo.getNonline()?null:userInfo.getNonline();
		}
		//获取当前用户根节点
		String strPathLike = SessionUtil.getUserSession().getRootOrgPath();
		SqlQuery sqlQuery = new SqlQuery();
		StringBuffer sql = new StringBuffer(
		        " SELECT u.id as id,u.strName as strName,u.sysRole as sysRole,u.userNo as userNo,"
		        + "u.nState as nstate,u.strCode as strCode,u.nonline as nonline,u.net as net ,"
		        + "u.strDuty as strDuty,u.strDate as strDate FROM users u JOIN region reg ON u.netId = reg.id WHERE 1=1  ");
		
		sql.append(" and reg.strPath like ? ");
		sqlQuery.addParams(strPathLike + "%");
		
		if (StringUtils.isNotBlank(strName)) {
			sql.append(" and u.strName like ? escape '/'");
			sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(strName) + "%");
		}
		if (StringUtils.isNotBlank(userNo)) {
			sql.append(" and u.userNo like ? escape '/'");
			sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(userNo) + "%");
		}
		if (StringUtils.isNotBlank(net)) {
			sql.append(" and u.net like ? escape '/'");
			sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(net) + "%");
		}
		if (null != nonline) {
			sql.append(" and u.nonline = ? ");
			sqlQuery.addParams(nonline);
		}
		// 默认倒序
		sql.append(" order by u.id desc");
		// 设置sqlQuery属性
		sqlQuery.setSql(sql.toString());
		sqlQuery.setDtoClass(UserInfo.class);
		return sqlQuery;
	}
	
    public IRoleDao getRoleDao() {
    	return roleDao;
    }

	
    public void setRoleDao(IRoleDao roleDao) {
    	this.roleDao = roleDao;
    }

	
    public IUserrolemappingDao getUserrolemappingDao() {
    	return userrolemappingDao;
    }

	
    public void setUserrolemappingDao(IUserrolemappingDao userrolemappingDao) {
    	this.userrolemappingDao = userrolemappingDao;
    }

	
    public IRegionDao getRegionDao() {
    	return regionDao;
    }

	
    public void setRegionDao(IRegionDao regionDao) {
    	this.regionDao = regionDao;
    }

    
    
}
