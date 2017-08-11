/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月5日 下午4:57:25
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.device.dao.impl;

import org.hibernate.Criteria;

import com.hikvision.finance.extend.hibernate.dao.BaseDao;
import com.hikvision.finance.fms.model.Cloudpoolmapping;
import com.hikvision.finance.fms.modules.device.dao.ICloudpoolmappingDao;
import com.hikvision.finance.fms.modules.device.qo.CloudpoolmappingQo;


/**
 * <p></p>
 * @author jinxindong 2016年4月5日 下午4:57:25
 * @version V1.0 
 */
public class CloudpoolmappingDaoImpl extends BaseDao<Cloudpoolmapping,CloudpoolmappingQo> implements ICloudpoolmappingDao{

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = -2130746767900654652L;

	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.hibernate.dao.BaseDao#buildCriteria(org.hibernate.Criteria, com.hikvision.finance.extend.hibernate.qo.BaseQo)
     */
    @Override
    protected Criteria buildCriteria(Criteria criteria, CloudpoolmappingQo qo) {
	    // TODO Auto-generated method stub
	    return null;
    }
	
}
