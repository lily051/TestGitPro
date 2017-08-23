package com.userDao;

import com.userEntity.Subnode;

import java.util.List;

/**
 * Created by churui on 2017/7/29.
 */
public interface SubDao {
    public void addsub(Subnode node);
    public List<Subnode> findbynodeid(Integer nodeid);
}
