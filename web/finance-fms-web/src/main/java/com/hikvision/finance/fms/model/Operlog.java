package com.hikvision.finance.fms.model;

import static javax.persistence.GenerationType.IDENTITY;

import java.util.Date;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.Id;
import javax.persistence.Table;

import org.apache.struts2.json.annotations.JSON;

/**
 * Operlog entity. @author MyEclipse Persistence Tools
 */
@Entity
@Table(name = "operlog")
public class Operlog implements java.io.Serializable {
	
	// Fields
	
	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 6135527553759932149L;
	private Integer id;
	private Integer userId;
	private Date operTime;
	private Integer clientType;
	private Integer operType;
	private String note;
	
	// Constructors
	
	/** default constructor */
	public Operlog() {
	}
	
	/** full constructor */
	public Operlog(Integer userId, Date operTime, Integer clientType, Integer operType, String note) {
		this.userId = userId;
		this.operTime = operTime;
		this.clientType = clientType;
		this.operType = operType;
		this.note = note;
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
	
	@Column(name = "userId")
	public Integer getUserId() {
		return this.userId;
	}
	
	public void setUserId(Integer userId) {
		this.userId = userId;
	}
	
	@JSON(format="yyyy-MM-dd HH:mm:ss")
	@Column(name = "operTime", length = 0)
	public Date getOperTime() {
		return operTime;
	}
	public void setOperTime(Date operTime) {
		this.operTime = operTime;
	}
	
	@Column(name = "clientType")
	public Integer getClientType() {
		return clientType;
	}
	
	public void setClientType(Integer clientType) {
		this.clientType = clientType;
	}
	
	@Column(name = "operType")
	public Integer getOperType() {
		return this.operType;
	}
	
	public void setOperType(Integer operType) {
		this.operType = operType;
	}
	
	@Column(name = "note", length = 256)
	public String getNote() {
		return this.note;
	}
	
	public void setNote(String note) {
		this.note = note;
	}
	
}
