/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年5月13日 下午3:21:55
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.common.cache;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.hikvision.cms.cache.core.common.CacheConstants.DATA_TYPE;
import com.hikvision.cms.cache.core.memory.SimpleMemoryCache;
import com.hikvision.cms.cache.manager.DataLoadException;
import com.hikvision.cms.cache.manager.IDataStore;
import com.hikvision.cms.cache.manager.MemoryStore;
import com.hikvision.finance.core.util.CollectionUtils;
import com.hikvision.finance.core.util.StringUtils;
import com.hikvision.finance.fms.common.util.Constants.SessionAttrbutes;

/**
 * <p>
 * </p>
 * @author jinxindong 2016年5月13日 下午3:21:55
 * @version V1.0
 */
public class FmsMemoryStore implements IDataStore {
	
	private Logger log = LoggerFactory.getLogger(MemoryStore.class);
	private static SimpleMemoryCache<String,TimeObject> dataStore = new SimpleMemoryCache<String,TimeObject>(10000);
	private static final long NEVER_EXPIRE = 30 * 24 * 60* 60 * 1000L;
	private static long checkTime = 0;
	private static final long CHECK_PERIOD = 1000 * 30;

	private boolean isNeedCheck(){
		long now = System.currentTimeMillis();
		if(now - checkTime > CHECK_PERIOD){
			log.info("start clear expire data.");
			ExpireDataCleaner cleaner = new ExpireDataCleaner();
			cleaner.start();
			checkTime = now;
			return true;
		}
		return false;
	}
	
	@Override
    public String get(String key) throws DataLoadException {
		isNeedCheck();
		
	    TimeObject value = dataStore.get(key);
		if(value == null || value.isExpire()){
			return null;
		}
		return String.valueOf(value.getData());
    }


	@Override
    public boolean set(String key, String value) throws DataLoadException {
		 setObject(key, value);
	     return true;
    }


	@Override
    public boolean set(String key, String value, long exp) throws DataLoadException {
		setObject(key, value, exp);
	     return true;
    }


	@Override
    public boolean remove(String key){
	    dataStore.remove(key);
	    return true;
    }


	@Override
    public String get(String key, DATA_TYPE type) throws DataLoadException {
	    return get(key);
    }


	@Override
    public boolean remove(String key, DATA_TYPE type) throws DataLoadException {
	    return remove(key);
    }


	@Override
    public boolean set(String key, String value, DATA_TYPE type) throws DataLoadException {
	    return set(key,value);
    }


	@Override
    public boolean set(String key, String value, long exp, DATA_TYPE type) throws DataLoadException {
	    return set(key,value,exp);
    }


	@Override
    public Object getObject(String key) throws DataLoadException {
		isNeedCheck();
		
		TimeObject value = dataStore.get(key);
		if(value == null || value.isExpire()){
			return null;
		}
		return value.getData();
    }
	

	@Override
    public boolean setObject(String key, Object value) throws DataLoadException {
		TimeObject data = new TimeObject(value, NEVER_EXPIRE);
		long now = System.currentTimeMillis();
		data.setUpdateTime(now);
		dataStore.put(key, data);
	    return true;
    }


	@Override
    public boolean setObject(String key, Object value, long exp) throws DataLoadException {
		TimeObject data = new TimeObject(value, exp);
		long now = System.currentTimeMillis();
		data.setUpdateTime(now);
		dataStore.put(key, data);
	    return true;
    }


	@Override
    public boolean setObject(String key, Object value, long exp, DATA_TYPE type) throws DataLoadException {
		TimeObject data = new TimeObject(value, exp);
		long now = System.currentTimeMillis();
		data.setUpdateTime(now);
		dataStore.put(key, data);
	    return true;
    }


	@Override
    public Object getObject(String key, DATA_TYPE type) throws DataLoadException {
		return getObject(key);
    }


	@Override
    public boolean setObject(String key, Object value, DATA_TYPE type) throws DataLoadException {
	    return setObject(key, value);
    }
	
	class TimeObject{
		private long updateTime = -1;
		private long expire = 0;
		private Object data;
		
		public boolean isExpire(){
			long now = System.currentTimeMillis();
			if(now > expire + updateTime){
				return true;
			}
			return false;
		}
		
		public TimeObject(Object data){
			this.data = data;
		}
		
		public TimeObject(Object data,long expire){
			this.data = data;
			this.expire = expire;
		}
		
        public long getUpdateTime() {
        	return updateTime;
        }
		
        public void setUpdateTime(long updateTime) {
        	this.updateTime = updateTime;
        }
		
        public long getExpire() {
        	return expire;
        }
		
        public void setExpire(long expire) {
        	this.expire = expire;
        }
		
        public Object getData() {
        	return data;
        }
		
        public void setData(Object data) {
        	this.data = data;
        }

		@Override
        public String toString() {
	        return String.valueOf(data);
        }
		
	}
	
	class ExpireDataCleaner extends Thread{
		
		private void putExpireData2List(Map<String,TimeObject> dataMap,List<String> container){
			if(dataMap == null || dataMap.size() < 1){
				return;
			}
			for(Map.Entry<String, TimeObject>entry : dataMap.entrySet()){
				String key = entry.getKey();
				TimeObject to = entry.getValue();
				if(to.isExpire()){
					if(log.isDebugEnabled()){
						log.debug("the value [{}] is expired",new Object[]{key});
					}
					container.add(key);
				}
			}
		}
		
		@Override
        public void run() {//比较过期时间，进行处理
			List<String> container = new ArrayList<String>();
			Map<String,TimeObject> edenMap = dataStore.getEdenMap();
			Map<String,TimeObject> longTermMap = dataStore.getLongTermMap();
			putExpireData2List(edenMap,container);
			putExpireData2List(longTermMap,container);
			if(!CollectionUtils.isEmpty(container)){
				for(String key : container){
					if(StringUtils.startsWith(key, SessionAttrbutes.SESSIONPRX)){
						
						log.error("*********************"+key+"执行用户离线操作*************************");
					}
					dataStore.remove(key);
				}
			}
        }
	}
}
