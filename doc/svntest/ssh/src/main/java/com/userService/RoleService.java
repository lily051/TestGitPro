package com.userService;

import com.userEntity.Role;
import com.userEntity.Rolepre;
import com.userEntity.Roles;

import java.util.List;

/**
 * Created by churui on 2017/7/11.
 */
public interface RoleService {
    //添加用户
    public void addRole(Role role);
    public Role findRole(String rolename);
    public Role login(Role role,String rolename,String rolepwd,String codevalue,String codevalidate,String ip);
    public List<Role> findbynode(Integer nodeid);
    public void deleterole(Integer roleid);
    public void updaterole(Role role);
    public List<Role> findall();
    public Role findbyroleid(Integer roleid);
    public List<Roles> findrolesbynode(Integer nodeid);

    //添加角色
    public void addRoles(Roles roles);
    public Roles findrolesbyname(String rname);
    public Roles findrolesbyrid(Integer rid);
    public void updateroles(Roles roles);
    public void deleteroles(Integer rid);
    public List<Role> findrolebyrid(Integer rid);
    public Roles findrolesbynodeidandname(Integer nodeid,String rname);

    //添加权限
    public void addrolepre(Rolepre rolepre);
    public Rolepre findroleprebyrid(Integer rid);
    public void updaterolepre(Rolepre rolepre);
    public void deleterolepre(Integer rid);

}
