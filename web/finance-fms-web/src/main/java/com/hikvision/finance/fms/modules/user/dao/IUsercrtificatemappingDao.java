/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月3日 下午6:12:19
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.user.dao;

import java.util.List;

import com.hikvision.finance.extend.hibernate.dao.IBaseDao;
import com.hikvision.finance.fms.model.Usercrtificatemapping;
import com.hikvision.finance.fms.modules.user.qo.UsercrtificatemappingQo;


/**
 * <p></p>
 * @author jinxindong 2016年3月3日 下午6:12:19
 * @version V1.0 
 */
public interface IUsercrtificatemappingDao extends IBaseDao<Usercrtificatemapping, UsercrtificatemappingQo>{
	
	/**
	 * 根据用户id删除用户证书关联关系
	 * @author jinxindong 2016年3月3日 下午7:10:23
	 * @param ids
	 */
	public void delUserCrtificateMapping(List<Integer> ids);
	
	/**
	 * 根据用户id获取该用户的证书ids
	 * @author jinxindong 2016年3月15日 下午5:50:16
	 * @param userId
	 * @return
	 */
	public List<Integer> getCertIdsByUserId(Integer userId);
	
	/**
	 * 保存关联关系
	 * @author jinxindong 2016年7月11日 下午8:10:14
	 * @param strsql
	 */
	public void saveMapping(String strsql);
	
	
	
}
