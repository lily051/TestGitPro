/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月7日 下午6:53:15
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.sale.action;

import org.apache.struts2.ServletActionContext;

import com.hikvision.finance.fms.common.util.Constants;
import com.hikvision.finance.fms.model.Clientinput;
import com.hikvision.finance.fms.modules.sale.dto.SaleInfo;
import com.hikvision.finance.fms.modules.sale.service.ISaleService;
import com.hikvision.finance.fwork.exception.ExpectedException;
import com.hikvision.finance.fwork.www.action.BaseAction;

/**
 * <p>销售查询Action</p>
 * @author jinxindong 2016年4月7日 下午6:53:15
 * @version V1.0
 */
public class SaleAction extends BaseAction<Clientinput> {
	
	/**
	 * 序列化ID
	 */
	private static final long serialVersionUID = 2057978255941854298L;
	private ISaleService saleService;
	private SaleInfo saleInfo;
	private Clientinput clientinput;
	
	/**
	 * 获取销售查询数据页面
	 * @author jinxindong 2016年4月8日 上午9:29:51
	 * @return
	 */
	public String getSaleDataPage() {
		String method = ServletActionContext.getRequest().getMethod();
		if ("POST".equals(method)) {// AppScan扫描出“查询中接受的主体参数”缺陷，需要禁用GET方法
			page = saleService.getSaleDataPage(limit, start, saleInfo);
		} else {
			throw new ExpectedException("", "请求方法错误");
		}
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
	 * 导出查询结果
	 * @author jinxindong 2016年4月8日 上午9:29:56
	 * @return
	 */
	public String exportSaleExcel() {
		String method = ServletActionContext.getRequest().getMethod();
		if ("POST".equals(method)) {// AppScan扫描出“查询中接受的主体参数”缺陷，需要禁用GET方法
			String[] titleNames = new String[] {"业务单号", "交易网点", "业务单状态", "业务单审批状态", "业务员工号", "业务员姓名", "办理时间", "产品类别", "产品编码",
			        "产品名称", "产品到期日", "产品保障期限", "产品发行机构", "产品风险等级", "视频保存时间", "客户姓名", "客户电话", "证件类别", "证件号", "客户银行卡号", "购买金额",
			        "备注"};
			
			String[] attrNames = new String[] {"nproId", "inputRegionName", "statusName", "strLockClient", "proUserNo",
			        "proName", "businessTimeString", "productTypeName", "productCode", "productName", "strEndDate",
			        "guarantee", "strGrade", "strRisklevel", "productSave", "name", "strCreditPhone", "creditCodeName",
			        "creditCode", "strBankNo", "strBuyMoney", "strRemarks"};
			String rootpath = ServletActionContext.getServletContext().getRealPath("/");
			String path = Constants.FILEUPLOAD + "excel/" + "saleQuery.xls";
			if (saleService.exportSaleExcel(saleInfo, titleNames, attrNames, rootpath + path)) {
				ajaxData.put("url", "/" + path);
				success = true;
			} else {
				msg = "导出失败";
				success = false;
			}
		} else {
			throw new ExpectedException("", "请求方法错误");
		}
		return AJAX;
	}
	
	
	//==========================getter、setter===========================================================
	
	public ISaleService getSaleService() {
		return saleService;
	}
	
	public void setSaleService(ISaleService saleService) {
		this.saleService = saleService;
	}
	
	public SaleInfo getSaleInfo() {
		return saleInfo;
	}
	
	public void setSaleInfo(SaleInfo saleInfo) {
		this.saleInfo = saleInfo;
	}

	
    public Clientinput getClientinput() {
    	return clientinput;
    }

	
    public void setClientinput(Clientinput clientinput) {
    	this.clientinput = clientinput;
    }
	
}
