/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月9日 上午10:30:17
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.device.dao.impl;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import org.hibernate.Criteria;
import org.hibernate.Query;
import org.hibernate.criterion.Restrictions;

import com.google.common.collect.Maps;
import com.hikvision.finance.core.util.SercurityUtil;
import com.hikvision.finance.core.util.StringUtils;
import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.hibernate.dao.BaseDao;
import com.hikvision.finance.extend.hibernate.qo.SqlQuery;
import com.hikvision.finance.fms.common.util.Base64Utils;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.common.util.Constants.DeviceType;
import com.hikvision.finance.fms.common.util.StringUtilExpand;
import com.hikvision.finance.fms.model.Cvrassociatemapping;
import com.hikvision.finance.fms.model.Device;
import com.hikvision.finance.fms.model.Region;
import com.hikvision.finance.fms.modules.device.dao.ICvrassociatemappingDao;
import com.hikvision.finance.fms.modules.device.dao.IDeviceDao;
import com.hikvision.finance.fms.modules.device.dto.DeviceInfo;
import com.hikvision.finance.fms.modules.device.qo.DeviceQo;
import com.hikvision.finance.fms.modules.region.dao.IRegionDao;
import com.hikvision.finance.fwork.exception.ExpectedException;


/**
 * <p></p>
 * @author jinxindong 2016年3月9日 上午10:30:17
 * @version V1.0 
 */
public class DeviceDaoImpl extends BaseDao<Device, DeviceQo> implements IDeviceDao {

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 6540432519436761067L;
    private IRegionDao regionDao;
    private ICvrassociatemappingDao cvrassociatemappingDao;

	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.hibernate.dao.BaseDao#buildCriteria(org.hibernate.Criteria, com.hikvision.finance.extend.hibernate.qo.BaseQo)
     */
    @Override
    protected Criteria buildCriteria(Criteria criteria, DeviceQo qo) {
		if (qo != null) {
			// 名称
			if (qo.getStrName() != null) {
				criteria.add(Restrictions.eq("strName", qo.getStrName()));
			}
			// 区域ID
			if (qo.getNregionId() != null) {
				criteria.add(Restrictions.eq("nregionId", qo.getNregionId()));
			}
			if (qo.getNregionId() != null) {
				criteria.add(Restrictions.in("ntype", qo.getNtypes()));
			}
		}
		return criteria;
    }
    
