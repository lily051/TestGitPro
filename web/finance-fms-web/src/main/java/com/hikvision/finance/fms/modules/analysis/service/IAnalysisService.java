/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月13日 下午7:08:06
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.analysis.service;

import com.hikvision.finance.extend.service.IBaseService;
import com.hikvision.finance.fms.modules.analysis.dto.AnalysisInfo;
import com.hikvision.finance.fms.modules.analysis.qo.AnalysisQo;


/**
 * <p></p>
 * @author jinxindong 2016年4月13日 下午7:08:06
 * @version V1.0 
 */
public interface IAnalysisService extends IBaseService<AnalysisInfo,AnalysisQo>{
	
	/**
	 * 获取销售统计数据
	 * @author jinxindong 2016年4月13日 下午7:48:24
	 * @param analysisInfo
	 * @return
	 */
	public AnalysisInfo getOrdersNum(AnalysisInfo analysisInfo);
	
	
	/**
	 * 获取合规统计数据
	 * @author jinxindong 2016年4月13日 下午7:50:43
	 * @param analysisInfo
	 * @return
	 */
	public AnalysisInfo getRateNum(AnalysisInfo analysisInfo);
	
	/**
	 * 首页获取合规数据
	 * @author jinxindong 2016年4月19日 上午9:51:38
	 * @param analysisInfo
	 * @return
	 */
	public AnalysisInfo getIndexRateNum(AnalysisInfo analysisInfo);
}
