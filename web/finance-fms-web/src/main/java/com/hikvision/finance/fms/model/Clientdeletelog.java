package com.hikvision.finance.fms.model;

import java.util.Date;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import static javax.persistence.GenerationType.IDENTITY;
import javax.persistence.Id;
import javax.persistence.Table;

/**
 * Clientdeletelog entity. @author MyEclipse Persistence Tools
 */
@Entity
@Table(name = "clientdeletelog")
public class Clientdeletelog implements java.io.Serializable {

	// Fields

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = -547238353770914872L;
	private Integer id;
	private String nproId;
	private String name;
	private String creditId;
	private Integer productType;
	private String productName;
	private Date inputTime;
	private String inputUser;
	private Integer inputRegionId;
	private Date deleteTime;
	private Integer soundNum;
	private Integer vedioNum;

	// Constructors

	/** default constructor */
	public Clientdeletelog() {
	}

	/** full constructor */
	public Clientdeletelog(String nproId, String name, String creditId,
			Integer productType, String productName, Date inputTime,
			String inputUser, Integer inputRegionId, Date deleteTime,
			Integer soundNum, Integer vedioNum) {
		this.nproId = nproId;
		this.name = name;
		this.creditId = creditId;
		this.productType = productType;
		this.productName = productName;
		this.inputTime = inputTime;
		this.inputUser = inputUser;
		this.inputRegionId = inputRegionId;
		this.deleteTime = deleteTime;
		this.soundNum = soundNum;
		this.vedioNum = vedioNum;
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

	@Column(name = "nProId", length = 50)
	public String getNproId() {
		return this.nproId;
	}

	public void setNproId(String nproId) {
		this.nproId = nproId;
	}

	@Column(name = "name", length = 50)
	public String getName() {
		return this.name;
	}

	public void setName(String name) {
		this.name = name;
	}

	@Column(name = "creditId", length = 18)
	public String getCreditId() {
		return this.creditId;
	}

	public void setCreditId(String creditId) {
		this.creditId = creditId;
	}

	@Column(name = "productType")
	public Integer getProductType() {
		return this.productType;
	}

	public void setProductType(Integer productType) {
		this.productType = productType;
	}

	@Column(name = "productName", length = 50)
	public String getProductName() {
		return this.productName;
	}

	public void setProductName(String productName) {
		this.productName = productName;
	}

	@Column(name = "inputTime", length = 0)
	public Date getInputTime() {
		return this.inputTime;
	}

	public void setInputTime(Date inputTime) {
		this.inputTime = inputTime;
	}

	@Column(name = "inputUser", length = 50)
	public String getInputUser() {
		return this.inputUser;
	}

	public void setInputUser(String inputUser) {
		this.inputUser = inputUser;
	}

	@Column(name = "inputRegionId")
	public Integer getInputRegionId() {
		return this.inputRegionId;
	}

	public void setInputRegionId(Integer inputRegionId) {
		this.inputRegionId = inputRegionId;
	}

	@Column(name = "deleteTime", length = 0)
	public Date getDeleteTime() {
		return this.deleteTime;
	}

	public void setDeleteTime(Date deleteTime) {
		this.deleteTime = deleteTime;
	}

	@Column(name = "soundNum")
	public Integer getSoundNum() {
		return this.soundNum;
	}

	public void setSoundNum(Integer soundNum) {
		this.soundNum = soundNum;
	}

	@Column(name = "vedioNum")
	public Integer getVedioNum() {
		return this.vedioNum;
	}

	public void setVedioNum(Integer vedioNum) {
		this.vedioNum = vedioNum;
	}

}