/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月3日 下午6:15:01
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.user.dao.impl;

import java.util.List;

import org.hibernate.Criteria;
import org.hibernate.Query;

import com.hikvision.finance.extend.hibernate.dao.BaseDao;
import com.hikvision.finance.fms.model.Usercrtificatemapping;
import com.hikvision.finance.fms.modules.user.dao.IUsercrtificatemappingDao;
import com.hikvision.finance.fms.modules.user.qo.UsercrtificatemappingQo;


/**
 * <p></p>
 * @author jinxindong 2016年3月3日 下午6:15:01
 * @version V1.0 
 */
public class UsercrtificatemappingDaoImpl extends BaseDao<Usercrtificatemapping, UsercrtificatemappingQo> implements IUsercrtificatemappingDao {

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 5367393756654282101L;

	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.hibernate.dao.BaseDao#buildCriteria(org.hibernate.Criteria, com.hikvision.finance.extend.hibernate.qo.BaseQo)
     */
    @Override
    protected Criteria buildCriteria(Criteria criteria, UsercrtificatemappingQo qo) {
	    // TODO Auto-generated method stub
	    return null;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.user.dao.IUsercrtificatemappingDao#delUserCrtificateMapping(java.util.List)
     */
    @Override
    public void delUserCrtificateMapping(List<Integer> ids) {
		String hql = "delete Usercrtificatemapping a where a.userId in(:ids)";
		Query q = this.createQuery(hql);
		q.setParameterList("ids", ids);
		q.executeUpdate(); 
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.user.dao.IUsercrtificatemappingDao#getCertIdsByUserId(java.lang.Integer)
     */
    @SuppressWarnings("unchecked")
    @Override
    public List<Integer> getCertIdsByUserId(Integer userId) {
    	String hql = "select a.ncrtificateId from Usercrtificatemapping a, Users b where a.userId = b.id and b.id = ?";
		return this.createQuery(hql, new Object[]{userId}).list();
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.user.dao.IUsercrtificatemappingDao#saveMapping(java.lang.String)
     */
    @Override
    public void saveMapping(String strsql) {
    	String sql = "INSERT INTO usercrtificatemapping(nCrtificateId,userId) VALUES "+strsql;
    	this.createSQLQuery(sql).executeUpdate();
    }


	

	
}
