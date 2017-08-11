/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月29日 下午2:43:55
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.role.dao;

import java.util.List;

import com.hikvision.finance.extend.hibernate.dao.IBaseDao;
import com.hikvision.finance.fms.model.Roleprivilegemapping;
import com.hikvision.finance.fms.modules.role.qo.RoleprivilegemappingQo;


/**
 * <p></p>
 * @author jinxindong 2016年2月29日 下午2:43:55
 * @version V1.0 
 */
public interface IRolePrivilegeMappingDao extends IBaseDao<Roleprivilegemapping, RoleprivilegemappingQo> {
	
	/**
	 * 获取角色的功能权限编码列表
	 * @author jinxindong 2016年2月29日 下午3:39:54
	 * @param id
	 * @return
	 */
	List<String> getRolePowerCode(Integer id);
	
	
	/**
	 * 根据角色ID删除角色功能权限，支持批量
	 * @author jinxindong 2016年2月29日 下午4:49:09
	 * @param ids
	 */
	public void deleteBatchByRole(List<Integer> ids);
}
