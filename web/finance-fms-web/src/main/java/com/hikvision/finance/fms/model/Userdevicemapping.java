package com.hikvision.finance.fms.model;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import static javax.persistence.GenerationType.IDENTITY;
import javax.persistence.Id;
import javax.persistence.Table;

/**
 * Userdevicemapping entity. @author MyEclipse Persistence Tools
 */
@Entity
@Table(name = "userdevicemapping")
public class Userdevicemapping implements java.io.Serializable {

	// Fields

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 2787992091274930335L;
	private Integer id;
	private Integer nuserId;
	private Integer devId;
	private Integer devType;
	private Integer devChannel;

	// Constructors

	/** default constructor */
	public Userdevicemapping() {
	}

	/** full constructor */
	public Userdevicemapping(Integer nuserId, Integer devId, Integer devType,
			Integer devChannel) {
		this.nuserId = nuserId;
		this.devId = devId;
		this.devType = devType;
		this.devChannel = devChannel;
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

	@Column(name = "nUserId")
	public Integer getNuserId() {
		return this.nuserId;
	}

	public void setNuserId(Integer nuserId) {
		this.nuserId = nuserId;
	}

	@Column(name = "devId")
	public Integer getDevId() {
		return this.devId;
	}

	public void setDevId(Integer devId) {
		this.devId = devId;
	}

	@Column(name = "devType")
	public Integer getDevType() {
		return this.devType;
	}

	public void setDevType(Integer devType) {
		this.devType = devType;
	}

	@Column(name = "devChannel")
	public Integer getDevChannel() {
		return this.devChannel;
	}

	public void setDevChannel(Integer devChannel) {
		this.devChannel = devChannel;
	}

}