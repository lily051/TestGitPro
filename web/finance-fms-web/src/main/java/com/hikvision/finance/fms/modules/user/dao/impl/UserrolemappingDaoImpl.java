/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月29日 下午4:33:50
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.user.dao.impl;

import java.util.List;

import org.hibernate.Criteria;
import org.hibernate.Query;
import org.hibernate.criterion.Restrictions;

import com.hikvision.finance.extend.hibernate.dao.BaseDao;
import com.hikvision.finance.fms.model.Userrolemapping;
import com.hikvision.finance.fms.modules.user.dao.IUserrolemappingDao;
import com.hikvision.finance.fms.modules.user.qo.UserrolemappingQo;



/**
 * <p></p>
 * @author jinxindong 2016年2月29日 下午4:33:50
 * @version V1.0 
 */
public class UserrolemappingDaoImpl  extends BaseDao<Userrolemapping, UserrolemappingQo> implements IUserrolemappingDao {

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 5528371466170805039L;

	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.hibernate.dao.BaseDao#buildCriteria(org.hibernate.Criteria, com.hikvision.finance.extend.hibernate.qo.BaseQo)
     */
    @Override
    protected Criteria buildCriteria(Criteria criteria, UserrolemappingQo qo) {
		if (qo != null) {
			// nuserId
			if (qo.getNuserId() != null) {
				criteria.add(Restrictions.eq("nuserId", qo.getNuserId()));
			}
			// nroleId
			if (qo.getNroleId()!= null) {
				criteria.add(Restrictions.eq("nroleId", qo.getNroleId()));
			}
		}
		return criteria;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.user.dao.IUserrolemappingDao#deleteBatchByRole(java.util.List)
     */
    @Override
    public void deleteBatchByRole(List<Integer> ids) {
		String hql = "delete Userrolemapping a where a.nroleId in(:ids)";
		Query q = this.createQuery(hql);
		q.setParameterList("ids", ids);
		q.executeUpdate();
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.user.dao.IUserrolemappingDao#delUserRoleMapping(java.util.List)
     */
    @Override
    public void delUserRoleMapping(List<Integer> ids) {
		String hql = "delete Userrolemapping a where a.nuserId in(:ids)";
		Query q = this.createQuery(hql);
		q.setParameterList("ids", ids);
		q.executeUpdate();
    }
	
}
