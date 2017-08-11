/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月7日 下午6:51:32
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.sale.service;

import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.service.IBaseService;
import com.hikvision.finance.fms.model.Clientinput;
import com.hikvision.finance.fms.modules.sale.dto.SaleInfo;
import com.hikvision.finance.fms.modules.sale.qo.SaleQo;

/**
 * @author jinxindong 2016年4月7日 下午6:51:32
 * @version V1.0
 */
public interface ISaleService extends IBaseService<Clientinput, SaleQo> {
	/**
	 * 获取销售查询数据页面
	 * @author jinxindong 2016年4月8日 上午9:32:50
	 * @param limit
	 * @param start
	 * @param saleInfo
	 * @return
	 */
	public Page getSaleDataPage(Integer limit, Integer start, SaleInfo saleInfo);
	
	/**
	 * 查询业务单详细信息
	 * @author jinxindong 2016年5月10日 下午2:56:15
	 * @param id
	 * @return
	 */
	public SaleInfo getBizOrderInfo(SaleInfo saleInfo);
	
	/**
	 * 销售查询导出表格
	 * @author jinxindong 2016年4月8日 上午10:39:04
	 * @param saleInfo
	 * @param titleName
	 * @param attrName
	 * @param path
	 * @return
	 */
	public boolean exportSaleExcel(SaleInfo saleInfo, String[] titleName, String[] attrName, String path);
}
