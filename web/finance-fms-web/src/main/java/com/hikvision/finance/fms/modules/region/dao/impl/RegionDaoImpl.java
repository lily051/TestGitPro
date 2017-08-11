/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月25日 上午9:55:54
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.region.dao.impl;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.hibernate.Criteria;
import org.hibernate.criterion.MatchMode;
import org.hibernate.criterion.Order;
import org.hibernate.criterion.Restrictions;
import org.hibernate.transform.Transformers;
import org.springframework.dao.DataAccessException;

import com.hikvision.finance.core.util.StringUtils;
import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.hibernate.dao.BaseDao;
import com.hikvision.finance.extend.hibernate.qo.SqlQuery;
import com.hikvision.finance.fms.common.model.UserSession;
import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.common.util.SessionUtil;
import com.hikvision.finance.fms.common.util.StringUtilExpand;
import com.hikvision.finance.fms.model.Region;
import com.hikvision.finance.fms.model.Users;
import com.hikvision.finance.fms.modules.region.dao.IRegionDao;
import com.hikvision.finance.fms.modules.region.dto.RegionInfo;
import com.hikvision.finance.fms.modules.region.qo.RegionQo;
import com.hikvision.finance.fwork.exception.ExpectedException;

/**
 * <p>
 * </p>
 * @author jinxindong 2016年2月25日 上午9:55:54
 * @version V1.0
 */
public class RegionDaoImpl extends BaseDao<Region, RegionQo> implements IRegionDao {
	
