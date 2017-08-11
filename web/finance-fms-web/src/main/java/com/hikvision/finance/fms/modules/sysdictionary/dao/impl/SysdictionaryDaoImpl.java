/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月9日 下午4:32:29
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.sysdictionary.dao.impl;

import org.hibernate.Criteria;
import org.hibernate.criterion.MatchMode;
import org.hibernate.criterion.Order;
import org.hibernate.criterion.Restrictions;

import com.hikvision.finance.core.util.CollectionUtils;
import com.hikvision.finance.core.util.StringUtils;
import com.hikvision.finance.extend.hibernate.dao.BaseDao;
import com.hikvision.finance.fms.model.Sysdictionary;
import com.hikvision.finance.fms.modules.sysdictionary.dao.ISysdictionaryDao;
import com.hikvision.finance.fms.modules.sysdictionary.qo.SysdictionaryQo;


/**
 * <p></p>
 * @author jinxindong 2016年3月9日 下午4:32:29
 * @version V1.0 
 */
public class SysdictionaryDaoImpl extends BaseDao<Sysdictionary, SysdictionaryQo> implements ISysdictionaryDao{

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = -3687650957988279521L;

	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.hibernate.dao.BaseDao#buildCriteria(org.hibernate.Criteria, com.hikvision.finance.extend.hibernate.qo.BaseQo)
     */
    @Override
    protected Criteria buildCriteria(Criteria criteria, SysdictionaryQo qo) {
    	if(qo!=null){
    		//key唯一,直接查询
    		if(qo.getNkey()!=null){
    			criteria.add(Restrictions.eq("nkey", qo.getNkey()));
    		}else if(CollectionUtils.isNotEmpty(qo.getNkeys())) {//字典key in 查询
				criteria.add(Restrictions.in("nkey", qo.getNkeys()));
			}else{    			
	    		if(qo.getNtype()!=null){
	    			criteria.add(Restrictions.eq("ntype", qo.getNtype()));
	    		}else if(CollectionUtils.isNotEmpty(qo.getNtypes())) {//字典类型 in 查询
	    			criteria.add(Restrictions.in("ntype", qo.getNtypes()));
	    		}else if(qo.getNbigType()!=null){
		    		criteria.add(Restrictions.eq("nbigType", qo.getNbigType()));
	    		}	    	  			
    			if(qo.getNmanType()!=null){
    				criteria.add(Restrictions.eq("nmanType", qo.getNmanType()));
    			}
    			if(qo.getNdevType()!=null){
    				criteria.add(Restrictions.eq("ndevType", qo.getNdevType()));
    			}
			} 
    		if (qo.getNenabled()!= null) {
				criteria.add(Restrictions.eq("nenabled", qo.getNenabled()));
			}
    		if(StringUtils.isNotEmpty(qo.getLikeStrValue())){
    			criteria.add(Restrictions.like("strValue", qo.getLikeStrValue(), MatchMode.ANYWHERE));
    		}
    		if(StringUtils.isNotEmpty(qo.getStrValue())){
    			criteria.add(Restrictions.eq("strValue", qo.getStrValue()));
    		}
    		if(qo.isOrderByNSortIdAsc()){
				criteria.addOrder(Order.asc("nsortId"));
			}
    		if(qo.getNotId()!=null){
    			criteria.add(Restrictions.ne("id", qo.getNotId()));
			}
    	}
    	return criteria;
    }
	
}
