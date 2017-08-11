/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月8日 下午4:17:01
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.log.dto;

import java.util.Date;

import org.apache.struts2.json.annotations.JSON;


/**
 * <p></p>
 * @author jinxindong 2016年4月8日 下午4:17:01
 * @version V1.0 
 */
public class LogsysInfo {
	private Integer id;
	private Integer nmoduleType;
	private Integer ntype;
	private String strUser;
	private Date strDate;
	private String strDateString;
	private String strIp;
	private String strVersionInfo;
	private String strDateStart;
	private String strDateEnd;
	private String ntypeName;
	private String  nmoduleTypeName;
	
    
    public String getNmoduleTypeName() {
    	return nmoduleTypeName;
    }

	
    public void setNmoduleTypeName(String nmoduleTypeName) {
    	this.nmoduleTypeName = nmoduleTypeName;
    }

	public Integer getId() {
    	return id;
    }
	
    public void setId(Integer id) {
    	this.id = id;
    }
	
    public Integer getNmoduleType() {
    	return nmoduleType;
    }
	
    public void setNmoduleType(Integer nmoduleType) {
    	this.nmoduleType = nmoduleType;
    }
	
    public Integer getNtype() {
    	return ntype;
    }
	
    public void setNtype(Integer ntype) {
    	this.ntype = ntype;
    }
	
    public String getStrUser() {
    	return strUser;
    }
	
    public void setStrUser(String strUser) {
    	this.strUser = strUser;
    }
    @JSON(format="yyyy-MM-dd HH:mm:ss")
    public Date getStrDate() {
    	return strDate;
    }
	
    public void setStrDate(Date strDate) {
    	this.strDate = strDate;
    }
	
    public String getStrIp() {
    	return strIp;
    }
	
    public void setStrIp(String strIp) {
    	this.strIp = strIp;
    }
	
    public String getStrVersionInfo() {
    	return strVersionInfo;
    }
	
    public void setStrVersionInfo(String strVersionInfo) {
    	this.strVersionInfo = strVersionInfo;
    }

	
    public String getStrDateStart() {
    	return strDateStart;
    }

	
    public void setStrDateStart(String strDateStart) {
    	this.strDateStart = strDateStart;
    }

	
    public String getStrDateEnd() {
    	return strDateEnd;
    }

	
    public void setStrDateEnd(String strDateEnd) {
    	this.strDateEnd = strDateEnd;
    }

	
    public String getNtypeName() {
    	return ntypeName;
    }

	
    public void setNtypeName(String ntypeName) {
    	this.ntypeName = ntypeName;
    }


	
    public String getStrDateString() {
    	return strDateString;
    }


	
    public void setStrDateString(String strDateString) {
    	this.strDateString = strDateString;
    }
	
	
}
