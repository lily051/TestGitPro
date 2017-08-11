/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月5日 下午3:00:14
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.role.dao;

import java.util.List;

import com.hikvision.finance.extend.hibernate.dao.IBaseDao;
import com.hikvision.finance.fms.model.Privilege;
import com.hikvision.finance.fms.modules.role.qo.PrivilegeQo;


/**
 * <p></p>
 * @author jinxindong 2016年3月5日 下午3:00:14
 * @version V1.0 
 */
public interface IPrivilegeDao extends IBaseDao<Privilege, PrivilegeQo>{
	/**
	 * 根据角色ID获取角色权限
	 * @author jinxindong 2016年3月5日 下午3:04:38
	 * @param id
	 * @return
	 */
	public List<Privilege> getPowersByRoleId(Integer id);
	
	/**
	 * 获取菜单
	 * @author jinxindong 2016年6月7日 下午7:58:47
	 * @param nenabled
	 * @param nsysNo
	 * @return
	 */
	public List<Privilege> getPowers(Integer nenabled,Integer nsysNo);
	
	/**
	 * 获取第一个菜单
	 * @author jinxindong 2016年4月11日 下午7:39:45
	 * @param flag
	 * @return
	 */
	public Privilege getFristByPower(Integer flag);
}
