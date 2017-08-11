/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月13日 下午7:09:26
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.analysis.dao.impl;

import org.hibernate.Criteria;

import com.hikvision.finance.extend.hibernate.dao.BaseDao;
import com.hikvision.finance.fms.modules.analysis.dao.IAnalysisDao;
import com.hikvision.finance.fms.modules.analysis.dto.AnalysisInfo;
import com.hikvision.finance.fms.modules.analysis.qo.AnalysisQo;


/**
 * <p></p>
 * @author jinxindong 2016年4月13日 下午7:09:26
 * @version V1.0 
 */
public class AnalysisDaoImpl extends BaseDao<AnalysisInfo,AnalysisQo> implements IAnalysisDao{

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 5104537308476416102L;

	/* (non-Javadoc)
     * @see com.hikvision.finance.extend.hibernate.dao.BaseDao#buildCriteria(org.hibernate.Criteria, com.hikvision.finance.extend.hibernate.qo.BaseQo)
     */
    @Override
    protected Criteria buildCriteria(Criteria criteria, AnalysisQo qo) {
	    // TODO Auto-generated method stub
	    return null;
    }
	
}
