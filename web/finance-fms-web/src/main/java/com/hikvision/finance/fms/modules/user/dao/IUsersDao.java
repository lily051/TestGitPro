/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月24日 下午6:00:45
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.user.dao;

import java.util.List;
import java.util.Map;

import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.hibernate.dao.IBaseDao;
import com.hikvision.finance.fms.model.Role;
import com.hikvision.finance.fms.model.Userrolemapping;
import com.hikvision.finance.fms.model.Users;
import com.hikvision.finance.fms.modules.user.dto.UserInfo;
import com.hikvision.finance.fms.modules.user.qo.UsersQo;


/**
 * <p></p>
 * @author jinxindong 2016年2月24日 下午6:00:45
 * @version V1.0 
 */
public interface IUsersDao extends IBaseDao<Users, UsersQo>{
	
	/**
	 * 重置密码
	 * @author jinxindong 2016年3月3日 下午5:07:04
	 * @param ids
	 * @param defaultPwd
	 */
	public void resetPassword(List<Integer> ids,String defaultPwd);
	
	/**
	 * 启用用户
	 * @author jinxindong 2016年3月3日 下午5:49:00
	 * @param ids
	 */
	public void enableUsers(List<Integer> ids);
	
	/**
	 * 禁用用户
	 * @author jinxindong 2016年3月3日 下午5:49:10
	 * @param ids
	 */
	public void disableUsers(List<Integer> ids);
	
	/**
	 * 获取组织分页数据
	 * @author jinxindong 2016年3月4日 上午10:40:06
	 * @param limit
	 * @param start
	 * @param strName
	 * @param userNo
	 * @param nonline
	 * @return
	 */
	public Page getDataPage(Integer limit, Integer start, UserInfo userInfo);
	
	/**
	 * 获取复核员
	 * @author jinxindong 2016年3月23日 上午9:34:49
	 * @param userName
	 * @return
	 */
	public List<Users> getCheckUsers(String userName);
	
	/**
	 * 导出用户表格
	 * @author jinxindong 2016年4月20日 下午2:33:16
	 * @param userInfo
	 * @return
	 */
	public List<UserInfo> exportUsersExcel(UserInfo userInfo);
	
	/**
	 * 封装UserRoleMap
	 * @author jinxindong 2016年5月19日 上午10:12:33
	 * @param userrolemappings
	 * @return
	 */
	public Map<Integer, Userrolemapping> buildUserRoleMap(List<Userrolemapping> userrolemappings);
	
	/**
	 * 封装buildRoleMap
	 * @author jinxindong 2016年5月19日 上午10:12:47
	 * @param roles
	 * @return
	 */
	public Map<Integer, Role> buildRoleMap(List<Role> roles);
}
