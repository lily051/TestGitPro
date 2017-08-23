package com.userDao;

import java.io.Serializable;

/**
 * Created by churui on 2017/7/12.
 */
public interface BaseDao<T> {
    T get(Class<T> entity, Serializable id);
    Serializable save(T entity);
    void update(T entity);
    void delete(T entity);
}
