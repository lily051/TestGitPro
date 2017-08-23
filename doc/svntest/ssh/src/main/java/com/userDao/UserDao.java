package com.userDao;

import com.userEntity.User;

import java.util.List;

/**
 * Created by churui on 2017/7/11.
 */
public interface UserDao {
    public String saveuser(User user);
    public List<User> findall(Integer nodeid);
    public void deleteuser(Integer id);
    public void updateuser(User user);
    public List<User> findall();
    public User finduserbyname(String name);
}
