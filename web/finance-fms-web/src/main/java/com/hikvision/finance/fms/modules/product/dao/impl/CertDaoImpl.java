/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月10日 上午9:24:41
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.product.dao.impl;

import java.util.ArrayList;
import java.util.List;

import org.hibernate.Criteria;
import org.hibernate.criterion.MatchMode;
import org.hibernate.criterion.Restrictions;

import com.hikvision.finance.core.util.StringUtils;
import com.hikvision.finance.extend.hibernate.dao.BaseDao;
import com.hikvision.finance.extend.hibernate.qo.SqlQuery;
import com.hikvision.finance.fms.common.util.StringUtilExpand;
import com.hikvision.finance.fms.model.Crtificate;
import com.hikvision.finance.fms.modules.product.dao.ICertDao;
import com.hikvision.finance.fms.modules.product.qo.CrtificateQo;


/**
 * <p></p>
 * @author jinxindong 2016年3月10日 上午9:24:41
 * @version V1.0 
 */
public class CertDaoImpl extends BaseDao<Crtificate,CrtificateQo> implements ICertDao{

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 2541294185538849596L;

	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.hibernate.dao.BaseDao#buildCriteria(org.hibernate.Criteria, com.hikvision.finance.extend.hibernate.qo.BaseQo)
     */
    @Override
    protected Criteria buildCriteria(Criteria criteria, CrtificateQo qo) {
		if (qo != null) {
			// 名称
			if (qo.getStrName() != null) {
				criteria.add(Restrictions.like("strName", qo.getStrName(), MatchMode.ANYWHERE));
			}
		}
		return criteria;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.product.dao.ICertDao#getCertNamesByIds(java.util.List)
     */
    @Override
    public List<Crtificate> getCertNamesByIds(List<Integer> ids) {
    	List<Crtificate> crtificates = new ArrayList<Crtificate>(); 
    	for(Integer id:ids){
    		Crtificate crtificate = this.get(id);
    		crtificates.add(crtificate);
    	}
    	return crtificates;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.product.dao.ICertDao#getCertByName(java.lang.String)
     */
    @SuppressWarnings("unchecked")
    @Override
    public List<Crtificate> getCertByName(String name) {
		SqlQuery sqlQuery = new SqlQuery();
		StringBuffer sql = new StringBuffer("SELECT r.* from crtificate r where 1=1 ");
		if (StringUtils.isNotBlank(name)) {
			sql.append(" and r.strName like ?  escape '/' ");
			sqlQuery.addParams("%" + StringUtilExpand.escapeSQLLike(name) + "%");
		}
		// 设置sqlQuery属性
		sqlQuery.setSql(sql.toString());
		sqlQuery.setDtoClass(Crtificate.class);
		return (List<Crtificate>)this.queryListBySql(sqlQuery);
    }
	
}
