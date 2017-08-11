/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月29日 下午2:17:03
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.role.service.impl;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

import com.hikvision.finance.core.util.BeanUtils;
import com.hikvision.finance.core.util.JsonUtils;
import com.hikvision.finance.core.util.ListUtil;
import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.service.BaseServiceImpl;
import com.hikvision.finance.fms.common.enums.LogModuledEnum;
import com.hikvision.finance.fms.common.util.Constants.OperStatus;
import com.hikvision.finance.fms.common.util.SessionUtil;
import com.hikvision.finance.fms.model.Role;
import com.hikvision.finance.fms.model.Roleprivilegemapping;
import com.hikvision.finance.fms.model.Userrolemapping;
import com.hikvision.finance.fms.model.Users;
import com.hikvision.finance.fms.modules.log.dao.IConfigLogDao;
import com.hikvision.finance.fms.modules.role.dao.IRoleDao;
import com.hikvision.finance.fms.modules.role.dao.IRolePrivilegeMappingDao;
import com.hikvision.finance.fms.modules.role.dto.RoleDto;
import com.hikvision.finance.fms.modules.role.dto.RoleInfo;
import com.hikvision.finance.fms.modules.role.qo.RoleQo;
import com.hikvision.finance.fms.modules.role.service.IRoleService;
import com.hikvision.finance.fms.modules.user.dao.IUserrolemappingDao;
import com.hikvision.finance.fms.modules.user.dao.IUsersDao;
import com.hikvision.finance.fms.modules.user.qo.UserrolemappingQo;
import com.hikvision.finance.fwork.exception.ExpectedException;
import com.hikvision.finance.fwork.util.AjaxData;
import com.hikvision.finance.fwork.util.GlobalMessageUtil;


/**
 * <p></p>
 * @author jinxindong 2016年2月29日 下午2:17:03
 * @version V1.0 
 */
public class RoleServiceImpl extends BaseServiceImpl<Role, RoleQo, IRoleDao> implements IRoleService {

	private IRoleDao roleDao;
	private IRolePrivilegeMappingDao rolePrivilegeMappingDao;
	private IUserrolemappingDao userrolemappingDao;
	private IConfigLogDao configLogDao;
	private IUsersDao usersDao;
	
	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.service.IBaseService#deleteByIds(java.io.Serializable[])
     */
    @Override
    public void deleteByIds(Serializable[] paramArrayOfSerializable) {
	    // TODO Auto-generated method stub
	    
    }
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.service.BaseServiceImpl#getDao()
     */
    @Override
    protected IRoleDao getDao() {
    	return roleDao;
    }
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.role.service.IRoleService#addRole(java.lang.String)
     */
    @Override
    public AjaxData addRole(String jsonStr) {
		AjaxData ajaxData = new AjaxData(false);
		RoleDto roleDto = JsonUtils.json2Object(jsonStr, RoleDto.class);
		Role role = roleDto.getRole();
		//验证角色名称是否存在
		if(roleDao.findUniqueBy("strName", role.getStrName()) != null){
			return ajaxData.setFailureMsg("角色名已存在");
		}
		//设置用户的id
		role.setNowner(SessionUtil.getUserSession().getUserId());
		Integer id = (Integer) roleDao.save(role);
		addRolePowerNRes(roleDto, id);
		configLogDao.setLogContent(OperStatus.ADD, LogModuledEnum.AUTH.getCode(),GlobalMessageUtil.getMessage("log.role.add",new String[] {role.getStrName()}));
		return ajaxData;
    }
	
