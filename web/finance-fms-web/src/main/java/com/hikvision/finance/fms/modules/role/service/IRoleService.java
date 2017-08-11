/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月29日 下午2:12:06
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.role.service;

import java.util.List;

import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.service.IBaseService;
import com.hikvision.finance.fms.model.Role;
import com.hikvision.finance.fms.modules.role.dto.RoleInfo;
import com.hikvision.finance.fms.modules.role.qo.RoleQo;
import com.hikvision.finance.fwork.util.AjaxData;


/**
 * <p></p>
 * @author jinxindong 2016年2月29日 下午2:12:06
 * @version V1.0 
 */
public interface IRoleService extends IBaseService<Role,RoleQo>{
	/**
	 * 增加角色
	 * @author jinxindong 2016年2月29日 下午2:27:59
	 * @param jsonStr json字符串格式的角色数据
	 * @return
	 */
	public AjaxData addRole(String jsonStr);
	
	/**
	 * 通过id获取角色的实体DTO对象
	 * @author jinxindong 2016年3月8日 下午2:16:08
	 * @param id
	 * @return
	 */
	public RoleInfo getRoleById(Integer id);
	
	/**
	 * 修改角色信息
	 * @author jinxindong 2016年2月29日 下午4:18:46
	 * @param jsonStr
	 * @return
	 */
	public AjaxData updateRole(String jsonStr);
	
	/**
	 * 删除角色，支持批量删除
	 * @author jinxindong 2016年2月29日 下午4:26:25
	 * @param ids
	 * @return
	 */
	public AjaxData deleteRoles(List<Integer> ids);
	
	/**
	 * 暴力删除角色，支持批量删除
	 * @author jinxindong 2016年2月29日 下午5:02:58
	 * @param ids
	 * @return
	 */
	public AjaxData deleteRolesForce(List<Integer> ids);
	
	/**
	 * 复制角色
	 * @author jinxindong 2016年2月29日 下午6:34:58
	 * @param id
	 * @return
	 */
	public AjaxData copyRoleForce(List<Integer> ids);
	
	/**
	 * 获取组织分页数据
	 * @author jinxindong 2016年3月2日 上午11:22:35
	 * @param limit
	 * @param start
	 * @param keywords
	 * @return
	 */
	public Page getDataPage(Integer limit,Integer start,String keywords);
}
