package com.hikvision.finance.fms.model;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import static javax.persistence.GenerationType.IDENTITY;
import javax.persistence.Id;
import javax.persistence.Table;

/**
 * Userregionmapping entity. @author MyEclipse Persistence Tools
 */
@Entity
@Table(name = "userregionmapping")
public class Userregionmapping implements java.io.Serializable {

	// Fields

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = -3319026725046700911L;
	private Integer id;
	private Integer nuserId;
	private Integer nregionId;

	// Constructors

	/** default constructor */
	public Userregionmapping() {
	}

	/** full constructor */
	public Userregionmapping(Integer nuserId, Integer nregionId) {
		this.nuserId = nuserId;
		this.nregionId = nregionId;
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

	@Column(name = "nRegionId")
	public Integer getNregionId() {
		return this.nregionId;
	}

	public void setNregionId(Integer nregionId) {
		this.nregionId = nregionId;
	}

}