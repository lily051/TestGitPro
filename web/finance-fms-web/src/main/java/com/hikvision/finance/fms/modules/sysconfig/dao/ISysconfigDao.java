/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月9日 下午4:42:17
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.sysconfig.dao;

import java.util.List;

import com.hikvision.finance.extend.hibernate.dao.IBaseDao;
import com.hikvision.finance.fms.model.Sysconfig;
import com.hikvision.finance.fms.modules.sysconfig.qo.SysconfigQo;


/**
 * <p></p>
 * @author jinxindong 2016年3月9日 下午4:42:17
 * @version V1.0 
 */
public interface ISysconfigDao extends IBaseDao<Sysconfig, SysconfigQo> {
	/**
	 * 获取指定key的系统配置信息
	 * @author jinxindong 2016年3月31日 下午3:51:12
	 * @param key
	 * @return
	 */
	public Sysconfig getSysconfigByKey(Integer key);
	
	/**
	 * 获取指定key的系统配置信息列表
	 * @author jinxindong 2016年3月31日 下午3:51:17
	 * @param keys
	 * @return
	 */
	public List<Sysconfig> getSysconfigByKey(List<Integer> keys);
}
