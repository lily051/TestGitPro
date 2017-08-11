/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月19日 上午11:02:41
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.license.dao.impl;

import org.hibernate.Criteria;
import org.hibernate.criterion.Restrictions;

import com.hikvision.finance.extend.hibernate.dao.BaseDao;
import com.hikvision.finance.fms.license.dao.ISoftdogDao;
import com.hikvision.finance.fms.license.qo.SoftdogQo;
import com.hikvision.finance.fms.license.util.LicenseConstant;
import com.hikvision.finance.fms.model.Softdongle;


/**
 * <p></p>
 * @author jinxindong 2016年4月19日 上午11:02:42
 * @version V1.0 
 */
public class SoftdogDaoImpl extends BaseDao<Softdongle, SoftdogQo> implements ISoftdogDao{

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 1707388814352435501L;

	/* (non-Javadoc)
     * @see com.hikvision.finance.fms.license.dao.ISoftdogDao#getAuthoriseDeviceChannel()
     */
    @Override
    public int getAuthoriseDeviceChannel() {
    	Criteria criteria = createCriteria();
    	criteria.add(Restrictions.eq("ntype", LicenseConstant.DOGTYPE.DOG_CHANNEL));
    	
    	Softdongle softdog = (Softdongle)criteria.uniqueResult();
    	if(softdog!=null){
    		return softdog.getNvalue().intValue();
    	}
	    return 0;
    }

	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.hibernate.dao.BaseDao#buildCriteria(org.hibernate.Criteria, com.hikvision.finance.extend.hibernate.qo.BaseQo)
     */
    @Override
    protected Criteria buildCriteria(Criteria criteria, SoftdogQo qo) {
    	  if(qo!=null){
  	    	if(qo.getnType()!=null){
  	    		criteria.add(Restrictions.eq("ntype", qo.getnType()));
  	    	}
  	    }
  	    return criteria;
    }
	
}
