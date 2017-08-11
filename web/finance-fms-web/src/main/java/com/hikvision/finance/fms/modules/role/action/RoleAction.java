/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月29日 下午2:10:06
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.role.action;

import java.util.List;

import com.hikvision.finance.fms.model.Region;
import com.hikvision.finance.fms.model.Role;
import com.hikvision.finance.fms.modules.role.dto.RoleInfo;
import com.hikvision.finance.fms.modules.role.service.IRoleService;
import com.hikvision.finance.fwork.exception.ExpectedException;
import com.hikvision.finance.fwork.www.action.BaseAction;


/**
 * <p>权限管理：角色管理</p>
 * @author jinxindong 2016年2月29日 下午2:10:06
 * @version V1.0 
 */
public class RoleAction extends BaseAction<Role>{

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 4552801551184007405L;
    private IRoleService roleService;
    private Region reg;
    private RoleInfo roleInfo;
    private List<Integer> ids;
    
	/**
	 * 获取角色管理页面
	 * @author jinxindong 2016年2月29日 下午7:54:23
	 * @return
	 */
	public String toRolePage() {
		operPage = "/modules/role/role.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 获取角色分页数据
	 * @author jinxindong 2016年2月29日 下午7:54:30
	 * @return
	 */
	public String getDataPage() {
		String strName = "";
		if (roleInfo!=null && roleInfo.getStrName()!=null) {
			strName = roleInfo.getStrName();
		}
		page = roleService.getDataPage(limit,start,strName);
		return PAGE;
	}
	
	/**
	 * to增加角色页面
	 * @author jinxindong 2016年2月29日 下午2:11:22
	 * @return
	 */
	public String toAddRole() {
		operPage = "/modules/role/dialogs/toAddRolePage.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 增加角色
	 * @author jinxindong 2016年2月29日 下午2:11:44
	 * @return
	 */
	public String addRole() {
		ajaxData = roleService.addRole(jsonStr);
		return AJAX;
	}
	
	/**
	 * 获取修改角色的页面
	 * @author jinxindong 2016年2月29日 下午3:10:53
	 * @return
	 */
	public String toUpdateRolePage() {
		roleInfo = roleService.getRoleById(roleInfo.getId());
		operPage = "/modules/role/dialogs/toEditRolePage.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 修改角色信息
	 * @author jinxindong 2016年2月29日 下午3:10:59
	 * @return
	 */
	public String updateRole() {
		ajaxData = roleService.updateRole(jsonStr);
		return AJAX;
	}
	
	/**
	 * 删除角色
	 * @author jinxindong 2016年2月29日 下午3:11:05
	 * @return
	 */
	public String deleteRole() {
		if (ids == null) {
			throw new ExpectedException("", "未获取到角色ID");
		}
		ajaxData = roleService.deleteRoles(ids);
		return AJAX;
	}
	
	/**
	 * 暴力删除角色
	 * @author jinxindong 2016年2月29日 下午3:11:19
	 * @return
	 */
	public String deleteRoleForce() {
		if (ids == null) {
			throw new ExpectedException("", "未获取到角色ID");
		}
		ajaxData = roleService.deleteRolesForce(ids);
		return AJAX;
	}

	/**
	 * 复制角色（单条或者多条）
	 * @author jinxindong 2016年2月29日 下午6:17:37
	 * @return
	 */
	public String copyRoleForce() {
		if (ids == null) {
			throw new ExpectedException("", "未获取到该角色ID");
		}
		ajaxData = roleService.copyRoleForce(ids);
		return AJAX; 
	}
	
	
	
	//================getter/setter====================
    public IRoleService getRoleService() {
    	return roleService;
    }
	
    public void setRoleService(IRoleService roleService) {
    	this.roleService = roleService;
    }
	
    public Region getReg() {
    	return reg;
    }
	
    public void setReg(Region reg) {
    	this.reg = reg;
    }

	
    public RoleInfo getRoleInfo() {
    	return roleInfo;
    }

	
    public void setRoleInfo(RoleInfo roleInfo) {
    	this.roleInfo = roleInfo;
    }

	
    public List<Integer> getIds() {
    	return ids;
    }

	
    public void setIds(List<Integer> ids) {
    	this.ids = ids;
    }
	
}
