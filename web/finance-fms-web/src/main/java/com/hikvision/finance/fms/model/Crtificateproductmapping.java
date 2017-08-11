package com.hikvision.finance.fms.model;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import static javax.persistence.GenerationType.IDENTITY;
import javax.persistence.Id;
import javax.persistence.Table;

/**
 * Crtificateproductmapping entity. @author MyEclipse Persistence Tools
 */
@Entity
@Table(name = "crtificateproductmapping")
public class Crtificateproductmapping implements java.io.Serializable {

	// Fields

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = -8894730359412417521L;
	private Integer id;
	private Integer ncrtificateId;
	private Integer nproductId;

	// Constructors

	/** default constructor */
	public Crtificateproductmapping() {
	}

	/** full constructor */
	public Crtificateproductmapping(Integer ncrtificateId, Integer nproductId) {
		this.ncrtificateId = ncrtificateId;
		this.nproductId = nproductId;
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

	@Column(name = "nProductId")
	public Integer getNproductId() {
		return this.nproductId;
	}

	public void setNproductId(Integer nproductId) {
		this.nproductId = nproductId;
	}

}