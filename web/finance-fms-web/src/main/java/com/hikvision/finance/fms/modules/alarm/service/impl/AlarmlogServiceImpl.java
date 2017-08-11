/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月6日 下午7:21:25
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.alarm.service.impl;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.service.BaseServiceImpl;
import com.hikvision.finance.fms.common.enums.AlarmTypeEnum;
import com.hikvision.finance.fms.common.enums.LogModuledEnum;
import com.hikvision.finance.fms.common.model.UserSession;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.common.util.SessionUtil;
import com.hikvision.finance.fms.common.util.Constants.OperStatus;
import com.hikvision.finance.fms.model.Alarmlog;
import com.hikvision.finance.fms.model.Region;
import com.hikvision.finance.fms.model.Users;
import com.hikvision.finance.fms.modules.alarm.dao.IAlarmlogDao;
import com.hikvision.finance.fms.modules.alarm.dto.AlarmlogInfo;
import com.hikvision.finance.fms.modules.alarm.qo.AlarmlogQo;
import com.hikvision.finance.fms.modules.alarm.service.IAlarmlogService;
import com.hikvision.finance.fms.modules.log.dao.IConfigLogDao;
import com.hikvision.finance.fms.modules.region.dao.IRegionDao;
import com.hikvision.finance.fms.modules.user.dao.IUsersDao;
import com.hikvision.finance.fwork.util.AjaxData;
import com.hikvision.finance.fwork.util.GlobalMessageUtil;


/**
 * <p></p>
 * @author jinxindong 2016年4月6日 下午7:21:25
 * @version V1.0 
 */
public class AlarmlogServiceImpl extends BaseServiceImpl<Alarmlog,AlarmlogQo,IAlarmlogDao> implements IAlarmlogService{

	private IAlarmlogDao alarmlogDao;
	private IUsersDao usersDao;
	private IConfigLogDao configLogDao;
	private IRegionDao regionDao;
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
    protected IAlarmlogDao getDao() {
	    // TODO Auto-generated method stub
	    return null;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.alarm.service.IAlarmlogService#getAlarmDataPage(java.lang.Integer, java.lang.Integer, com.hikvision.finance.fms.modules.alarm.dto.AlarmlogInfo)
     */
    @SuppressWarnings("unchecked")
    @Override
	public Page getAlarmDataPage(Integer limit, Integer start, AlarmlogInfo alarmlogInfo) {
		Page page = alarmlogDao.getAlarmDataPage(limit, start, alarmlogInfo);
		List<Region> regions = regionDao.getAll();
		Map<Integer,Region> regionMap = new HashMap<Integer, Region>();
		for (Region region : regions) {
			regionMap.put(region.getId(), region);
		}
		
		// 循环page 插入报警类型名称
		List<AlarmlogInfo> alarmlogInfos = (List<AlarmlogInfo>)page.getResult();
		List<AlarmlogInfo> alarmlogInfoList = new ArrayList<AlarmlogInfo>();
		Map<String, String> alarmMap = AlarmTypeEnum.getMap();
		String alarmType = "";
		for (AlarmlogInfo alarmOld : alarmlogInfos) {
			alarmType = alarmOld.getAlarmType().toString();
			alarmOld.setAlarmTypeName(alarmMap.get(alarmType));
			alarmOld.setAlarmRegionName(regionMap.get(0 == alarmOld.getAlarmRegionId()?1:alarmOld.getAlarmRegionId()).getStrName());			
			alarmlogInfoList.add(alarmOld);
		}
		page.setResult(alarmlogInfoList);
		return page;
	}

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.alarm.service.IAlarmlogService#deleteAlarmLogs(java.util.List)
     */
    @Override
    public AjaxData deleteAlarmLogs(List<Integer> ids) {
	    AjaxData ajaxData = new AjaxData(false);
	    for(Integer alarmId : ids){
	    	alarmlogDao.deleteById(alarmId);
	    }
	    configLogDao.setLogContent(OperStatus.DELETE, LogModuledEnum.ALARM.getCode(),GlobalMessageUtil.getMessage("log.alarm.delete"));
	    return ajaxData;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.alarm.service.IAlarmlogService#updateAlarmLogs(com.hikvision.finance.fms.modules.alarm.dto.AlarmlogInfo)
     */
    @Override
	public AjaxData updateAlarmLogs(AlarmlogInfo alarmlogInfo, List<Integer> ids) {
		AjaxData ajaxData = new AjaxData(false);
		UserSession userSession = SessionUtil.getUserSession();
		Users user = usersDao.get(userSession.getUserId());
		for (Integer id : ids) {
			Alarmlog alarmlog = alarmlogDao.get(id);
			alarmlog.setDealcomment(alarmlogInfo.getDealcomment());
			alarmlog.setDealTime(new Date());
			alarmlog.setChecker(user.getStrName());
			alarmlog.setUserNo(user.getUserNo());
			alarmlog.setStatus(Constants.CONS_1);
			alarmlogDao.update(alarmlog);
		}
		configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.ALARM.getCode(),GlobalMessageUtil.getMessage("log.alarm.update"));
		return ajaxData;
	}
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.alarm.service.IAlarmlogService#getIndexAlarm()
     */
    @Override
    public List<AlarmlogInfo> getIndexAlarm() {
    	List<AlarmlogInfo> list = alarmlogDao.getIndexAlarm(6);
    	List<AlarmlogInfo> alarmlogList = new ArrayList<AlarmlogInfo>();
    	if(list!=null && list.size()>0){
    		Map<String, String> alarmMap = AlarmTypeEnum.getMap();
    		for (AlarmlogInfo alarmOld : list) {
    			String alarmType = alarmOld.getAlarmType().toString();
    			alarmOld.setAlarmTypeName(alarmMap.get(alarmType));
    			alarmlogList.add(alarmOld);
    		}	
    	}
	    return alarmlogList;
    }
	//======================getter/setter========================
    public IAlarmlogDao getAlarmlogDao() {
    	return alarmlogDao;
    }

	
    public void setAlarmlogDao(IAlarmlogDao alarmlogDao) {
    	this.alarmlogDao = alarmlogDao;
    }

	
    public IUsersDao getUsersDao() {
    	return usersDao;
    }

	
    public void setUsersDao(IUsersDao usersDao) {
    	this.usersDao = usersDao;
    }

	
    public IConfigLogDao getConfigLogDao() {
    	return configLogDao;
    }

	
    public void setConfigLogDao(IConfigLogDao configLogDao) {
    	this.configLogDao = configLogDao;
    }

	
    public IRegionDao getRegionDao() {
    	return regionDao;
    }

	
    public void setRegionDao(IRegionDao regionDao) {
    	this.regionDao = regionDao;
    }

	
    
}
