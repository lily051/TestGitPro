package com.redis;

import org.apache.log4j.Logger;
import redis.clients.jedis.Jedis;
import redis.clients.jedis.JedisPool;
import redis.clients.jedis.JedisPoolConfig;

public class RedisPoolManager {

    public static Logger log = Logger.getLogger(RedisPoolManager.class);

    private static JedisPool jedisPool = null;

    /**
     * @Title: createPool
     * @Description: 初始化JedisPool。
     * @author dongying 设定文件
     * @return void 返回类型
     * @throws
     */
    public synchronized static void createPool() {
        try {
            ICacheConfig cacheConnect = AppContext.getBean("cacheDbConfig");
            BaseConfiguration config = cacheConnect.getConfig();
            JedisPoolConfig jedisPoolConfig = new JedisPoolConfig();
            jedisPoolConfig.setMaxTotal(config.getMaxTotal());
            jedisPoolConfig.setMaxIdle(config.getMaxIdle());
            jedisPoolConfig.setMaxWaitMillis(config.getMaxWaitMillis());
            jedisPoolConfig.setTestOnBorrow(config.getTestOnBorrow());
            jedisPoolConfig.setTestOnReturn(config.getTestOnReturn());
            jedisPool = new JedisPool(jedisPoolConfig, config.getServerIp(), config.getPort(), config.getTimeout());

        } catch (Exception e) {
            log.error(e);
        }
    }

    /**
     * 返回一个实例
     * 
     * @return ShardedJedis
     * @exception
     * @since 1.0.0
     */
    public synchronized static Jedis getJedis() {
        try {
            if (jedisPool == null) {
                createPool();
            }
            Jedis resource = jedisPool.getResource();
            return resource;
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    /**
     * 释放一个实例
     * 
     * @param jedis
     *            void
     * @exception
     * @since 1.0.0
     */
    public static void returnResource(final Jedis jedis) {
        if (jedis != null) {
            jedis.close();
        }
    }
}
