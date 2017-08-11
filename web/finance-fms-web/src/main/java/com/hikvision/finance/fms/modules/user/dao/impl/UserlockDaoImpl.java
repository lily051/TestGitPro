/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年11月3日 上午11:11:31
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.user.dao.impl;


import org.hibernate.Criteria;
import org.hibernate.criterion.Order;
import org.hibernate.criterion.Restrictions;

import com.hikvision.finance.extend.hibernate.dao.BaseDao;
import com.hikvision.finance.fms.model.Userlock;
import com.hikvision.finance.fms.modules.user.dao.IUserlockDao;
import com.hikvision.finance.fms.modules.user.qo.UserlockQo;


/**
 * 
 * <p></p>
 * @author zhoujiajun 2016年11月3日 上午11:11:31
 * @version V1.0
 */
public class UserlockDaoImpl extends BaseDao<Userlock, UserlockQo> implements IUserlockDao {
	
	/**
	 * 序列化ID
	 */
	private static final long serialVersionUID = 1424196259180411668L;
	
	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.hibernate.dao.BaseDao#buildCriteria(org.hibernate.Criteria, com.hikvision.finance.extend.hibernate.qo.BaseQo)
     */
    @Override
    protected Criteria buildCriteria(Criteria criteria, UserlockQo qo) {
    	if(qo != null) {
    		if(qo.getUserId() != null) {
    			criteria.add(Restrictions.eq("userId", qo.getUserId()));
    		}
    		if(qo.getStrName() != null) {
    			criteria.add(Restrictions.eq("strName", qo.getStrName()));
    		}
    	}
    	criteria.addOrder(Order.desc("id"));
    	return criteria;
    }
}
