/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月8日 下午4:54:25
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.log.action;

import java.util.List;

import org.apache.struts2.ServletActionContext;

import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.model.Privilege;
import com.hikvision.finance.fms.model.Recordlog;
import com.hikvision.finance.fms.modules.log.dto.ClientdeletelogInfo;
import com.hikvision.finance.fms.modules.log.dto.ConfiglogInfo;
import com.hikvision.finance.fms.modules.log.dto.LogsysInfo;
import com.hikvision.finance.fms.modules.log.dto.RecordlogInfo;
import com.hikvision.finance.fms.modules.log.service.ILogService;
import com.hikvision.finance.fwork.www.action.BaseAction;


/**
 * <p>日志查询Action</p>
 * @author jinxindong 2016年4月8日 下午4:54:25
 * @version V1.0 
 */
public class LogAction extends BaseAction<Recordlog> {

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = -1275655969502658947L;
    private ILogService logService;
    private ClientdeletelogInfo clientdeletelogInfo;
    private LogsysInfo logsysInfo;
    private RecordlogInfo recordlogInfo;
    private ConfiglogInfo configlogInfo;
    private List<Privilege> privilegeList;
    
  /**
   * 跳转到回放日志页面
   * @author jinxindong 2016年4月11日 下午4:36:16
   * @return
   */
	public String toRecordLog() {
		operPage = "/modules/log/page/recordLog.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 跳转到删除日志页面
	 * @author jinxindong 2016年4月11日 下午4:36:40
	 * @return
	 */
	public String toDelLog() {
		operPage = "/modules/log/page/delLog.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 跳转到配置日志页面
	 * @author jinxindong 2016年4月11日 下午4:36:50
	 * @return
	 */
	public String toConfigLog() {
		operPage = "/modules/log/page/configLog.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 跳转到系统日志页面
	 * @author jinxindong 2016年4月11日 下午4:36:58
	 * @return
	 */
	public String toLogSys() {
		operPage = "/modules/log/page/logSys.jsp";
		return DISPATCHER;
	}
	
	
    /**
     * 回放日志数据获取
     * @author jinxindong 2016年4月9日 上午10:12:24
     * @return
     */
	public String getRecordLogDataPage() {
		page = logService.getRecordLogDataPage(limit, start, recordlogInfo);
		return PAGE;
	}
	
    /**
     * 删除日志数据获取
     * @author jinxindong 2016年4月9日 下午4:08:03
     * @return
     */
	public String getClientDelLogDataPage() {
		page = logService.getClientDelLogDataPage(limit, start, clientdeletelogInfo);
		return PAGE;
	}
	
    /**
     * 系统日志数据获取
     * @author jinxindong 2016年4月9日 下午4:08:08
     * @return
     */
	public String getLogsysDataPage() {
		page = logService.getLogsysDataPage(limit, start, logsysInfo);
		return PAGE;
	}
	
    /**
     * 配置日志数据获取
     * @author jinxindong 2016年4月9日 下午4:08:15
     * @return
     */
	public String getConfiglogDataPage() {
		page = logService.getConfiglogDataPage(limit, start, configlogInfo);
		return PAGE;
	}
	
	
	/**
	 * 获取操作模块下拉列表
	 * @author jinxindong 2016年4月11日 上午10:27:52
	 * @return
	 */
	public String getNtypeList() {
		privilegeList = logService.getPrivilegeList();
		ajaxData.put("privilegeList",privilegeList);
		return AJAX;
	}
	
	
	/**
	 * 导出删除日志表格
	 * @author jinxindong 2016年4月9日 下午4:08:25
	 * @return
	 */
	public String exportDelLogExcel() {
		String[] titleNames = new String[] {"业务单号", "产品名称",  "客户证件号", "客户姓名", "业务员姓名", "删除时间"};
		String[] attrNames = new String[] {"nproId", "productName",  "creditId", "name", "inputUser", "deleteTimeString"};
		String rootpath = ServletActionContext.getServletContext().getRealPath("/");
		String path = Constants.FILEUPLOAD + "excel/" + "clientDeleteLogQuery.xls";
		if(logService.exportDelLogExcel(clientdeletelogInfo, titleNames, attrNames, rootpath + path)) {
			ajaxData.put("url", "/" + path);
    		success = true;
		} else {
			msg = "导出失败";
    		success = false;
		}
		return AJAX;
	}
	
	/**
	 * 导出配置日志表格
	 * @author jinxindong 2016年4月9日 下午4:08:48
	 * @return
	 */
	public String exportConfiglogExcel() {
		String[] titleNames = new String[] {"操作用户","操作模块", "IP地址", "操作类型", "操作备注","操作时间"};
		String[] attrNames = new String[] {"strUser","ntypeName","strIp",  "nactionTypeName", "strText", "strDateString"};
		String rootpath = ServletActionContext.getServletContext().getRealPath("/");
		String path = Constants.FILEUPLOAD + "excel/" + "configLogQuery.xls";
		if(logService.exportConfiglogExcel(configlogInfo, titleNames, attrNames, rootpath + path)) {
			ajaxData.put("url", "/" + path);
    		success = true;
		} else {
			msg = "导出失败";
    		success = false;
		}
		return AJAX;
	}
	
	/**
	 * 导出系统日志表格
	 * @author jinxindong 2016年4月9日 下午4:08:57
	 * @return
	 */
	public String exportLogsysExcel() {
		String[] titleNames = new String[] { "操作用户", "用户类型", "IP地址", "操作类型","操作时间"};
		String[] attrNames = new String[] {"strUser","nmoduleTypeName","strIp", "ntypeName","strDateString"};
		String rootpath = ServletActionContext.getServletContext().getRealPath("/");
		String path = Constants.FILEUPLOAD + "excel/" + "logSysQuery.xls";
		if(logService.exportLogsysExcel(logsysInfo, titleNames, attrNames, rootpath + path)) {
			ajaxData.put("url", "/" + path);
    		success = true;
		} else {
			msg = "导出失败";
    		success = false;
		}
		return AJAX;
	}
	
	/**
	 * 导出回放日志表格
	 * @author jinxindong 2016年4月9日 下午4:09:05
	 * @return
	 */
	public String exportRecordLogExcel() {
		String[] titleNames = new String[] {"业务单号", "录像名称", "录像类型", "回放时间", "操作用户"};
		String[] attrNames = new String[] {"nproId", "recordName", "recordTypeName", "operDateString", "checkUserName"};
		String rootpath = ServletActionContext.getServletContext().getRealPath("/");
		String path = Constants.FILEUPLOAD + "excel/" + "recordLogQuery.xls";
		if(logService.exportRecordLogExcel(recordlogInfo, titleNames, attrNames, rootpath + path)) {
			ajaxData.put("url", "/" + path);
    		success = true;
		} else {
			msg = "导出失败";
    		success = false;
		}
		return AJAX;
	}
    //===========================getter/setter==================================================
    public ILogService getLogService() {
    	return logService;
    }
	
    public void setLogService(ILogService logService) {
    	this.logService = logService;
    }
	
    public ClientdeletelogInfo getClientdeletelogInfo() {
    	return clientdeletelogInfo;
    }
	
    public void setClientdeletelogInfo(ClientdeletelogInfo clientdeletelogInfo) {
    	this.clientdeletelogInfo = clientdeletelogInfo;
    }
	
    public LogsysInfo getLogsysInfo() {
    	return logsysInfo;
    }
	
    public void setLogsysInfo(LogsysInfo logsysInfo) {
    	this.logsysInfo = logsysInfo;
    }
	
    public RecordlogInfo getRecordlogInfo() {
    	return recordlogInfo;
    }
	
    public void setRecordlogInfo(RecordlogInfo recordlogInfo) {
    	this.recordlogInfo = recordlogInfo;
    }
	
    public ConfiglogInfo getConfiglogInfo() {
    	return configlogInfo;
    }
	
    public void setConfiglogInfo(ConfiglogInfo configlogInfo) {
    	this.configlogInfo = configlogInfo;
    }

	
    public List<Privilege> getPrivilegeList() {
    	return privilegeList;
    }

	
    public void setPrivilegeList(List<Privilege> privilegeList) {
    	this.privilegeList = privilegeList;
    }
	
    
}
