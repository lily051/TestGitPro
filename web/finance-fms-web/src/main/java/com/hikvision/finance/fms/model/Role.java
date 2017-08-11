package com.hikvision.finance.fms.model;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import static javax.persistence.GenerationType.IDENTITY;
import javax.persistence.Id;
import javax.persistence.Table;

/**
 * Role entity. @author MyEclipse Persistence Tools
 */
@Entity
@Table(name = "role")
public class Role implements java.io.Serializable {

	// Fields

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 7119535938251791528L;
	private Integer id;
	private String strName;
	private Integer ntype;
	private Integer nright;
	private Integer nowner;
	private String strComment;

	// Constructors

	/** default constructor */
	public Role() {
	}

	/** full constructor */
	public Role(String strName, Integer ntype, Integer nright, Integer nowner,
			String strComment) {
		this.strName = strName;
		this.ntype = ntype;
		this.nright = nright;
		this.nowner = nowner;
		this.strComment = strComment;
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

	@Column(name = "strName", length = 50)
	public String getStrName() {
		return this.strName;
	}

	public void setStrName(String strName) {
		this.strName = strName;
	}

	@Column(name = "nType")
	public Integer getNtype() {
		return this.ntype;
	}

	public void setNtype(Integer ntype) {
		this.ntype = ntype;
	}

	@Column(name = "nRight")
	public Integer getNright() {
		return this.nright;
	}

	public void setNright(Integer nright) {
		this.nright = nright;
	}

	@Column(name = "nOwner")
	public Integer getNowner() {
		return this.nowner;
	}

	public void setNowner(Integer nowner) {
		this.nowner = nowner;
	}

	@Column(name = "strComment", length = 100)
	public String getStrComment() {
		return this.strComment;
	}

	public void setStrComment(String strComment) {
		this.strComment = strComment;
	}

}