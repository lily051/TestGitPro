/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月7日 下午7:41:30
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.duerecord.service.impl;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Date;
import java.util.LinkedHashMap;
import java.util.List;

import org.apache.log4j.Logger;

import com.hikvision.finance.core.util.DateUtils;
import com.hikvision.finance.core.util.ListUtil;
import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.service.BaseServiceImpl;
import com.hikvision.finance.fms.common.enums.ClientInputEnum;
import com.hikvision.finance.fms.common.enums.LogModuledEnum;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.common.util.Constants.OperStatus;
import com.hikvision.finance.fms.common.util.ExcelUtil;
import com.hikvision.finance.fms.model.Clientinput;
import com.hikvision.finance.fms.model.Sysconfig;
import com.hikvision.finance.fms.modules.duerecord.dto.DueRecordInfo;
import com.hikvision.finance.fms.modules.duerecord.service.IDueRecordService;
import com.hikvision.finance.fms.modules.log.dao.IConfigLogDao;
import com.hikvision.finance.fms.modules.region.service.IRegionService;
import com.hikvision.finance.fms.modules.sale.dao.IClientInputDao;
import com.hikvision.finance.fms.modules.sale.qo.SaleQo;
import com.hikvision.finance.fms.modules.sysconfig.service.ISysconfigService;
import com.hikvision.finance.fwork.exception.ExpectedException;
import com.hikvision.finance.fwork.util.GlobalMessageUtil;


/**
 * <p>到期信息模块</p>
 * @author jinchenwu 2016年4月7日 下午7:41:30
 * @version V1.0 
 */
public class DueRecordServiceImpl extends BaseServiceImpl<Clientinput, SaleQo, IClientInputDao> implements IDueRecordService {
	
	private static final Logger logger = Logger.getLogger(DueRecordServiceImpl.class);
	private IClientInputDao clientInputDao; 
	private ISysconfigService sysconfigService;
	private IRegionService regionService;
	private IConfigLogDao configLogDao;
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.duerecord.service.IDueRecordService#getIndexDueRecord()
	 */
	public List<DueRecordInfo> getIndexDueRecord(Integer limit){
		DueRecordInfo dueRecordInfo = new DueRecordInfo();
		setThresholdDate(dueRecordInfo);// 设置录像删除提示天数
		dueRecordInfo.setNisRead(0);
		List<DueRecordInfo> dueRecordInfoList = clientInputDao.getDataList(dueRecordInfo, limit);
		return dueRecordInfoList;
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.duerecord.service.IDueRecordService#getDataPage(com.hikvision.finance.fms.modules.record.dto.RecordInfo,
	 * java.lang.Integer, java.lang.Integer)
	 */
	@SuppressWarnings("unchecked")
    public Page getDataPage(DueRecordInfo dueRecordInfo, Integer start, Integer limit) {
		setThresholdDate(dueRecordInfo);// 设置录像删除提示天数
		String strRegionIds = regionService.getLikeStrPathIds(dueRecordInfo.getRegionId(),dueRecordInfo.getIsFlag());
		dueRecordInfo.setStrRegionIds(strRegionIds);// 设置过滤的区域节点
		Page page = clientInputDao.getDataPage(dueRecordInfo, start, limit);
		List<DueRecordInfo> dueRecordInfoList = (List<DueRecordInfo>)page.getResult();
		Date dateNow = getDateNow();
		List<Integer> busiIds = new ArrayList<Integer>();
		for (DueRecordInfo newDueRecordInfo : dueRecordInfoList) {
			int daysAway = (int)((newDueRecordInfo.getStrVideoEndDate().getTime() - dateNow.getTime())/(24*60*60*1000));
			daysAway++;//等于0的时候算距离一天到期   等于0的时候按未到期算
			if (daysAway <=0) {
				daysAway = 0;
				newDueRecordInfo.setNisPay(Constants.VideoDueStatus.IS_DUE);// 设置为已到期
			}
			newDueRecordInfo.setDaysAway(daysAway);
			busiIds.add(newDueRecordInfo.getBusiId());
		}
		//查询业务单关联的录像数目
		List<DueRecordInfo> videoNumInfoList = new ArrayList<DueRecordInfo>();
		if (busiIds.size() > 0) {
			videoNumInfoList = clientInputDao.getVideoNumList(busiIds);
		}
		for(DueRecordInfo newDueRecordInfo : dueRecordInfoList){
			for (DueRecordInfo videoNumInfo : videoNumInfoList) {
				if(videoNumInfo.getBusiId().equals(newDueRecordInfo.getBusiId())){
					newDueRecordInfo.setVideoNum(videoNumInfo.getVideoNum());
					break;
				}
			}
		}
		page.setResult(dueRecordInfoList);
		return page;
	}
	
