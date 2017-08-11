/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月8日 下午4:49:52
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.log.service.impl;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;

import org.apache.struts2.ServletActionContext;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.hikvision.finance.core.util.DateUtils;
import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.service.BaseServiceImpl;
import com.hikvision.finance.fms.common.enums.LogModuledEnum;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.common.util.Constants.OperStatus;
import com.hikvision.finance.fms.common.util.ExcelUtil;
import com.hikvision.finance.fms.model.Logsys;
import com.hikvision.finance.fms.model.Privilege;
import com.hikvision.finance.fms.model.Recordlog;
import com.hikvision.finance.fms.model.Sysconfig;
import com.hikvision.finance.fms.modules.alarm.dao.IAlarmlogDao;
import com.hikvision.finance.fms.modules.log.dao.IClientDeleteLogDao;
import com.hikvision.finance.fms.modules.log.dao.IConfigLogDao;
import com.hikvision.finance.fms.modules.log.dao.ILogsysDao;
import com.hikvision.finance.fms.modules.log.dao.IRecordLogDao;
import com.hikvision.finance.fms.modules.log.dto.ClientdeletelogInfo;
import com.hikvision.finance.fms.modules.log.dto.ConfiglogInfo;
import com.hikvision.finance.fms.modules.log.dto.LogsysInfo;
import com.hikvision.finance.fms.modules.log.dto.RecordlogInfo;
import com.hikvision.finance.fms.modules.log.qo.RecordlogQo;
import com.hikvision.finance.fms.modules.log.service.ILogService;
import com.hikvision.finance.fms.modules.role.dao.IPrivilegeDao;
import com.hikvision.finance.fms.modules.sysconfig.service.ISysconfigService;
import com.hikvision.finance.fms.webdevice.log.dao.ICollLogDao;
import com.hikvision.finance.fwork.util.GlobalMessageUtil;
import com.hikvision.finance.fwork.util.ServletUtil;


/**
 * <p></p>
 * @author jinxindong 2016年4月8日 下午4:49:52
 * @version V1.0 
 */
public class LogServiceImpl extends BaseServiceImpl<Recordlog,RecordlogQo, IRecordLogDao> implements ILogService{

