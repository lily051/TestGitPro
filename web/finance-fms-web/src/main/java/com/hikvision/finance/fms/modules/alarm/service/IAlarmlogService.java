/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月6日 下午7:20:54
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.alarm.service;

import java.util.List;

import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.service.IBaseService;
import com.hikvision.finance.fms.model.Alarmlog;
import com.hikvision.finance.fms.modules.alarm.dto.AlarmlogInfo;
import com.hikvision.finance.fms.modules.alarm.qo.AlarmlogQo;
import com.hikvision.finance.fwork.util.AjaxData;


/**
 * <p>报警日志管理Service</p>
 * @author jinxindong 2016年4月6日 下午7:20:54
 * @version V1.0 
 */
public interface IAlarmlogService extends IBaseService<Alarmlog,AlarmlogQo>{
	
	/**
	 * 获取报警管理数据页面
	 * @author jinxindong 2016年4月7日 下午3:01:40
	 * @param limit
	 * @param start
	 * @param alarmlogInfo
	 * @return
	 */
	public Page getAlarmDataPage(Integer limit,Integer start,AlarmlogInfo alarmlogInfo);
	
	/**
	 * 删除报警日志
	 * @author jinxindong 2016年4月7日 下午3:01:55
	 * @param ids
	 * @return
	 */
	public AjaxData deleteAlarmLogs(List<Integer> ids);
	
	/**
	 * 处理报警日志
	 * @author jinxindong 2016年4月7日 下午3:02:08
	 * @param alarmlogInfo
	 * @return
	 */
	public AjaxData updateAlarmLogs(AlarmlogInfo alarmlogInfo,List<Integer> ids);
	
	/**
	 * 首页获取报警数据
	 * @author jinxindong 2016年4月14日 下午2:26:33
	 * @return
	 */
	public List<AlarmlogInfo> getIndexAlarm();
}
