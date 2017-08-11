/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月28日 下午1:54:09
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.card.service;

import java.util.List;

import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.service.IBaseService;
import com.hikvision.finance.fms.model.Paramdictionary;
import com.hikvision.finance.fms.modules.card.dto.ParamdictionaryInfo;
import com.hikvision.finance.fms.modules.card.qo.ParamdictionaryQo;
import com.hikvision.finance.fwork.util.AjaxData;


/**
 * @author jinxindong 2016年3月28日 下午1:54:09
 * @version V1.0 
 */
public interface ICardService extends IBaseService<Paramdictionary,ParamdictionaryQo> {
	
	/**
	 * 查询证件page数据
	 * @author jinxindong 2016年3月29日 下午4:31:51
	 * @param limit
	 * @param start
	 * @param paramdictionaryInfo
	 * @return
	 */
	public Page getDataPage(Integer limit,Integer start,ParamdictionaryInfo paramdictionaryInfo);
	
	/**
	 * 添加证件类型
	 * @author jinxindong 2016年3月29日 下午4:32:49
	 * @param paramdictionaryInfo
	 * @return
	 */
	public AjaxData addCard(ParamdictionaryInfo paramdictionaryInfo);
	
	/**
	 * 根据id获取证件类型
	 * @author jinxindong 2016年3月29日 下午4:35:08
	 * @param cardId
	 * @return
	 */
	public ParamdictionaryInfo getCardById(Integer cardId);
	
	/**
	 * 修改证件类型
	 * @author jinxindong 2016年3月29日 下午4:35:54
	 * @param paramdictionaryInfo
	 * @return
	 */
	public AjaxData updateCard(ParamdictionaryInfo paramdictionaryInfo);
	
	/**
	 * 根据证件类型ids删除证件类型
	 * @author jinxindong 2016年3月29日 下午4:36:39
	 * @param ids
	 * @return
	 */
	public AjaxData deleteCards(List<Integer> ids);
	
	/**
	 * 启用证书
	 * @author jinxindong 2016年4月26日 下午1:58:43
	 * @param ids
	 * @return
	 */
	public AjaxData enableCards(List<Integer> ids);
	
	/**
	 * 禁用证书
	 * @author jinxindong 2016年4月26日 下午1:58:48
	 * @param ids
	 * @return
	 */
	public AjaxData disableCards(List<Integer> ids);
	
}
