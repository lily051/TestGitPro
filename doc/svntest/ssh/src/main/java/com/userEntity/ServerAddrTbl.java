package com.userEntity;

import javax.persistence.*;

/**
 * ServerAddrTbl entity. @author MyEclipse Persistence Tools
 */
@Entity
@Table(name = "server_addr_tbl")
public class ServerAddrTbl implements java.io.Serializable {

	// Fields

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private Integer addrid;
	private Servertbl servertbl;
	private Integer routenum;
	private String serverip;
	private Integer serverport;
	private Integer serverportCu;
	private Integer webserviceport;
	private String remark;

	// Constructors

	/** default constructor */
	public ServerAddrTbl() {
	}

	/** minimal constructor */
	public ServerAddrTbl(Servertbl servertbl, Integer routenum,
                         String serverip, Integer serverport, Integer serverportCu, Integer webserviceport) {
		this.servertbl = servertbl;
		this.routenum = routenum;
		this.serverip = serverip;
		this.serverport = serverport;
		this.serverportCu = serverportCu;
		this.webserviceport = webserviceport;
	}

	/** full constructor */
	public ServerAddrTbl(Servertbl servertbl, Integer routenum,
                         String serverip, Integer serverport, Integer serverportCu,
                         Integer webserviceport, String remark) {
		this.servertbl = servertbl;
		this.routenum = routenum;
		this.serverip = serverip;
		this.serverport = serverport;
		this.serverportCu = serverportCu;
		this.webserviceport = webserviceport;
		this.remark = remark;
	}

	// Property accessors
	@Id
	@GeneratedValue(strategy = GenerationType.SEQUENCE, generator = "s_server_addr_tbl")
	@SequenceGenerator(name = "s_server_addr_tbl", sequenceName = "s_server_addr_tbl", allocationSize = 1)
	@Column(name = "addrid", unique = true, nullable = false)
	public Integer getAddrid() {
		return this.addrid;
	}

	public void setAddrid(Integer addrid) {
		this.addrid = addrid;
	}

	@ManyToOne(cascade= CascadeType.ALL, fetch = FetchType.EAGER)
	@JoinColumn(name = "serverid", nullable = false)
	public Servertbl getServertbl() {
		return this.servertbl;
	}

	public void setServertbl(Servertbl servertbl) {
		this.servertbl = servertbl;
	}

	@Column(name = "routenum", nullable = false)
	public Integer getRoutenum() {
		return this.routenum;
	}

	public void setRoutenum(Integer routenum) {
		this.routenum = routenum;
	}

	@Column(name = "serverip", nullable = false, length = 20)
	public String getServerip() {
		return this.serverip;
	}

	public void setServerip(String serverip) {
		this.serverip = serverip;
	}

	@Column(name = "serverport", nullable = false)
	public Integer getServerport() {
		return this.serverport;
	}

	public void setServerport(Integer serverport) {
		this.serverport = serverport;
	}

	@Column(name = "serverport_cu", nullable = false)
	public Integer getServerportCu() {
		return this.serverportCu;
	}

	public void setServerportCu(Integer serverportCu) {
		this.serverportCu = serverportCu;
	}

	@Column(name = "webserviceport")
	public Integer getWebserviceport() {
		return webserviceport;
	}

	public void setWebserviceport(Integer webserviceport) {
		this.webserviceport = webserviceport;
	}

	@Column(name = "remark", length = 65535)
	public String getRemark() {
		return this.remark;
	}

	public void setRemark(String remark) {
		this.remark = remark;
	}

}