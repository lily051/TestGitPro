/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月19日 下午7:45:06
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.common.cache;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.List;
import java.util.concurrent.ConcurrentHashMap;

import org.apache.log4j.Logger;

import com.hikvision.finance.fms.common.model.CacheData;
import com.hikvision.finance.fms.webdevice.datacollect.dao.IRecordsDao;
import com.hikvision.finance.fms.webdevice.datacollect.dto.BasicRecordInfo;
import com.hikvision.rpc.init.AppContext;


/**
 * <p>数据缓存</p>
 * @author jinchenwu 2016年4月19日 下午7:45:06
 * @version V1.0 
 */
public class DataCache {
	
	private static Logger logger = Logger.getLogger(DataCache.class);
	private static ConcurrentHashMap<String, CacheData> CACHE_MAP = new ConcurrentHashMap<String, CacheData>();
	
	/**
	 * 获取缓存的数据
	 * @author jinchenwu 2016年5月12日 下午7:13:53
	 * @param cacheName
	 * @param refreshCycle
	 * @return
	 */
	@SuppressWarnings("rawtypes")
	public synchronized static List getCache(String cacheName, long refreshCycle) {
		CacheData cacheData = CACHE_MAP.get(cacheName);
		long now = Calendar.getInstance().getTimeInMillis();
		// 如果缓存为空，或者缓存数据与数据库不一致并且缓存过期
		if (cacheData == null || (now - cacheData.getLastUpdateTime().getTimeInMillis() > refreshCycle)
		        || (now < cacheData.getLastUpdateTime().getTimeInMillis())) {
			IRecordsDao recordsDao = AppContext.getBean("recordsDao");
			if (null == recordsDao) {// Coverity要求判断null
				logger.error("datacollectDao为null");
				return new ArrayList<BasicRecordInfo>();
			}
			List dataList = recordsDao.getVideoList(null);
			cacheData = new CacheData();
			cacheData.setLastUpdateTime(Calendar.getInstance());
			cacheData.setDataList(dataList);
			CACHE_MAP.put(cacheName, cacheData);
		}
		return cacheData.getDataList();
	}
}
