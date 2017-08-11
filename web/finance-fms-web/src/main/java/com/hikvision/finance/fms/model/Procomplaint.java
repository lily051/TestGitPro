package com.hikvision.finance.fms.model;

import java.util.Date;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import static javax.persistence.GenerationType.IDENTITY;
import javax.persistence.Id;
import javax.persistence.Table;

/**
 * Procomplaint entity. @author MyEclipse Persistence Tools
 */
@Entity
@Table(name = "procomplaint")
public class Procomplaint implements java.io.Serializable {

	// Fields

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 557833367972953263L;
	private Integer id;
	private Integer clientId;
	private Integer ntype;
	private String compReason;
	private Date compTime;
	private String compUser;
	private Integer regionId;
	private String dealResult;
	private Date dealTime;
	private String dealUser;
	private String liableUser;
	private Integer nRecordId;

	// Constructors

	/** default constructor */
	public Procomplaint() {
	}

	/** full constructor */
	public Procomplaint(Integer clientId, Integer ntype, String compReason,
			Date compTime, String compUser, Integer regionId,
			String dealResult, Date dealTime, String dealUser, String liableUser, Integer nRecordId) {
		this.clientId = clientId;
		this.ntype = ntype;
		this.compReason = compReason;
		this.compTime = compTime;
		this.compUser = compUser;
		this.regionId = regionId;
		this.dealResult = dealResult;
		this.dealTime = dealTime;
		this.dealUser = dealUser;
		this.liableUser = liableUser;
		this.nRecordId = nRecordId;
	}

	// Property accessors
	@Id
	@GeneratedValue(strategy = IDENTITY)
	@Column(name = "id", unique = true, nullable = false)
	public Integer getId() {
		return this.id;
	}

	public void setId(Integer id) {
		this.id = id;
	}

	@Column(name = "clientId")
	public Integer getClientId() {
		return this.clientId;
	}

	public void setClientId(Integer clientId) {
		this.clientId = clientId;
	}

	@Column(name = "nType")
	public Integer getNtype() {
		return this.ntype;
	}

	public void setNtype(Integer ntype) {
		this.ntype = ntype;
	}

	@Column(name = "compReason", length = 200)
	public String getCompReason() {
		return this.compReason;
	}

	public void setCompReason(String compReason) {
		this.compReason = compReason;
	}

	@Column(name = "compTime", length = 0)
	public Date getCompTime() {
		return this.compTime;
	}

	public void setCompTime(Date compTime) {
		this.compTime = compTime;
	}

	@Column(name = "compUser", length = 50)
	public String getCompUser() {
		return this.compUser;
	}

	public void setCompUser(String compUser) {
		this.compUser = compUser;
	}

	@Column(name = "regionId")
	public Integer getRegionId() {
		return this.regionId;
	}

	public void setRegionId(Integer regionId) {
		this.regionId = regionId;
	}

	@Column(name = "dealResult", length = 200)
	public String getDealResult() {
		return this.dealResult;
	}

	public void setDealResult(String dealResult) {
		this.dealResult = dealResult;
	}

	@Column(name = "dealTime", length = 0)
	public Date getDealTime() {
		return this.dealTime;
	}

	public void setDealTime(Date dealTime) {
		this.dealTime = dealTime;
	}

	@Column(name = "dealUser", length = 50)
	public String getDealUser() {
		return this.dealUser;
	}

	public void setDealUser(String dealUser) {
		this.dealUser = dealUser;
	}

	@Column(name = "liableUser", length = 50)
	public String getLiableUser() {
		return this.liableUser;
	}

	public void setLiableUser(String liableUser) {
		this.liableUser = liableUser;
	}

	@Column(name = "nRecordId")
	public Integer getnRecordId() {
		return nRecordId;
	}

	public void setnRecordId(Integer nRecordId) {
		this.nRecordId = nRecordId;
	}

}