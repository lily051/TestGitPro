package com.hikvision.finance.fms.model;

import java.util.Date;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import static javax.persistence.GenerationType.IDENTITY;
import javax.persistence.Id;
import javax.persistence.Table;

/**
 * Recordlog entity. @author MyEclipse Persistence Tools
 */
@Entity
@Table(name = "recordlog")
public class Recordlog implements java.io.Serializable {

	// Fields

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 6018348090451126119L;
	private Integer id;
	private String recordName;
	private Integer recordType;
	private String nproId;
	private Integer checkUserNo;
	private String regionId;
	private String remark;
	private Date operDate;
	private String checkUserName;

	// Constructors

	/** default constructor */
	public Recordlog() {
	}

	/** full constructor */
	public Recordlog(String recordName, Integer recordType, String nproId,
			Integer checkUserNo, String regionId, String remark, Date operDate,
			String checkUserName) {
		this.recordName = recordName;
		this.recordType = recordType;
		this.nproId = nproId;
		this.checkUserNo = checkUserNo;
		this.regionId = regionId;
		this.remark = remark;
		this.operDate = operDate;
		this.checkUserName = checkUserName;
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

	@Column(name = "recordName", length = 50)
	public String getRecordName() {
		return this.recordName;
	}

	public void setRecordName(String recordName) {
		this.recordName = recordName;
	}

	@Column(name = "recordType")
	public Integer getRecordType() {
		return this.recordType;
	}

	public void setRecordType(Integer recordType) {
		this.recordType = recordType;
	}

	@Column(name = "nProId", length = 50)
	public String getNproId() {
		return this.nproId;
	}

	public void setNproId(String nproId) {
		this.nproId = nproId;
	}

	@Column(name = "checkUserNo")
	public Integer getCheckUserNo() {
		return this.checkUserNo;
	}

	public void setCheckUserNo(Integer checkUserNo) {
		this.checkUserNo = checkUserNo;
	}

	@Column(name = "regionId", length = 50)
	public String getRegionId() {
		return this.regionId;
	}

	public void setRegionId(String regionId) {
		this.regionId = regionId;
	}

	@Column(name = "remark", length = 100)
	public String getRemark() {
		return this.remark;
	}

	public void setRemark(String remark) {
		this.remark = remark;
	}

	@Column(name = "operDate", length = 0)
	public Date getOperDate() {
		return this.operDate;
	}

	public void setOperDate(Date operDate) {
		this.operDate = operDate;
	}

	@Column(name = "checkUserName", length = 50)
	public String getCheckUserName() {
		return this.checkUserName;
	}

	public void setCheckUserName(String checkUserName) {
		this.checkUserName = checkUserName;
	}

}