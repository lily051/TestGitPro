/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月29日 下午2:44:25
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.role.dao.impl;

import java.util.List;

import org.hibernate.Criteria;
import org.hibernate.Query;

import com.hikvision.finance.extend.hibernate.dao.BaseDao;
import com.hikvision.finance.fms.model.Roleprivilegemapping;
import com.hikvision.finance.fms.modules.role.dao.IRolePrivilegeMappingDao;
import com.hikvision.finance.fms.modules.role.qo.RoleprivilegemappingQo;


/**
 * <p></p>
 * @author jinxindong 2016年2月29日 下午2:44:25
 * @version V1.0 
 */
public class RolePrivilegeMappingDaoImpl extends BaseDao<Roleprivilegemapping, RoleprivilegemappingQo> implements IRolePrivilegeMappingDao {

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 1316992481589327775L;

	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.hibernate.dao.BaseDao#buildCriteria(org.hibernate.Criteria, com.hikvision.finance.extend.hibernate.qo.BaseQo)
     */
    @Override
    protected Criteria buildCriteria(Criteria criteria, RoleprivilegemappingQo qo) {
	    // TODO Auto-generated method stub
	    return null;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.role.dao.IRolePrivilegeMappingDao#getRolePowerCode(java.lang.Integer)
     */
    @SuppressWarnings("unchecked")
    @Override
    public List<String> getRolePowerCode(Integer id) {
    	String hql = "select a.strCode from Privilege a, Roleprivilegemapping b where a.strCode = b.nprivilegeCode and b.nroleId = ?";
		return this.createQuery(hql, new Object[]{id}).list();
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.role.dao.IRolePrivilegeMappingDao#deleteBatchByRole(java.util.List)
     */
    @Override
    public void deleteBatchByRole(List<Integer> ids) {
		String hql = "delete Roleprivilegemapping a where a.nroleId in(:ids)";
		Query q = this.createQuery(hql);
		q.setParameterList("ids", ids);
		q.executeUpdate();
    }

	
}
