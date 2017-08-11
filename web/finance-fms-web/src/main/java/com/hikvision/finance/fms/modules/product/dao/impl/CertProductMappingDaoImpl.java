/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月23日 下午4:58:43
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.product.dao.impl;

import java.util.List;

import org.hibernate.Criteria;

import com.hikvision.finance.extend.hibernate.dao.BaseDao;
import com.hikvision.finance.fms.model.Crtificateproductmapping;
import com.hikvision.finance.fms.model.Product;
import com.hikvision.finance.fms.modules.product.dao.ICertProductMappingDao;
import com.hikvision.finance.fms.modules.product.qo.CrtificateProductMappingQo;


/**
 * <p></p>
 * @author jinxindong 2016年3月23日 下午4:58:43
 * @version V1.0 
 */
public class CertProductMappingDaoImpl extends BaseDao<Crtificateproductmapping,CrtificateProductMappingQo> implements ICertProductMappingDao {

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = -8260970323643055410L;

	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.hibernate.dao.BaseDao#buildCriteria(org.hibernate.Criteria, com.hikvision.finance.extend.hibernate.qo.BaseQo)
     */
    @Override
    protected Criteria buildCriteria(Criteria criteria, CrtificateProductMappingQo qo) {
	    // TODO Auto-generated method stub
	    return null;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.product.dao.ICertProductMappingDao#getProductbyCertIds(java.util.List)
     */
    @SuppressWarnings("unchecked")
    @Override
	public List<Integer> getProductIdsbyCertId(Integer certId) {		
		String hql = "select a.nproductId from Crtificateproductmapping a where  a.ncrtificateId = ?";
		List<Integer> productIds = this.createQuery(hql, new Object[] {certId}).list();		
		return productIds;
	}

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.product.dao.ICertProductMappingDao#getProductbyCertId(java.lang.Integer)
     */
    @SuppressWarnings("unchecked")
    @Override
    public List<Product> getProductbyCertId(Integer id) {
    	String sql = "select b.* from crtificateproductmapping a ,product b WHERE  a.nProductId = b.id  and a.nCrtificateId = ? ";
		List<Product> products = this.createSQLQuery(sql.toString()).addEntity(Product.class).setParameter(0, id).list();
	    return products;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.product.dao.ICertProductMappingDao#meger()
     */
    @Override
    public void meger(Crtificateproductmapping entity) {
	    getHibernateTemplate().merge(entity);
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.product.dao.ICertProductMappingDao#saveMapping(java.lang.String)
     */
    @Override
    public void saveMapping(String strsql) {
    	String sql = "INSERT INTO crtificateproductmapping  (nProductId, nCrtificateId) VALUES "+strsql;
    	this.createSQLQuery(sql).executeUpdate();
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.product.dao.ICertProductMappingDao#deleteMapping(java.lang.String)
     */
    @Override
    public void deleteMapping(Integer certId,String strsql) {
    	String sql = "delete from crtificateproductmapping where nCrtificateId = ? and nProductId in ( "+strsql+" )";
    	this.createSQLQuery(sql).setParameter(0, certId).executeUpdate();
    }
	
}
