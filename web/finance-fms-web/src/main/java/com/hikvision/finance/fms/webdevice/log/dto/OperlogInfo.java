package com.hikvision.finance.fms.webdevice.log.dto;

import java.util.Date;

import org.apache.struts2.json.annotations.JSON;

public class OperlogInfo {
	
	private Integer id;
	private Integer userId;
	private Date operTime;
	private Integer operType;
	private String note;
	
	private Date startoperTime;
	private Date endoperTime;
	
	public Integer getId() {
		return id;
	}
	
	public void setId(Integer id) {
		this.id = id;
	}
	
	public Integer getUserId() {
		return userId;
	}
	
	public void setUserId(Integer userId) {
		this.userId = userId;
	}
	
	@JSON(format = "yyyy-MM-dd HH:mm:ss")
	public Date getOperTime() {
		return operTime;
	}
	
	public void setOperTime(Date operTime) {
		this.operTime = operTime;
	}
	
	public Integer getOperType() {
		return operType;
	}
	
	public void setOperType(Integer operType) {
		this.operType = operType;
	}
	
	public String getNote() {
		return note;
	}
	
	public void setNote(String note) {
		this.note = note;
	}
	
	public Date getStartoperTime() {
		return startoperTime;
	}
	
	public void setStartoperTime(Date startoperTime) {
		this.startoperTime = startoperTime;
	}
	
	public Date getEndoperTime() {
		return endoperTime;
	}
	
	public void setEndoperTime(Date endoperTime) {
		this.endoperTime = endoperTime;
	}
	
}
