/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月8日 下午4:17:50
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.log.dto;

import java.util.Date;

import org.apache.struts2.json.annotations.JSON;


/**
 * <p></p>
 * @author jinxindong 2016年4月8日 下午4:17:50
 * @version V1.0 
 */
public class ConfiglogInfo {
	private Integer id;
	private Integer ntype;
	private String strText;
	private String strUser;
	private Date strDate;
	private String strDateString;
	private String strIp;
	private Integer nactionType;
	private String strDateStart;
	private String strDateEnd;
	private String ntypeName;
	private String nactionTypeName;
    public Integer getId() {
    	return id;
    }
	
    public void setId(Integer id) {
    	this.id = id;
    }
	
    public Integer getNtype() {
    	return ntype;
    }
	
    public void setNtype(Integer ntype) {
    	this.ntype = ntype;
    }
	
    public String getStrText() {
    	return strText;
    }
	
    public void setStrText(String strText) {
    	this.strText = strText;
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
	
    public Integer getNactionType() {
    	return nactionType;
    }
	
    public void setNactionType(Integer nactionType) {
    	this.nactionType = nactionType;
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

	
    public String getNactionTypeName() {
    	return nactionTypeName;
    }

	
    public void setNactionTypeName(String nactionTypeName) {
    	this.nactionTypeName = nactionTypeName;
    }

	
    public String getStrDateString() {
    	return strDateString;
    }

	
    public void setStrDateString(String strDateString) {
    	this.strDateString = strDateString;
    }

	

	
    
}
