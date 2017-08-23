package com.userEntity;

import javax.persistence.*;

/**
 * Created by churui on 2017/7/11.
 */
@Entity
@Table(name = "usertbl")
public class User implements java.io.Serializable{

    private static final long serialVersionUID = 1L;
    private Integer id;
    private String name;
    private Integer age;
    private String phonenum;
    private String address;
    private Node nodetbl;

    public User() {
    }

    public User(Integer id, String name, Integer age, String phonenum, String address, Node nodetbl) {
        this.id = id;
        this.name = name;
        this.age = age;
        this.phonenum = phonenum;
        this.address = address;
        this.nodetbl = nodetbl;
    }

    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE, generator = "s_usertbl")
    @SequenceGenerator(name = "s_usertbl", sequenceName = "s_usertbl", allocationSize = 1)
    @Column(name = "id", unique = true, nullable = false)
    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }
    @Column(name = "name", nullable = false, length = 64)
    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }
    @Column(name = "age", nullable = true, length = 64)
    public Integer getAge() {
        return age;
    }

    public void setAge(Integer age) {
        this.age = age;
    }
    @Column(name = "phonenum", nullable = false, length = 64)
    public String getPhonenum() {
        return phonenum;
    }

    public void setPhonenum(String phonenum) {
        this.phonenum = phonenum;
    }
    @Column(name = "address", nullable = true, length = 64)
    public String getAddress() {
        return address;
    }

    public void setAddress(String address) {
        this.address = address;
    }
    @ManyToOne(fetch = FetchType.EAGER)
    @JoinColumn(name = "nodeid", nullable = false)
    public Node getNodetbl() {
        return nodetbl;
    }

    public void setNodetbl(Node nodetbl) {
        this.nodetbl = nodetbl;
    }
}
