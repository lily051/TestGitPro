/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月19日 上午11:00:39
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.license.dao;

import com.hikvision.finance.extend.hibernate.dao.IBaseDao;
import com.hikvision.finance.fms.license.qo.SoftdogQo;
import com.hikvision.finance.fms.model.Softdongle;


/**
 * <p></p>
 * @author jinxindong 2016年4月19日 上午11:00:39
 * @version V1.0 
 */
public interface ISoftdogDao extends IBaseDao<Softdongle, SoftdogQo>{
	/**
	 * 获取授权的信道数量
	 * @author jinxindong 2016年4月19日 上午11:02:01
	 * @return
	 */
	public int getAuthoriseDeviceChannel();
	
}
