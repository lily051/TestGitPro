/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年10月25日 下午5:20:11
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.common.timetask;

import java.util.List;

import org.apache.log4j.Logger;

import com.google.common.collect.Lists;
import com.hikvision.cms.cache.manager.DataLoadException;
import com.hikvision.cms.cache.manager.DataManager;
import com.hikvision.finance.fms.common.util.Constants.CacheKeyPrefix;
import com.hikvision.finance.fms.common.util.Constants.SysConfigType;
import com.hikvision.finance.fms.model.Sysconfig;
import com.hikvision.finance.fms.modules.sysconfig.service.ISysconfigService;
import com.hikvision.finance.fwork.core.task.AbstractTaskEntity;

/**
 * <p>
 * </p>
 * @author jinxindong 2016年10月25日 下午5:20:11
 * @version V1.0
 */
public class MemoryTimeTask extends AbstractTaskEntity {
	
	private static Logger logger = Logger.getLogger(MemoryTimeTask.class);
	
	private DataManager dataManager;
	private ISysconfigService sysconfigService;
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.fwork.core.task.AbstractTaskEntity#run()
	 */
	@Override
	public synchronized void run() {
		logger.info("开始缓存更新");
		setSysParam(dataManager, sysconfigService);
		logger.info("结束缓存更新");
	}
	
	/**
	 * 设置系统参数到缓存中
	 * @author jinxindong 2016年10月25日 下午5:26:01
	 * @param dataManager
	 * @param sysconfigService
	 */
	private void setSysParam(DataManager dataManager, ISysconfigService sysconfigService) {
		String prefix = CacheKeyPrefix.PREFIX_SYSCONFIG;
		List<Integer> nkeys = Lists.newArrayList(
				// 以下五个是平台参数
				SysConfigType.PLATFORM_DISCRIBE,
				SysConfigType.PLATFORM_COPYRIGHT,
		        SysConfigType.IMG_LOGO,
		        SysConfigType.IMG_LOGIN,
		        SysConfigType.IMG_BAR,
		        SysConfigType.PRODUCT_REVIEW,// 启用禁用产品复核
		        SysConfigType.OVERLAY_OSD,// 启用禁用OSD叠加
		        SysConfigType.MAXIMUM_VIDEO_LENGTH,// 最大录像时长 
		        SysConfigType.VIDEO_PLAYBACK_BANDWIDTH// 录像回放带宽
		);
		List<Sysconfig> sysparams = sysconfigService.getSysconfigByKey(nkeys);
		for (Sysconfig sysconfig : sysparams) {
			try {
				dataManager.set(prefix + sysconfig.getNkey(), sysconfig.getStrValue());
			} catch (DataLoadException e) {
				logger.error("connect to cache error.", e);
			}
		}
	}
	
	public DataManager getDataManager() {
		return dataManager;
	}
	
	public void setDataManager(DataManager dataManager) {
		this.dataManager = dataManager;
	}
	
	public ISysconfigService getSysconfigService() {
		return sysconfigService;
	}
	
	public void setSysconfigService(ISysconfigService sysconfigService) {
		this.sysconfigService = sysconfigService;
	}
	
}
