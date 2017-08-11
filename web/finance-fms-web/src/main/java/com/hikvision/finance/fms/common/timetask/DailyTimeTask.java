/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月14日 下午4:06:58
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.common.timetask;

import org.apache.log4j.Logger;

import com.hikvision.finance.fms.common.cache.HeartbeatCache;
import com.hikvision.finance.fms.modules.log.service.ILogService;
import com.hikvision.finance.fms.webdevice.datacollect.service.IDatacollectService;
import com.hikvision.finance.fwork.core.task.AbstractTaskEntity;


/**
 * <p>每天夜里执行一次的定时任务</p>
 * @author jinchenwu 2016年4月14日 下午4:06:58
 * @version V1.0 
 */
public class DailyTimeTask extends AbstractTaskEntity {
	
	private static Logger logger = Logger.getLogger(DailyTimeTask.class);
	
    private ILogService logService;
	private IDatacollectService datacollectService;// 用AppContext.getBean("datacollectService")的话Coverity会提示判断null
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fwork.core.task.AbstractTaskEntity#run()
	 */
	@Override
	public void run() {
		logger.info("开始删除日志信息");
		deleteLog();
		logger.info("结束删除日志信息，开始删除录像信息");
		deleteRecord();
		logger.info("结束删除录像信息，开始删除心跳缓存");
		HeartbeatCache.removeHeartBeat();
		logger.info("结束删除心跳缓存, 开始删除录像信息(用户删除掉, 录像上传失败)");
		deleteFailRecord();
	}
	
	/**
	 * 删除日志
	 * @author jinchenwu 2016年4月15日 下午2:49:53
	 */
	private void deleteLog() {
		logService.deleteLog();
	}
	
	/**
	 * 删除录像
	 * @author jinchenwu 2016年4月28日 上午9:35:06
	 */
	private void deleteRecord(){
		datacollectService.deleteDailyRecord();
	}

	/**
	 * 删除录像信息(用户删除掉, 录像上传失败)
	 * @author zhoujiajun 2016年11月26日 下午1:34:48
	 */
	private void deleteFailRecord(){
		datacollectService.deleteFailRecord();
	}
	
	public ILogService getLogService() {
		return logService;
	}
	
	public void setLogService(ILogService logService) {
		this.logService = logService;
	}
	
	public IDatacollectService getDatacollectService() {
		return datacollectService;
	}
	
	public void setDatacollectService(IDatacollectService datacollectService) {
		this.datacollectService = datacollectService;
	}
	
}