	/**
	 * 序列化ID
	 */
	private static final long serialVersionUID = 6009888514546547882L;
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.extend.hibernate.dao.BaseDao#buildCriteria(org.hibernate.Criteria, com.hikvision.finance.extend.hibernate.qo.BaseQo)
	 */
	@Override
	protected Criteria buildCriteria(Criteria criteria, RegionQo qo) {
		if (qo != null) {
			// 名称
			if (qo.getStrName() != null) {
				criteria.add(Restrictions.eq("strName", qo.getStrName()));
			}
			// strCode
			if (qo.getStrCode() != null) {
				criteria.add(Restrictions.eq("strCode", qo.getStrCode()));
			}
			// 组织编码
			if (qo.getStrInterAreaCode() != null) {
				criteria.add(Restrictions.eq("strInterAreaCode", qo.getStrInterAreaCode()));
			}
			// nSortId排序
			if (qo.isOrderByNSortIdAsc()) {
				criteria.addOrder(Order.asc("nsortId"));
			}
			if (qo.getNparentId() != null) {
				criteria.add(Restrictions.eq("nparentId", qo.getNparentId()));
			}
			if (qo.getStrPathLike() != null) {
				criteria.add(Restrictions.like("strPath", qo.getStrPathLike(), MatchMode.ANYWHERE));
			}
		}
		return criteria;
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.region.dao.IRegionDao#deleteRegByIds(java.util.List)
	 */
	@Override
	public boolean deleteRegByIds(List<Integer> ids) {
		for (Integer id : ids) {
			this.deleteById(id);
		}
		return true;
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.region.dao.IRegionDao#getDataPage(java.lang.Integer, java.lang.Integer, java.lang.String)
	 */
	@Override
	public Page getDataPage(Integer limit, Integer start,RegionInfo regionInfo) {
		//查询条件封装
		SqlQuery sqlQuery = conditionQuery(regionInfo);
		// 设置page 属性
		Page page = new Page();
		page.setCondition(sqlQuery);
		page.setPageNo(start);
		page.setPageSize(limit);
		page = this.queryPaginationBySql(page);
		return page;
	}

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.region.dao.IRegionDao#getRootOrg(com.hikvision.finance.fms.model.Role)
     */
	@SuppressWarnings("unchecked")
    @Override
    public Region getRootOrg(Users user) {
		String sql = "select a.* from Region a, userregionmapping b where a.id = b.nRegionId and b.nUserId = ? ORDER BY a.nSortId ASC";
        List<Region> list = (List<Region>)this.createSQLQuery(sql).addEntity(Region.class).setParameter(0, user.getId()).list();
		if(list != null && list.size() > 0){
			return list.get(0);
		}
		return null;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.region.dao.IRegionDao#getLikeStrPath(com.hikvision.finance.fms.model.Region)
     */
    @SuppressWarnings("unchecked")
    @Override
    public List<Region> getLikeStrPath(Region region) {
		String sql = "select a.* from Region a where a.strPath like ?";
		String parm = region.getStrPath() + "%";
        List<Region> list = (List<Region>)this.createSQLQuery(sql).addEntity(Region.class).setParameter(0, parm).list();
		return list;
    }
    
    @SuppressWarnings("unchecked")
    public List<Integer> getIdsLikeStrPath(Region region) {
    	String sql = "select a.id from Region a where a.strPath like ?";
    	String parm = region.getStrPath() + "%";
    	List<Integer> list = this.createSQLQuery(sql).setResultTransformer(Transformers.TO_LIST).setParameter(0, parm).list();
    	return list;
    }
    
    /*
     * (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.region.dao.IRegionDao#getLikeStrPathIds(com.hikvision.finance.fms.model.Region)
     */
    @SuppressWarnings("unchecked")
    public List<Integer> getLikeStrPathIds(Region region){
    	List<Integer> regionIds = findListByHQL("select id from Region where strPath like '"+ region.getStrPath() + "%'");
    	return regionIds;
    }
    
	@SuppressWarnings("rawtypes")
    private List findListByHQL(String queryString)
			throws DataAccessException {
		return getHibernateTemplate().find(queryString);
	}

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.region.dao.IRegionDao#getChildStrPath(java.lang.Integer)
     */
    @SuppressWarnings("unchecked")
    @Override
    public List<Region> getChildStrPath(Region region) {
    	String sql = "select a.* from Region a where a.strPath like ?";
		String parm = region.getStrPath() + "-%";
        List<Region> list = (List<Region>)this.createSQLQuery(sql).addEntity(Region.class).setParameter(0, parm).list();
		return list;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.region.dao.IRegionDao#exportRegionExcel(com.hikvision.finance.fms.modules.region.dto.RegionInfo)
     */
    @SuppressWarnings("unchecked")
    @Override
    public List<RegionInfo> exportRegionExcel(RegionInfo regionInfo) {	
    	//1.查询条件封装
    	SqlQuery sqlQuery = conditionQuery(regionInfo);
    	//2.导出条件限制
		List<RegionInfo> rinfos = new ArrayList<RegionInfo>();
		List<RegionInfo> regionInfos = (List<RegionInfo>)this.queryListBySql(sqlQuery);
		Integer count = regionInfos.size();		
		if(count > Constants.EXCEL_MAX_NUM){
			throw new ExpectedException("", "导出数据超过"+Constants.EXCEL_MAX_NUM+"条,无法导出");
		}
		//3.封装map<组织id，组织实例>
		List<Region> regions = this.getAll();
		Map<Integer, Region> map = new  HashMap<Integer, Region>();
		for(Region region :regions){
			map.put(region.getId(), region);
		}
		
		//4.剔除根节点，封装父节点名称
		for(RegionInfo info:regionInfos){
			String nparentName = "无";
			if(info.getId()==1){
				info.setNparentName(nparentName);
			}else{
				Region region = map.get(info.getNparentId());
				nparentName = region.getStrName();
				info.setNparentName(nparentName);
			}
			rinfos.add(info);
		}
		return rinfos;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.region.dao.IRegionDao#getNetName()
     */
    @SuppressWarnings("unchecked")
    @Override
    public List<String> getNetName() {
    	String hql = "select r.strName FROM Region r where r.id!=1";
        List<String> list = findListByHQL(hql);
		return list;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.region.dao.IRegionDao#getRegLevel(int)
     */
    @Override
    public int getRegLevel(int regionId) {
		Region org = super.get(regionId);
		if(org != null) {
			if(org.getNparentId() != 0) {
				return 1+getRegLevel(org.getNparentId());
			}
			return 1;
		}
		return 0;
    }
    
    /**
     * 查询条件封装方法
     * @author jinxindong 2016年5月18日 下午6:10:53
     * @param regionInfo
     * @return
     */
	private SqlQuery conditionQuery(RegionInfo regionInfo) {
		SqlQuery sqlQuery = new SqlQuery();
		StringBuffer sql = null;
		String keywords = null == regionInfo.getStrName()?"":regionInfo.getStrName();
		Integer regionId = regionInfo.getId();
		if (0 == regionInfo.getIsShow()) {// 不显示子机构
			sql = new StringBuffer(" SELECT  reg.id,reg.strName,reg.strCode,reg.strTxt,reg.nsortId AS nsortId,reg.nParentId as nparentId  FROM region reg WHERE 1=1 ");	
			sql.append(" and ( reg.id = ?  ");
			sqlQuery.addParams(regionId);
			sql.append(" or reg.nParentId = ? ) ");
			sqlQuery.addParams(regionId);
		} else {// 显示所有子机构
			sql = new StringBuffer(" SELECT  reg.id,reg.strName,reg.strCode,reg.strTxt,reg.nsortId AS nsortId,reg.nParentId as nparentId  FROM region reg WHERE 1=1 ");
			String strParentPath = "";
			// 1. 控制单元,根据用户权限显示
			if (regionInfo.getId()==Constants.ROOT_REG_ID) {
				UserSession userSession = SessionUtil.getUserSession();
				// strParentPath = userSession.getRootOrgPath()+"-";
				strParentPath = userSession.getRootOrgPath();
			} else {// 2. 非控制单元
				Region region = this.get(regionId);
				strParentPath = region.getStrPath();
			}
			sql.append(" and reg.strPath like ? ");
			sqlQuery.addParams(strParentPath + "%");
			
		}
		
		if (StringUtils.isNotBlank(keywords)) {
			sql.append(" and reg.strName like ?  escape '/' ");
			sqlQuery.addParams("%" +  StringUtilExpand.escapeSQLLike(keywords) + "%");
		}
		// 默认倒序
		sql.append(" order by nsortId asc");
		// 设置sqlQuery属性
		sqlQuery.setSql(sql.toString());
		sqlQuery.setDtoClass(RegionInfo.class);
		return sqlQuery;
	}
	
}
