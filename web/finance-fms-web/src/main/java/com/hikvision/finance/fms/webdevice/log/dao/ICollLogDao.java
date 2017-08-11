/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-4-28 下午1:32:04
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.webdevice.log.dao;

import java.util.Date;

import com.hikvision.finance.core.util.bean.Page;
import com.hikvision.finance.extend.hibernate.dao.IBaseDao;
import com.hikvision.finance.fms.model.Operlog;
import com.hikvision.finance.fms.webdevice.log.dto.OperlogInfo;
import com.hikvision.finance.fms.webdevice.log.qo.OperlogsQo;

/**
 * <p></p>
 * @author xujiangfei 2016-4-28 下午1:32:04
 * @version V1.0 
 */
public interface ICollLogDao extends IBaseDao<Operlog, OperlogsQo> {
	
	/**
	 * 获得采集操作日志
	 * @author jinchenwu 2016年5月16日 下午7:17:30
	 * @param limit
	 * @param start
	 * @param operlogInfo
	 * @return
	 */
	Page getDataPage(int limit, int start, OperlogInfo operlogInfo);
	
	/**
	 * 记录BS客户端操作日志
	 * @author jinchenwu 2016年5月16日 下午6:24:06
	 * @param operType
	 * @param note
	 */
	void writeOperLog(int operType, String note);
	
	/**
	 * 删除操作日志
	 * @author jinchenwu 2016年5月7日 上午10:02:23
	 * @param thresholdDate
	 */
	public void deleteLog(Date thresholdDate);
	
	/**
	 * 针对于cs客户端不能取到sessionId, 而从手动添加userId，记录CS客户端操作日志
	 * @author zhoujiajun 2016年10月20日 下午12:23:45
	 * @param operType
	 * @param note
	 * @param nUserId 
	 */
	public void writeOperLog(int operType, String note, Integer nUserId);
}
