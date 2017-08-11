package com.hikvision.finance.fms.model;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import static javax.persistence.GenerationType.IDENTITY;
import javax.persistence.Id;
import javax.persistence.Table;

/**
 * Cloudpoolmapping entity. @author MyEclipse Persistence Tools
 */
@Entity
@Table(name = "cloudpoolmapping")
public class Cloudpoolmapping implements java.io.Serializable {

	// Fields

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 554736475579044949L;
	private Integer id;
	private Integer ndeviceId;
	private Integer poolId;
	private String poolName;
	private Integer poolType;
	private Integer poolSize;
	private Integer poolFreeSize;

	// Constructors

	/** default constructor */
	public Cloudpoolmapping() {
	}

	/** full constructor */
	public Cloudpoolmapping(Integer ndeviceId, Integer poolId, String poolName,
			Integer poolType, Integer poolSize, Integer poolFreeSize) {
		this.ndeviceId = ndeviceId;
		this.poolId = poolId;
		this.poolName = poolName;
		this.poolType = poolType;
		this.poolSize = poolSize;
		this.poolFreeSize = poolFreeSize;
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

	@Column(name = "poolId")
	public Integer getPoolId() {
		return this.poolId;
	}

	public void setPoolId(Integer poolId) {
		this.poolId = poolId;
	}

	@Column(name = "poolName", length = 100)
	public String getPoolName() {
		return this.poolName;
	}

	public void setPoolName(String poolName) {
		this.poolName = poolName;
	}

	@Column(name = "poolType")
	public Integer getPoolType() {
		return this.poolType;
	}

	public void setPoolType(Integer poolType) {
		this.poolType = poolType;
	}

	@Column(name = "poolSize")
	public Integer getPoolSize() {
		return this.poolSize;
	}

	public void setPoolSize(Integer poolSize) {
		this.poolSize = poolSize;
	}

	@Column(name = "poolFreeSize")
	public Integer getPoolFreeSize() {
		return this.poolFreeSize;
	}

	public void setPoolFreeSize(Integer poolFreeSize) {
		this.poolFreeSize = poolFreeSize;
	}

}