package com.redis;

import redis.clients.jedis.ShardedJedis;

/**
 * Created by churui on 2017/8/14.
 */
public interface RedisDataSource {

    public abstract ShardedJedis getRedisClient();

    public void returnResource(ShardedJedis shardedJedis);

    public void returnResource(ShardedJedis shardedJedis,boolean broken);

}
