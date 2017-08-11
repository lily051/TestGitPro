/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月5日 下午3:01:53
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.role.dao.impl;

import java.util.List;

import org.hibernate.Criteria;

import com.hikvision.finance.extend.hibernate.dao.BaseDao;
import com.hikvision.finance.fms.common.model.UserSession;
import com.hikvision.finance.fms.common.util.SessionUtil;
import com.hikvision.finance.fms.model.Privilege;
import com.hikvision.finance.fms.model.Userrolemapping;
import com.hikvision.finance.fms.modules.role.dao.IPrivilegeDao;
import com.hikvision.finance.fms.modules.role.qo.PrivilegeQo;


/**
 * <p></p>
 * @author jinxindong 2016年3月5日 下午3:01:53
 * @version V1.0 
 */
public class PrivilegeDaoImpl extends BaseDao<Privilege, PrivilegeQo> implements IPrivilegeDao {

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = -8768090894148419192L;

	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.hibernate.dao.BaseDao#buildCriteria(org.hibernate.Criteria, com.hikvision.finance.extend.hibernate.qo.BaseQo)
     */
    @Override
    protected Criteria buildCriteria(Criteria criteria, PrivilegeQo qo) {
	    // TODO Auto-generated method stub
	    return null;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.role.dao.IPrivilegeDao#getPowersByRoleId(java.lang.Integer)
     */
    @SuppressWarnings("unchecked")
    @Override
    public List<Privilege> getPowersByRoleId(Integer id) {
		String hql = "select a from Privilege a, Roleprivilegemapping b "
				+ "where a.strCode = b.nprivilegeCode "
				+ "and b.nroleId = ? and a.nenabled = 1  "
				+ "order by a.strSuper, a.nsortId ASC";
		return (List<Privilege>) this.createQuery(hql, new Object[]{id}).list();
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.role.dao.IPrivilegeDao#getPowers(java.lang.Integer, java.lang.Integer)
     */
    @SuppressWarnings("unchecked")
    @Override
    public List<Privilege> getPowers(Integer nenabled, Integer nsysNo) {
    	String hql = "select a from Privilege a where a.nenabled=? AND  a.nsysNo=? order by a.strSuper, a.nsortId ASC";
		return (List<Privilege>) this.createQuery(hql,  new Object[]{nenabled,nsysNo}).list();
    }
	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.role.dao.IPrivilegeDao#getFristByPower(java.lang.Integer)
     */
    @SuppressWarnings("unchecked")
    @Override
    public Privilege getFristByPower(Integer flag) {
    	Privilege privilege = null;
    	List<Privilege>  privileges = null;
	    UserSession userSession = SessionUtil.getUserSession();
	    if(userSession.isAdmin()){
		    String hql1 = " select p from Privilege p  where  p.nbtype = ? ORDER BY p.strCode ASC";
		    privileges = (List<Privilege>)this.createQuery(hql1).setProperties(Privilege.class).setParameter(0, flag).list();
	    }else{
		    String hql = "select u from Role r, Userrolemapping u where u.nroleId=r.id and u.nuserId = ? ";
		    List<Userrolemapping>  userrolemappings = (List<Userrolemapping>)this.createQuery(hql).setProperties(Userrolemapping.class).setParameter(0, userSession.getUserId()).list();
		    String hql2 = " select p from Privilege p , Roleprivilegemapping r where  r.nprivilegeCode = p.strCode  and r.nroleId = ? AND p.nbtype = ? ORDER BY p.strCode ASC";
		    privileges = (List<Privilege>)this.createQuery(hql2).setProperties(Privilege.class).setParameter(0,userrolemappings.get(0).getNroleId()).setParameter(1, flag).list();
	    }
	    if(privileges.size()>0){
	    	 privilege = privileges.get(0);
	    }
	    return privilege;
    }


	
}
