package com.redis;


import com.common.ConfigManager;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class CacheDbConfig implements ICacheConfig {
	private static final Logger logger = LoggerFactory.getLogger(CacheDbConfig.class);

//	@Autowired(required = false)
//	private IServerAddrDAO<ServerAddrTbl> serverAddrDAOImpl;

	@Override
	public BaseConfiguration getConfig() {
		BaseConfiguration config = new BaseConfiguration();
		try {
			config.setTimeout(Integer.parseInt(ConfigManager.getConfiguration("cache", "cache.timeout")));
			config.setMaxTotal(Integer.parseInt(ConfigManager.getConfiguration("cache", "cache.pool.max-total")));
			config.setMaxIdle(Integer.parseInt(ConfigManager.getConfiguration("cache", "cache.pool.max-idle")));
			config.setMaxWaitMillis(Integer.parseInt(ConfigManager.getConfiguration("cache", "cache.pool.max-wait-millis")));
			config.setTestOnBorrow(Boolean.parseBoolean(ConfigManager.getConfiguration("cache", "cache.pool.test-on-brrow")));
			config.setTestOnReturn(Boolean.parseBoolean(ConfigManager.getConfiguration("cache", "cache.pool.test-on-return")));
		} catch (Exception e) {
			logger.error("读取缓存服务器配置文件失败。" + e.toString());
		}

//		List<ServerAddrTbl> serverAddrtblList = serverAddrDAOImpl.findByServerTypeAndInternalRoute(ConstParameter.SERVER_TYPE_CACHE);
//		if (serverAddrtblList == null || serverAddrtblList.isEmpty()) {
//			logger.error("找不到缓存服务器。");
//			return null;
//		}
//		config.setServerIp(serverAddrtblList.get(0).getServerip());
//		config.setPort(serverAddrtblList.get(0).getServerport());
		return config;
	}
}
