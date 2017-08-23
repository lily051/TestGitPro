package com.userEntity;


import javax.persistence.*;
import java.util.HashSet;
import java.util.Set;

/**
 * Created by churui on 2017/7/11.
 */

/**
 * 用户表
 */
@Entity
@Table(name = "roletbl")
public class Role implements java.io.Serializable{

    private static final long serialVersionUID = 1L;
    private Integer roleid;
    private String rolename;
    private String rolepwd;
    private String roledesc;
    private String remark;
    private Node nodetbl;
    private Roles roles;//角色

    public  Role(){

    }
    public Role(Integer roleid, String rolename, String rolepwd) {
        this.roleid = roleid;
        this.rolename = rolename;
        this.rolepwd = rolepwd;
    }
    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE, generator = "s_roletbl")
    @SequenceGenerator(name = "s_roletbl", sequenceName = "s_roletbl", allocationSize=1)
    @Column(name = "roleid", unique = true, nullable = false)
    public Integer getRoleid() {
        return roleid;
    }

    public void setRoleid(Integer roleid) {
        this.roleid = roleid;
    }
    @Column(name = "rolename",nullable= false, length = 64)
    public String getRolename() {
        return rolename;
    }

    public void setRolename(String rolename) {
        this.rolename = rolename;
    }

    @Column(name = "rolepwd", nullable = false, length = 64)
    public String getRolepwd() {
        return rolepwd;
    }

    public void setRolepwd(String rolepwd) {
        this.rolepwd = rolepwd;
    }

    @Column(name = "roledesc", nullable = false, length = 64)
    public String getRoledesc() {
        return roledesc;
    }
    public void setRoledesc(String roledesc) {
        this.roledesc = roledesc;
    }

    @Column(name = "remark", nullable = false, length = 64)
    public String getRemark() {
        return remark;
    }

    public void setRemark(String remark) {
        this.remark = remark;
    }
    @ManyToOne(fetch = FetchType.EAGER)
    @JoinColumn(name = "nodeid", nullable = false)
    public Node getNodetbl() {
        return nodetbl;
    }

    public void setNodetbl(Node nodetbl) {
        this.nodetbl = nodetbl;
    }
    @ManyToOne(fetch = FetchType.EAGER)
    @JoinColumn(name = "rid", nullable = false)
    public Roles getRoles() {
        return roles;
    }

    public void setRoles(Roles roles) {
        this.roles = roles;
    }
}
