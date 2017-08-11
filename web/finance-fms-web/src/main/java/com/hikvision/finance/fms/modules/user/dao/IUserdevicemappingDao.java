/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月3日 下午6:12:47
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.user.dao;

import java.util.List;

import com.hikvision.finance.extend.hibernate.dao.IBaseDao;
import com.hikvision.finance.fms.model.Userdevicemapping;
import com.hikvision.finance.fms.modules.user.qo.UserdevicemappingQo;


/**
 * <p></p>
 * @author jinxindong 2016年3月3日 下午6:12:47
 * @version V1.0 
 */
public interface IUserdevicemappingDao extends IBaseDao<Userdevicemapping, UserdevicemappingQo>{
	
	/**
	 * 根据用户id删除用户设备关联关系
	 * @author jinxindong 2016年3月3日 下午7:11:25
	 * @param ids
	 */
	public void delUserDeviceMapping(List<Integer> ids);
	
	/**
	 * 根据用户id，获取信道id
	 * @author jinxindong 2016年3月10日 下午2:51:42
	 * @param userId
	 * @return
	 */
	public List<Integer> getChannelByUserId(Integer userId);
	
	/**
	 * 保存用户设备信道关系表
	 * @author jinxindong 2016年7月13日 下午4:23:48
	 * @param strsql
	 */
	public void saveMapping(String strsql);
	
	/**
	 * 删除关联关系
	 * @author jinxindong 2016年7月13日 下午4:25:00
	 * @param id
	 * @param strsql
	 */
	public void deleteMapping(Integer id,String strsql);
}
