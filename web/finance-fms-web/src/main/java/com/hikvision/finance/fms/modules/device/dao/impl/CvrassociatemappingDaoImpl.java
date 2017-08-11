/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月5日 下午6:22:33
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.device.dao.impl;

import org.hibernate.Criteria;

import com.hikvision.finance.extend.hibernate.dao.BaseDao;
import com.hikvision.finance.fms.model.Cvrassociatemapping;
import com.hikvision.finance.fms.modules.device.dao.ICvrassociatemappingDao;
import com.hikvision.finance.fms.modules.device.qo.CvrassociatemappingQo;


/**
 * <p></p>
 * @author jinxindong 2016年4月5日 下午6:22:33
 * @version V1.0 
 */
public class CvrassociatemappingDaoImpl extends BaseDao<Cvrassociatemapping,CvrassociatemappingQo> implements ICvrassociatemappingDao{

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 7484034304572288545L;

	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.hibernate.dao.BaseDao#buildCriteria(org.hibernate.Criteria, com.hikvision.finance.extend.hibernate.qo.BaseQo)
     */
    @Override
    protected Criteria buildCriteria(Criteria criteria, CvrassociatemappingQo qo) {
	    // TODO Auto-generated method stub
	    return null;
    }
	
}
