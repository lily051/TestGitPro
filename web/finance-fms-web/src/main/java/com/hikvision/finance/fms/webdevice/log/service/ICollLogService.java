/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-4-17 下午4:45:14
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.webdevice.log.service;

import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.service.IBaseService;
import com.hikvision.finance.fms.model.Operlog;
import com.hikvision.finance.fms.webdevice.log.dto.OperlogInfo;
import com.hikvision.finance.fms.webdevice.log.qo.OperlogsQo;

/**
 * <p></p>
 * @author xujiangfei 2016-4-27 下午4:45:14
 * @version V1.0 
 */
public interface ICollLogService extends IBaseService<Operlog, OperlogsQo> {
	
	/**
	 * 获得理财操作日志的列表
	 * @author jinchenwu 2016年5月16日 下午7:05:45
	 * @param limit
	 * @param start
	 * @param operlogInfo
	 * @return
	 */
	Page getDataPage(int limit, int start, OperlogInfo operlogInfo);
	
}
