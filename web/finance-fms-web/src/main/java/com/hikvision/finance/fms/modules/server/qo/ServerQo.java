/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月11日 下午5:14:07
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.server.qo;

import com.hikvision.finance.extend.hibernate.qo.BaseQo;


/**
 * <p></p>
 * @author jinxindong 2016年3月11日 下午5:14:07
 * @version V1.0 
 */
public class ServerQo extends BaseQo{
	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 6408326976763593508L;
	private Integer id;
	private Integer ntype;
	private String strIp;
	private Integer nctrlPort;
	private Integer ndataPort;
	private String strUser;
	private String strComment;
	private Integer nmultId;
	private Integer nmultVer;
	private Integer nmultSysId;
	private String strConfig;
	private Integer nonline;
	
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
	
    public String getStrIp() {
    	return strIp;
    }
	
    public void setStrIp(String strIp) {
    	this.strIp = strIp;
    }
	
    public Integer getNctrlPort() {
    	return nctrlPort;
    }
	
    public void setNctrlPort(Integer nctrlPort) {
    	this.nctrlPort = nctrlPort;
    }
	
    public Integer getNdataPort() {
    	return ndataPort;
    }
	
    public void setNdataPort(Integer ndataPort) {
    	this.ndataPort = ndataPort;
    }
	
    public String getStrUser() {
    	return strUser;
    }
	
    public void setStrUser(String strUser) {
    	this.strUser = strUser;
    }
	
    public String getStrComment() {
    	return strComment;
    }
	
    public void setStrComment(String strComment) {
    	this.strComment = strComment;
    }
	
    public Integer getNmultId() {
    	return nmultId;
    }
	
    public void setNmultId(Integer nmultId) {
    	this.nmultId = nmultId;
    }
	
    public Integer getNmultVer() {
    	return nmultVer;
    }
	
    public void setNmultVer(Integer nmultVer) {
    	this.nmultVer = nmultVer;
    }
	
    public Integer getNmultSysId() {
    	return nmultSysId;
    }
	
    public void setNmultSysId(Integer nmultSysId) {
    	this.nmultSysId = nmultSysId;
    }
	
    public String getStrConfig() {
    	return strConfig;
    }
	
    public void setStrConfig(String strConfig) {
    	this.strConfig = strConfig;
    }
	
    public Integer getNonline() {
    	return nonline;
    }
	
    public void setNonline(Integer nonline) {
    	this.nonline = nonline;
    }
	
	
	
}
