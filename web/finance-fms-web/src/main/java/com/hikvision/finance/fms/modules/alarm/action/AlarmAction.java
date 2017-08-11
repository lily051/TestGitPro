/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月6日 下午7:23:05
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.alarm.action;

import java.util.List;

import org.apache.struts2.ServletActionContext;

import com.hikvision.finance.fms.model.Alarmlog;
import com.hikvision.finance.fms.modules.alarm.dto.AlarmlogInfo;
import com.hikvision.finance.fms.modules.alarm.service.IAlarmlogService;
import com.hikvision.finance.fwork.exception.ExpectedException;
import com.hikvision.finance.fwork.www.action.BaseAction;

/**
 * <p>报警管理Action</p>
 * @author jinxindong 2016年4月6日 下午7:23:05
 * @version V1.0
 */
public class AlarmAction extends BaseAction<Alarmlog> {
	
	/**
	 * 序列化ID
	 */
	private static final long serialVersionUID = -9188224405938044285L;
	private IAlarmlogService alarmlogService;
	private List<Integer> ids;
	private AlarmlogInfo alarmlogInfo;
	private List<AlarmlogInfo> alarmlogList;
	/**
	 * 获取报警管理数据页面
	 * @author jinxindong 2016年4月7日 下午2:11:15
	 * @return
	 */
	public String getAlarmDataPage() {
		String method = ServletActionContext.getRequest().getMethod();
		if ("POST".equals(method)) {// AppScan扫描出“查询中接受的主体参数”缺陷，需要禁用GET方法
			page = alarmlogService.getAlarmDataPage(limit, start, alarmlogInfo);
		} else {
			throw new ExpectedException("", "请求方法错误");
		}
		return PAGE;
	}
	
	/**
	 * 删除报警日志
	 * @author jinxindong 2016年4月7日 下午2:12:18
	 * @return
	 */
	public String deleteAlarmLogs() {
		ajaxData = alarmlogService.deleteAlarmLogs(ids);
		return AJAX;
	}
	
	/**
	 * 跳到处理报警日志界面
	 * @author jinxindong 2016年4月7日 下午6:04:58
	 * @return
	 */
	public String toUpdateAlarmPage() {
		operPage = "/modules/alarm/dialogs/alarmDeal.jsp";
		return DISPATCHER;
	}
	/**
	 * 处理报警日志
	 * @author jinxindong 2016年4月7日 下午2:14:13
	 * @return
	 */
	public String updateAlarmLogs() {
		ajaxData = alarmlogService.updateAlarmLogs(alarmlogInfo,ids);
		return AJAX;
	}

	/**
	 * 导出报警日志
	 * @author jinxindong 2016年4月7日 下午2:16:21
	 * @return
	 */
	public String exportAlarmLogs() {
		//ajaxData = deviceService.deleteStos(ids);
		return AJAX;
	}
	
	//=======================getter/setter==========================================
    public IAlarmlogService getAlarmlogService() {
    	return alarmlogService;
    }

	
    public void setAlarmlogService(IAlarmlogService alarmlogService) {
    	this.alarmlogService = alarmlogService;
    }

	
    public List<Integer> getIds() {
    	return ids;
    }

	
    public void setIds(List<Integer> ids) {
    	this.ids = ids;
    }

	
    public AlarmlogInfo getAlarmlogInfo() {
    	return alarmlogInfo;
    }

	
    public void setAlarmlogInfo(AlarmlogInfo alarmlogInfo) {
    	this.alarmlogInfo = alarmlogInfo;
    }

	
    public List<AlarmlogInfo> getAlarmlogList() {
    	return alarmlogList;
    }

	
    public void setAlarmlogList(List<AlarmlogInfo> alarmlogList) {
    	this.alarmlogList = alarmlogList;
    }

	


	




	
	
}
