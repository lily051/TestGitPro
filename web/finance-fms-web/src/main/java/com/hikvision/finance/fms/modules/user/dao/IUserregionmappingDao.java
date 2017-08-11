/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月3日 下午6:11:55
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.user.dao;

import java.util.List;

import com.hikvision.finance.extend.hibernate.dao.IBaseDao;
import com.hikvision.finance.fms.model.Userregionmapping;
import com.hikvision.finance.fms.modules.user.qo.UserregionmappingQo;



/**
 * <p></p>
 * @author jinxindong 2016年3月3日 下午6:11:55
 * @version V1.0 
 */
public interface IUserregionmappingDao extends IBaseDao<Userregionmapping, UserregionmappingQo>{
	
	/**
	 * 根据用户id删除用户组织关联关系
	 * @author jinxindong 2016年3月3日 下午7:07:27
	 * @param ids
	 */
	public void delUserRegionMapping(List<Integer> ids);
	
	/**
	 * 获取用户已选区域列表的编码列表
	 * @author jinxindong 2016年3月5日 下午1:18:06
	 * @param userId
	 * @return
	 */
	public List<Integer> getUserRegionCode(Integer userId);
}
