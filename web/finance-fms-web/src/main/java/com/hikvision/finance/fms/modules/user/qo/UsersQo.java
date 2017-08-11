/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月24日 下午6:03:02
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.user.qo;

import java.util.Date;

import com.hikvision.finance.extend.hibernate.qo.BaseQo;


/**
 * <p></p>
 * @author jinxindong 2016年2月24日 下午6:03:02
 * @version V1.0 
 */
public class UsersQo extends BaseQo{
	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 8336693835618005794L;
	private Integer id;
	private String strName;
	private String strCode;
	private Integer sysRole;
	private String strPassword;
	private Integer npasswordLevel;
	private String strTel;
	private String strEmail;
	private String strComment;
	private Integer nstate;
	private Date strDate;
	private Integer nsex;
	private String net;
	private String strDuty;
	private String userNo;
	private Date strUpdatePwdTime;
	private Integer nowner;
	private Integer nupdated;
	private Integer nonline;
	private String strCredentialNo;
	private Date lastTimeOnline;
	private Integer ncrtificateId;
	
	private String strNameLike;
	private String strCodeLike;
	
	// 与其他表关联的链接方式
	//private Integer joinType = JoinFragment.LEFT_OUTER_JOIN;
	
    public Integer getId() {
    	return id;
    }
	
    public void setId(Integer id) {
    	this.id = id;
    }
	
    public String getStrName() {
    	return strName;
    }
	
    public void setStrName(String strName) {
    	this.strName = strName;
    }
	
    public String getStrCode() {
    	return strCode;
    }
	
    public void setStrCode(String strCode) {
    	this.strCode = strCode;
    }
	
    public Integer getSysRole() {
    	return sysRole;
    }
	
    public void setSysRole(Integer sysRole) {
    	this.sysRole = sysRole;
    }
	
    public String getStrPassword() {
    	return strPassword;
    }
	
    public void setStrPassword(String strPassword) {
    	this.strPassword = strPassword;
    }
	
    public Integer getNpasswordLevel() {
    	return npasswordLevel;
    }
	
    public void setNpasswordLevel(Integer npasswordLevel) {
    	this.npasswordLevel = npasswordLevel;
    }
	
    public String getStrTel() {
    	return strTel;
    }
	
    public void setStrTel(String strTel) {
    	this.strTel = strTel;
    }
	
    public String getStrEmail() {
    	return strEmail;
    }
	
    public void setStrEmail(String strEmail) {
    	this.strEmail = strEmail;
    }
	
    public String getStrComment() {
    	return strComment;
    }
	
    public void setStrComment(String strComment) {
    	this.strComment = strComment;
    }
	
    public Integer getNstate() {
    	return nstate;
    }
	
    public void setNstate(Integer nstate) {
    	this.nstate = nstate;
    }
	
    public Date getStrDate() {
    	return strDate;
    }
	
    public void setStrDate(Date strDate) {
    	this.strDate = strDate;
    }
	
    public Integer getNsex() {
    	return nsex;
    }
	
    public void setNsex(Integer nsex) {
    	this.nsex = nsex;
    }
	
    public String getNet() {
    	return net;
    }
	
    public void setNet(String net) {
    	this.net = net;
    }
	
    public String getStrDuty() {
    	return strDuty;
    }
	
    public void setStrDuty(String strDuty) {
    	this.strDuty = strDuty;
    }
	
    public String getUserNo() {
    	return userNo;
    }
	
    public void setUserNo(String userNo) {
    	this.userNo = userNo;
    }
	
    public Date getStrUpdatePwdTime() {
    	return strUpdatePwdTime;
    }
	
    public void setStrUpdatePwdTime(Date strUpdatePwdTime) {
    	this.strUpdatePwdTime = strUpdatePwdTime;
    }
	
    public Integer getNowner() {
    	return nowner;
    }
	
    public void setNowner(Integer nowner) {
    	this.nowner = nowner;
    }
	
    public Integer getNupdated() {
    	return nupdated;
    }
	
    public void setNupdated(Integer nupdated) {
    	this.nupdated = nupdated;
    }
	
    public Integer getNonline() {
    	return nonline;
    }
	
    public void setNonline(Integer nonline) {
    	this.nonline = nonline;
    }
	
    public String getStrCredentialNo() {
    	return strCredentialNo;
    }
	
    public void setStrCredentialNo(String strCredentialNo) {
    	this.strCredentialNo = strCredentialNo;
    }
	
    public Date getLastTimeOnline() {
    	return lastTimeOnline;
    }
	
    public void setLastTimeOnline(Date lastTimeOnline) {
    	this.lastTimeOnline = lastTimeOnline;
    }
	
    public Integer getNcrtificateId() {
    	return ncrtificateId;
    }
	
    public void setNcrtificateId(Integer ncrtificateId) {
    	this.ncrtificateId = ncrtificateId;
    }

	
    public String getStrNameLike() {
    	return strNameLike;
    }

	
    public void setStrNameLike(String strNameLike) {
    	this.strNameLike = strNameLike;
    }

	
    public String getStrCodeLike() {
    	return strCodeLike;
    }

	
    public void setStrCodeLike(String strCodeLike) {
    	this.strCodeLike = strCodeLike;
    }
	
	
}
