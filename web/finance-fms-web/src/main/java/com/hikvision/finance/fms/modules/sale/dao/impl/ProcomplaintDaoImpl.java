/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年10月24日 下午3:08:59
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.sale.dao.impl;

import org.hibernate.Criteria;

import com.hikvision.finance.extend.hibernate.dao.BaseDao;
import com.hikvision.finance.fms.model.Procomplaint;
import com.hikvision.finance.fms.modules.sale.dao.IProcomplaintDao;
import com.hikvision.finance.fms.modules.sale.qo.ProcomplaintQo;


/**
 * <p>投诉信息表Dao</p>
 * @author jinchenwu 2016年10月24日 下午3:08:59
 * @version V1.0 
 */
public class ProcomplaintDaoImpl extends BaseDao<Procomplaint ,ProcomplaintQo> implements IProcomplaintDao{

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = -4002238884214550950L;

	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.hibernate.dao.BaseDao#buildCriteria(org.hibernate.Criteria, com.hikvision.finance.extend.hibernate.qo.BaseQo)
     */
    @Override
    protected Criteria buildCriteria(Criteria criteria, ProcomplaintQo qo) {
	    return criteria;
    }
	
}
