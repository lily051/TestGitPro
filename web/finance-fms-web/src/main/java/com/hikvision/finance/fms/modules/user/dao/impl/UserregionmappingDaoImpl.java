/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月3日 下午6:16:03
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.user.dao.impl;

import java.util.List;

import org.hibernate.Criteria;
import org.hibernate.Query;

import com.hikvision.finance.extend.hibernate.dao.BaseDao;
import com.hikvision.finance.fms.model.Userregionmapping;
import com.hikvision.finance.fms.modules.user.dao.IUserregionmappingDao;
import com.hikvision.finance.fms.modules.user.qo.UserregionmappingQo;


/**
 * <p></p>
 * @author jinxindong 2016年3月3日 下午6:16:03
 * @version V1.0 
 */
public class UserregionmappingDaoImpl extends BaseDao<Userregionmapping, UserregionmappingQo> implements IUserregionmappingDao {

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 6683309373799032036L;

	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.hibernate.dao.BaseDao#buildCriteria(org.hibernate.Criteria, com.hikvision.finance.extend.hibernate.qo.BaseQo)
     */
    @Override
    protected Criteria buildCriteria(Criteria criteria, UserregionmappingQo qo) {
	    // TODO Auto-generated method stub
	    return null;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.user.dao.IUserregionmappingDao#delUserRegionMapping(java.util.List)
     */
    @Override
    public void delUserRegionMapping(List<Integer> ids) {
		String hql = "delete Userregionmapping a where a.nuserId in(:ids)";
		Query q = this.createQuery(hql);
		q.setParameterList("ids", ids);
		q.executeUpdate();
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.user.dao.IUserregionmappingDao#getUserRegionCode(java.lang.Integer)
     */
    @SuppressWarnings("unchecked")
    @Override
    public List<Integer> getUserRegionCode(Integer userId) {
    	String hql = "SELECT b.nregionId FROM  Users a,Userregionmapping b where a.id=b.nuserId AND b.nuserId = ?";
		return this.createQuery(hql, new Object[]{userId}).list();
    }
	
	
	
}
