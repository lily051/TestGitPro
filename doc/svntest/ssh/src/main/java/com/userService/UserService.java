package com.userService;

import com.userEntity.User;

import java.util.List;

/**
 * Created by churui on 2017/7/11.
 */
public interface UserService {
    public String addUser(User user);
    public List<User> findallbyid(Integer nodeid);
    public void deleteUser(Integer id);
    public void updateUser(User user);
    public List<User> findall();
    public User finduserbyname(String name);
}