    public List<Device> getUsbDeviceByNtype() {
    	StringBuffer sql = new StringBuffer("select * from device d where d.nType in (50050, 50060) ORDER BY id ");
    	List<Device> usbDeviceList = this.createSQLQuery(sql.toString()).addEntity(Device.class).list();
		 return usbDeviceList;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.device.dao.IDeviceDao#getDeviceByRegionId()
     */
    @Override
    public List<Device> getDeviceByRegionId(Integer nregionId) {
    	DeviceQo deviceQo = new DeviceQo();
    	deviceQo.setNregionId(nregionId);
	    return this.queryList(deviceQo);
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.device.dao.IDeviceDao#getStoDataPage(java.lang.Integer, java.lang.Integer, com.hikvision.finance.fms.modules.device.dto.DeviceInfo)
     */
    @SuppressWarnings("unchecked")
    @Override
	public Page getStoDataPage(Integer limit, Integer start, DeviceInfo deviceInfo) {
		
		// 1.查询条件封装
		SqlQuery sqlQuery = conditionQuerySto(deviceInfo);
		
		// 2.设置page 属性
		Page page = new Page();
		page.setCondition(sqlQuery);
		page.setPageNo(start);
		page.setPageSize(limit);
		page = this.queryPaginationBySql(page);
		
		// 3.封装map
		List<Region> regions = regionDao.getAll();
		Map<Integer, Region> regionMap = buildRegionMap(regions);
		
		// 4.循环page 插入地区名 关联信息等
		List<DeviceInfo> deviceInfos = (List<DeviceInfo>)page.getResult();
		List<DeviceInfo> deviceInfoList = new ArrayList<DeviceInfo>();
		for (DeviceInfo devOld : deviceInfos) {
			Region region = regionMap.get(devOld.getNregionId());
			devOld.setRegionName(region.getStrName());
			if (StringUtils.isNotBlank(devOld.getStrPassword())) {// 导入的对象存储没有密码
				String pass = Base64Utils.getBase64(SercurityUtil.decrypt(devOld.getStrPassword()));
				devOld.setStrPassword(pass);// 解密
			}
			// 是否获取到信道 storageRelate 0未 1有
			Integer total = null == devOld.getUiTotalSpace()?0:devOld.getUiTotalSpace();
			if (total > 0) {
				devOld.setStorageRelate(Constants.CONS_1);
			} else {
				devOld.setStorageRelate(Constants.CONS_0);
			}
			deviceInfoList.add(devOld);
		}
		page.setResult(deviceInfoList);
		return page;
	}

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.device.dao.IDeviceDao#getCollDataPage(java.lang.Integer, java.lang.Integer, com.hikvision.finance.fms.modules.device.dto.DeviceInfo)
     */
    @SuppressWarnings("unchecked")
    @Override
	public Page getCollDataPage(Integer limit, Integer start, DeviceInfo deviceInfo) {
		// 1.查询条件封装
		SqlQuery sqlQuery = conditionQueryColl(deviceInfo);
		// 2. 设置page 属性
		Page page = new Page();
		page.setCondition(sqlQuery);
		page.setPageNo(start);
		page.setPageSize(limit);
		page = this.queryPaginationBySql(page);
		
		// 3.封装map
		List<Region> regions = regionDao.getAll();
		Map<Integer, Region> regionMap = buildRegionMap(regions);
		
		// 4.循环page 插入地区名 关联关系等
		List<DeviceInfo> deviceInfos = (List<DeviceInfo>)page.getResult();
		List<DeviceInfo> deviceInfoList = new ArrayList<DeviceInfo>();
		for (DeviceInfo devOld : deviceInfos) {
			Region region = regionMap.get(devOld.getNregionId());
			devOld.setRegionName(region.getStrName());
			String pass = Base64Utils.getBase64(SercurityUtil.decrypt(devOld.getStrPassword()));
			devOld.setStrPassword(pass);// 解密
			// 判断是否关联存储设备
			List<Cvrassociatemapping> cvrassociatemappings = cvrassociatemappingDao.findBy("ncollId", devOld.getId());
			if (cvrassociatemappings != null && cvrassociatemappings.size() > 0) {
				devOld.setCollectRelate(Constants.CONS_1);
			} else {
				devOld.setCollectRelate(Constants.CONS_0);
			}
			deviceInfoList.add(devOld);
		}
		page.setResult(deviceInfoList);
		return page;
	}

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.device.dao.IDeviceDao#getAllStoDevice()
     */
    @SuppressWarnings("unchecked")
    @Override
	public List<Device> getAllStoDevice() {
		String sql = " SELECT * FROM device d where d.nType in(50011,50050,50015) AND uiTotalSpace>0 ";
		List<Device> list = (List<Device>)this.createSQLQuery(sql).addEntity(Device.class).list();
		return list;
	}

    /* (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.device.dao.IDeviceDao#getDeviceinfobyntype()
	 */
    @SuppressWarnings("unchecked")
	@Override
	public List<Device> getDeviceinfobyntype() {
		String sql = " SELECT * FROM device WHERE nType in ('50011','50015','50050', '50060') ";
		List<Device> list = (List<Device>)this.createSQLQuery(sql).addEntity(Device.class).list();
		return list;
	}
    
    /* (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.device.dao.IDeviceDao#updateDeviceOnline(java.lang.String, int, int)
	 */
	@Override
	public void updateDeviceOnline(String strIp, int port, int online) {
		StringBuffer sql  = new StringBuffer("update device set online = ? where strIp = ? and nport = ?");
		Query query = createSQLQuery(sql.toString());
		query.setParameter(0, online).setParameter(1, strIp).setParameter(2, port);
		query.executeUpdate();
	}
    
	/* (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.device.dao.IDeviceDao#updateDeviceSpace(java.lang.String, int, int, int)
	 */
	@Override
	public void updateDeviceSpace(String strIp, int port, int uiTotalSpace,
			int uiFreeSpace) {
		StringBuffer sql  = new StringBuffer("update device set uiTotalSpace = ? ,uiFreeSpace= ? where strIp = ? and nport = ?");
		Query query = createSQLQuery(sql.toString());
		query.setParameter(0, uiTotalSpace).setParameter(1, uiFreeSpace).setParameter(2, strIp).setParameter(3, port);
		query.executeUpdate();
	}
	
	/* (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.device.dao.IDeviceDao#getAllDvrServiceInfo()
	 */
	@SuppressWarnings("unchecked")
	@Override
	public List<Device> getAllDvrServiceInfo() {
		StringBuffer sql = new  StringBuffer("SELECT * FROM Device WHERE nType in ('50000','50012') " );
		return this.createSQLQuery(sql.toString()).addEntity(Device.class).list();
	}

	/* (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.device.dao.IDeviceDao#getDCServiceInfo(int, java.lang.String)
	 */
	@SuppressWarnings("unchecked")
	@Override
	public List<Device> getDCServiceInfobyCvrid(int cvrid) {
		StringBuffer sql = new  StringBuffer("SELECT * FROM device WHERE ID = ?" );
		return this.createSQLQuery(sql.toString()).addEntity(Device.class).setParameter(0, cvrid).list();
	}
	
	/* (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.device.dao.IDeviceDao#getStorageInfo(java.lang.String, int)
	 */
	@Override
	public com.hikvision.finance.fms.cms.dto.DeviceInfo getStorageInfo(String ip, int port) {
		SqlQuery sqlQuery = new SqlQuery();
		StringBuffer sql  = new StringBuffer("select dev.nType as ntype, dev.strIP as strIp,dev.nPort as nport,dev.strUser as strUser,dev.strPassword as strPassword,cvr.groupIndex as groupIndex from device dev LEFT JOIN cvrassociatemapping cvr on dev.id = cvr.nCvrId where cvr.nCvrId =  ");
		sql.append(" ( select distinct cvrmap.nCvrId from cvrassociatemapping cvrmap where cvrmap.nCollId = (");
		sql.append(" select id from device de where de.strIP =");
		sql.append("'");
		sql.append(ip);
		sql.append("'");
		sql.append(" and de.nPort = ");
		sql.append(port);
		sql.append(") )");
		
		sql.append(" and cvr.nCollId = (");
		sql.append(" select id from device de where de.strIP =");
		sql.append("'");
		sql.append(ip);
		sql.append("'");
		sql.append(" and de.nPort = ");
		sql.append(port);
		sql.append(") ");
		sqlQuery.setSql(sql.toString());
		sqlQuery.setDtoClass(com.hikvision.finance.fms.cms.dto.DeviceInfo.class);
		return (com.hikvision.finance.fms.cms.dto.DeviceInfo) queryUniqueBySql(sqlQuery);
		
	}
	
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.device.dao.IDeviceDao#exportDevExcel(com.hikvision.finance.fms.modules.device.dto.DeviceInfo)
     */
    @SuppressWarnings("unchecked")
    @Override
	public List<DeviceInfo> exportStoDevExcel(DeviceInfo deviceInfo) {		
		// 1.查询条件封装
		SqlQuery sqlQuery = conditionQuerySto(deviceInfo);
		
		// 2.循环，插入区域名，设备类型名等
		List<DeviceInfo> deviceInfos = (List<DeviceInfo>)this.queryListBySql(sqlQuery);
		List<DeviceInfo> deviceInfosNew = new ArrayList<DeviceInfo>();
		
		if (deviceInfos != null && deviceInfos.size() > 0) {
			Integer count = deviceInfos.size();
			if (count > Constants.EXCEL_MAX_NUM) {
				throw new ExpectedException("", "导出数据超过" + Constants.EXCEL_MAX_NUM + "条,无法导出");
			}
			List<Region> regions = regionDao.getAll();
			Map<Integer, Region> regionMap = buildRegionMap(regions);
			for (DeviceInfo infoOld : deviceInfos) {
				// 所在区域
				Region region = regionMap.get(infoOld.getNregionId());
				infoOld.setRegionName(region.getStrName());
				
				// 判断设备类型
				if (DeviceType.DEV_TYPE_CVR == infoOld.getNtype()) {
					infoOld.setNtypeName("CVR");
				} else if (DeviceType.DEV_TYPE_KMS == infoOld.getNtype()) {
					infoOld.setNtypeName("KMS");
				} else if (DeviceType.DEV_TYPE_CLOUD_STORAGE == infoOld.getNtype()) {
					infoOld.setNtypeName("云存储");
				} else if (DeviceType.DEV_TYPE_CLOUD_STORAGE_OBJECT_STORAGE == infoOld.getNtype()) {
					infoOld.setNtypeName("云存储-对象存储");
				} else {
					infoOld.setNtypeName("未知");
				}
				
				// 判断密码等级
				if ("0".equals(infoOld.getStrPasswordLevel())) {
					infoOld.setStrPasswordLevelName("风险密码");
				} else if ("1".equals(infoOld.getStrPasswordLevel())) {
					infoOld.setStrPasswordLevelName("弱密码");
				} else if ("2".equals(infoOld.getStrPasswordLevel())) {
					infoOld.setStrPasswordLevelName("中密码");
				} else if ("3".equals(infoOld.getStrPasswordLevel())) {
					infoOld.setStrPasswordLevelName("强密码");
				} else if ("4".equals(infoOld.getStrPasswordLevel())) {
					infoOld.setStrPasswordLevelName("");
				} else {
					infoOld.setStrPasswordLevelName("未知");
				}
				
				// 在线状态
				if (Constants.CONS_1 == infoOld.getOnline()) {
					infoOld.setOnlineName("在线");
				} else if (Constants.CONS_0 == infoOld.getOnline()) {
					infoOld.setOnlineName("离线");
				} else {
					infoOld.setOnlineName("未知");
				}
				deviceInfosNew.add(infoOld);
			}
		}
		
		return deviceInfos;
	}

    
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.device.dao.IDeviceDao#exportCollDevExcel(com.hikvision.finance.fms.modules.device.dto.DeviceInfo)
     */
    @SuppressWarnings("unchecked")
    @Override
    public List<DeviceInfo> exportCollDevExcel(DeviceInfo deviceInfo) {
    	
    	//1.查询条件封装
    	SqlQuery sqlQuery = conditionQueryColl(deviceInfo);
    	
    	// 2.循环，插入区域名，设备类型名等
		List<DeviceInfo> deviceInfos = (List<DeviceInfo>)this.queryListBySql(sqlQuery);
		List<DeviceInfo> deviceInfosNew = new ArrayList<DeviceInfo>();

		if(deviceInfos!=null && deviceInfos.size()>0){
			Integer count = deviceInfos.size();		
			if(count > Constants.EXCEL_MAX_NUM){
				throw new ExpectedException("", "导出数据超过"+Constants.EXCEL_MAX_NUM+"条,无法导出");
			}
			List<Region> regions = regionDao.getAll();
			Map<Integer, Region> regionMap = buildRegionMap(regions);
			
			for(DeviceInfo infoOld :deviceInfos){
				// 所在区域
				Region region = regionMap.get(infoOld.getNregionId());
				infoOld.setRegionName(region.getStrName());
				
		        //判断设备类型
				if(DeviceType.DEV_TYPE_DVR==infoOld.getNtype()){
					infoOld.setNtypeName("DVR");
				}else if(DeviceType.DEV_TYPE_NVR == infoOld.getNtype()){
					infoOld.setNtypeName("NVR");
				}else{
					infoOld.setNtypeName("未知");
				}
				
				//判断密码等级
				if("0".equals(infoOld.getStrPasswordLevel())){
					infoOld.setStrPasswordLevelName("风险密码");
				}else if("1".equals(infoOld.getStrPasswordLevel())){
					infoOld.setStrPasswordLevelName("弱密码");
				}else if("2".equals(infoOld.getStrPasswordLevel())){
					infoOld.setStrPasswordLevelName("中密码");
				}else if("3".equals(infoOld.getStrPasswordLevel())){
					infoOld.setStrPasswordLevelName("强密码");
				}else{
					infoOld.setStrPasswordLevelName("未知");
				}
				
				//在线状态
				if(Constants.CONS_1 ==infoOld.getOnline()){
					infoOld.setOnlineName("在线");
				}else if( Constants.CONS_0== infoOld.getOnline()){
					infoOld.setOnlineName("离线");
				}else{
					infoOld.setOnlineName("未知");
				}				
				deviceInfosNew.add(infoOld);
			}
		}
		
		return deviceInfos;
    }
    
    /*
     * (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.device.dao.IDeviceDao#getDeviceByName(java.lang.String)
     */
    public Device getDeviceByName(String strName){
		return findUniqueBy("strName", strName);
	}
    
    
    /**
     * 存储设备查询条件封装
     * @author jinxindong 2016年5月19日 上午11:39:47
     * @param deviceInfo
     * @return
     */
	private SqlQuery conditionQuerySto(DeviceInfo deviceInfo) {
		String strName = "";// 设备名称
		String strIp = "";// ip地址
		String strPasswordLevel = "";// 密码等级
		Integer online = null;// 在线状态
		
		Integer isflag = 0;
		String strPathLike = "";
		if (deviceInfo != null) {
			// 区域查询参数
			Integer nregionId = null == deviceInfo.getNregionId()?1:deviceInfo.getNregionId();
			isflag = null == deviceInfo.getIsFlag()?0:deviceInfo.getIsFlag();
			Region region = regionDao.get(nregionId);
			strPathLike = region.getStrPath();
			strName = null == deviceInfo.getStrName()?"":deviceInfo.getStrName();
			strIp = null == deviceInfo.getStrIp()?"":deviceInfo.getStrIp();
			strPasswordLevel = null == deviceInfo.getStrPasswordLevel()?"":deviceInfo.getStrPasswordLevel();
			online = null == deviceInfo.getOnline()?null:deviceInfo.getOnline();
		}
		SqlQuery sqlQuery = new SqlQuery();
		StringBuffer sql = new StringBuffer("select d.id as id," 
		        + "d.nType as ntype," 
				+ "d.strName as strName,"
		        + "d.strIP as strIp," 
		        + "d.strPasswordLevel as strPasswordLevel, " 
		        + "d.nPort as nport,"
		        + "d.uiFreeSpace as uiFreeSpace," 
		        + "d.uiTotalSpace as uiTotalSpace,"
		        + "d.`online` as `online`,"
		        + "d.strUser AS strUser,"
		        + "d.strPassword AS strPassword," 
		        + "d.strIndexCode AS strIndexCode,"
		        + "d.ntpEnable AS ntpEnable, "
		        + "d.nRegionId as nregionId " 
		        + "FROM device d JOIN region r ON d.nRegionId=r.id"
		        + " where d.nType in(50011,50050,50015, 50060)  ");
		
		if (isflag == Constants.CONS_1) {// 显示旗下所有子孙节点
			sql.append(" and r.strPath like ? ");
			sqlQuery.addParams(strPathLike + "%");
		} else if (isflag == Constants.CONS_0) {// 只显示该节点
			sql.append(" and r.strPath = ? ");
			sqlQuery.addParams(strPathLike);
		}
		
		if (StringUtils.isNotBlank(strName)) {
			sql.append(" and d.strName like ?  escape '/'");
			sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(strName) + "%");
		}
		if (StringUtils.isNotBlank(strIp)) {
			sql.append(" and d.strIp like ?  escape '/'");
			sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(strIp) + "%");
		}
		if (StringUtils.isNotBlank(strPasswordLevel)) {
			sql.append(" and d.strPasswordLevel = ? ");
			sqlQuery.addParams(strPasswordLevel);
		}
		if (null != online) {
			sql.append(" and d.online = ? ");
			sqlQuery.addParams(online);
		}
		// 默认倒序
		sql.append(" order by d.id desc");
		// 设置sqlQuery属性
		sqlQuery.setSql(sql.toString());
		sqlQuery.setDtoClass(DeviceInfo.class);
		return sqlQuery;
	}
    
