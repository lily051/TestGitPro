package com.userDao;


import com.userEntity.Role;
import com.userEntity.Rolepre;
import com.userEntity.Roles;

import java.util.List;

/**
 * Created by churui on 2017/7/11.
 */
public interface RoleDao{
    //用户
    public void save(Role role);
    public Role findRole(String rolename);
    public List<Role> findbynodeid(Integer nodeid);
    public void deleterole(Integer roleid);
    public void update(Role role);
    public List<Role> findall();
    public Role findbyroleid(Integer roleid);
    public  List<Roles> findrolesbynode(Integer nodeid);

    //角色
    public void saveroles(Roles roles);
    public Roles findrolesbyname(String rname);
    public Roles findrolesbyrid(Integer rid);
    public void updateroles(Roles roles);
    public void deleteroles(Integer rid);
    public List<Role> findrolebyrid(Integer rid);
    public Roles findrolesbynodeidandname(Integer nodeid,String rname);

    //权限
    public void addrolepre(Rolepre rolepre);
    public  Rolepre findroleprebyrid(Integer rid);
    public void updaterolepre(Rolepre rolepre);
    public void deleterolepre(Integer rid);

    public void sysdate();
}
