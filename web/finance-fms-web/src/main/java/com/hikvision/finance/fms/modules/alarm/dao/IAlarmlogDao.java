/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月6日 下午7:22:14
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.alarm.dao;

import java.util.Date;
import java.util.List;

import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.hibernate.dao.IBaseDao;
import com.hikvision.finance.fms.model.Alarmlog;
import com.hikvision.finance.fms.modules.alarm.dto.AlarmlogInfo;
import com.hikvision.finance.fms.modules.alarm.qo.AlarmlogQo;


/**
 * <p></p>
 * @author jinxindong 2016年4月6日 下午7:22:14
 * @version V1.0 
 */
public interface IAlarmlogDao extends IBaseDao<Alarmlog,AlarmlogQo>{
	/**
	 * 获取报警管理数据页面
	 * @author jinxindong 2016年4月7日 下午3:09:35
	 * @param limit
	 * @param start
	 * @param alarmlogInfo
	 * @return
	 */
	public Page getAlarmDataPage(Integer limit,Integer start,AlarmlogInfo alarmlogInfo);
	
	/**
	 * 首页获取报警数据
	 * @author jinxindong 2016年4月14日 下午2:44:09
	 * @param limit
	 * @return
	 */
	public List<AlarmlogInfo> getIndexAlarm(Integer limit);
	
	/**
	 * 删除报警日志
	 * @author jinchenwu 2016年5月7日 上午10:02:23
	 * @param thresholdDate
	 */
	public void deleteLog(Date thresholdDate);

	/**
	 * @author xujiangfei 2016-5-9 下午3:15:01
	 * @param request
	 * @return
	 */
	public com.hikvision.finance.fms.model.Alarmlog getAlarmlogStatus(
			com.hikvision.finance.fms.cms.hpp.center.AlarmlogProto.AlarmlogInfo request);
}
