package com.redis;

import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import redis.clients.jedis.ShardedJedis;

/**
 * Created by churui on 2017/8/14.
 */
public class RedisClientTemplate {

    private static final org.slf4j.Logger log = LoggerFactory.getLogger(RedisClientTemplate.class);

    @Autowired
    private RedisDataSource redisDataSource;

    public RedisDataSource getRedisDataSource() {
        return redisDataSource;
    }

    public void setRedisDataSource(RedisDataSource redisDataSource) {
        this.redisDataSource = redisDataSource;
    }

    public void disconnect() {
        ShardedJedis shardedJedis = redisDataSource.getRedisClient();
        shardedJedis.disconnect();
    }
    /**
     * 将值 value 关联到 key ，并将 key 的生存时间设为 seconds (以秒为单位)
     * @param key
     * @param seconds
     * @param value
     * @return
     */
    public String setex(String key, int seconds, String value) {
        String result = null;
        ShardedJedis shardedJedis = redisDataSource.getRedisClient();
        if (shardedJedis == null) {
            return result;
        }
        boolean broken = false;
        try {
            result = shardedJedis.setex(key, seconds, value);

        } catch (Exception e) {
            log.error(e.getMessage(), e);
            broken = true;
        } finally {
            redisDataSource.returnResource(shardedJedis, broken);
        }
        return result;
    }
    public String set(String key, String value){
        String result=null;
        ShardedJedis shardedJedis=redisDataSource.getRedisClient();
        if(shardedJedis==null){
            return result;
        }
        boolean broken=false;
        try {
            result=shardedJedis.set(key, value);
        } catch (Exception e) {
            broken=true;
            e.printStackTrace();
        }finally{
            redisDataSource.returnResource(shardedJedis, broken);
        }

        return result;
    }

    /**
     * @Description: 获取redis单个值
     * @author Mr.chen
     * @date 2016-10-21 下午04:40:57
     */
    public String get(String key) {
        String result = null;
        ShardedJedis shardedJedis = redisDataSource.getRedisClient();
        if (shardedJedis == null) {
            return result;
        }
        boolean broken = false;
        try {
            result = shardedJedis.get(key);

        } catch (Exception e) {
            log.error(e.getMessage(), e);
            broken = true;
        } finally {
            redisDataSource.returnResource(shardedJedis, broken);
        }
        return result;
    }

}