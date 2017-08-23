package com.userEntity;

import javax.persistence.*;

/**
 * Created by churui on 2017/7/29.
 */
@Entity
@Table(name = "subtbl")
public class Subnode implements java.io.Serializable{
    private static final long serialVersionUID = 1L;
    private Integer subid;
    private String subname;
    private Node nodetbl;
    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE, generator = "s_subtbl")
    @SequenceGenerator(name = "s_subtbl", sequenceName = "s_subtbl", allocationSize = 1)
    @Column(name = "subid", unique = true, nullable = false)
    public Integer getSubid() {
        return subid;
    }

    public void setSubid(Integer subid) {
        this.subid = subid;
    }
    @Column(name = "subname", nullable = false, length = 64)
    public String getSubname() {
        return subname;
    }

    public void setSubname(String subname) {
        this.subname = subname;
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
