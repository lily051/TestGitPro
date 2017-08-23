package com.userEntity;

import javax.persistence.*;
import java.util.HashSet;
import java.util.Set;

/**
 * Created by churui on 2017/8/8.
 */

@Entity
@Table(name = "roles")
public class Roles implements java.io.Serializable{
private static final long serialVersionUID = 1L;
    private Integer rid;
    private String rname;
    private String rdesc;
    private Node nodetbl;
    private Set<Role> role = new HashSet<Role>();
    private Set<Rolepre> rolepre = new HashSet<Rolepre>();
    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE, generator = "s_roles")
    @SequenceGenerator(name = "s_roles", sequenceName = "s_roles", allocationSize=1)
    @Column(name = "rid", unique = true, nullable = false)
    public Integer getRid() {
        return rid;
    }

    public void setRid(Integer rid) {
        this.rid = rid;
    }
    @Column(name = "rname",nullable= false, length = 64)
    public String getRname() {
        return rname;
    }

    public void setRname(String rname) {
        this.rname = rname;
    }
    @Column(name = "rdesc",nullable= false, length = 64)
    public String getRdesc() {
        return rdesc;
    }

    public void setRdesc(String rdesc) {
        this.rdesc = rdesc;
    }
    @ManyToOne(fetch = FetchType.EAGER)
    @JoinColumn(name = "nodeid", nullable = false)
    public Node getNodetbl() {
        return nodetbl;
    }

    public void setNodetbl(Node nodetbl) {
        this.nodetbl = nodetbl;
    }
    @OneToMany(cascade = CascadeType.ALL, fetch = FetchType.EAGER, mappedBy = "roles")
    public Set<Role> getRole() {
        return role;
    }

    public void setRole(Set<Role> roles) {
        this.role = roles;
    }
    @OneToMany(cascade = CascadeType.ALL, fetch = FetchType.EAGER, mappedBy = "roles")
    public Set<Rolepre> getRolepre() {
        return rolepre;
    }

    public void setRolepre(Set<Rolepre> rolepre) {
        this.rolepre = rolepre;
    }
}
