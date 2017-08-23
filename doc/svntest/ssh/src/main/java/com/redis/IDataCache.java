package com.redis;

import java.util.Collection;
import java.util.List;
import java.util.Map;

/**
 * Created by churui on 2017/7/24.
 */
public interface IDataCache {
    /**
     * @Title: get
     * @Description: 查找值。
     * @author dongying
     * @param key
     * @return String
     * @throws
     */
    public String get(final String key);

    /**
     * @Title: getList
     * @Description: 查询字符串集合的值。
     * @author dongying
     * @param key
     * @return 设定文件
     * @return List<String> 返回类型
     * @throws
     */
    public List<String> getList(String key);

    /**
     * @Title: get
     * @Description: 查找某个对象。
     * @author dongying
     * @param key
     * @param clazz
     * @return 设定文件
     * @return T 返回类型
     * @throws
     */
    public <T> T get(String key, Class<T> clazz);

    /**
     * @Title: get
     * @Description: 查询多个对象。
     * @author dongying
     * @param keys
     * @param clazz
     * @return 设定文件
     * @return HashMap<String, T> 返回类型
     * @throws
     */
    public <T> Map<String, T> mget(Collection<String> keys, Class<T> clazz);

    /**
     * @Title: set
     * @Description: 缓存一个有过期时间的字符串。
     * @author dongying
     * @param key
     * @param value
     * @param expire
     *            设定文件
     * @return void 返回类型
     * @throws
     */
    public void set(final String key, final String value, Integer expire);

    /**
     * @Title: set
     * @Description: 缓存一个有过期时间的对象。
     * @author dongying
     * @param key
     * @param value
     * @param expire
     *            设定文件
     * @return void 返回类型
     * @throws
     */
    public void set(final String key, final Object value, Integer expire);

    /**
     * @Title: delete
     * @Description: 删除一个值。
     * @author dongying
     * @param key
     *            设定文件
     * @return void 返回类型
     * @throws
     */
    public void delete(final String key);

    /**
     * @Title: set
     * @Description: 缓存一个map。
     * @author dongying
     * @param key
     * @param field
     * @param value
     *            设定文件
     * @return void 返回类型
     * @throws
     */
    public void setMap(final String key, final Map<String, String> value, Integer expire);

    /**
     * @Title: get
     * @Description: 从Map中获取一个值。
     * @author dongying
     * @param key
     * @param field
     * @param clazz
     * @return 设定文件
     * @return T 返回类型
     * @throws
     */
    public <T> T get(final String key, final String field, Class<T> clazz);

    /**
     * @Title: set
     * @Description: 向Map中添加一个键值。
     * @author dongying
     * @param key
     * @param field
     * @param value
     *            设定文件
     * @return void 返回类型
     * @throws
     */
    public void setToMap(final String key, final String field, final String value);

    /**
     * @Title: set
     * @Description: 向Map中添加一个键值。
     * @author dongying
     * @param key
     * @param field
     * @param value
     *            设定文件
     * @return void 返回类型
     * @throws
     */
    public void setToMap(final String key, final String field, final Object value);

    /**
     * @Title: deleteMapItem
     * @Description: 从Map中删除一个键。
     * @author dongying
     * @param key
     * @param field
     *            设定文件
     * @return void 返回类型
     * @throws
     */
    public void deleteMapItem(final String key, final String field);
}
