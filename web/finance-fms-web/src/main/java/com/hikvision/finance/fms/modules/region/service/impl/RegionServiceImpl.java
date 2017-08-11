/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月25日 上午9:49:25
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.region.service.impl;

import java.io.File;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

import org.apache.commons.collections.CollectionUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.google.common.collect.Lists;
import com.google.common.collect.Maps;
import com.google.common.collect.Multimap;
import com.hikvision.finance.core.util.BeanUtils;
import com.hikvision.finance.core.util.ListUtil;
import com.hikvision.finance.core.util.StringUtils;
import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.service.BaseServiceImpl;
import com.hikvision.finance.fms.common.enums.LogModuledEnum;
import com.hikvision.finance.fms.common.excel.ExcelHelper;
import com.hikvision.finance.fms.common.excel.ExcelHelperFactory;
import com.hikvision.finance.fms.common.excel.model.ExcelHead;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.common.util.Constants.OperStatus;
import com.hikvision.finance.fms.common.util.ExcelUtil;
import com.hikvision.finance.fms.common.util.StringUtilExpand;
import com.hikvision.finance.fms.common.util.ValidateUtil;
import com.hikvision.finance.fms.model.Clientinput;
import com.hikvision.finance.fms.model.Device;
import com.hikvision.finance.fms.model.Region;
import com.hikvision.finance.fms.model.Userregionmapping;
import com.hikvision.finance.fms.model.Users;
import com.hikvision.finance.fms.modules.device.dao.IDeviceDao;
import com.hikvision.finance.fms.modules.log.dao.IConfigLogDao;
import com.hikvision.finance.fms.modules.region.dao.IRegionDao;
import com.hikvision.finance.fms.modules.region.dto.RegionInfo;
import com.hikvision.finance.fms.modules.region.qo.RegionQo;
import com.hikvision.finance.fms.modules.region.service.IRegionService;
import com.hikvision.finance.fms.modules.sale.dao.IClientInputDao;
import com.hikvision.finance.fms.modules.user.dao.IUserregionmappingDao;
import com.hikvision.finance.fms.modules.user.dao.IUsersDao;
import com.hikvision.finance.fwork.exception.ExpectedException;
import com.hikvision.finance.fwork.util.AjaxData;
import com.hikvision.finance.fwork.util.GlobalMessageUtil;

/**
 * <p>
 * </p>
 * @author jinxindong 2016年2月25日 上午9:49:25
 * @version V1.0
 */
public class RegionServiceImpl extends BaseServiceImpl<Region, RegionQo, IRegionDao> implements IRegionService {
	
