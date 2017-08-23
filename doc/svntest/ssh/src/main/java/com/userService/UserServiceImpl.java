package com.userService;

import com.userDao.UserDao;
import com.userEntity.User;
import org.springframework.stereotype.Service;

import javax.annotation.Resource;
import java.util.List;

/**
 * Created by churui on 2017/7/11.
 */
@Service("userService")
class UserServiceImpl implements UserService{
    @Resource(name = "userDao")
    private UserDao userDao;

    public void setUserDao(UserDao userDao) {
        this.userDao = userDao;
    }

    @Override
    public String addUser(User user) {
        //System.out.println("begin Transaction");
        String msg = null;
        try{
            msg = userDao.saveuser(user);
        }catch (Exception e){
            e.printStackTrace();
        }
        System.out.println(msg);

        return msg;
    }

    @Override
    public List<User> findallbyid(Integer nodeid) {
        return userDao.findall(nodeid);
    }

    @Override
    public void deleteUser(Integer id) {
        userDao.deleteuser(id);
    }

    @Override
    public void updateUser(User user) {
        userDao.updateuser(user);
    }

    @Override
    public List<User> findall() {
        return userDao.findall();
    }

    @Override
    public User finduserbyname(String name) {
        return userDao.finduserbyname(name);
    }
}
