package com.hikvision.finance.fms.model;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import static javax.persistence.GenerationType.IDENTITY;
import javax.persistence.Id;
import javax.persistence.Table;

/**
 * Cvrassociatemapping entity. @author MyEclipse Persistence Tools
 */
@Entity
@Table(name = "cvrassociatemapping")
public class Cvrassociatemapping implements java.io.Serializable {

	// Fields

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = -6530576302824708331L;
	private Integer id;
	private Integer ncvrId;
	private Integer ncollId;
	private Integer ntype;
	private Integer groupIndex;

	// Constructors

	/** default constructor */
	public Cvrassociatemapping() {
	}

	/** full constructor */
	public Cvrassociatemapping(Integer ncvrId, Integer ncollId, Integer ntype,
			Integer groupIndex) {
		this.ncvrId = ncvrId;
		this.ncollId = ncollId;
		this.ntype = ntype;
		this.groupIndex = groupIndex;
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

	@Column(name = "nCvrId")
	public Integer getNcvrId() {
		return this.ncvrId;
	}

	public void setNcvrId(Integer ncvrId) {
		this.ncvrId = ncvrId;
	}

	@Column(name = "nCollId")
	public Integer getNcollId() {
		return this.ncollId;
	}

	public void setNcollId(Integer ncollId) {
		this.ncollId = ncollId;
	}

	@Column(name = "nType")
	public Integer getNtype() {
		return this.ntype;
	}

	public void setNtype(Integer ntype) {
		this.ntype = ntype;
	}

	@Column(name = "groupIndex")
	public Integer getGroupIndex() {
		return this.groupIndex;
	}

	public void setGroupIndex(Integer groupIndex) {
		this.groupIndex = groupIndex;
	}

}