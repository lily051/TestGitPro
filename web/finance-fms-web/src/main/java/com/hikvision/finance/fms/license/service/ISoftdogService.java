/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月19日 上午10:54:46
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.license.service;

import java.util.Date;

import com.hikvision.finance.extend.service.IBaseService;
import com.hikvision.finance.fms.license.qo.SoftdogQo;
import com.hikvision.finance.fms.model.Softdongle;


/**
 * <p></p>
 * @author jinxindong 2016年4月19日 上午10:54:46
 * @version V1.0 
 */
public interface ISoftdogService extends IBaseService<Softdongle, SoftdogQo>{
	
	/**
	 * 检查授权
	 * @author jinchenwu 2016年11月1日 上午10:50:17
	 * @return
	 */
	public Boolean checkDBAuthorization();

	/**
	 * 写入或者更新授权信息
	 * @author jinxindong 2016年4月19日 上午10:58:15
	 * @param softdog
	 */
	public void saveOrUpdateSoftDog(Softdongle softdog);

	/**
	 * 获取数据库时间
	 * @author jinxindong 2016年4月19日 上午10:58:20
	 * @return
	 */
	public Date getSysDate();
	
	/**
	 * 更新sysconfig表中的授权信息
	 * @author jinchenwu 2016年11月1日 下午3:12:01
	 * @param key
	 * @param value
	 */
	public void updateSysConfig(int key, String value);
	
}
