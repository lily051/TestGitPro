/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-4-17 下午4:36:18
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.webdevice.contact.dao;

import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.hibernate.dao.IBaseDao;
import com.hikvision.finance.fms.model.Businessorder;
import com.hikvision.finance.fms.model.Clientinput;
import com.hikvision.finance.fms.webdevice.contact.dto.RealOrdersInfo;
import com.hikvision.finance.fms.webdevice.contact.qo.BusinessOrderQo;

/**
 * <p></p>
 * @author xujiangfei 2016-4-17 下午4:36:18
 * @version V1.0 
 */
public interface IBusinessOrderDao extends IBaseDao<Businessorder, BusinessOrderQo> {
	
	/**
	 * 录入投诉单和选择历史业务单时获取历史业务单
	 * @author jinchenwu 2016年5月17日 上午9:16:56
	 * @param limit
	 * @param start
	 * @param clientinput
	 * @return
	 */
	public Page getDataPage(int limit, int start, Clientinput clientinput, Integer type);
	
	/**
	 * 获取录像信息和业务信息分页数据
	 * @author jinchenwu 2016年5月17日 上午9:24:22
	 * @param limit
	 * @param start
	 * @param realOrdersInfo
	 * @return
	 */
	public Page getRecordsDataPage(int limit, int start, RealOrdersInfo realOrdersInfo);

}