	/* (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.duerecord.service.IDueRecordService#saveDelayDays(com.hikvision.finance.fms.modules.duerecord.dto.DueRecordInfo)
	 */
	public void saveDelayDays(DueRecordInfo dueRecordInfo) {
		//增加对已到期的校验，已到期不允许延时
		for(Integer busiId:dueRecordInfo.getBusiIds()){
			Clientinput clientinput = clientInputDao.get(busiId);
			Date realEndTime = DateUtils.addDays(clientinput.getStrVideoEndDate(), 1);// 到期日后一天才算到期
			if(realEndTime.before(new Date())){
				throw new ExpectedException("", "含有已到期业务单,无法延期处理");
			}
		}
		clientInputDao.setDelayDays(dueRecordInfo);
		// 写配置日志
		SaleQo saleQo = new SaleQo();
		saleQo.setIds(dueRecordInfo.getBusiIds());
		List<Clientinput> clientinputList = clientInputDao.queryList(saleQo);
		List<String> proIdList = new ArrayList<String>();
		for (Clientinput clientinput : clientinputList) {
			proIdList.add(clientinput.getNproId());
		}
		if(dueRecordInfo.getBusiIds().size()>5){
			configLogDao.setLogContent(
			        OperStatus.UPDATE,
			        LogModuledEnum.DUERECORD.getCode(),
			        GlobalMessageUtil.getMessage("log.duerecord.set.batch", new String[] {dueRecordInfo.getDelayDays().toString()}));
		}else{
			configLogDao.setLogContent(
			        OperStatus.UPDATE,
			        LogModuledEnum.DUERECORD.getCode(),
			        GlobalMessageUtil.getMessage("log.duerecord.set", new String[] {ListUtil.listToStr(proIdList),
			                dueRecordInfo.getDelayDays().toString()}));	
		}
	}
	
