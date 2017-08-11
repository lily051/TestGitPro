package com.hikvision.finance.fms.model;

import java.util.Date;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import static javax.persistence.GenerationType.IDENTITY;
import javax.persistence.Id;
import javax.persistence.Table;

/**
 * Sysconfig entity. @author MyEclipse Persistence Tools
 */
@Entity
@Table(name = "sysconfig")
public class Sysconfig implements java.io.Serializable {

	// Fields

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 1336746187108350022L;
	private Integer id;
	private Integer nkey;
	private String strValue;
	private String strComment;
	private Integer nenabled;
	private Integer ntype;
	private String strDescribe;
	private Date strUpdatetime;

	// Constructors

	/** default constructor */
	public Sysconfig() {
	}

	/** full constructor */
	public Sysconfig(Integer nkey, String strValue, String strComment,
			Integer nenabled, Integer ntype, String strDescribe,
			Date strUpdatetime) {
		this.nkey = nkey;
		this.strValue = strValue;
		this.strComment = strComment;
		this.nenabled = nenabled;
		this.ntype = ntype;
		this.strDescribe = strDescribe;
		this.strUpdatetime = strUpdatetime;
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

	@Column(name = "nKey")
	public Integer getNkey() {
		return this.nkey;
	}

	public void setNkey(Integer nkey) {
		this.nkey = nkey;
	}

	@Column(name = "strValue", length = 5000)
	public String getStrValue() {
		return this.strValue;
	}

	public void setStrValue(String strValue) {
		this.strValue = strValue;
	}

	@Column(name = "strComment", length = 200)
	public String getStrComment() {
		return this.strComment;
	}

	public void setStrComment(String strComment) {
		this.strComment = strComment;
	}

	@Column(name = "nEnabled")
	public Integer getNenabled() {
		return this.nenabled;
	}

	public void setNenabled(Integer nenabled) {
		this.nenabled = nenabled;
	}

	@Column(name = "nType")
	public Integer getNtype() {
		return this.ntype;
	}

	public void setNtype(Integer ntype) {
		this.ntype = ntype;
	}

	@Column(name = "strDescribe", length = 50)
	public String getStrDescribe() {
		return this.strDescribe;
	}

	public void setStrDescribe(String strDescribe) {
		this.strDescribe = strDescribe;
	}

	@Column(name = "strUpdatetime", length = 0)
	public Date getStrUpdatetime() {
		return this.strUpdatetime;
	}

	public void setStrUpdatetime(Date strUpdatetime) {
		this.strUpdatetime = strUpdatetime;
	}

}