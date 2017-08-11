/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月29日 下午2:19:18
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.role.dao.impl;

import java.util.List;

import org.hibernate.Criteria;
import org.springframework.dao.DataAccessException;

import com.hikvision.finance.core.util.StringUtils;
import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.hibernate.dao.BaseDao;
import com.hikvision.finance.extend.hibernate.qo.SqlQuery;
import com.hikvision.finance.fms.common.util.StringUtilExpand;
import com.hikvision.finance.fms.model.Role;
import com.hikvision.finance.fms.model.Users;
import com.hikvision.finance.fms.modules.role.dao.IRoleDao;
import com.hikvision.finance.fms.modules.role.dto.RoleInfo;
import com.hikvision.finance.fms.modules.role.qo.RoleQo;
import com.hikvision.finance.fms.modules.user.dao.IUserrolemappingDao;
import com.hikvision.finance.fms.modules.user.dao.IUsersDao;

/**
 * <p>
 * </p>
 * @author jinxindong 2016年2月29日 下午2:19:18
 * @version V1.0
 */
public class RoleDaoImpl extends BaseDao<Role, RoleQo> implements IRoleDao {
	
	private IUsersDao usersDao;
	private IUserrolemappingDao userrolemappingDao;
	/**
	 * 序列化ID
	 */
	private static final long serialVersionUID = 3927497750020126425L;
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.extend.hibernate.dao.BaseDao#buildCriteria(org.hibernate.Criteria, com.hikvision.finance.extend.hibernate.qo.BaseQo)
	 */
	@Override
	protected Criteria buildCriteria(Criteria criteria, RoleQo qo) {
		// TODO Auto-generated method stub
		return null;
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fms.modules.role.dao.IRoleDao#getDataPage(java.lang.Integer, java.lang.Integer, java.lang.String)
	 */
    @Override
	public Page getDataPage(Integer limit, Integer start, String keywords) {
		SqlQuery sqlQuery = new SqlQuery();
		StringBuffer sql = new StringBuffer(" SELECT r.id as id ,r.nType as ntype, r.strName as strName ,r.nRight as nright,r.nOwner as nowner,r.strComment as strComment FROM role r WHERE 1=1  AND r.strName != 'admin' ");
		if (StringUtils.isNotBlank(keywords)) {
			sql.append(" and r.strName like ?  escape '/' ");
			sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(keywords) + "%");
		}
		// 默认倒序
		sql.append(" order by r.id desc");
		// 设置sqlQuery属性
		sqlQuery.setSql(sql.toString());
		sqlQuery.setDtoClass(RoleInfo.class);
		
		// 设置page 属性
		Page page = new Page();
		page.setCondition(sqlQuery);
		page.setPageNo(start);
		page.setPageSize(limit);
		page = this.queryPaginationBySql(page);
		return page;
	}
	
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.role.dao.IRoleDao#getRoleByUserId(com.hikvision.finance.fms.model.Users)
     */
    @SuppressWarnings("unchecked")
    @Override
    public List<Role> getRoleByUserId(Users user) {
    	String hql = "select b from Userrolemapping a, Role b where a.nroleId = b.id and a.nuserId = ?";
		return (List<Role>) this.createQuery(hql, new Object[]{user.getId()}).list();
    }
	
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.role.dao.IRoleDao#getRoleName()
     */
    @SuppressWarnings("unchecked")
    @Override
    public List<String> getRoleName() {
    	String hql = "select r.strName from Role r where r.strName!='admin'";
        List<String> list = findListByHQL(hql);
		return list;
    }
    
	@SuppressWarnings("rawtypes")
    private List findListByHQL(String queryString)
			throws DataAccessException {
		return getHibernateTemplate().find(queryString);
	}

	// ============================getter/setter======================================
	public IUsersDao getUsersDao() {
		return usersDao;
	}
	
	public void setUsersDao(IUsersDao usersDao) {
		this.usersDao = usersDao;
	}

	
    public IUserrolemappingDao getUserrolemappingDao() {
    	return userrolemappingDao;
    }

	
    public void setUserrolemappingDao(IUserrolemappingDao userrolemappingDao) {
    	this.userrolemappingDao = userrolemappingDao;
    }



	
	
}