	/* (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.duerecord.service.IDueRecordService#saveReadStatus(com.hikvision.finance.fms.modules.duerecord.dto.DueRecordInfo)
	 */
	public void saveReadStatus(DueRecordInfo dueRecordInfo) {
		clientInputDao.setReadStatus(dueRecordInfo);
		// 写配置日志
		SaleQo saleQo = new SaleQo();
		saleQo.setIds(dueRecordInfo.getBusiIds());
		List<Clientinput> clientinputList = clientInputDao.queryList(saleQo);
		List<String> proIdList = new ArrayList<String>();
		for (Clientinput clientinput : clientinputList) {
			proIdList.add(clientinput.getNproId());
		}
		if(dueRecordInfo.getBusiIds().size()>5){
			configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.DUERECORD.getCode(), GlobalMessageUtil.getMessage("log.duerecord.status.batch"));
		}else{
			configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.DUERECORD.getCode(), GlobalMessageUtil.getMessage("log.duerecord.status", new String[] {ListUtil.listToStr(proIdList)}));	
		}
	}
	
    /* 
     * (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.duerecord.service.IDueRecordService#saveLogAndexportExcel(com.hikvision.finance.fms.modules.duerecord.dto.DueRecordInfo, java.lang.String)
     */
    public boolean saveLogAndexportExcel(DueRecordInfo dueRecordInfo, String path) {
    	setThresholdDate(dueRecordInfo);// 设置录像删除提示天数
		String strRegionIds = regionService.getLikeStrPathIds(dueRecordInfo.getRegionId(),dueRecordInfo.getIsFlag());
		dueRecordInfo.setStrRegionIds(strRegionIds);// 设置过滤的区域节点
		// 注意与页面显示内容保持一致！！！
    	String[] titleName = new String[] {"业务单号", "交易网点", "办理时间",  "业务员姓名", "客户姓名", "产品名称", "录像个数", "录像状态", "距离到期天数", "是否已读"};
    	String[] attrName = new String[] {"nproId", "inputRegionName", "strBusinessTime", "proName", "name", "productName", "videoNum", "strIsPay", "daysAway", "strIsRead"};
		// 1，依次准备标题、标题映射属性、Sheet以及对应内容
		List<String[]> titleNameList = new ArrayList<String[]>();
		List<String[]> attrNameList = new ArrayList<String[]>();
		titleNameList.add(titleName);
		attrNameList.add(attrName);
		LinkedHashMap<String, List<?>> map = new LinkedHashMap<String, List<?>>();
		List<DueRecordInfo> dueRecordInfoList = clientInputDao.getDataList(dueRecordInfo, null);
		if(dueRecordInfoList.size() > Constants.EXCEL_MAX_NUM){
			throw new ExpectedException("", "导出数据超过"+Constants.EXCEL_MAX_NUM+"条，无法导出");
		}
		Date dateNow = getDateNow();
		for(DueRecordInfo newDueRecordInfo:dueRecordInfoList){
			String strBusinessTime = DateUtils.getStringDate(newDueRecordInfo.getBusinessTime());
			newDueRecordInfo.setStrBusinessTime(strBusinessTime);
			int daysAway = (int)((newDueRecordInfo.getStrVideoEndDate().getTime() - dateNow.getTime())/(24*60*60*1000));
			daysAway++;//等于0的时候算距离一天到期   等于0的时候按未到期算
			if (daysAway <= 0) {
				daysAway = 0;
				newDueRecordInfo.setNisPay(Constants.VideoDueStatus.IS_DUE);// 设置为已到期
			}
			newDueRecordInfo.setDaysAway(daysAway);
			newDueRecordInfo.setStrIsPay(ClientInputEnum.VideoDueStatus.getStrByN(newDueRecordInfo.getNisPay()));
			newDueRecordInfo.setStrIsRead(ClientInputEnum.BusiReadStatus.getStrByN(newDueRecordInfo.getNisRead()));
		}
		map.put("Sheet1", dueRecordInfoList);
		// 2，创建Excel的数据对象并导出表格
		ExcelUtil.ExcelExportData setInfo = new ExcelUtil.ExcelExportData();
		setInfo.setColumnNames(titleNameList);
		setInfo.setFieldNames(attrNameList);
		setInfo.setDataMap(map);
		boolean exportResult = false;
		try {
			exportResult = ExcelUtil.export2File(setInfo, path);
			configLogDao.setLogContent(OperStatus.EXPORT, LogModuledEnum.DUERECORD.getCode(), GlobalMessageUtil.getMessage("log.duerecord.export"));
		} catch (Exception e) {
			logger.error("导出异常", e);
		}
		return exportResult;
		
    }
	
    /**
     * 设置录像到期显示阈值
     * @author jinchenwu 2016年4月13日 上午10:46:08
     * @param dueRecordInfo
     */
    private void setThresholdDate(DueRecordInfo dueRecordInfo){
		// 查找录像删除提示天数
		List<Integer> keyList = new ArrayList<Integer>();
		keyList.add(Constants.SysConfigType.VIDEO_DELETE_DAYS);
		Sysconfig sysconfig = sysconfigService.getSysconfigByKey(keyList).get(0);
		Integer tipDays = Integer.valueOf(sysconfig.getStrValue());
		Date dateNow = getDateNow();
		Date thresholdDate = DateUtils.addDays(dateNow, tipDays);
		dueRecordInfo.setThresholdDate(thresholdDate);
    }
    
    /**
     * 获取去除时分秒的date类型
     * @author jinchenwu 2016年4月20日 下午2:52:53
     * @return
     */
    private Date getDateNow(){
		String strDateNow = DateUtils.getStringDate(new Date());
		Date dateNow = DateUtils.getDateTime(DateUtils.yyyy_MM_dd, strDateNow);
		return dateNow;
    }
    
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.extend.service.IBaseService#deleteByIds(java.io.Serializable[])
	 */
	@Override
	public void deleteByIds(Serializable[] paramArrayOfSerializable) {
	}
	
	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.service.BaseServiceImpl#getDao()
     */
    @Override
    protected IClientInputDao getDao() {
	    return clientInputDao;
    }
	
	// =======================================getter/setter================================================================
	
	public ISysconfigService getSysconfigService() {
		return sysconfigService;
	}
	
	public void setSysconfigService(ISysconfigService sysconfigService) {
		this.sysconfigService = sysconfigService;
	}
	
	public IRegionService getRegionService() {
		return regionService;
	}
	
	public void setRegionService(IRegionService regionService) {
		this.regionService = regionService;
	}
	
	public IConfigLogDao getConfigLogDao() {
		return configLogDao;
	}
	
	public void setConfigLogDao(IConfigLogDao configLogDao) {
		this.configLogDao = configLogDao;
	}
	
	public IClientInputDao getClientInputDao() {
		return clientInputDao;
	}
	
	public void setClientInputDao(IClientInputDao clientInputDao) {
		this.clientInputDao = clientInputDao;
	}
	
}
