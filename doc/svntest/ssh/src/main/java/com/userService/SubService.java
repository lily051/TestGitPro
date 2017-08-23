package com.userService;

import com.userEntity.Subnode;

import java.util.List;

/**
 * Created by churui on 2017/7/29.
 */
public interface SubService {
    public void addSub(Subnode subnode);
    public List<Subnode> findbynodeid(Integer nodeid);
}