    /**
     * 采集设备查询条件封装
     * @author jinxindong 2016年5月19日 上午11:41:28
     * @param deviceInfo
     * @return
     */
	private SqlQuery conditionQueryColl(DeviceInfo deviceInfo) {
		String strName = "";// 设备名称
		String strIp = "";// ip地址
		String strPasswordLevel = "";// 密码等级
		Integer online = null;// 在线状态
		
		Integer isflag = 0;
		String strPathLike = "";
		
		if (deviceInfo != null) {
			// 区域查询参数
			Integer nregionId = null == deviceInfo.getNregionId()?1:deviceInfo.getNregionId();
			isflag = null == deviceInfo.getIsFlag()?0:deviceInfo.getIsFlag();
			Region region = regionDao.get(nregionId);
			strPathLike = region.getStrPath();
			strName = null == deviceInfo.getStrName()?"":deviceInfo.getStrName();
			strIp = null == deviceInfo.getStrIp()?"":deviceInfo.getStrIp();
			strPasswordLevel = null == deviceInfo.getStrPasswordLevel()?"":deviceInfo.getStrPasswordLevel();
			online = null == deviceInfo.getOnline()?null:deviceInfo.getOnline();
		}
		SqlQuery sqlQuery = new SqlQuery();
		StringBuffer sql = new StringBuffer("select d.id as id," 
		        + "d.nType as ntype,"
				+ "d.strName as strName,"
		        + "d.strIP as strIp," 
				+ "d.strPasswordLevel as strPasswordLevel, "
		        + "d.nPort as nport,"
		        + "d.nChanNum AS nchanNum," 
		        + "d.telChanNum AS telChanNum," 
		        + "d.`online` as `online`,"
		        + "d.strUser AS strUser," 
		        + "d.strPassword AS strPassword," 
		        + "d.strIndexCode AS strIndexCode,"
		        + "d.ntpEnable AS ntpEnable, "
		        + "d.nRegionId as nregionId "
		        + "FROM device d JOIN region r ON d.nRegionId=r.id"
		        + " where d.nType in(50000,50012)");
		
		if (isflag == Constants.CONS_1) {// 显示旗下所有子孙节点
			sql.append(" and r.strPath like ? ");
			sqlQuery.addParams(strPathLike + "%");
		} else if (isflag == Constants.CONS_0) {// 只显示该节点
			sql.append(" and r.strPath = ? ");
			sqlQuery.addParams(strPathLike);
		}
		
		if (StringUtils.isNotBlank(strName)) {
			sql.append(" and d.strName like ?  escape '/'");
			sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(strName) + "%");
		}
		if (StringUtils.isNotBlank(strIp)) {
			sql.append(" and d.strIp like ?  escape '/'");
			sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(strIp) + "%");
		}
		if (StringUtils.isNotBlank(strPasswordLevel)) {
			sql.append(" and d.strPasswordLevel = ? ");
			sqlQuery.addParams(strPasswordLevel);
		}
		if (null != online) {
			sql.append(" and d.online = ? ");
			sqlQuery.addParams(online);
		}
		
		// 默认倒序
		sql.append(" order by d.id desc");
		// 设置sqlQuery属性
		sqlQuery.setSql(sql.toString());
		sqlQuery.setDtoClass(DeviceInfo.class);
		return sqlQuery;
	}
	
	/**
	 * 构造regionMap<id,Region>
	 * @author jinxindong 2016年5月19日 下午1:51:09
	 * @param regions
	 * @return
	 */
	public Map<Integer, Region> buildRegionMap(List<Region> regions) {
		Map<Integer, Region> map = Maps.newHashMap();
		for (Region region : regions) {
			map.put(region.getId(), region);
		}
		return map;
	}
	
	
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.device.dao.IDeviceDao#updateDeviceNtp(java.lang.Integer, java.lang.Integer)
     */
    @Override
    public void updateDeviceNtp(Integer ntp, Integer id) {
	    String sql = "UPDATE device SET ntpEnable =  ? ";
    	List<Object> param = new ArrayList<Object>();
	    param.add(ntp);
	    if(id != null){
	    	sql += "  WHERE id = ? ";
	    	param.add(id);
	    }
	    this.createSQLQuery(sql, param).executeUpdate();
    }
	
    public IRegionDao getRegionDao() {
    	return regionDao;
    }

	
    public void setRegionDao(IRegionDao regionDao) {
    	this.regionDao = regionDao;
    }

	
    public ICvrassociatemappingDao getCvrassociatemappingDao() {
    	return cvrassociatemappingDao;
    }

	
    public void setCvrassociatemappingDao(ICvrassociatemappingDao cvrassociatemappingDao) {
    	this.cvrassociatemappingDao = cvrassociatemappingDao;
    }

}
