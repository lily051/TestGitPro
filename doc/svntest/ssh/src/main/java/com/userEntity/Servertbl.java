package com.userEntity;

import javax.persistence.*;
import java.util.HashSet;
import java.util.Set;


/**
 * Servertbl entity. @author MyEclipse Persistence Tools
 */
@Entity
@Table(name="servertbl")

public class Servertbl implements java.io.Serializable {


    // Fields

	 private static final long serialVersionUID = 1L;
	 private Integer serverid;
	 private String serverIndexCode;
     private Node nodetbl;
     private Integer servertype;
     private String servername;
     private Integer subuse;
     private Integer devicesnum;
     private Integer channelsnum;
     private Integer networkspace;
     private String remark;
     private String username;
 	 private String userpwd;
 	 private Integer alarmserverid;
 	 private Integer referServerid;
     //private Set<Deviceconnservertbl> deviceconnservertbls = new HashSet<Deviceconnservertbl>(0);
     private Set<ServerAddrTbl> serverAddrTbls = new HashSet<ServerAddrTbl>(0);


    // Constructors

    /** default constructor */
    public Servertbl() {
    }

	/** minimal constructor */
    public Servertbl(Node nodetbl, Integer servertype, String servername, Integer networkspace) {
        this.nodetbl = nodetbl;
        this.servertype = servertype;
        this.servername = servername;
        this.networkspace = networkspace;
    }

    /** full constructor */
    public Servertbl(String serverIndexCode, Node nodetbl, Integer servertype, String servername, Integer subuse, Integer devicesnum, Integer channelsnum,
                     Integer networkspace, String remark, String username, String userpwd, Integer alarmserverid, Integer referServerid,
                      Set<ServerAddrTbl> serverAddrTbls) {
    	this.serverIndexCode = serverIndexCode;
    	this.nodetbl = nodetbl;
        this.servertype = servertype;
        this.servername = servername;
        this.subuse = subuse;
        this.devicesnum = devicesnum;
        this.channelsnum = channelsnum;
        this.networkspace = networkspace;
        this.remark = remark;
        this.username = username;
		this.userpwd = userpwd;
		this.alarmserverid = alarmserverid;
		this.referServerid = referServerid;
        //this.deviceconnservertbls = deviceconnservertbls;
        this.serverAddrTbls = serverAddrTbls;
    }


    // Property accessors
	@Id
	@GeneratedValue(strategy = GenerationType.SEQUENCE, generator = "s_servertbl")
	@SequenceGenerator(name = "s_servertbl", sequenceName = "s_servertbl", allocationSize = 1)
    @Column(name="serverid", unique=true, nullable=false)
    public Integer getServerid() {
        return this.serverid;
    }

    public void setServerid(Integer serverid) {
        this.serverid = serverid;
    }

    @Column(name="server_index_code", nullable=false, length=64)
	public String getServerIndexCode() {
		return serverIndexCode;
	}

	public void setServerIndexCode(String serverIndexCode) {
		this.serverIndexCode = serverIndexCode;
	}

	@ManyToOne(fetch= FetchType.LAZY)
    @JoinColumn(name="nodeid", nullable=false)
    public Node getNodetbl() {
        return this.nodetbl;
    }

    public void setNodetbl(Node nodetbl) {
        this.nodetbl = nodetbl;
    }

    @Column(name="servertype", nullable=false)
    public Integer getServertype() {
        return this.servertype;
    }

    public void setServertype(Integer servertype) {
        this.servertype = servertype;
    }

    @Column(name="servername", nullable=false, length=64)
    public String getServername() {
        return this.servername;
    }

    public void setServername(String servername) {
        this.servername = servername;
    }

    @Column(name="subuse")
    public Integer getSubuse() {
        return this.subuse;
    }

    public void setSubuse(Integer subuse) {
        this.subuse = subuse;
    }

    @Column(name="devicesnum")
    public Integer getDevicesnum() {
        return this.devicesnum;
    }

    public void setDevicesnum(Integer devicesnum) {
        this.devicesnum = devicesnum;
    }

    @Column(name="channelsnum")
    public Integer getChannelsnum() {
        return this.channelsnum;
    }

    public void setChannelsnum(Integer channelsnum) {
        this.channelsnum = channelsnum;
    }

    @Column(name="networkspace", nullable=false)
    public Integer getNetworkspace() {
        return this.networkspace;
    }

    public void setNetworkspace(Integer networkspace) {
        this.networkspace = networkspace;
    }

    @Column(name="remark", length=65535)
    public String getRemark() {
        return this.remark;
    }

    public void setRemark(String remark) {
        this.remark = remark;
    }

    @Column(name = "username", length = 64)
	public String getUsername() {
		return this.username;
	}

	public void setUsername(String username) {
		this.username = username;
	}

	@Column(name = "userpwd", length = 64)
	public String getUserpwd() {
		return this.userpwd;
	}

	public void setUserpwd(String userpwd) {
		this.userpwd = userpwd;
	}

	@Column(name="alarmserverid")
    public Integer getAlarmserverid() {
		return alarmserverid;
	}

	public void setAlarmserverid(Integer alarmserverid) {
		this.alarmserverid = alarmserverid;
	}

	@Column(name="refer_serverid")
	public Integer getReferServerid() {
		return referServerid;
	}

	public void setReferServerid(Integer referServerid) {
		this.referServerid = referServerid;
	}
    @OneToMany(cascade= CascadeType.ALL, fetch= FetchType.LAZY, mappedBy="servertbl")
    public Set<ServerAddrTbl> getServerAddrTbls() {
        return this.serverAddrTbls;
    }

    public void setServerAddrTbls(Set<ServerAddrTbl> serverAddrTbls) {
        this.serverAddrTbls = serverAddrTbls;
    }

}