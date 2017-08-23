package com.userEntity;

import javax.persistence.*;

/**
 * Created by churui on 2017/8/3.
 */
@Entity
@Table(name = "rolepre")//权限
public class Rolepre implements java.io.Serializable {
    private static final long serialVersionUID = 1L;
    private Integer preid;
    private Integer modulenum;
    private Roles roles;//角色

    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE, generator = "s_rolepre")
    @SequenceGenerator(name = "s_rolepre", sequenceName = "s_rolepre", allocationSize = 1)
    @Column(name = "preid", unique = true, nullable = false)
    public Integer getPreid() {
        return preid;
    }

    public void setPreid(Integer preid) {
        this.preid = preid;
    }
    @Column(name = "modulenum", nullable = false, length = 64)
    public Integer getModulenum() {
        return modulenum;
    }

    public void setModulenum(Integer modulenum) {
        this.modulenum = modulenum;
    }
//    @ManyToOne(fetch = FetchType.EAGER)
//    @JoinColumn(name = "roleid", nullable = false)
//    public Role getRoletbl() {
//        return roletbl;
//    }
//
//    public void setRoletbl(Role roletbl) {
//        this.roletbl = roletbl;
//    }
    @ManyToOne(fetch = FetchType.EAGER)
    @JoinColumn(name = "rid", nullable = false)
    public Roles getRoles() {
        return roles;
    }

    public void setRoles(Roles roles) {
        this.roles = roles;
    }
}
