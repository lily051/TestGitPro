package com.hikvision.finance.fms.model;

import java.util.Date;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import static javax.persistence.GenerationType.IDENTITY;
import javax.persistence.Id;
import javax.persistence.Table;

/**
 * Userlock entity. @author MyEclipse Persistence Tools
 */
@Entity
@Table(name = "userlock")
public class Userlock implements java.io.Serializable {

	/**
	 * 序列化ID
	 */
	private static final long serialVersionUID = -1194558886825275851L;
	private Integer id;
	private String strName;
	private Integer nfailCount;
	private Date dtLoginTime;
	private Integer userId;
	// Constructors

	
	public Userlock(){
		
	}
	/**
	 * 创建一个新的实例Userlock.
	 * @param id
	 * @param strName
	 * @param nfailCount
	 * @param dtLoginTime
	 * @param userId
	 */
	public Userlock(Integer id, String strName, Integer nfailCount, Date dtLoginTime, Integer userId) {
		super();
		this.id = id;
		this.strName = strName;
		this.nfailCount = nfailCount;
		this.dtLoginTime = dtLoginTime;
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
	
	@Column(name = "strName")
	public String getStrName() {
		return strName;
	}
	public void setStrName(String strName) {
		this.strName = strName;
	}
	
	@Column(name = "nfailCount")
	public Integer getNfailCount() {
		return nfailCount;
	}
	public void setNfailCount(Integer nfailCount) {
		this.nfailCount = nfailCount;
	}
	
	@Column(name = "dtLoginTime")
	public Date getDtLoginTime() {
		return dtLoginTime;
	}
	public void setDtLoginTime(Date dtLoginTime) {
		this.dtLoginTime = dtLoginTime;
	}
	
	@Column(name = "userId")
	public Integer getUserId() {
		return userId;
	}
	public void setUserId(Integer userId) {
		this.userId = userId;
	}


	
	
}