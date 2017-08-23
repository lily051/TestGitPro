package com.userService;

import com.userDao.SubDao;
import com.userDao.UserDao;
import com.userEntity.Subnode;
import org.springframework.stereotype.Service;

import javax.annotation.Resource;
import java.util.List;

/**
 * Created by churui on 2017/7/29.
 */
@Service("subService")
public class SubServiceImpl implements SubService{
    @Resource(name = "subDao")
    private SubDao subDao;
    @Override
    public void addSub(Subnode subnode) {
        subDao.addsub(subnode);
    }

    @Override
    public List<Subnode> findbynodeid(Integer nodeid) {
        return subDao.findbynodeid(nodeid);
    }

    public SubDao getSubDao() {
        return subDao;
    }

    public void setSubDao(SubDao subDao) {
        this.subDao = subDao;
    }
}
