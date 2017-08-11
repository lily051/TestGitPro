/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月28日 下午1:58:17
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.card.dao.impl;

import org.hibernate.Criteria;
import org.hibernate.criterion.Restrictions;

import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.hibernate.dao.BaseDao;
import com.hikvision.finance.extend.hibernate.qo.SqlQuery;
import com.hikvision.finance.fms.model.Paramdictionary;
import com.hikvision.finance.fms.modules.card.dao.ICardDao;
import com.hikvision.finance.fms.modules.card.dto.ParamdictionaryInfo;
import com.hikvision.finance.fms.modules.card.qo.ParamdictionaryQo;


/**
 * <p></p>
 * @author jinxindong 2016年3月28日 下午1:58:17
 * @version V1.0 
 */
public class CardDaoImpl extends BaseDao<Paramdictionary,ParamdictionaryQo> implements ICardDao{

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = -172909155207582691L;

	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.hibernate.dao.BaseDao#buildCriteria(org.hibernate.Criteria, com.hikvision.finance.extend.hibernate.qo.BaseQo)
     */
    @Override
	protected Criteria buildCriteria(Criteria criteria, ParamdictionaryQo qo) {
		if (qo != null) {
			if (qo.getEnable() != null) {
				criteria.add(Restrictions.eq("enable", qo.getEnable()));
			}
		}
		return criteria;
	}

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.card.dao.ICardDao#getDataPage(java.lang.Integer, java.lang.Integer, com.hikvision.finance.fms.modules.card.dto.ParamdictionaryInfo)
     */
    @Override
    public Page getDataPage(Integer limit, Integer start, ParamdictionaryInfo paramdictionaryInfo) {

    	SqlQuery sqlQuery = new SqlQuery();
		StringBuffer sql = new StringBuffer("select p.strName AS strName,p.id AS id, p.strCode AS strCode,p.`enable` AS `enable` FROM paramdictionary p where 1=1");
		// 默认倒序
		sql.append(" order by p.id desc");
		// 设置sqlQuery属性
		sqlQuery.setSql(sql.toString());
		sqlQuery.setDtoClass(ParamdictionaryInfo.class);
		
		// 设置page 属性
		Page page = new Page();
		page.setCondition(sqlQuery);
		page.setPageNo(start);
		page.setPageSize(limit);
	    page = this.queryPaginationBySql(page);
	    return page;
    }
	
}
