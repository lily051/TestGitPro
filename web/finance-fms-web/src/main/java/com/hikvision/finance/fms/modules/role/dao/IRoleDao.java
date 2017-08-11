/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月29日 下午2:18:49
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.role.dao;

import java.util.List;

import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.hibernate.dao.IBaseDao;
import com.hikvision.finance.fms.model.Role;
import com.hikvision.finance.fms.model.Users;
import com.hikvision.finance.fms.modules.role.qo.RoleQo;


/**
 * <p></p>
 * @author jinxindong 2016年2月29日 下午2:18:49
 * @version V1.0 
 */
public interface IRoleDao  extends IBaseDao<Role, RoleQo>{
	
	/**
	 * 获取组织分页数据
	 * @author jinxindong 2016年3月2日 上午11:27:50
	 * @param limit
	 * @param start
	 * @param keywords
	 * @return
	 */
	public Page getDataPage(Integer limit,Integer start,String keywords);
	
	/**
	 * 通过用户id查询角色
	 * @author jinxindong 2016年3月7日 上午9:50:22
	 * @param user
	 * @return
	 */
	public List<Role> getRoleByUserId(Users user);
	
	/**
	 * 获取角色名称
	 * @author jinxindong 2016年4月25日 上午10:04:36
	 * @return
	 */
	public List<String> getRoleName();
}
