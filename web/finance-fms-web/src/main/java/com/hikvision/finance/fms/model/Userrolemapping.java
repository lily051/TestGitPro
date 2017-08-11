package com.hikvision.finance.fms.model;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import static javax.persistence.GenerationType.IDENTITY;
import javax.persistence.Id;
import javax.persistence.Table;

/**
 * Userrolemapping entity. @author MyEclipse Persistence Tools
 */
@Entity
@Table(name = "userrolemapping")
public class Userrolemapping implements java.io.Serializable {

	// Fields

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 576121448940009308L;
	private Integer id;
	private Integer nuserId;
	private Integer nroleId;

	// Constructors

	/** default constructor */
	public Userrolemapping() {
	}

	/** full constructor */
	public Userrolemapping(Integer nuserId, Integer nroleId) {
		this.nuserId = nuserId;
		this.nroleId = nroleId;
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

	@Column(name = "nRoleId")
	public Integer getNroleId() {
		return this.nroleId;
	}

	public void setNroleId(Integer nroleId) {
		this.nroleId = nroleId;
	}

}