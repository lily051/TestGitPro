/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月28日 下午1:56:26
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.card.dao;

import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.hibernate.dao.IBaseDao;
import com.hikvision.finance.fms.model.Paramdictionary;
import com.hikvision.finance.fms.modules.card.dto.ParamdictionaryInfo;
import com.hikvision.finance.fms.modules.card.qo.ParamdictionaryQo;


/**
 * @author jinxindong 2016年3月28日 下午1:56:26
 * @version V1.0 
 */
public interface ICardDao  extends IBaseDao<Paramdictionary,ParamdictionaryQo>{
	
	/**
	 * 查询证件page数据
	 * @author jinxindong 2016年3月29日 下午4:40:26
	 * @param limit
	 * @param start
	 * @param paramdictionaryInfo
	 * @return
	 */
	public Page getDataPage(Integer limit,Integer start,ParamdictionaryInfo paramdictionaryInfo);
}
