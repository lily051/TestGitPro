package com.redis;

import com.alibaba.fastjson.JSONArray;
import com.alibaba.fastjson.JSONObject;
import redis.clients.jedis.Jedis;

import java.util.*;

public final class RedisCache implements IDataCache {

    /*
     * (非 Javadoc) <p>Title: get</p> <p>Description: </p>
     * 
     * @param key
     * 
     * @return
     * 
     * @see com.hikvision.www.cache.IDataCache#get(java.lang.String)
     */
    @Override
    public String get(String key) {
        Jedis jedis = null;
        try {
            jedis = RedisPoolManager.getJedis();
            return jedis.get(key);
        } finally {
            RedisPoolManager.returnResource(jedis);
        }
    }

    /*
     * (非 Javadoc) <p>Title: get</p> <p>Description: </p>
     * 
     * @param key
     * 
     * @param clazz
     * 
     * @return
     * 
     * @see com.hikvision.www.cache.IDataCache#get(java.lang.String, java.lang.Class)
     */
    @Override
    public <T> T get(String key, Class<T> clazz) {
        Jedis jedis = null;
        try {
            jedis = RedisPoolManager.getJedis();
            String value = jedis.get(key);
            JSONObject jsonObject = JSONObject.parseObject(value);
            return (T) JSONObject.toJavaObject(jsonObject, clazz);
        } finally {
            RedisPoolManager.returnResource(jedis);
        }

    }

    /*
     * (非 Javadoc) <p>Title: getList</p> <p>Description: </p>
     * 
     * @param key
     * 
     * @return
     * 
     * @see com.hikvision.www.cache.IDataCache#getList(java.lang.String)
     */
    public List<String> getList(String key) {

        Jedis jedis = null;
        try {
            jedis = RedisPoolManager.getJedis();
            List<String> list = new ArrayList<String>();
            String value = jedis.get(key);
            if (value != null) {
                JSONArray jsonArray = JSONArray.parseArray(value);
                for (int i = 0; i < jsonArray.size(); i++) {
                    list.add(jsonArray.getString(i));
                }
                return list;
            } else {
                return null;
            }
        } finally {
            RedisPoolManager.returnResource(jedis);
        }
    }

    /*
     * (非 Javadoc) <p>Title: mget</p> <p>Description: </p>
     * 
     * @param keys
     * 
     * @param clazz
     * 
     * @return
     * 
     * @see com.hikvision.www.cache.IDataCache#mget(Collection, Class)
     */
    @Override
    public <T> Map<String, T> mget(Collection<String> keys, Class<T> clazz) {
        Jedis jedis = null;
        try {
            Map<String, T> map = new HashMap<String, T>();
            jedis = RedisPoolManager.getJedis();
            for (String key : keys) {
                String value = jedis.get(key);
                JSONObject jsonObject = JSONObject.parseObject(value);
                map.put(key, (T) JSONObject.toJavaObject(jsonObject, clazz));
            }
            return map;
        } finally {
            RedisPoolManager.returnResource(jedis);
        }
    }

    /*
     * (非 Javadoc) <p>Title: set</p> <p>Description: </p>
     * 
     * @param key
     * 
     * @param value
     * 
     * @param expire
     * 
     * @see com.hikvision.www.cache.IDataCache#set(java.lang.String, java.lang.String, java.lang.Integer)
     */
    @Override
    public void set(String key, String value, Integer expire) {
        Jedis jedis = null;
        try {
            jedis = RedisPoolManager.getJedis();
            jedis.setex(key, expire, value);
        } finally {
            RedisPoolManager.returnResource(jedis);
        }
    }

    /*
     * (非 Javadoc) <p>Title: set</p> <p>Description: </p>
     * 
     * @param key
     * 
     * @param value
     * 
     * @param expire
     * 
     * @see com.hikvision.www.cache.IDataCache#set(java.lang.String, java.lang.Object, java.lang.Integer)
     */
    @Override
    public void set(String key, Object value, Integer expire) {
        Jedis jedis = null;
        try {
            jedis = RedisPoolManager.getJedis();
            jedis.setex(key, expire, JSONObject.toJSONString(value));
        } finally {
            RedisPoolManager.returnResource(jedis);
        }
    }

    /*
     * (non-Javadoc)
     * 
     * @see com.hikvision.www.cache.IDataCache#delete(java.lang.String)
     */
    @Override
    public void delete(String key) {
        Jedis jedis = null;
        try {
            jedis = RedisPoolManager.getJedis();
            jedis.del(key);
        } finally {
            RedisPoolManager.returnResource(jedis);
        }
    }

    /*
     * (非 Javadoc) <p>Title: setMap</p> <p>Description: </p>
     * 
     * @param key
     * 
     * @param value
     * 
     * @see com.ivms.cache.IDataCache#setMap(java.lang.String, java.util.Map)
     */
    public void setMap(final String key, final Map<String, String> value, Integer expire) {
        Jedis jedis = null;
        try {
            jedis = RedisPoolManager.getJedis();
            jedis.hmset(key, value);
            jedis.expire(key, expire);
        } finally {
            RedisPoolManager.returnResource(jedis);
        }
    }

    /*
     * (非 Javadoc) <p>Title: get</p> <p>Description: </p>
     * 
     * @param key
     * 
     * @param field
     * 
     * @param clazz
     * 
     * @return
     * 
     * @see com.ivms.cache.IDataCache#get(java.lang.String, java.lang.String, java.lang.Class)
     */
    public <T> T get(final String key, final String field, Class<T> clazz) {
        Jedis jedis = null;
        try {
            jedis = RedisPoolManager.getJedis();
            String value = jedis.hget(key, field);
            JSONObject jsonObject = JSONObject.parseObject(value);
            return (T) JSONObject.toJavaObject(jsonObject, clazz);
        } finally {
            RedisPoolManager.returnResource(jedis);
        }
    }

    /*
     * (非 Javadoc) <p>Title: set</p> <p>Description: </p>
     * 
     * @param key
     * 
     * @param field
     * 
     * @param value
     * 
     * @param expire
     * 
     * @see com.ivms.cache.IDataCache#set(java.lang.String, java.lang.String, java.lang.String, java.lang.Integer)
     */
    @Override
    public void setToMap(String key, String field, String value) {
        Jedis jedis = null;
        try {
            jedis = RedisPoolManager.getJedis();
            jedis.hset(key, field, value);
        } finally {
            RedisPoolManager.returnResource(jedis);
        }
    }

    /*
     * (非 Javadoc) <p>Title: set</p> <p>Description: </p>
     * 
     * @param key
     * 
     * @param field
     * 
     * @param value
     * 
     * @param expire
     * 
     * @see com.ivms.cache.IDataCache#set(java.lang.String, java.lang.String, java.lang.Object, java.lang.Integer)
     */
    @Override
    public void setToMap(String key, String field, Object value) {
        Jedis jedis = null;
        try {
            jedis = RedisPoolManager.getJedis();
            jedis.hset(key, field, JSONObject.toJSONString(value));
        } finally {
            RedisPoolManager.returnResource(jedis);
        }
    }

    /*
     * (非 Javadoc) <p>Title: deleteMapItem</p> <p>Description: </p>
     * 
     * @param key
     * 
     * @param field
     * 
     * @see com.ivms.cache.IDataCache#deleteMapItem(java.lang.String, java.lang.String)
     */
    @Override
    public void deleteMapItem(String key, String field) {
        Jedis jedis = null;
        try {
            jedis = RedisPoolManager.getJedis();
            jedis.hdel(key, field);
        } finally {
            RedisPoolManager.returnResource(jedis);
        }
    }
}
