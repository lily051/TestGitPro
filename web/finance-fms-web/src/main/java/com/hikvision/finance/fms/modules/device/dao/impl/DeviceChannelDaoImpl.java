/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月9日 上午10:38:40
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.device.dao.impl;

import java.util.List;

import org.hibernate.Criteria;

import com.hikvision.finance.core.util.StringUtils;
import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.hibernate.dao.BaseDao;
import com.hikvision.finance.extend.hibernate.qo.SqlQuery;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.common.util.StringUtilExpand;
import com.hikvision.finance.fms.model.Devicechannel;
import com.hikvision.finance.fms.model.Region;
import com.hikvision.finance.fms.modules.device.dao.IDeviceChannelDao;
import com.hikvision.finance.fms.modules.device.dto.DeviceChannelInfo;
import com.hikvision.finance.fms.modules.device.qo.DeviceChannelQo;
import com.hikvision.finance.fms.modules.region.dao.IRegionDao;

/**
 * <p>
 * </p>
 * @author jinxindong 2016年3月9日 上午10:38:40
 * @version V1.0
 */
public class DeviceChannelDaoImpl extends BaseDao<Devicechannel, DeviceChannelQo> implements IDeviceChannelDao {
	
	private IRegionDao regionDao;
	/**
	 * 序列化ID
	 */
	private static final long serialVersionUID = -7650664508932521929L;
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.device.dao.IDeviceChannelDao#getChannels(java.lang.Integer)
	 */
	@SuppressWarnings("unchecked")
	public List<Devicechannel> getChannels(Integer userId) {
		SqlQuery sqlQuery = new SqlQuery();
		sqlQuery.setSql("select d.id as id, d.nDeviceId as ndeviceId, d.nChannelNo as nchannelNo, d.nChannelName as nchannelName"
		        + " from devicechannel d, userdevicemapping u where d.id = u.devChannel and u.nUserId = ?");
		sqlQuery.addParams(userId);
		sqlQuery.setDtoClass(Devicechannel.class);
		List<Devicechannel> devicechannelList = (List<Devicechannel>)queryListBySql(sqlQuery);
		return devicechannelList;
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.device.dao.IDeviceDao#getChannelsDataPage(int, int,
	 * com.hikvision.finance.fms.modules.device.dto.DeviceChannelInfo)
	 */
	@Override
	public Page getChannelsDataPage(int limit, int start, DeviceChannelInfo deviceChannelInfo) {
		// 1.查询条件封装
		String ndeviceName = "";// 设备名称
		String userIp = "";// 通道ip地址
		String nchannelName = "";
		
		Integer isflag = 0;
		String strPathLike = "";
		if (deviceChannelInfo != null) {
			// 区域查询参数
			Integer nregionId = null == deviceChannelInfo.getNregionId()?1:deviceChannelInfo.getNregionId();
			isflag = null == deviceChannelInfo.getIsFlag()?0:deviceChannelInfo.getIsFlag();
			Region region = regionDao.get(nregionId);
			strPathLike = region.getStrPath();
			
			ndeviceName = null == deviceChannelInfo.getNdeviceName()?"":deviceChannelInfo.getNdeviceName();
			userIp = null == deviceChannelInfo.getUserIp()?"":deviceChannelInfo.getUserIp();
			nchannelName = null == deviceChannelInfo.getNchannelName()?"":deviceChannelInfo.getNchannelName();
		}
		SqlQuery sqlQuery = new SqlQuery();
		//ehome通道不查询nflag  设备名称 通道号 通道名称 通道ip 
		StringBuffer sql = new StringBuffer("SELECT dc.id AS id,"
				+ " dc.nRegionId AS nregionId,"
				+ " dc.nChannelNo AS nchannelNo,"
				+ " dc.nChannelName AS nchannelName,"
				+ " dc.userIp AS userIp,"
				+ " d.strName AS ndeviceName, "
				+ " r.strName AS regionName "
				+ " FROM devicechannel dc "
				+ " JOIN region r ON dc.nRegionId = r.id "
				+ " JOIN device d ON dc.nDeviceId = d.id "
				+ " WHERE 1=1 ");
		
		if (isflag == Constants.CONS_1) {// 显示旗下所有子孙节点
			sql.append(" and r.strPath like ? ");
			sqlQuery.addParams(strPathLike + "%");
		} else if (isflag == Constants.CONS_0) {// 只显示该节点
			sql.append(" and r.strPath = ? ");
			sqlQuery.addParams(strPathLike);
		}
		
		if (StringUtils.isNotBlank(ndeviceName)) {
			sql.append(" and d.strName like ?  escape '/'");
			sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(ndeviceName) + "%");
		}
		
		if (StringUtils.isNotBlank(userIp)) {
			sql.append(" and dc.userIp like ?  escape '/'");
			sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(userIp) + "%");
		}
		
		if (StringUtils.isNotBlank(nchannelName)) {
			sql.append(" and dc.nchannelName like ?  escape '/'");
			sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(nchannelName) + "%");
		}
		
		// 默认倒序
		sql.append(" order by dc.id desc");
		// 设置sqlQuery属性
		sqlQuery.setSql(sql.toString());
		sqlQuery.setDtoClass(DeviceChannelInfo.class);
		// 设置page 属性
		Page page = new Page();
		page.setCondition(sqlQuery);
		page.setPageNo(start);
		page.setPageSize(limit);
		page = this.queryPaginationBySql(page);
		return page;
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.extend.hibernate.dao.BaseDao#buildCriteria(org.hibernate.Criteria, com.hikvision.finance.extend.hibernate.qo.BaseQo)
	 */
	@Override
	protected Criteria buildCriteria(Criteria criteria, DeviceChannelQo qo) {
		return criteria;
	}
	
	public IRegionDao getRegionDao() {
		return this.regionDao;
	}
	
	public void setRegionDao(IRegionDao regionDao) {
		this.regionDao = regionDao;
	}
	
}
