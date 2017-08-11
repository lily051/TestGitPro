package com.hikvision.finance.fms.model;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import static javax.persistence.GenerationType.IDENTITY;
import javax.persistence.Id;
import javax.persistence.Table;

/**
 * Roleprivilegemapping entity. @author MyEclipse Persistence Tools
 */
@Entity
@Table(name = "roleprivilegemapping")
public class Roleprivilegemapping implements java.io.Serializable {

	// Fields

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 9213525395566073511L;
	private Integer id;
	private Integer nroleId;
	private String nprivilegeCode;
	private Integer nsortId;

	// Constructors

	/** default constructor */
	public Roleprivilegemapping() {
	}

	/** full constructor */
	public Roleprivilegemapping(Integer nroleId, String nprivilegeCode,
			Integer nsortId) {
		this.nroleId = nroleId;
		this.nprivilegeCode = nprivilegeCode;
		this.nsortId = nsortId;
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

	@Column(name = "nRoleId")
	public Integer getNroleId() {
		return this.nroleId;
	}

	public void setNroleId(Integer nroleId) {
		this.nroleId = nroleId;
	}

	@Column(name = "nPrivilegeCode", length = 50)
	public String getNprivilegeCode() {
		return this.nprivilegeCode;
	}

	public void setNprivilegeCode(String nprivilegeCode) {
		this.nprivilegeCode = nprivilegeCode;
	}

	@Column(name = "nSortId")
	public Integer getNsortId() {
		return this.nsortId;
	}

	public void setNsortId(Integer nsortId) {
		this.nsortId = nsortId;
	}

}