/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-4-17 下午4:45:14
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.webdevice.contact.service;

import java.util.List;

import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.service.IBaseService;
import com.hikvision.finance.fms.model.Businessorder;
import com.hikvision.finance.fms.model.Clientinput;
import com.hikvision.finance.fms.model.Paramdictionary;
import com.hikvision.finance.fms.webdevice.contact.dto.BasicProductInfo;
import com.hikvision.finance.fms.webdevice.contact.dto.OrderEnableInfo;
import com.hikvision.finance.fms.webdevice.contact.dto.PrintOrderInfo;
import com.hikvision.finance.fms.webdevice.contact.dto.ProductCodeInfo;
import com.hikvision.finance.fms.webdevice.contact.dto.RealOrdersInfo;
import com.hikvision.finance.fms.webdevice.contact.qo.BusinessOrderQo;
import com.hikvision.finance.fwork.util.AjaxData;

/**
 * <p></p>
 * @author xujiangfei 2016-4-17 下午4:45:14
 * @version V1.0 
 */
public interface IContactService extends IBaseService<Businessorder,BusinessOrderQo>{

	/**
	 * 获取业务单，包括签约单和投诉单，关联时使用，获取OSD使用
	 * @author jinchenwu 2016年5月9日 下午4:34:47
	 * @return
	 */
	public Businessorder getOrder();
	
	/**
	 * 获取话术模板
	 * @author jinchenwu 2016年4月22日 下午4:11:58
	 * @return
	 */
	public String getWordTemplate(int id);
	
	/**
	 * 获取录入的签约单，打开签约单录单页面时使用
	 * @author jinchenwu 2016年5月9日 下午1:40:15
	 * @return
	 */
	public Businessorder getSignOrder();
	
	/**
	 * 获取业务单使能状态
	 * @author jinchenwu 2016年5月20日 下午3:35:20
	 * @return
	 */
	public OrderEnableInfo getOrderEnable();
	
	/**
	 * 理财经理获取有权限的产品编码
	 * @author jinchenwu 2016年4月23日 上午10:40:59
	 * @return
	 */
    public List<ProductCodeInfo> getPermitProductCodeList();
    
	/**
	 * 理财经理选择产品编码后获取基本产品信息
	 * @author jinchenwu 2016年4月23日 下午12:13:59
	 * @param id
	 * @return
	 */
	public BasicProductInfo getBasicProductbyId(int id);
	
	/**
	 * 理财经理获取证件类型
	 * @author jinchenwu 2016年4月23日 下午12:51:32
	 * @return
	 */
	public List<Paramdictionary> getCardList();
	
	/**
	 * 添加签约单
	 * @author xujiangfei 2016-4-18 下午4:12:11
	 * @param businessorder
	 * @return
	 */
	AjaxData addSignOrder(Businessorder businessorder);
	
	/**
	 * 录入投诉单和选择历史业务单时获取历史业务单
	 * @author xujiangfei 2016-4-19 下午5:03:41
	 * @param limit
	 * @param start
	 * @param userid
	 * @return
	 */
	Page getDataPage(int limit, int start, Clientinput clientinput, Integer type);
	
	/**添加投诉单
	 * @author xujiangfei 2016-4-21 下午1:51:11
	 * @param businessorder
	 * @return
	 */
	AjaxData addcomplainOrder(Businessorder businessorder);
	
	/**
	 * 关联业务单并存储（新签约单or投诉单）
	 * @author jinchenwu 2016年5月10日 上午9:50:52
	 * @param recordid
	 * @return
	 */
	AjaxData saveRelateRecords(Integer recordid);
	
	/**
	 * 关联历史业务单并存储
	 * @author jinchenwu 2016年5月10日 上午9:53:25
	 * @param recordsid
	 * @param clientinputid
	 * @return
	 */
	AjaxData saveHistoryOrder(Integer recordsid, Integer clientinputid);
	
	/**
	 * 数据采集中的打印
	 * @author xujiangfei 2016-4-25 下午6:16:28
	 * @param recordid 
	 * @return
	 */
	public PrintOrderInfo getPrintInfo(Integer recordid);
	
	/**
	 * 业务关联中的打印
	 * @author jinchenwu 2016年5月12日 上午10:49:18
	 * @param id
	 * @return
	 */
	public PrintOrderInfo getPermPrintInfo(int id);
	
	/**
	 * 获取录像信息和业务信息分页数据
	 * @author jinchenwu 2016年5月10日 下午7:00:56
	 * @param limit
	 * @param start
	 * @param realOrdersInfo
	 * @return
	 */
	Page getRecordsDataPage(int limit, int start, RealOrdersInfo realOrdersInfo);
	
	/**
	 * 获取编辑业务单信息
	 * @author jinchenwu 2016年5月10日 下午8:28:44
	 * @param id
	 * @param recordId 录像id
	 * @return
	 */
	public Businessorder getOrderEditInfo(int id, Integer recordId);
	
	/**
	 * 编辑业务单
	 * @author jinchenwu 2016年5月10日 下午9:02:08
	 * @param clientId
	 * @param businessorder
	 * @return
	 */
	public AjaxData editOrderInfo(int clientId, Businessorder businessorder, Integer recordId);
	
	/**
	 * 获取当前用户的业务单
	 * @author jinxindong 2016年5月25日 上午11:35:14
	 * @return
	 */
	public Businessorder getOrderForTip();
	
	/**
	 * 编辑历史录像记录
	 * @author zhengquanwu 2016年10月20日 下午8:33:56
	 * @param prosaverecord
	 */
	public AjaxData updateProsaverecord(PrintOrderInfo printOrderInfo);

}