	private IRegionDao regionDao;
	private IConfigLogDao configLogDao;
	private IUserregionmappingDao userregionmappingDao;
	private IDeviceDao deviceDao; 
	private IClientInputDao clientInputDao;
	private IUsersDao usersDao;
	private static Logger logger = LoggerFactory.getLogger(RegionServiceImpl.class);
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.region.service.IRegionService#addReg(com.hikvision.finance.fms.model.Region)
	 */
	@Override
	public AjaxData addReg(RegionInfo regionInfo) {
		AjaxData ajaxData = new AjaxData(false);
		try {
			// 1，验证必要信息
			if (regionInfo.getNparentId() == null) {
				ajaxData.setFailureMsg("未获取父组织的id");
				return ajaxData;
			}
			// 2，验证唯一性
			if (regionInfo.getStrName() != null) {
				AjaxData isExist = isExistReg(1, regionInfo.getStrName(), null);
				if (isExist.getSuccess()) {
					ajaxData.setFailureMsg("已存在名称为" + regionInfo.getStrName());
					return ajaxData;
				}
			}
			if (regionInfo.getStrCode() != null) {
				AjaxData isExist = isExistReg(2, regionInfo.getStrCode(), null);
				if (isExist.getSuccess()) {
					ajaxData.setFailureMsg("已存在代码为" + regionInfo.getStrCode());
					return ajaxData;
				}
			}
			if (regionInfo.getStrInterAreaCode() != null) {
				AjaxData isExist = isExistReg(3, regionInfo.getStrInterAreaCode(), null);
				if (isExist.getSuccess()) {
					ajaxData.setFailureMsg("已存在编码为" + regionInfo.getStrInterAreaCode());
					return ajaxData;
				}
			}
			
			// 3，若添加的为组织，验证最大层级
			if (getOrgLevel(regionInfo.getNparentId()) >= Constants.RegNum.REGION_NUM) {
				ajaxData.setFailureMsg("组织" + regionDao.get(regionInfo.getNparentId()).getStrName() + "已达到最大层级");
				return ajaxData;
			}
			
			Region region = new Region();
			region.setNparentId(regionInfo.getNparentId());
			region.setStrName(regionInfo.getStrName());
			region.setStrCode(regionInfo.getStrCode());
			region.setStrTxt(regionInfo.getStrTxt());
			region.setStrInterAreaCode(regionInfo.getStrInterAreaCode());
			// 设置排序号
			region.setNsortId(getNsort());
			// 设置strPath
			region.setStrPath(getStrPath(regionInfo.getNparentId(), regionInfo.getStrInterAreaCode()));
			regionDao.save(region);
			configLogDao.setLogContent(OperStatus.ADD, LogModuledEnum.REGION.getCode(),GlobalMessageUtil.getMessage("log.org.add",new String[] {region.getStrName()}));
		} catch (NullPointerException e) {
			throw new ExpectedException("", e.getMessage());
		}
		
		return ajaxData;
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.region.service.IRegionService#getOrgSeril(java.lang.Integer)
	 */
	@Override
	public String getRegSeril(Integer nparentId) {
		String ex = null;
		String code = null;
		boolean isExistCode = false;
		// 控制单元
		if (nparentId == 1) {
			ex = "100000";
		} else {
			Region reg = (Region)regionDao.get(nparentId);
			ex = StringUtilExpand.getLengthStr(reg.getStrInterAreaCode(), 6);
		}
		// B规则：区域代码： 1000000000+6位随机数+90
		code = ex + "0000" + StringUtilExpand.getSixRandomSeril() + 90;
		// 查找是否存在
		RegionQo regQo = new RegionQo();
		regQo.setStrInterAreaCode(code);
		Region region = regionDao.queryUnique(regQo);
		if (region != null) {
			isExistCode = true;
		}
		// 如果存在，重新生成
		while (isExistCode) {
			code = ex + "0000" + StringUtilExpand.getSixRandomSeril() + 90;
			regQo.setStrInterAreaCode(code);
			region = regionDao.queryUnique(regQo);
			if (region != null) {
				isExistCode = true;
			}
		}
		return code;
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.region.service.IRegionService#deleteReg(java.util.List)
	 */
	@Override
	public AjaxData deleteReg(List<Integer> ids) {
		AjaxData ajaxData = new AjaxData(false);
		
		// 1,获取组织的所有子孙节点ids
		List<Region> orgs = regionDao.getAll();
		List<Integer> delIds = new ArrayList<Integer>();
		for (Integer delId : ids) {			
			delIds.add(delId);
			getAllChildrenIds(orgs, delId, delIds);
		}
		
		// 2,校验关联关系
		for(Integer dId :delIds){
			//判断用户关联、设备关联、clientinput、prosaverecord、用户net
			List<Userregionmapping> userregionmappings = userregionmappingDao.findBy("nregionId", dId);
			if(userregionmappings != null && userregionmappings.size()>0){
				throw new ExpectedException("", "无法删除,所选的组织中,已有用户关联组织权限");
			}
			
			List<Device> devices = deviceDao.findBy("nregionId", dId);
			if(devices != null && devices.size()>0){
				throw new ExpectedException("", "无法删除,所选的组织中,已有设备关联组织");
			}
			
			List<Clientinput> clientinputs = clientInputDao.findBy("inputRegionId", dId);
			if(clientinputs != null && clientinputs.size()>0){
				throw new ExpectedException("", "无法删除,所选的组织中,已有业务单关联组织");
			}
			
			List<Users> userList = usersDao.findBy("netId", dId);
			if(userList != null && userList.size()>0){
				throw new ExpectedException("", "无法删除,所选的组织中,含有用户网点");
			}
/*			List<Prosaverecord>  prosaverecords = recordDao.findBy("regionId", dId);
			if (prosaverecords != null && prosaverecords.size() > 0) {
				throw new ExpectedException("", "删除的组织中,已有录像关联组织");
			}*/
			
		}
		
		//3,删除并写日志
		RegionQo regionQo = new RegionQo();
		regionQo.setIds(delIds);
		List<Region> regs = regionDao.queryList(regionQo);
		List<String> regionNames = new ArrayList<String>();
		for (Region reg : regs) {
			regionNames.add(reg.getStrName());
		}
		
		regionDao.deleteRegByIds(delIds);
		configLogDao.setLogContent(OperStatus.DELETE, LogModuledEnum.REGION.getCode(),GlobalMessageUtil.getMessage("log.org.delete",new String[] {ListUtil.listToStr(regionNames)}));
		return ajaxData;
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.region.service.IRegionService#editReg(com.hikvision.finance.fms.modules.region.dto.RegionInfo)
	 */
	@Override
	public AjaxData editReg(RegionInfo regionInfo) {
		AjaxData ajaxData = new AjaxData(false);
		Region region = regionDao.get(regionInfo.getId());
		if(regionDao.findUniqueBy("strName", regionInfo.getStrName(), region.getId()) != null){
			return ajaxData.setFailureMsg("已存在命名为" + regionInfo.getStrName());
		}
		if(regionDao.findUniqueBy("strCode", regionInfo.getStrCode(), region.getId()) != null){
			return ajaxData.setFailureMsg("已存在代码为" + regionInfo.getStrCode());
		}
		region.setStrCode(regionInfo.getStrCode());
		region.setStrName(regionInfo.getStrName());
		region.setStrTxt(regionInfo.getStrTxt());
		this.update(region);
		configLogDao.setLogContent(OperStatus.UPDATE, LogModuledEnum.REGION.getCode(),GlobalMessageUtil.getMessage("log.org.update", new String[] {region.getStrName()}));
		return ajaxData;
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.region.service.IRegionService#getDataPage(java.lang.Integer, java.lang.Integer, java.lang.String)
	 */
	@Override
	public Page getDataPage(Integer limit, Integer start, RegionInfo regionInfo) {
		return regionDao.getDataPage(limit, start, regionInfo);
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.region.service.IRegionService#getLikeStrPathIds(java.lang.Integer, java.lang.Integer)
	 */
	@Override
    public String getLikeStrPathIds(Integer regionId, Integer isFlag){
		// 区域ID为空或者选中区域根节点，则不进行区域过滤
		if (isFlag == Constants.TreeReadStrategy.ONLY_SELF) {// 只显示选中区域的理财信息
			return regionId.toString();
		}
		Region region = regionDao.get(regionId);
		List<Integer> regionIdList = regionDao.getLikeStrPathIds(region);
		String regionIds = ListUtil.listToStr(regionIdList);
		return regionIds;
    }
	
	
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.region.service.IRegionService#exportRegionExcel(com.hikvision.finance.fms.modules.region.dto.RegionInfo, java.lang.String[], java.lang.String[], java.lang.String)
     */
    @Override
	public boolean exportRegionExcel(RegionInfo regionInfo, String[] titleName, String[] attrName, String path) {
		// 1，依次准备标题、标题映射属性、Sheet以及对应内容
		List<String[]> titleNames = new ArrayList<String[]>();
		List<String[]> attrNames = new ArrayList<String[]>();
		LinkedHashMap<String, List<?>> map = new LinkedHashMap<String, List<?>>();
		titleNames.add(titleName);
		attrNames.add(attrName);
		List<RegionInfo> regionInfos = regionDao.exportRegionExcel(regionInfo);
		map.put("Sheet1", regionInfos);
		// 2，创建Excel的数据对象并导出表格
		ExcelUtil.ExcelExportData setInfo = new ExcelUtil.ExcelExportData();
		setInfo.setColumnNames(titleNames);
		setInfo.setFieldNames(attrNames);
		setInfo.setDataMap(map);
		boolean exportResult = false;
		try {
			exportResult = ExcelUtil.export2File(setInfo, path);
			configLogDao.setLogContent(OperStatus.EXPORT, LogModuledEnum.REGION.getCode(),GlobalMessageUtil.getMessage("log.org.export"));
		} catch (Exception e) {
			e.printStackTrace();
		}
		return exportResult;
	}
    
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.region.service.IRegionService#parseRegFromExcel(java.io.File, java.lang.String, com.hikvision.finance.fms.common.excel.model.ExcelHead)
     */
    @SuppressWarnings("unchecked")
    @Override
	public AjaxData addRegFromExcel(File orgFile, String orgFileFileName, ExcelHead excelHead) {
		AjaxData ajaxData = new AjaxData(false);
		Map<String, Object> map2 = Maps.newHashMap();
		ExcelHelper<RegionInfo> excelHelper = ExcelHelperFactory.createExcelHelper(orgFileFileName);
		//String nregionName = excelHelper.getRootDate(orgFile);// 1、区域导入第一行为根节点 需要到数据库中校验其父节点是否存在
		Map<String,Object> map1 = excelHelper.getRootDate(orgFile);// 1、区域导入第一行为根节点 需要到数据库中校验其父节点是否存在
		if(!(Boolean)map1.get("errorNo")){
		   ajaxData.setFailureMsg(map1.get("errorInfo").toString());
		   return ajaxData;
		}
		Region region = regionDao.findUniqueBy("strName", map1.get("nregionName").toString());
		RegionInfo regionInfo = new RegionInfo();
		try {
			BeanUtils.copyNotNullProperties(regionInfo, region);
		} catch (Exception e) {
			throw new ExpectedException("", "拷贝bean出错");
		}
		if (region != null) {
			Multimap<String, RegionInfo> map;
			map2 = excelHelper.validateDate(orgFile);
			if(!(Boolean)map2.get("errorNo")){
				ajaxData.setFailureMsg(map2.get("errorInfo").toString());
				return ajaxData;
			}
			map = (Multimap<String, RegionInfo>)map2.get("myMultimap");			
			// 验证数据准确性
			// 获取数据库中已有的编码和代码code
			Map<String, Region> mapName = new HashMap<String, Region>();
			Map<String, Region> mapCode = new HashMap<String, Region>();
			List<Region> regions = regionDao.getAll();
			for (Region region1 : regions) {
				mapName.put(region1.getStrName(), region1);
				mapCode.put(region1.getStrCode(), region1);
			}
			
			//验证表中数据是否在数据库当中存在，与数据库中的代码和名字做唯一性校验
			Collection<RegionInfo> regionInfos = map.get(regionInfo.getStrName());	
			StringBuffer buff = new StringBuffer();
			for(RegionInfo info:regionInfos){
				Region regionCode = mapCode.get(info.getStrCode());
				if(regionCode!=null){
					buff.append("代码:"+info.getStrCode()+" | ");
				}
				Region regionName = mapName.get(info.getStrName());
				if(regionName!=null){
					buff.append("名称:"+info.getStrName()+" | ");
				}
				if(StringUtils.isNotBlank(buff.toString())){
					Map<String,Object> data = Maps.newHashMap();
					String str = buff.toString();
					data.put("alType", str);
					ajaxData.setData(data);
					ajaxData.setSuccess(false);
				}
			}
			//**************************************************************
			saveRegionMap(regionInfo, map, mapName, mapCode);
			configLogDao.setLogContent(OperStatus.IMPORT, LogModuledEnum.REGION.getCode(),GlobalMessageUtil.getMessage("log.org.import"));
		} else {
			//throw new ExpectedException("", "根节点在数据库中不存在");
			ajaxData.setFailureMsg("表格中所属组织第一个单元格组织,在数据库中不存在");
		}
		return ajaxData;
	}
    
    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.region.service.IRegionService#validateExcel(java.io.File, java.lang.String, com.hikvision.finance.fms.common.excel.model.ExcelHead)
     */
    public AjaxData addAndValidateExcel(File orgFile, String orgFileFileName, ExcelHead excelHead) {
    	AjaxData ajaxData = new AjaxData(false);
		StringBuffer stringBuffer = new StringBuffer();
		int failNum=0;
		int successNum=0;
		ajaxData = validateRegions(stringBuffer, failNum, successNum, orgFile, orgFileFileName, excelHead);
		if(!ajaxData.getSuccess()){
			return ajaxData;
		}	
		
		successNum = (Integer)ajaxData.get("successNum");
		failNum = (Integer)ajaxData.get("failNum");
		
		if(successNum > 0){
			configLogDao.setLogContent(OperStatus.IMPORT, LogModuledEnum.REGION.getCode(),GlobalMessageUtil.getMessage("log.org.import"));	
		}
		//map用户暂时未导入成功的组织
		if(failNum > 0){
			Map<String, Object> failRegionName = Maps.newHashMap();
			failRegionName.put("failRegionName", "导入失败的组织：失败个数( "+failNum+" )个。&"+stringBuffer.toString());
			ajaxData.setData(failRegionName);
		}
		return ajaxData;
	}
    
	public AjaxData validateRegions(StringBuffer stringBuffer, int failNum, int successNum, File orgFile, String orgFileFileName,
	        ExcelHead excelHead) {
		AjaxData ajaxData = new AjaxData(false);
		try {
			Map<Integer, Object> areaCodeMap = Maps.newHashMap();// 用于存放区域码的Map：数据库+Excel表格数据
			Map<Integer, Object> areaRegionMap = Maps.newHashMap();// 用于存放区域的Map：数据库+Excel表格数据
			List<Integer> sortList = Lists.newArrayList();// 用于存放排序号的List：数据库+Excel表格数据
			List<RegionInfo> regionInfoList = Lists.newArrayList();
			List<Object> mapStrNameList = Lists.newArrayList();// 存放Excel表格中的组织名称
			List<Object> mapCodeList = Lists.newArrayList();// 存放Excel表格中的组织代码
			ExcelHelper<RegionInfo> excelHelper = ExcelHelperFactory.createExcelHelper(orgFileFileName);
			regionInfoList = excelHelper.importToObjectList(excelHead, orgFile, RegionInfo.class);
			// 获取数据库中组织的map
			Map<String, Region> mapStrName = Maps.newHashMap();
			Map<String, Region> mapCode = Maps.newHashMap();
			List<Region> regions = regionDao.getAll();
			for (Region region : regions) {
				mapStrName.put(region.getStrName(), region);
				mapCode.put(region.getStrCode(), region);
				areaRegionMap.put(region.getId(), region);
				areaCodeMap.put(region.getId(), region.getStrInterAreaCode());
				sortList.add(region.getNsortId());
			}
			// 校验有无数据
			if (CollectionUtils.isEmpty(regionInfoList)) {
				return ajaxData.setFailureMsg("导入的表格内关键数据缺失或无数据");
			}
			
			// 校验导入的表格数据合法性
			for (int i = 0; i < regionInfoList.size(); i++) {
				RegionInfo info = regionInfoList.get(i);
				// 判断必填项是否为空
				if (StringUtils.isBlank(info.getNparentName())) {
					return ajaxData.setFailureMsg("所属组织列中数据不全");
				}
				if (StringUtils.isBlank(info.getStrName())) {
					return ajaxData.setFailureMsg("组织名称列中数据不全");
				}
				if (StringUtils.isBlank(info.getStrCode())) {
					return ajaxData.setFailureMsg("组织代码列中数据不全");
				}
				
				// 校验表格内组织代码和组织名称是否有重复
				if (mapStrNameList.contains(info.getStrName())) {
					return ajaxData.setFailureMsg("组织名称列表中存在重复数据：" + info.getStrName());
				}
				
				if (mapCodeList.contains(info.getStrCode())) {
					return ajaxData.setFailureMsg("组织代码列表中存在重复数据：" + info.getStrCode());
				}
				
				mapStrNameList.add(info.getStrName());
				mapCodeList.add(info.getStrCode());
				
				// 数据有效性校验
				if (!validateCoulm(info)) {
					return ajaxData.setFailureMsg("文件内容含非法数据或有特殊字符\\/:*?\"<|'%>&");
				}
				
			}
			// 获取数据库中的区域码，保存在list中，生成暂未提交的区域码也将放入list。生成的区域码将和list的对比，无重复则可使用
			for (int i = 0; i < regionInfoList.size(); i++) {
				RegionInfo info = regionInfoList.get(i);
				RegionInfo regionInfo = new RegionInfo();
				if (mapStrName.get(info.getNparentName()) == null) {
					stringBuffer.append(info.getStrName() + "&");// 保存到stringBuffer，到时候给前台页面统一展示
					failNum++;
					continue;
				}
				Integer npid = mapStrName.get(info.getNparentName()).getId();
				if (npid == null) {
					stringBuffer.append(info.getStrName() + "&");// 保存到stringBuffer，到时候给前台页面统一展示
					failNum++;
					continue;
				}
				regionInfo.setId(npid);
				
				// 生成区域码
				String code = getRegSerilByMap(regionInfo.getId(), areaRegionMap, areaCodeMap);
				info.setStrInterAreaCode(code);
				info.setNparentId(regionInfo.getId());
				Region region = new Region();
				region.setNparentId(info.getNparentId());
				region.setStrName(info.getStrName());
				region.setStrCode(info.getStrCode());
				region.setStrTxt(info.getStrTxt());
				region.setStrInterAreaCode(info.getStrInterAreaCode());
				region.setNsortId(getNsortByList(sortList));// 设置排序号
				region.setStrPath(getStrPathByMap(info.getNparentId(), areaRegionMap, info.getStrInterAreaCode())); // 设置strPath
				// 3，若添加的为组织，验证最大层级
				String[] nameList = region.getStrPath().split("-");
				if (nameList.length > Constants.RegNum.REGION_NUM) {
					logger.info("组织" + info.getStrName() + "已达到最大层级");
					stringBuffer.append(info.getStrName() + "&");// 保存到stringBuffer，到时候给前台页面统一展示
					failNum++;
					continue;
				}
				if (mapStrName.get(region.getStrName()) != null) {// 校验组织名称列表在数据库中是否存在
					continue;
				}
				Integer areaId = (Integer)regionDao.save(region);
				successNum++;
				areaCodeMap.put(areaId, code);
				areaRegionMap.put(areaId, region);
				mapStrName.put(region.getStrName(), region);
				sortList.add(region.getNsortId());
			}
		} catch (ExpectedException e) {
			return ajaxData.setFailureMsg("导入文件的内容格式有误,请检查文件内容是否合规");
		}
		Map<String, Object> numMap = new HashMap<String, Object>();
		numMap.put("successNum", successNum);
		numMap.put("failNum", failNum);
		ajaxData.setData(numMap);
		
		return ajaxData;
	}
    
    /**
     * 校验导入的数据
     * @author jinxindong 2016年6月15日 下午5:02:37
     * @param regionInfo
     * @return
     */
	private Boolean validateCoulm(RegionInfo regionInfo) {
		if(ValidateUtil.validateChar(regionInfo.getStrName())){
			return false;
		}
		if(ValidateUtil.validateChar(regionInfo.getStrCode())){
			return false;
		}
		if(ValidateUtil.validateChar(regionInfo.getNparentName())){
			return false;
		}
		if(ValidateUtil.validateChar(regionInfo.getStrTxt())){
			return false;
		}
		return true;
	}
    /**
     * 导入获取区域码
     * @author jinxindong 2016年6月2日 上午9:25:55
     * @param nparentId
     * @param areaRegionMap
     * @param areaCodeMap
     * @return
     */
	public String getRegSerilByMap(Integer nparentId,Map<Integer, Object> areaRegionMap,Map<Integer, Object> areaCodeMap) {
		String ex = null;
		String code = null;
		boolean isExistCode = false;
		// 控制单元
		if (nparentId == 1) {
			ex = "100000";
		} else {
			Region reg = (Region)areaRegionMap.get(nparentId);
			ex = StringUtilExpand.getLengthStr(reg.getStrInterAreaCode(), 6);
		}
		// B规则：区域代码： 1000000000+6位随机数+90
		code = ex + "0000" + StringUtilExpand.getSixRandomSeril() + 90;
		// 判断是否存在
		isExistCode = areaCodeMap.values().contains(code);
		// 如果存在，重新生成
		while (isExistCode) {
			code = ex + "0000" + StringUtilExpand.getSixRandomSeril() + 90;
			isExistCode = areaCodeMap.values().contains(code);
		}
		return code;
	}
	
	/**
	 * 导入获取排序号
	 * @author jinxindong 2016年6月2日 上午9:27:03
	 * @return
	 */
	private Integer getNsortByList(List<Integer> sortList) {
		Integer nsort = Collections.max(sortList) + 1;
		return nsort;
	}
	
	/**
	 * 导入获取组织编码路径
	 * @author jinxindong 2016年6月2日 上午9:37:29
	 * @param panrentId
	 * @param areaRegionMap
	 * @param areaCode
	 * @return
	 */
	private String getStrPathByMap(Integer panrentId, Map<Integer, Object> areaRegionMap, String areaCode) {
		Region region = (Region)areaRegionMap.get(panrentId);
		if (region != null && region.getStrPath() != null) {
			areaCode = region.getStrPath() + "-" + areaCode;
		}
		return areaCode;
	}
	
	/**
	 * 递归保存节点
	 * @author jinxindong 2016年4月21日 下午4:10:29
	 * @param regionInfo 父节点对象
	 * @param map
	 */
    private void saveRegionMap(RegionInfo nregionInfo,Multimap<String, RegionInfo> map,Map<String,Region> mapName,Map<String,Region> mapCode){
		Collection<RegionInfo> regionInfos = map.get(nregionInfo.getStrName());		
		for(RegionInfo info:regionInfos){
			//通过校验之后插入数据
			info.setNparentId(nregionInfo.getId());
			info.setStrInterAreaCode(getRegSeril(nregionInfo.getId()));
			Region region = new Region();
			region.setNparentId(info.getNparentId());
			region.setStrName(info.getStrName());
			region.setStrCode(info.getStrCode());
			region.setStrTxt(info.getStrTxt());
			region.setStrInterAreaCode(info.getStrInterAreaCode());// 设置排序号
			region.setNsortId(getNsort());// 设置strPath
			region.setStrPath(getStrPath(info.getNparentId(), info.getStrInterAreaCode()));
			regionDao.save(region);
			try {
				 BeanUtils.copyNotNullProperties(info, region);
            } catch (Exception e) {
            	throw new ExpectedException("", "拷贝bean出错");
            }
			saveRegionMap(info,map, mapName, mapCode);
		}
		
	}


    
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.extend.service.IBaseService#deleteByIds(java.io.Serializable[])
	 */
	@Override
	public void deleteByIds(Serializable[] paramArrayOfSerializable) {
		
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.extend.service.BaseServiceImpl#getDao()
	 */
	@Override
	protected IRegionDao getDao() {
		return regionDao;
	}
	
	// =============================私有方法===========================================================
    /**
     * 获取指定组织的当前层级
     * @author jinxindong 2016年5月4日 下午3:14:30
     * @param regionId
     * @return
     */
    private int getOrgLevel(int regionId) {
    	return regionDao.getRegLevel(regionId);
    }
    
	/**
	 * 获取组织路径编码
	 * @author jinxindong 2016年2月26日 上午9:34:38
	 * @param panrentId
	 * @param areaCode
	 * @return
	 */
	private String getStrPath(Integer panrentId, String areaCode) {
		RegionQo qo = new RegionQo();
		qo.setId(panrentId);
		Region region = regionDao.queryUnique(qo);
		if (region != null && region.getStrPath() != null) {
			areaCode = region.getStrPath() + "-" + areaCode;
		}
		return areaCode;
	}
	
	/**
	 * 获取排序号
	 * @author jinxindong 2016年2月26日 上午9:39:20
	 * @return
	 */
	private Integer getNsort() {
		RegionQo regQo = new RegionQo();
		Integer nsort = (Integer)regionDao.maxProperty("nsortId", regQo) + 1;
		return nsort;
	}
	
	/**
	 * 验证是否存在相同条件的其他组织
	 * @author jinxindong 2016年2月26日 上午9:59:19
	 * @param type
	 * @param value
	 * @param notId
	 * @return
	 */
	private AjaxData isExistReg(Integer type, String value, Integer notId) {
		AjaxData ajaxData = new AjaxData(false);
		RegionQo qo = new RegionQo();
		if (notId != null) {
			qo.setNotId(notId);
		}
		switch (type) {
			case 1:
				qo.setStrName(value);
				break;
			case 2:
				qo.setStrCode(value);
				break;
			case 3:
				qo.setStrInterAreaCode(value);
				break;
			default:
				break;
		}
		Region reg = regionDao.queryUnique(qo);
		if (reg == null) {
			ajaxData.setSuccess(false);
		}
		return ajaxData;
	}
	
	/**
	 * 获取子区域全部的ids
	 * @author jinxindong 2016年2月26日 上午11:57:09
	 * @param regs
	 * @param id
	 * @param deleteIds
	 */
	private void getAllChildrenIds(List<Region> regs, Integer id, List<Integer> deleteIds) {
		for (Region org : regs) {
			if (org.getNparentId().equals(id)) {
				deleteIds.add(org.getId());
				getAllChildrenIds(regs, org.getId(), deleteIds);
			}
		}
	}
	

	// ==================getter/setter=================================
	public IRegionDao getRegionDao() {
		return regionDao;
	}
	
	public void setRegionDao(IRegionDao regionDao) {
		this.regionDao = regionDao;
	}
	
	public IConfigLogDao getConfigLogDao() {
		return configLogDao;
	}
	
	public void setConfigLogDao(IConfigLogDao configLogDao) {
		this.configLogDao = configLogDao;
	}
	
	public IUserregionmappingDao getUserregionmappingDao() {
		return userregionmappingDao;
	}
	
	public void setUserregionmappingDao(IUserregionmappingDao userregionmappingDao) {
		this.userregionmappingDao = userregionmappingDao;
	}
	
	public IDeviceDao getDeviceDao() {
		return deviceDao;
	}
	
	public void setDeviceDao(IDeviceDao deviceDao) {
		this.deviceDao = deviceDao;
	}
	
	public IUsersDao getUsersDao() {
		return usersDao;
	}
	
	public void setUsersDao(IUsersDao usersDao) {
		this.usersDao = usersDao;
	}
	
	public IClientInputDao getClientInputDao() {
		return clientInputDao;
	}
	
	public void setClientInputDao(IClientInputDao clientInputDao) {
		this.clientInputDao = clientInputDao;
	}
	
}
