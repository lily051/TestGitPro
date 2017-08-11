/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月29日 下午4:33:02
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.user.dao;

import java.util.List;

import com.hikvision.finance.extend.hibernate.dao.IBaseDao;
import com.hikvision.finance.fms.model.Userrolemapping;
import com.hikvision.finance.fms.modules.user.qo.UserrolemappingQo;


/**
 * <p></p>
 * @author jinxindong 2016年2月29日 下午4:33:02
 * @version V1.0 
 */
public interface IUserrolemappingDao extends IBaseDao<Userrolemapping, UserrolemappingQo>{
	/**
	 * 根据角色删除角色跟用户的关联关系
	 * @author jinxindong 2016年2月29日 下午4:50:34
	 * @param ids
	 */
	public void deleteBatchByRole(List<Integer> ids);
	
	/**
	 * 根据用户id删除用户和角色的关联关系
	 * @author jinxindong 2016年3月3日 下午7:08:44
	 * @param ids
	 */
	public void delUserRoleMapping(List<Integer> ids);
}
