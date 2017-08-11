package com.hikvision.finance.fms.model;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import static javax.persistence.GenerationType.IDENTITY;
import javax.persistence.Id;
import javax.persistence.Table;

/**
 * Devicechannel entity. @author MyEclipse Persistence Tools
 */
@Entity
@Table(name = "devicechannel")
public class Devicechannel implements java.io.Serializable {

	// Fields

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 1681032640700826420L;
	private Integer id;
	private Integer ndeviceId;
	private Integer nchannelNo;
	private String nchannelName;
	private Integer nregionId;
	private String userIp;

	// Constructors

	/** default constructor */
	public Devicechannel() {
	}

	/** full constructor */
	public Devicechannel(Integer ndeviceId, Integer nchannelNo, String nchannelName, Integer nregionId,
	        String userIp) {
		this.ndeviceId = ndeviceId;
		this.nchannelNo = nchannelNo;
		this.nchannelName = nchannelName;
		this.nregionId = nregionId;
		this.userIp = userIp;
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

	@Column(name = "nDeviceId")
	public Integer getNdeviceId() {
		return this.ndeviceId;
	}

	public void setNdeviceId(Integer ndeviceId) {
		this.ndeviceId = ndeviceId;
	}

	@Column(name = "nChannelNo")
	public Integer getNchannelNo() {
		return this.nchannelNo;
	}

	public void setNchannelNo(Integer nchannelNo) {
		this.nchannelNo = nchannelNo;
	}

	@Column(name = "nChannelName", length = 50)
	public String getNchannelName() {
		return this.nchannelName;
	}

	public void setNchannelName(String nchannelName) {
		this.nchannelName = nchannelName;
	}

	@Column(name = "nRegionId")
	public Integer getNregionId() {
		return this.nregionId;
	}

	public void setNregionId(Integer nregionId) {
		this.nregionId = nregionId;
	}
	
	@Column(name = "userIp")
	public String getUserIp() {
		return userIp;
	}
	public void setUserIp(String userIp) {
		this.userIp = userIp;
	}

}