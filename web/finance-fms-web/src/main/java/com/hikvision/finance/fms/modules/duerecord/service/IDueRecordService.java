/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月7日 下午7:40:43
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.duerecord.service;

import java.util.List;

import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.service.IBaseService;
import com.hikvision.finance.fms.model.Clientinput;
import com.hikvision.finance.fms.modules.duerecord.dto.DueRecordInfo;
import com.hikvision.finance.fms.modules.sale.qo.SaleQo;


/**
 * <p>到期信息模块</p>
 * @author jinchenwu 2016年4月7日 下午7:40:43
 * @version V1.0 
 */
public interface IDueRecordService extends IBaseService<Clientinput, SaleQo> {
	
	/**
	 * 获取到期信息分页数据
	 * @author jinchenwu 2016年4月7日 下午8:14:14
	 * @param dueRecordInfo
	 * @param start
	 * @param limit
	 * @return
	 */
	public Page getDataPage(DueRecordInfo dueRecordInfo, Integer start, Integer limit);
	
	/**
	 * 导出数据
	 * @author jinchenwu 2016年4月13日 上午9:52:48
	 * @param dueRecordInfo
	 * @param string
	 * @return
	 */
	public boolean saveLogAndexportExcel(DueRecordInfo dueRecordInfo, String string);
	
	/**
	 * 设置录像延迟删除天数
	 * @author jinchenwu 2016年4月12日 下午7:53:28
	 * @param dueRecordInfo
	 */
	public void saveDelayDays(DueRecordInfo dueRecordInfo);
	
	/**
	 * 设置录像已读
	 * @author jinchenwu 2016年4月12日 下午8:32:26
	 * @param dueRecordInfo
	 */
	public void saveReadStatus(DueRecordInfo dueRecordInfo);
	
	/**
	 * 首页获取到期信息
	 * @author jinchenwu 2016年4月19日 上午10:56:55
	 * @param limit
	 * @return
	 */
	public List<DueRecordInfo> getIndexDueRecord(Integer limit);
	
}
