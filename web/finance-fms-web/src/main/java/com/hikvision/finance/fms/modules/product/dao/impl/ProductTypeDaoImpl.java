/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月17日 上午10:13:04
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.product.dao.impl;

import java.util.List;

import org.hibernate.Criteria;
import org.hibernate.criterion.MatchMode;
import org.hibernate.criterion.Restrictions;
import org.springframework.dao.DataAccessException;

import com.hikvision.finance.extend.hibernate.dao.BaseDao;
import com.hikvision.finance.fms.model.Producttype;
import com.hikvision.finance.fms.modules.product.dao.IProductTypeDao;
import com.hikvision.finance.fms.modules.product.qo.ProductTypeQo;


/**
 * <p></p>
 * @author jinxindong 2016年3月17日 上午10:13:04
 * @version V1.0 
 */
public class ProductTypeDaoImpl extends BaseDao<Producttype,ProductTypeQo> implements IProductTypeDao{

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = -8845948884414610441L;

	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.hibernate.dao.BaseDao#buildCriteria(org.hibernate.Criteria, com.hikvision.finance.extend.hibernate.qo.BaseQo)
     */
    @Override
    protected Criteria buildCriteria(Criteria criteria, ProductTypeQo qo) {
		if (qo != null) {
			// 名称
			if (qo.getStrName() != null) {
				criteria.add(Restrictions.eq("strName", qo.getStrName()));
			}
			if (qo.getNsortId() != null) {
				criteria.add(Restrictions.eq("nsortId", qo.getNsortId()));
			}
			if (qo.getPid() != null) {
				criteria.add(Restrictions.eq("pid", qo.getPid()));
			}
			// 组织编码
			if (qo.getStrInterAreaCode() != null) {
				criteria.add(Restrictions.eq("strInterAreaCode", qo.getStrInterAreaCode()));
			}
			if (qo.getStrPath() != null) {
				criteria.add(Restrictions.like("strPath", qo.getStrPath(), MatchMode.ANYWHERE));
			}
		}
		return criteria;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.product.dao.IProductTypeDao#getLikeStrPath(com.hikvision.finance.fms.model.Producttype)
     */
    @SuppressWarnings("unchecked")
    @Override
    public List<Producttype> getLikeStrPath(Producttype producttype) {
		String sql = "select a.* from Producttype a where a.strPath like ?";
		String parm = producttype.getStrPath() + "%";
        List<Producttype> list = (List<Producttype>)this.createSQLQuery(sql).addEntity(Producttype.class).setParameter(0, parm).list();
		return list;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.product.dao.IProductTypeDao#getProductType()
     */
    @SuppressWarnings("unchecked")
    @Override
    public List<String> getProductType() {
    	String hql = "select strName from Producttype where id !=1";
        List<String> list = findListByHQL(hql);
		return list;
    }
    
	@SuppressWarnings("rawtypes")
    private List findListByHQL(String queryString)
			throws DataAccessException {
		return getHibernateTemplate().find(queryString);
	}

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.product.dao.IProductTypeDao#getRegLevel(int)
     */
    @Override
    public int getRegLevel(int regionId) {
		Producttype org = super.get(regionId);
		if(org != null) {
			if(org.getPid()!= 0) {
				return 1+getRegLevel(org.getPid());
			}
			return 1;
		}
		return 0;
    }

	
}