	/**
	 * 增加角色的功能权限和资源权限信息
	 * @author jinxindong 2016年2月29日 下午3:02:02
	 * @param roleDto 角色信息
	 * @param id 角色ID
	 */
	private void addRolePowerNRes(RoleDto roleDto, Integer id) {
		//保存角色功能权限信息
		List<Roleprivilegemapping> powerList = roleDto.getPowers();
		if(powerList != null){
			for(Roleprivilegemapping power : powerList){
				power.setNroleId(id);
				rolePrivilegeMappingDao.save(power);
			}
		}
	}
	
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.role.service.IRoleService#updateRole(java.lang.String)
     */
    @Override
    public AjaxData updateRole(String jsonStr) {
		AjaxData ajaxData = new AjaxData(false);
		RoleDto roleDto = JsonUtils.json2Object(jsonStr, RoleDto.class);
		Role role = roleDto.getRole();
		int value = (int)role.getId();
		if(roleDao.findUniqueBy("strName", role.getStrName(), value) != null){
			return ajaxData.setFailureMsg("角色名已存在");
		}
		Role roleNew = roleDao.get(value);
		roleNew.setStrName(role.getStrName());
		roleNew.setStrComment(role.getStrComment());
		roleDao.update(roleNew);
		//删除角色原来的权限信息
		rolePrivilegeMappingDao.deleteBy("nroleId", role.getId());
		//增加角色新的权限信息
		addRolePowerNRes(roleDto, role.getId());
		configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.AUTH.getCode(),GlobalMessageUtil.getMessage("log.role.update",new String[] {role.getStrName()}));
		return ajaxData;
    }
	
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.role.service.IRoleService#deleteRoles(java.util.List)
     */
    @Override
	public AjaxData deleteRoles(List<Integer> ids) {
		AjaxData ajaxData = new AjaxData(false);
		
		// 判断是否关联用户
		if (ids != null && isUserExisted(ids)) {
			throw new ExpectedException("", "待删角色存在用户关联,无法删除");
		}
		
		RoleQo roleQo = new RoleQo();
		roleQo.setIds(ids);
		if (ids != null) {
			List<Role> roles = roleDao.queryList(roleQo);
			roleDao.deleteAll(roles);
			rolePrivilegeMappingDao.deleteBatchByRole(ids);// 删除该角色和权限的关联关系
			StringBuffer names = new StringBuffer();
			for (int i = 0; i < roles.size(); i++) {
				Role role = roles.get(i);
				if (i != 0) {
					names.append(",");
				}
				names.append(role.getStrName());
			}
			configLogDao.setLogContent(OperStatus.DELETE, LogModuledEnum.AUTH.getCode(),
			        GlobalMessageUtil.getMessage("log.role.delete", new String[] {names.toString()}));
			ajaxData.setSuccess(true);
		}
		return ajaxData;
	}
    
	/**
	 * 判断多个角色是否存在用户关联
	 * @author jinxindong 2016年2月29日 下午4:30:21
	 * @param roleIds
	 * @return
	 */
	private boolean isUserExisted(List<Integer> roleIds) {
		for(Integer roleId:roleIds) {
			if(this.isUserExisted(roleId)) {
				return true;
			}
		}
		return false;
	}
	
	/**
	 * 判断单个角色是否存在用户关联
	 * @author jinxindong 2016年2月29日 下午4:30:27
	 * @param roleId
	 * @return
	 */
	private boolean isUserExisted(Integer roleId) {
		List<Userrolemapping> list = userrolemappingDao.findBy("nroleId", roleId);
		if(list != null && list.size() > 0) {
			return true;
		}
		return false;
	}
	
	/**
	 * 删除角色，同时删除角色用户对应关系、角色权限对应关系和角色资源对应关系
	 * @author jinxindong 2016年2月29日 下午4:45:34
	 * @param roleQo
	 * @return
	 */
	public AjaxData deleteRolesOnly(List<Integer> ids) {
		AjaxData ajaxData = new AjaxData(false);
		RoleQo roleQo = new RoleQo();
		roleQo.setIds(ids);
		if(ids != null) {
			List<Role> roles = roleDao.queryList(roleQo);
			roleDao.deleteAll(roles);
			rolePrivilegeMappingDao.deleteBatchByRole(ids);
			//roleResourceMappingDao.deleteBatchByRole(ids);
			userrolemappingDao.deleteBatchByRole(ids);
			StringBuffer names = new StringBuffer();
			for(int i = 0; i < roles.size(); i++) {
				Role role = roles.get(i);
				if(i != 0) {
					names.append(",");
				}
				names.append(role.getStrName());
			}
			configLogDao.setLogContent(OperStatus.DELETE, LogModuledEnum.AUTH.getCode(),GlobalMessageUtil.getMessage("log.role.delete",new String[] {names.toString()}));
			ajaxData.setSuccess(true);
		}
		return ajaxData;
	}
	
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.role.service.IRoleService#deleteRolesForce(java.util.List)
     */
    @Override
    public AjaxData deleteRolesForce(List<Integer> ids) {
    	return deleteRolesOnly(ids);
    }
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.role.service.IRoleService#copyRoleForce(java.lang.Long)
     */
    @Override
    public AjaxData copyRoleForce(List<Integer> ids) {
    	AjaxData ajaxData = new AjaxData(false);
    	List<String> roleNames = new ArrayList<String>();
    	//迭代ids、通过ids取出每条记录并复制，赋值一个新的id名称，id名称不能相等
    	for(Integer id:ids){
    		Role role = roleDao.get(id);
    		Role roleNew = new Role();
    		int sNum = (int) (Math.random() * 9000) + 1000;
    		roleNew.setStrName(role.getStrName()+"_"+String.valueOf(sNum));//名字+一个四位数的随机数
    		roleNew.setNowner(role.getNowner());
    		roleNew.setNright(role.getNright());
    		roleNew.setNtype(role.getNtype());
    		roleNew.setStrComment(role.getStrComment());
    		if(roleNew.getStrName().length()>50){
   			    throw new ExpectedException("", "复制的角色名称过长");
   		     }
    		Integer idNew = (Integer)roleDao.save(roleNew);
    		String strName = "复制角色_"+idNew+"("+role.getStrName()+")";
    		if(strName.length()>50){
    			 throw new ExpectedException("", "复制的角色名称过长");
    		}
    		roleNew.setStrName(strName);
    		roleNames.add(role.getStrName());
    		//将角色对应的权限关系也复制nroleId;nprivilegeId;
    		List<Roleprivilegemapping> rpms = rolePrivilegeMappingDao.findBy("nroleId", role.getId());
    		for(Roleprivilegemapping rpm:rpms){
    			Roleprivilegemapping rpmNew = new Roleprivilegemapping();
    			rpmNew.setNprivilegeCode(rpm.getNprivilegeCode());
    			rpmNew.setNroleId(idNew);
    			rolePrivilegeMappingDao.save(rpmNew);
    		}
    	}
    	configLogDao.setLogContent(OperStatus.ADD, LogModuledEnum.AUTH.getCode(),GlobalMessageUtil.getMessage("log.role.copy",new String[] {ListUtil.listToStr(roleNames)}));
    	ajaxData.setSuccess(true);
    	return ajaxData;
    }
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.role.service.IRoleService#getDataPage(java.lang.Integer, java.lang.Integer, java.lang.String)
     */
    @SuppressWarnings("unchecked")
    @Override
	public Page getDataPage(Integer limit, Integer start, String keywords) {
		Page page = roleDao.getDataPage(limit, start, keywords);
		
		// 获取到page结果集，对结果集迭代，封装每个结果，将创建者名称和用户数量
		List<RoleInfo> roleList = (List<RoleInfo>)page.getResult();
		List<RoleInfo> roleNewList = new ArrayList<RoleInfo>();
		for (RoleInfo roleOld : roleList) {
			Users user = usersDao.get(roleOld.getNowner());
			if(null != user){
				roleOld.setNownerName(user.getStrName());// 设置创建者名称
			}
			// 设置用户数量
			UserrolemappingQo userrolemappingQo = new UserrolemappingQo();
			userrolemappingQo.setNroleId(roleOld.getId());
			List<Userrolemapping> userrolemappings = userrolemappingDao.queryList(userrolemappingQo);
			if (userrolemappings == null || userrolemappings.size() < 1) {
				roleOld.setUserNum(0);
			} else {
				roleOld.setUserNum(userrolemappings.size());
			}
			roleNewList.add(roleOld);
		}
		
		page.setResult(roleNewList);
		return page;
	}
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.role.service.IRoleService#getRoleById(java.lang.Integer)
     */
    @Override
    public RoleInfo getRoleById(Integer id) {
    	RoleInfo roleInfo = new RoleInfo();
    	Role  role = roleDao.get(id);
    	try {
	        BeanUtils.copyNotNullProperties(roleInfo, role);
        } catch (Exception e) {
	        throw new ExpectedException("", "copy实体属性失败[getRoleById]");
        } 
	    return roleInfo;
    }
	//========================================gettter/setter================================
	
    public IRoleDao getRoleDao() {
    	return roleDao;
    }
	
    public void setRoleDao(IRoleDao roleDao) {
    	this.roleDao = roleDao;
    }
	
    public IRolePrivilegeMappingDao getRolePrivilegeMappingDao() {
    	return rolePrivilegeMappingDao;
    }
	
	public void setRolePrivilegeMappingDao(IRolePrivilegeMappingDao rolePrivilegeMappingDao) {
		this.rolePrivilegeMappingDao = rolePrivilegeMappingDao;
	}
	
	public IUserrolemappingDao getUserrolemappingDao() {
		return userrolemappingDao;
	}
	
    public void setUserrolemappingDao(IUserrolemappingDao userrolemappingDao) {
    	this.userrolemappingDao = userrolemappingDao;
    }

	
    public IConfigLogDao getConfigLogDao() {
    	return configLogDao;
    }

	
    public void setConfigLogDao(IConfigLogDao configLogDao) {
    	this.configLogDao = configLogDao;
    }

	
    public IUsersDao getUsersDao() {
    	return usersDao;
    }

	
    public void setUsersDao(IUsersDao usersDao) {
    	this.usersDao = usersDao;
    }

}
