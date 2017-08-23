package com.userEntity;

import javax.persistence.*;
import java.util.HashSet;
import java.util.Set;

/**
 * Created by churui on 2017/7/26.
 */
@Entity
@Table(name = "nodetbl")
public class Node implements java.io.Serializable{

    private static final long serialVersionUID = 1L;
    private Integer nodeid;
    private String nodename;
    private Integer parentid;
    private Integer level;
    private Set<User> users = new HashSet<User>();
    private Set<Subnode> subnodes = new HashSet<Subnode>();

    public Node(){

    }

    public Node(Integer nodeid, String nodename, Set<User> users) {
        this.nodeid = nodeid;
        this.nodename = nodename;
        this.users = users;
    }

    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE, generator = "s_nodetbl")
    @SequenceGenerator(name = "s_nodetbl", sequenceName = "s_nodetbl", allocationSize=1)
    @Column(name = "nodeid", unique = true, nullable = false)
    public Integer getNodeid() {
        return nodeid;
    }

    public void setNodeid(Integer nodeid) {
        this.nodeid = nodeid;
    }
    @Column(name = "nodename",nullable= false, length = 64)
    public String getNodename() {
        return nodename;
    }
    public void setNodename(String nodename) {
        this.nodename = nodename;
    }

    @OneToMany(cascade = CascadeType.ALL, fetch = FetchType.EAGER, mappedBy = "nodetbl")
    public Set<User> getUsers() {
        return users;
    }

    public void setUsers(Set<User> users) {
        this.users = users;
    }

    @OneToMany(cascade = CascadeType.ALL, fetch = FetchType.EAGER, mappedBy = "nodetbl")
    public Set<Subnode> getSubnodes() {
        return subnodes;
    }

    public void setSubnodes(Set<Subnode> subnodes) {
        this.subnodes = subnodes;
    }
    @Column(name = "parentid",nullable= false, length = 64)
    public Integer getParentid() {
        return parentid;
    }

    public void setParentid(Integer parentid) {
        this.parentid = parentid;
    }
    @Column(name = "level",nullable= false, length = 64)
    public Integer getLevel() {
        return level;
    }

    public void setLevel(Integer level) {
        this.level = level;
    }
}
