/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月29日 下午2:29:53
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.card.action;

import java.util.List;

import com.hikvision.finance.fms.model.Paramdictionary;
import com.hikvision.finance.fms.modules.card.dto.ParamdictionaryInfo;
import com.hikvision.finance.fms.modules.card.service.ICardService;
import com.hikvision.finance.fwork.exception.ExpectedException;
import com.hikvision.finance.fwork.www.action.BaseAction;


/**
 * <p>系统参数-证件类型管理Action</p>
 * @author jinxindong 2016年3月29日 下午2:29:53
 * @version V1.0 
 */
public class CardAction extends BaseAction<Paramdictionary>{

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = -8280008404294427233L;
    private ICardService cardService;
    private List<Integer> ids;
    private ParamdictionaryInfo paramdictionaryInfo;
    
    /**
     * 跳到证件管理页面
     * @author jinxindong 2016年4月11日 下午6:45:04
     * @return
     */
	public String toCardPage() {
		operPage = "/modules/card/card.jsp";
		return DISPATCHER;
	}
    /**
     * 获取page页面
     * @author jinxindong 2016年3月29日 下午3:12:00
     * @return
     */
    public String getDataPage(){
    	page = cardService.getDataPage(limit, start, paramdictionaryInfo);
    	return PAGE;
    }
    
    /**
     * 跳到证件类型添加页面
     * @author jinxindong 2016年3月29日 下午3:12:40
     * @return
     */
	public String toAddCardPage() {
		operPage = "/modules/card/dialogs/addCardPage.jsp";
		return DISPATCHER;
	}
	
    /**
     * 证件类型添加
     * @author jinxindong 2016年3月29日 下午3:13:15
     * @return
     */
	public String addCard() {
		ajaxData = cardService.addCard(paramdictionaryInfo);
		return AJAX;
	}
	
    /**
     * /跳到证件类型修改页面
     * @author jinxindong 2016年3月29日 下午3:13:38
     * @return
     */
	public String toUpdateCardPage() {
		if (paramdictionaryInfo == null || paramdictionaryInfo.getId() == null) {
			throw new ExpectedException("", "未获取到该证件的id");
		}
		//根据id获取证件类型
		paramdictionaryInfo = cardService.getCardById(paramdictionaryInfo.getId());
		operPage = "/modules/card/dialogs/editCardPage.jsp";
		return DISPATCHER;
	}
	
    /**
     * 证件类型修改
     * @author jinxindong 2016年3月29日 下午3:14:03
     * @return
     */
	public String updateCard() {
		if (paramdictionaryInfo == null || paramdictionaryInfo.getId() == null) {
			throw new ExpectedException("", "未获取到该证件的id");
		}
		ajaxData = cardService.updateCard(paramdictionaryInfo);
		return AJAX;
	}
	
    /**
     * 删除证件类型ids
     * @author jinxindong 2016年3月29日 下午3:14:30
     * @return
     */
	public String deleteCards() {
		ajaxData = cardService.deleteCards(ids);
		return AJAX;
	}
	
	/**
	 * 启用证件
	 * @author jinxindong 2016年4月26日 下午1:57:13
	 * @return
	 */
	public String enableCards() {
		ajaxData = cardService.enableCards(ids);
		return AJAX;
	}
	
	/**
	 * 禁用证件
	 * @author jinxindong 2016年4月26日 下午1:57:18
	 * @return
	 */
	public String disableCards() {
		ajaxData = cardService.disableCards(ids);
		return AJAX;
	}
	//====================================getter/setter==========================================================
    public ICardService getCardService() {
    	return cardService;
    }

	
    public void setCardService(ICardService cardService) {
    	this.cardService = cardService;
    }

	
    public List<Integer> getIds() {
    	return ids;
    }

	
    public void setIds(List<Integer> ids) {
    	this.ids = ids;
    }

	
    public ParamdictionaryInfo getParamdictionaryInfo() {
    	return paramdictionaryInfo;
    }

	
    public void setParamdictionaryInfo(ParamdictionaryInfo paramdictionaryInfo) {
    	this.paramdictionaryInfo = paramdictionaryInfo;
    }
	
	
	
	
	
}
