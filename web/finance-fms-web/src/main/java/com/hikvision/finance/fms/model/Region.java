package com.hikvision.finance.fms.model;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import static javax.persistence.GenerationType.IDENTITY;
import javax.persistence.Id;
import javax.persistence.Table;

/**
 * Region entity. @author MyEclipse Persistence Tools
 */
@Entity
@Table(name = "region")
public class Region implements java.io.Serializable {

	// Fields

	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 6558017173424407368L;
	private Integer id;
	private Integer nparentId;
	private Integer nsortId;
	private String strInterAreaCode;
	private String strPath;
	private String strName;
	private String strCode;
	private String strTxt;
	
	// Constructors
	
	/** default constructor */
	public Region() {
	}
	
	/** full constructor */
	public Region(Integer nparentId, Integer nsortId, String strInterAreaCode, String strPath, String strName, String strCode,
	        String strTxt) {
		this.nparentId = nparentId;
		this.nsortId = nsortId;
		this.strInterAreaCode = strInterAreaCode;
		this.strPath = strPath;
		this.strName = strName;
		this.strCode = strCode;
		this.strTxt = strTxt;
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
	
	@Column(name = "nParentId")
	public Integer getNparentId() {
		return this.nparentId;
	}
	
	public void setNparentId(Integer nparentId) {
		this.nparentId = nparentId;
	}
	
	@Column(name = "nSortId")
	public Integer getNsortId() {
		return this.nsortId;
	}
	
	public void setNsortId(Integer nsortId) {
		this.nsortId = nsortId;
	}
	
	@Column(name = "strInterAreaCode", length = 128)
	public String getStrInterAreaCode() {
		return this.strInterAreaCode;
	}
	
	public void setStrInterAreaCode(String strInterAreaCode) {
		this.strInterAreaCode = strInterAreaCode;
	}
	
	@Column(name = "strPath", length = 300)
	public String getStrPath() {
		return this.strPath;
	}
	
	public void setStrPath(String strPath) {
		this.strPath = strPath;
	}
	
	@Column(name = "strName", length = 50)
	public String getStrName() {
		return this.strName;
	}
	
	public void setStrName(String strName) {
		this.strName = strName;
	}
	
	@Column(name = "strCode", length = 50)
	public String getStrCode() {
		return this.strCode;
	}
	
	public void setStrCode(String strCode) {
		this.strCode = strCode;
	}
	
	@Column(name = "strTxt", length = 200)
	public String getStrTxt() {
		return this.strTxt;
	}
	
	public void setStrTxt(String strTxt) {
		this.strTxt = strTxt;
	}
	
}