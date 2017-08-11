package com.hikvision.finance.fms.model;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import static javax.persistence.GenerationType.IDENTITY;
import javax.persistence.Id;
import javax.persistence.Table;

/**
 * Usercrtificatemapping entity. @author MyEclipse Persistence Tools
 */
@Entity
@Table(name = "usercrtificatemapping")
public class Usercrtificatemapping implements java.io.Serializable {

	// Fields

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = -8605256765983547900L;
	private Integer id;
	private Integer ncrtificateId;
	private Integer userId;

	// Constructors

	/** default constructor */
	public Usercrtificatemapping() {
	}

	/** full constructor */
	public Usercrtificatemapping(Integer ncrtificateId, Integer userId) {
		this.ncrtificateId = ncrtificateId;
		this.userId = userId;
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

	@Column(name = "nCrtificateId")
	public Integer getNcrtificateId() {
		return this.ncrtificateId;
	}

	public void setNcrtificateId(Integer ncrtificateId) {
		this.ncrtificateId = ncrtificateId;
	}

	@Column(name = "userId")
	public Integer getUserId() {
		return this.userId;
	}

	public void setUserId(Integer userId) {
		this.userId = userId;
	}

}