	private ILogsysDao logsysDao;
	private IRecordLogDao recordLogDao;
	private IConfigLogDao configLogDao;
	private IClientDeleteLogDao clientDeleteLogDao;
	private IAlarmlogDao alarmlogDao;
	private ICollLogDao collLogDao;
	private IPrivilegeDao privilegeDao;
	private ISysconfigService sysconfigService;
	private static Logger logger = LoggerFactory.getLogger(LogServiceImpl.class);
	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.service.IBaseService#deleteByIds(java.io.Serializable[])
     */
    @Override
    public void deleteByIds(Serializable[] paramArrayOfSerializable) {
	    // TODO Auto-generated method stub
	    
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.service.BaseServiceImpl#getDao()
     */
    @Override
    protected IRecordLogDao getDao() {
	    // TODO Auto-generated method stub
	    return null;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.log.service.ILogService#getRecordLogDataPage(java.lang.Integer, java.lang.Integer, com.hikvision.finance.fms.modules.log.dto.RecordlogInfo)
     */
    @Override
    public Page getRecordLogDataPage(Integer limit, Integer start, RecordlogInfo recordlogInfo) {
    	Page page = recordLogDao.getRecordLogDataPage(limit, start, recordlogInfo);
    	return page;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.log.service.ILogService#getClientDelLogDataPage(java.lang.Integer, java.lang.Integer, com.hikvision.finance.fms.modules.log.dto.ClientdeletelogInfo)
     */
    @Override
    public Page getClientDelLogDataPage(Integer limit, Integer start, ClientdeletelogInfo clientdeletelogInfo) {
    	Page page = clientDeleteLogDao.getClientDelLogDataPage(limit, start, clientdeletelogInfo);
		return page;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.log.service.ILogService#getLogsysDataPage(java.lang.Integer, java.lang.Integer, com.hikvision.finance.fms.modules.log.dto.LogsysInfo)
     */
    @Override
    public Page getLogsysDataPage(Integer limit, Integer start, LogsysInfo logsysInfo) {
    	Page page = logsysDao.getLogsysDataPage(limit, start, logsysInfo);
		return page;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.log.service.ILogService#getConfiglogDataPage(java.lang.Integer, java.lang.Integer, com.hikvision.finance.fms.modules.log.dto.ConfiglogInfo)
     */
    @SuppressWarnings("unchecked")
    @Override
    public Page getConfiglogDataPage(Integer limit, Integer start, ConfiglogInfo configlogInfo) {
    	Page page = configLogDao.getConfiglogDataPage(limit, start, configlogInfo);
    	//数据库中取出可用的模块名
    	List<Privilege> privileges = privilegeDao.findBy("nenabled", Constants.CONS_1);
    	Map<String,Privilege> privilegeMap = new HashMap<String, Privilege>();
    	for(Privilege privilege:privileges){
    		privilegeMap.put(privilege.getStrCode(), privilege);
    	}
		// 循环page 插入模块名
		List<ConfiglogInfo> configlogInfos = (List<ConfiglogInfo>)page.getResult();
		List<ConfiglogInfo> configlogInfoList = new ArrayList<ConfiglogInfo>();
		for (ConfiglogInfo configlogOld : configlogInfos) {
			Privilege privilege = privilegeMap.get(configlogOld.getNtype().toString());
			String ntypeName = privilege.getStrName();
			configlogOld.setNtypeName(ntypeName);
			configlogInfoList.add(configlogOld);
		}
		page.setResult(configlogInfoList);
		return page;
    }
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.log.service.ILogService#exportDelLogExcel(com.hikvision.finance.fms.modules.log.dto.ClientdeletelogInfo, java.lang.String[], java.lang.String[], java.lang.String)
     */
    @Override
    public boolean exportDelLogExcel(ClientdeletelogInfo clientdeletelogInfo, String[] titleName, String[] attrName, String path) {
		// 1，依次准备标题、标题映射属性、Sheet以及对应内容
		List<String[]> titleNames = new ArrayList<String[]>();
		List<String[]> attrNames = new ArrayList<String[]>();
		LinkedHashMap<String, List<?>> map = new LinkedHashMap<String, List<?>>();
		titleNames.add(titleName);
		attrNames.add(attrName);
		List<ClientdeletelogInfo> clientdeletelogInfos1 = clientDeleteLogDao.exportDelLogExcel(clientdeletelogInfo);
		List<ClientdeletelogInfo> clientdeletelogInfos = new ArrayList<ClientdeletelogInfo>();
		for(ClientdeletelogInfo info: clientdeletelogInfos1){
			String date = DateUtils.getStringDateTime(DateUtils.yyyy_MM_dd_HH_mm_ss, info.getDeleteTime());
			info.setDeleteTimeString(date);
			clientdeletelogInfos.add(info);
		}
		map.put("Sheet1", clientdeletelogInfos);
		// 2，创建Excel的数据对象并导出表格
		ExcelUtil.ExcelExportData setInfo = new ExcelUtil.ExcelExportData();
		setInfo.setColumnNames(titleNames);
		setInfo.setFieldNames(attrNames);
		setInfo.setDataMap(map);
		boolean exportResult = false;
		try {
			exportResult = ExcelUtil.export2File(setInfo, path);
			configLogDao.setLogContent(OperStatus.EXPORT, LogModuledEnum.LOG.getCode(),GlobalMessageUtil.getMessage("log.dellog.export"));
		} catch (Exception e) {
			e.printStackTrace();
			logger.error(e.getMessage());
		}
		return exportResult;
		
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.log.service.ILogService#exportConfiglogExcel(com.hikvision.finance.fms.modules.log.dto.ConfiglogInfo, java.lang.String[], java.lang.String[], java.lang.String)
     */
    @Override
    public boolean exportConfiglogExcel(ConfiglogInfo configlogInfo, String[] titleName, String[] attrName, String path) {
		// 1，依次准备标题、标题映射属性、Sheet以及对应内容
		List<String[]> titleNames = new ArrayList<String[]>();
		List<String[]> attrNames = new ArrayList<String[]>();
		LinkedHashMap<String, List<?>> map = new LinkedHashMap<String, List<?>>();
		titleNames.add(titleName);
		attrNames.add(attrName);
		List<ConfiglogInfo> configlogInfos1 = configLogDao.exportConfiglogExcel(configlogInfo);
		List<ConfiglogInfo> configlogInfos = new ArrayList<ConfiglogInfo>();
		for(ConfiglogInfo info: configlogInfos1){
			String date = DateUtils.getStringDateTime(DateUtils.yyyy_MM_dd_HH_mm_ss, info.getStrDate());
			info.setStrDateString(date);
			configlogInfos.add(info);
		}
		map.put("Sheet1", configlogInfos);
		// 2，创建Excel的数据对象并导出表格
		ExcelUtil.ExcelExportData setInfo = new ExcelUtil.ExcelExportData();
		setInfo.setColumnNames(titleNames);
		setInfo.setFieldNames(attrNames);
		setInfo.setDataMap(map);
		boolean exportResult = false;
		try {
			exportResult = ExcelUtil.export2File(setInfo, path);
			configLogDao.setLogContent(OperStatus.EXPORT, LogModuledEnum.LOG.getCode(),GlobalMessageUtil.getMessage("log.configlog.export"));
		} catch (Exception e) {
			e.printStackTrace();
		}
		return exportResult;
		
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.log.service.ILogService#exportLogsysExcel(com.hikvision.finance.fms.modules.log.dto.LogsysInfo, java.lang.String[], java.lang.String[], java.lang.String)
     */
    @Override
    public boolean exportLogsysExcel(LogsysInfo logsysInfo, String[] titleName, String[] attrName, String path) {
		// 1，依次准备标题、标题映射属性、Sheet以及对应内容
		List<String[]> titleNames = new ArrayList<String[]>();
		List<String[]> attrNames = new ArrayList<String[]>();
		LinkedHashMap<String, List<?>> map = new LinkedHashMap<String, List<?>>();
		titleNames.add(titleName);
		attrNames.add(attrName);
		List<LogsysInfo> logsysInfos1 = logsysDao.exportLogsysExcel(logsysInfo);
		List<LogsysInfo> logsysInfos = new ArrayList<LogsysInfo>();
		for(LogsysInfo info: logsysInfos1){
			String date = DateUtils.getStringDateTime(DateUtils.yyyy_MM_dd_HH_mm_ss, info.getStrDate());
			info.setStrDateString(date);
			logsysInfos.add(info);
		}
		map.put("Sheet1", logsysInfos);
		// 2，创建Excel的数据对象并导出表格
		ExcelUtil.ExcelExportData setInfo = new ExcelUtil.ExcelExportData();
		setInfo.setColumnNames(titleNames);
		setInfo.setFieldNames(attrNames);
		setInfo.setDataMap(map);
		boolean exportResult = false;
		try {
			exportResult = ExcelUtil.export2File(setInfo, path);
			configLogDao.setLogContent(OperStatus.EXPORT, LogModuledEnum.LOG.getCode(),GlobalMessageUtil.getMessage("log.syslog.export"));
		} catch (Exception e) {
			e.printStackTrace();
		}
		return exportResult;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.log.service.ILogService#exportRecordLogExcel(com.hikvision.finance.fms.modules.log.dto.RecordlogInfo, java.lang.String[], java.lang.String[], java.lang.String)
     */
    @Override
    public boolean exportRecordLogExcel(RecordlogInfo recordlogInfo, String[] titleName, String[] attrName, String path) {
		// 1，依次准备标题、标题映射属性、Sheet以及对应内容
		List<String[]> titleNames = new ArrayList<String[]>();
		List<String[]> attrNames = new ArrayList<String[]>();
		LinkedHashMap<String, List<?>> map = new LinkedHashMap<String, List<?>>();
		titleNames.add(titleName);
		attrNames.add(attrName);
		List<RecordlogInfo> recordlogInfos1 = recordLogDao.exportRecordLogExcel(recordlogInfo);
		List<RecordlogInfo> recordlogInfos = new ArrayList<RecordlogInfo>();
		for(RecordlogInfo info: recordlogInfos1){
			String date = DateUtils.getStringDateTime(DateUtils.yyyy_MM_dd_HH_mm_ss, info.getOperDate());
			info.setOperDateString(date);
			recordlogInfos.add(info);
		}
		map.put("Sheet1", recordlogInfos);
		// 2，创建Excel的数据对象并导出表格
		ExcelUtil.ExcelExportData setInfo = new ExcelUtil.ExcelExportData();
		setInfo.setColumnNames(titleNames);
		setInfo.setFieldNames(attrNames);
		setInfo.setDataMap(map);
		boolean exportResult = false;
		try {
			exportResult = ExcelUtil.export2File(setInfo, path);
			configLogDao.setLogContent(OperStatus.EXPORT, LogModuledEnum.LOG.getCode(),GlobalMessageUtil.getMessage("log.rerordlog.export"));
		} catch (Exception e) {
			e.printStackTrace();
		}
		return exportResult;
		
    }
    
    /*
     * (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.log.service.ILogService#deleteLog()
     */
    @Override
    public void deleteLog(){
		List<Integer> sysconfigKeyList = new ArrayList<Integer>();
		sysconfigKeyList.add(Constants.SysConfigType.CONFIG_LOG);// 配置日志
		sysconfigKeyList.add(Constants.SysConfigType.SYSTEM_LOG);// 系统日志
		sysconfigKeyList.add(Constants.SysConfigType.REVIEW_LOG);// 回放日志
		sysconfigKeyList.add(Constants.SysConfigType.DELETE_LOG);// 删除日志
		sysconfigKeyList.add(Constants.SysConfigType.ALARM_LOG);// 报警日志
		sysconfigKeyList.add(Constants.SysConfigType.OPERA_LOG);// 操作日志
		List<Sysconfig> sysconfigList = sysconfigService.getSysconfigByKey(sysconfigKeyList);
		Map<Integer, Integer> sysconfigMap = new HashMap<Integer, Integer>();
		for (Sysconfig sysconfig : sysconfigList) {
			sysconfigMap.put(sysconfig.getNkey(), Integer.valueOf(sysconfig.getStrValue()));
		}
		String strDateNow = DateUtils.getStringDate(new Date());
		Date dateNow = DateUtils.getDateTime(DateUtils.yyyy_MM_dd, strDateNow);
		Date configLogthresholdDate = DateUtils.addDays(dateNow, -sysconfigMap.get(Constants.SysConfigType.CONFIG_LOG));
		Date sysLogthresholdDate = DateUtils.addDays(dateNow, -sysconfigMap.get(Constants.SysConfigType.SYSTEM_LOG));		
		Date recordLogthresholdDate = DateUtils.addDays(dateNow, -sysconfigMap.get(Constants.SysConfigType.REVIEW_LOG));
		Date deleteLogthresholdDate = DateUtils.addDays(dateNow, -sysconfigMap.get(Constants.SysConfigType.DELETE_LOG));
		Date alarmLogthresholdDate = DateUtils.addDays(dateNow, -sysconfigMap.get(Constants.SysConfigType.ALARM_LOG));
		Date operLogthresholdDate = DateUtils.addDays(dateNow, -sysconfigMap.get(Constants.SysConfigType.OPERA_LOG));
		configLogDao.deleteLog(configLogthresholdDate);// 配置日志
		logsysDao.deleteLog(sysLogthresholdDate);// 系统日志
		recordLogDao.deleteLog(recordLogthresholdDate);// 回放日志
		clientDeleteLogDao.deleteLog(deleteLogthresholdDate);// 删除日志
		alarmlogDao.deleteLog(alarmLogthresholdDate);// 报警日志
		collLogDao.deleteLog(operLogthresholdDate);// 操作日志
    } 
    
    /*
     * (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.log.service.ILogService#setSyslog(com.hikvision.finance.fms.model.Logsys)
     */
    public void saveSyslog(String userName, Integer ntype,Integer nmoduleType){
    	Logsys logsys= new Logsys();
    	logsys.setNmoduleType(nmoduleType);//采集还是中心登录
    	logsys.setStrUser(userName);
    	logsys.setNtype(ntype);
    	logsys.setStrDate(new Date());
		HttpServletRequest request = ServletActionContext.getRequest();
    	logsys.setStrIp(ServletUtil.getRemortIP(request));
    	logsysDao.save(logsys);
    }
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.log.service.ILogService#getPrivilegeList()
     */
    @Override
    public List<Privilege> getPrivilegeList() {
    	List<Privilege> privileges =  privilegeDao.findBy(new String[]{"nenabled","strSuper","nsysNo"}, new Object[]{Constants.CONS_1,Constants.CONS_0_STR,Constants.CONS_0});
    	List<Privilege> newPrivileges = new ArrayList<Privilege>();
    	//剔除统计分析
    	for(Privilege pri: privileges){
    		if(!"/web/analysis.action".equals(pri.getStrHref())){
    			newPrivileges.add(pri);
    		}
    	}
    	Privilege privilege = new Privilege();
    	privilege.setStrCode("");
    	privilege.setStrName("全部");
    	newPrivileges.add(0, privilege);
    	return newPrivileges;
    }
    
	//=======================================getter、setter========================================================
    
	public ILogsysDao getLogsysDao() {
		return logsysDao;
	}
	
	public void setLogsysDao(ILogsysDao logsysDao) {
		this.logsysDao = logsysDao;
	}
	
	public IRecordLogDao getRecordLogDao() {
		return recordLogDao;
	}
	
	public void setRecordLogDao(IRecordLogDao recordLogDao) {
		this.recordLogDao = recordLogDao;
	}
	
	public IConfigLogDao getConfigLogDao() {
		return configLogDao;
	}
	
	public void setConfigLogDao(IConfigLogDao configLogDao) {
		this.configLogDao = configLogDao;
	}
	
	public IClientDeleteLogDao getClientDeleteLogDao() {
		return clientDeleteLogDao;
	}
	
	public void setClientDeleteLogDao(IClientDeleteLogDao clientDeleteLogDao) {
		this.clientDeleteLogDao = clientDeleteLogDao;
	}
	
	public IPrivilegeDao getPrivilegeDao() {
		return privilegeDao;
	}
	
	public void setPrivilegeDao(IPrivilegeDao privilegeDao) {
		this.privilegeDao = privilegeDao;
	}
	
	public ISysconfigService getSysconfigService() {
		return sysconfigService;
	}
	
	public void setSysconfigService(ISysconfigService sysconfigService) {
		this.sysconfigService = sysconfigService;
	}
	
	public IAlarmlogDao getAlarmlogDao() {
		return alarmlogDao;
	}
	
	public void setAlarmlogDao(IAlarmlogDao alarmlogDao) {
		this.alarmlogDao = alarmlogDao;
	}
	
	public ICollLogDao getCollLogDao() {
		return collLogDao;
	}
	
	public void setCollLogDao(ICollLogDao collLogDao) {
		this.collLogDao = collLogDao;
	}
	
}
