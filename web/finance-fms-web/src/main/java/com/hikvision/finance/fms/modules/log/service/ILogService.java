/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月8日 下午4:49:26
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.log.service;

import java.util.List;

import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.service.IBaseService;
import com.hikvision.finance.fms.model.Privilege;
import com.hikvision.finance.fms.model.Recordlog;
import com.hikvision.finance.fms.modules.log.dto.ClientdeletelogInfo;
import com.hikvision.finance.fms.modules.log.dto.ConfiglogInfo;
import com.hikvision.finance.fms.modules.log.dto.LogsysInfo;
import com.hikvision.finance.fms.modules.log.dto.RecordlogInfo;
import com.hikvision.finance.fms.modules.log.qo.RecordlogQo;

/**
 * @author jinxindong 2016年4月8日 下午4:49:26
 * @version V1.0
 */
public interface ILogService extends IBaseService<Recordlog, RecordlogQo> {
	
	/**
	 * 回放日志
	 * @author jinxindong 2016年4月9日 上午10:16:44
	 * @param limit
	 * @param start
	 * @param recordlogInfo
	 * @return
	 */
	public Page getRecordLogDataPage(Integer limit, Integer start, RecordlogInfo recordlogInfo);
	
	/**
	 * 删除日志
	 * @author jinxindong 2016年4月9日 上午10:16:53
	 * @param limit
	 * @param start
	 * @param clientdeletelogInfo
	 * @return
	 */
	public Page getClientDelLogDataPage(Integer limit, Integer start, ClientdeletelogInfo clientdeletelogInfo);
	
	/**
	 * 系统日志
	 * @author jinxindong 2016年4月9日 上午10:17:03
	 * @param limit
	 * @param start
	 * @param logsysInfo
	 * @return
	 */
	public Page getLogsysDataPage(Integer limit, Integer start, LogsysInfo logsysInfo);
	
	/**
	 * 配置日志
	 * @author jinxindong 2016年4月9日 上午10:17:16
	 * @param limit
	 * @param start
	 * @param configlogInfo
	 * @return
	 */
	public Page getConfiglogDataPage(Integer limit, Integer start, ConfiglogInfo configlogInfo);
	
	/**
	 * 获取模块下拉列表
	 * @author jinxindong 2016年4月11日 上午10:32:20
	 * @return
	 */
	public List<Privilege> getPrivilegeList();
	
	/**
	 * 删除日志导出表格
	 * @author jinxindong 2016年4月9日 下午4:01:19
	 * @param clientdeletelogInfo
	 * @param titleName
	 * @param attrName
	 * @param path
	 * @return
	 */
	public boolean exportDelLogExcel(ClientdeletelogInfo clientdeletelogInfo, String[] titleName, String[] attrName, String path);
	
	/**
	 * 配置日志导出表格
	 * @author jinxindong 2016年4月9日 下午4:01:43
	 * @param configlogInfo
	 * @param titleName
	 * @param attrName
	 * @param path
	 * @return
	 */
	public boolean exportConfiglogExcel(ConfiglogInfo configlogInfo, String[] titleName, String[] attrName, String path);
	
	/**
	 * 系统日志导出表格
	 * @author jinxindong 2016年4月9日 下午4:01:51
	 * @param logsysInfo
	 * @param titleName
	 * @param attrName
	 * @param path
	 * @return
	 */
	public boolean exportLogsysExcel(LogsysInfo logsysInfo, String[] titleName, String[] attrName, String path);
	
	/**
	 * 回放日志导出表格
	 * @author jinxindong 2016年4月9日 下午4:02:05
	 * @param recordlogInfo
	 * @param titleName
	 * @param attrName
	 * @param path
	 * @return
	 */
	public boolean exportRecordLogExcel(RecordlogInfo recordlogInfo, String[] titleName, String[] attrName, String path);
	
	/**
	 * 删除日志
	 * @author jinchenwu 2016年4月14日 下午3:52:14
	 */
    public void deleteLog();
    
    /**
     * 写入系统日志
     * @author jinchenwu 2016年4月15日 下午1:33:42
     * @param userName
     * @param ntype
     * @param nmoduleType
     */
    public void saveSyslog(String userName, Integer ntype,Integer nmoduleType);
}
