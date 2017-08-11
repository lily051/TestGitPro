/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月7日 下午7:39:36
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.duerecord.action;

import org.apache.struts2.ServletActionContext;

import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.model.Prosaverecord;
import com.hikvision.finance.fms.modules.duerecord.dto.DueRecordInfo;
import com.hikvision.finance.fms.modules.duerecord.service.IDueRecordService;
import com.hikvision.finance.fms.modules.sale.dto.SaleInfo;
import com.hikvision.finance.fms.modules.sale.service.ISaleService;
import com.hikvision.finance.fwork.www.action.BaseAction;


/**
 * <p>到期信息模块</p>
 * @author jinchenwu 2016年4月7日 下午7:39:36
 * @version V1.0 
 */
public class DueRecordAction extends BaseAction<Prosaverecord> {
	
	/**
	 * 序列化ID
	 */
	private static final long serialVersionUID = 4229284714016930558L;
	private IDueRecordService dueRecordService;
	private ISaleService saleService;
	private DueRecordInfo dueRecordInfo;
	private SaleInfo saleInfo;
	
	// 获取区域树
	// web/tree/getRegionTree.action
	
	/**
	 * 获取到期信息列表
	 * @author jinchenwu 2016年4月7日 下午8:11:25
	 * @return
	 */
	public String getDataPage() {
		page = dueRecordService.getDataPage(dueRecordInfo, start, limit);
		return PAGE;
	}
		
	/**
	 * 获取业务单信息
	 * @author jinxindong 2016年5月10日 下午2:30:33
	 * @return
	 */
	public String getBizOrderInfo(){
		saleInfo = saleService.getBizOrderInfo(saleInfo);
		operPage = "/modules/sale/dialogs/viewBusiness.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 跳到延迟到期操作界面
	 * @author jinxindong 2016年6月16日 下午5:02:59
	 * @return
	 */
	public String todelayVideoPage(){
		operPage = "/modules/duerecord/dialogs/delayVideo.jsp";
		return DISPATCHER;
	}
	
	/**
	 * 设置录像延期天数
	 * @author jinchenwu 2016年4月12日 下午7:53:02
	 * @return
	 */
	public String setDelayDays(){
		dueRecordService.saveDelayDays(dueRecordInfo);
		return AJAX;
	}
	
	/**
	 * 标记为已读
	 * @author jinchenwu 2016年4月12日 下午8:50:36
	 * @return
	 */
	public String setReadStatus(){
		dueRecordService.saveReadStatus(dueRecordInfo);
		return AJAX;
	}
	
	/**
	 * 导出数据
	 * @author jinchenwu 2016年4月13日 上午9:43:32
	 * @return
	 */
	public String exportExcel(){
		String rootpath = ServletActionContext.getServletContext().getRealPath("/");
		String path = Constants.FILEUPLOAD + "excel/" + "DueRecords.xls";
		if(dueRecordService.saveLogAndexportExcel(dueRecordInfo, rootpath + path)) {
			ajaxData.put("url", "/" + path);
    		success = true;
		} else {
			msg = "导出失败";
    		success = false;
		}
		return AJAX;
	}
	
	// ******************************************getter/setter*************************************************
	public IDueRecordService getDueRecordService() {
		return dueRecordService;
	}
	
	public void setDueRecordService(IDueRecordService dueRecordService) {
		this.dueRecordService = dueRecordService;
	}
	
	public DueRecordInfo getDueRecordInfo() {
		return dueRecordInfo;
	}
	
	public void setDueRecordInfo(DueRecordInfo dueRecordInfo) {
		this.dueRecordInfo = dueRecordInfo;
	}
	
	public SaleInfo getSaleInfo() {
		return saleInfo;
	}
	
	public void setSaleInfo(SaleInfo saleInfo) {
		this.saleInfo = saleInfo;
	}
	
	public ISaleService getSaleService() {
		return saleService;
	}
	
	public void setSaleService(ISaleService saleService) {
		this.saleService = saleService;
	}
	
}
