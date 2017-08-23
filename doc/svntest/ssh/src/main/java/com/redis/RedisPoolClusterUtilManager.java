package com.redis;

import org.apache.log4j.Logger;
import redis.clients.jedis.JedisPoolConfig;
import redis.clients.jedis.JedisShardInfo;
import redis.clients.jedis.ShardedJedis;
import redis.clients.jedis.ShardedJedisPool;

import java.io.InputStream;
import java.util.LinkedList;
import java.util.List;
import java.util.Properties;

public class RedisPoolClusterUtilManager {

    public static Logger log = Logger.getLogger(RedisPoolClusterUtilManager.class);

    private static ShardedJedisPool jedisPool = null;

    static {
        try {
            Properties p = new Properties();
            InputStream in = RedisPoolClusterUtilManager.class.getClassLoader().getResourceAsStream("system.properties");
            p.load(in);
            JedisPoolConfig config = new JedisPoolConfig();
            config.setMaxTotal(Integer.parseInt(p.getProperty("cache.pool.max-total")));
            config.setMaxIdle(Integer.parseInt(p.getProperty("cache.pool.max-idle")));
            config.setMaxWaitMillis(Integer.parseInt(p.getProperty("cache.pool.max-wait-millis")));
            config.setTestOnBorrow(Boolean.parseBoolean(p.getProperty("cache.pool.test-on-brrow")));
            config.setTestOnReturn(Boolean.parseBoolean(p.getProperty("cache.pool.test-on-return")));

            JedisShardInfo jedisShardInfo1 = new JedisShardInfo(p.getProperty("cache.1.server"), Integer.valueOf(p.getProperty("cache.1.port")));
            JedisShardInfo jedisShardInfo2 = new JedisShardInfo(p.getProperty("cache.2.server"), Integer.valueOf(p.getProperty("cache.2.port")));
            JedisShardInfo jedisShardInfo3 = new JedisShardInfo(p.getProperty("cache.3.server"), Integer.valueOf(p.getProperty("cache.3.port")));

            List<JedisShardInfo> list = new LinkedList<JedisShardInfo>();
            list.add(jedisShardInfo1);
            list.add(jedisShardInfo2);
            list.add(jedisShardInfo3);
            jedisPool = new ShardedJedisPool(config, list);
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
    public synchronized static ShardedJedis getJedis() {
        try {
            if (jedisPool != null) {
                ShardedJedis resource = jedisPool.getResource();
                return resource;
            } else {
                return null;
            }
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
    public static void returnResource(final ShardedJedis jedis) {
        if (jedis != null) {
            jedis.close();
        }
    }
}
