package com.redis;

import redis.clients.jedis.Jedis;
import redis.clients.jedis.JedisPool;
import redis.clients.jedis.JedisPoolConfig;

/**
 * Created by churui on 2017/8/14.
 */
public class RedisUtil {

    private static JedisPool jedisPool;

    //每分钟允许最大访问次数
    private static int MAX_ALLOWED_TIMES = 60;

    static {
        String host = "10.16.37.12";
        int port = 6379;

        JedisPoolConfig poolConfig = new JedisPoolConfig();
        poolConfig.setMaxIdle(100);
        poolConfig.setMaxTotal(500);
        poolConfig.setMaxWaitMillis(20000);
        poolConfig.setTestOnBorrow(true);

        jedisPool = new JedisPool(poolConfig, host, port);
    }

    /**
     * 如果key不存在，设置值，如果key存在，覆盖值
     * @param key
     * @param value
     */
    public void set(String key, String value) {
        Jedis jedis = jedisPool.getResource();
        jedis.set(key, value);
        //用完后放回连接池
       returnResource(jedis);
    }
    public String get(String key) {
        Jedis jedis = jedisPool.getResource();
        try {
            jedis.get(key);
        } catch (Exception e) {
           e.printStackTrace();
        }
        returnResource(jedis);
        return jedis.get(key);

    }
    public void del(String key) {
        Jedis jedis = jedisPool.getResource();
        try {
            jedis.del(key);
        } catch (Exception e) {
            e.printStackTrace();
        }
        returnResource(jedis);
    }

    public void setex(String key,int seconds,String value){
        Jedis jedis = jedisPool.getResource();
        jedis.setex(key,seconds,value);
        returnResource(jedis);
    }
    public void returnResource(Jedis jedis){
        if (jedis != null) {
            jedis.close();
        }
    }

}
