/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月13日 下午7:06:47
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.analysis.action;

import org.apache.struts2.ServletActionContext;

import com.hikvision.finance.fms.modules.analysis.dto.AnalysisInfo;
import com.hikvision.finance.fms.modules.analysis.service.IAnalysisService;
import com.hikvision.finance.fwork.exception.ExpectedException;
import com.hikvision.finance.fwork.www.action.BaseAction;


/**
 * <p>统计分析Action</p>
 * @author jinxindong 2016年4月13日 下午7:06:47
 * @version V1.0 
 */
public class AnalysisAction extends BaseAction<AnalysisInfo>{

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = -5837491004284338883L;
	private AnalysisInfo analysisInfo;
	private IAnalysisService analysisService;
	
	/**
	 * 跳到合规检查页面
	 * @author jinxindong 2016年4月18日 下午5:00:28
	 * @return
	 */
	public String toBusinessSts() {
		operPage = "/modules/analysis/page/businessSts.jsp";
		return DISPATCHER;
	}
	
	
	/**
	 * 获取统计数据
	 * @author jinxindong 2016年4月13日 下午7:47:37
	 * @return
	 */
	public String getOrdersNum() {
		analysisInfo = analysisService.getOrdersNum(analysisInfo);
		ajaxData.put("analysisInfo", analysisInfo);
		return AJAX;
	}
	
	
	/**
	 * 获取合规统计数据
	 * @author jinxindong 2016年4月13日 下午7:49:58
	 * @return
	 */
	public String getRateNum() {
		String method = ServletActionContext.getRequest().getMethod();
		if ("POST".equals(method)) {// AppScan扫描出“查询中接受的主体参数”缺陷，需要禁用GET方法
			analysisInfo = analysisService.getRateNum(analysisInfo);
			ajaxData.put("analysisInfo", analysisInfo);
		} else {
			throw new ExpectedException("", "请求方法错误");
		}
		return AJAX;
	}
	
	
	
	//*******************************getter/setter**************************************
	
    public AnalysisInfo getAnalysisInfo() {
    	return analysisInfo;
    }
	
    public void setAnalysisInfo(AnalysisInfo analysisInfo) {
    	this.analysisInfo = analysisInfo;
    }
	
    public IAnalysisService getAnalysisService() {
    	return analysisService;
    }
	
    public void setAnalysisService(IAnalysisService analysisService) {
    	this.analysisService = analysisService;
    }
	
	
}
