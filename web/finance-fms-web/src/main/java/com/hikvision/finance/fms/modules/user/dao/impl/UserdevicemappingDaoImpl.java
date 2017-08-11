/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月3日 下午6:15:34
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.user.dao.impl;

import java.util.List;

import org.hibernate.Criteria;
import org.hibernate.Query;

import com.hikvision.finance.extend.hibernate.dao.BaseDao;
import com.hikvision.finance.fms.model.Userdevicemapping;
import com.hikvision.finance.fms.modules.user.dao.IUserdevicemappingDao;
import com.hikvision.finance.fms.modules.user.qo.UserdevicemappingQo;


/**
 * <p></p>
 * @author jinxindong 2016年3月3日 下午6:15:34
 * @version V1.0 
 */
public class UserdevicemappingDaoImpl extends BaseDao<Userdevicemapping, UserdevicemappingQo>implements IUserdevicemappingDao {

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 2049668697233172246L;

	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.hibernate.dao.BaseDao#buildCriteria(org.hibernate.Criteria, com.hikvision.finance.extend.hibernate.qo.BaseQo)
     */
    @Override
    protected Criteria buildCriteria(Criteria criteria, UserdevicemappingQo qo) {
	    // TODO Auto-generated method stub
	    return null;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.user.dao.IUserdevicemappingDao#delUserDeviceMapping(java.util.List)
     */
    @Override
    public void delUserDeviceMapping(List<Integer> ids) {
		String hql = "delete Userdevicemapping a where a.nuserId in(:ids)";
		Query q = this.createQuery(hql);
		q.setParameterList("ids", ids);
		q.executeUpdate();
	    
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.user.dao.IUserdevicemappingDao#getChannelByUserId(java.lang.Integer)
     */
    @SuppressWarnings("unchecked")
    @Override
    public List<Integer> getChannelByUserId(Integer userId) {
    	String hql = "SELECT b.devChannel as devChannel  FROM  Users a,Userdevicemapping b where a.id=b.nuserId AND b.nuserId = ?";
		return this.createQuery(hql, new Object[]{userId}).list();
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.user.dao.IUserdevicemappingDao#saveMapping(java.lang.String)
     */
    @Override
    public void saveMapping(String strsql) {
    	String sql = "INSERT INTO userdevicemapping(nUserId,devId,devChannel) VALUES "+strsql;
    	this.createSQLQuery(sql).executeUpdate();
	    
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.user.dao.IUserdevicemappingDao#deleteMapping(java.lang.Integer, java.lang.String)
     */
    @Override
    public void deleteMapping(Integer userId, String strsql) {
    	String sql = "delete from userdevicemapping  where nUserId = ? and devChannel in ( "+strsql+" )";
    	this.createSQLQuery(sql).setParameter(0, userId).executeUpdate();	    
    }
	

	
}
