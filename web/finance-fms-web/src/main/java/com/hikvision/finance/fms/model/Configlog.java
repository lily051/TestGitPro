package com.hikvision.finance.fms.model;

import java.util.Date;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import static javax.persistence.GenerationType.IDENTITY;
import javax.persistence.Id;
import javax.persistence.Table;

/**
 * Configlog entity. @author MyEclipse Persistence Tools
 */
@Entity
@Table(name = "configlog")
public class Configlog implements java.io.Serializable {

	// Fields

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = -8491987933639389845L;
	private Integer id;
	private Integer ntype;
	private String strText;
	private String strUser;
	private Date strDate;
	private String strIp;
	private Integer nactionType;

	// Constructors

	/** default constructor */
	public Configlog() {
	}

	/** full constructor */
	public Configlog(Integer ntype, String strText, String strUser,
			Date strDate, String strIp, Integer nactionType) {
		this.ntype = ntype;
		this.strText = strText;
		this.strUser = strUser;
		this.strDate = strDate;
		this.strIp = strIp;
		this.nactionType = nactionType;
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

	@Column(name = "nType")
	public Integer getNtype() {
		return this.ntype;
	}

	public void setNtype(Integer ntype) {
		this.ntype = ntype;
	}

	@Column(name = "strText", length = 500)
	public String getStrText() {
		return this.strText;
	}

	public void setStrText(String strText) {
		this.strText = strText;
	}

	@Column(name = "strUser", length = 50)
	public String getStrUser() {
		return this.strUser;
	}

	public void setStrUser(String strUser) {
		this.strUser = strUser;
	}

	@Column(name = "strDate", length = 0)
	public Date getStrDate() {
		return this.strDate;
	}

	public void setStrDate(Date strDate) {
		this.strDate = strDate;
	}

	@Column(name = "strIp", length = 20)
	public String getStrIp() {
		return this.strIp;
	}

	public void setStrIp(String strIp) {
		this.strIp = strIp;
	}

	@Column(name = "nActionType")
	public Integer getNactionType() {
		return this.nactionType;
	}

	public void setNactionType(Integer nactionType) {
		this.nactionType = nactionType;
	}

}