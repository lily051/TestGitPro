package com.userService;

import com.redis.IDataCache;
import com.userDao.RoleDao;
import com.userEntity.Role;
import com.userEntity.Rolepre;
import com.userEntity.Roles;
import org.springframework.stereotype.Service;

import javax.annotation.Resource;
import java.util.List;

/**
 * Created by churui on 2017/7/11.
 */
@Service("roleService")
public class RoleServiceImpl implements RoleService{
    @Resource(name = "roleDao")
    private RoleDao roleDao;
    private IDataCache dataCache;


    @Override
    public void addRole(Role role) {
        roleDao.save(role);
    }

    @Override
    public Role findRole(String rolename) {
        return roleDao.findRole(rolename);
    }

    @Override
    public Role login(Role role,String rolename, String rolepwd, String codevalue, String codevalidate,String ip) {
        return null;
    }

    @Override
    public List<Role> findbynode(Integer nodeid) {
        return roleDao.findbynodeid(nodeid);
    }

    @Override
    public void deleterole(Integer roleid) {
        roleDao.deleterole(roleid);
    }

    @Override
    public void updaterole(Role role) {
        roleDao.update(role);
    }

    @Override
    public List<Role> findall() {
        return roleDao.findall();
    }

    @Override
    public Role findbyroleid(Integer roleid) {
        return roleDao.findbyroleid(roleid);
    }

    @Override
    public List<Roles> findrolesbynode(Integer nodeid) {
        return roleDao.findrolesbynode(nodeid);
    }

    @Override
    public void addRoles(Roles roles) {
        roleDao.saveroles(roles);
    }

    @Override
    public Roles findrolesbyname(String rname) {
        return roleDao.findrolesbyname(rname);
    }

    @Override
    public Roles findrolesbyrid(Integer rid) {
        return roleDao.findrolesbyrid(rid);
    }

    @Override
    public void updateroles(Roles roles) {
        roleDao.updateroles(roles);
    }

    @Override
    public void deleteroles(Integer rid) {
        roleDao.deleteroles(rid);
    }

    @Override
    public List<Role> findrolebyrid(Integer rid) {
        return roleDao.findrolebyrid(rid);
    }

    @Override
    public Roles findrolesbynodeidandname(Integer nodeid, String rname) {
        return roleDao.findrolesbynodeidandname(nodeid,rname);
    }

    @Override
    public void deleterolepre(Integer rid) {
        roleDao.deleterolepre(rid);
    }

    @Override
    public void addrolepre(Rolepre rolepre) {
        roleDao.addrolepre(rolepre);
    }

    @Override
    public Rolepre findroleprebyrid(Integer rid) {
        return roleDao.findroleprebyrid(rid);
    }

    @Override
    public void updaterolepre(Rolepre rolepre) {
        roleDao.updaterolepre(rolepre);
    }


}
