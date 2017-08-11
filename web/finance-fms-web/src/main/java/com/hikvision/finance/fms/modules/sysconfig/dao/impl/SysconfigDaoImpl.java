/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月9日 下午4:43:15
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.sysconfig.dao.impl;

import java.util.List;

import org.hibernate.Criteria;
import org.hibernate.criterion.Restrictions;

import com.hikvision.finance.core.util.CollectionUtils;
import com.hikvision.finance.extend.hibernate.dao.BaseDao;
import com.hikvision.finance.fms.model.Sysconfig;
import com.hikvision.finance.fms.modules.sysconfig.dao.ISysconfigDao;
import com.hikvision.finance.fms.modules.sysconfig.qo.SysconfigQo;

/**
 * <p>
 * </p>
 * @author jinxindong 2016年3月9日 下午4:43:15
 * @version V1.0
 */
public class SysconfigDaoImpl extends BaseDao<Sysconfig, SysconfigQo> implements ISysconfigDao {
	
	/**
	 * 序列化ID
	 */
	private static final long serialVersionUID = 6483389896147195689L;
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.extend.hibernate.dao.BaseDao#buildCriteria(org.hibernate.Criteria, com.hikvision.finance.extend.hibernate.qo.BaseQo)
	 */
	@Override
	protected Criteria buildCriteria(Criteria criteria, SysconfigQo qo) {
		if (qo != null) {
			if (CollectionUtils.isNotEmpty(qo.getNkeys())) {
				criteria.add(Restrictions.in("nkey", qo.getNkeys()));
			}
			if (qo.getNkey() != null) {
				criteria.add(Restrictions.eq("nkey", qo.getNkey()));
			}
		}
		return criteria;
	}

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.sysconfig.dao.ISysconfigDao#getSysconfigByKey(java.lang.Integer)
     */
    @Override
    public Sysconfig getSysconfigByKey(Integer key) {
    	return findUniqueBy("nkey", key);
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.modules.sysconfig.dao.ISysconfigDao#getSysconfigByKey(java.util.List)
     */
    @Override
    public List<Sysconfig> getSysconfigByKey(List<Integer> keys) {
    	return findAllEntitysBy("nkey", keys);
    }
	
}
