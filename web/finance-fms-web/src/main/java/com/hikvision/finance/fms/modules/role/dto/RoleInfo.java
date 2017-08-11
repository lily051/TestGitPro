/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月29日 下午3:14:27
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.role.dto;


/**
 * <p></p>
 * @author jinxindong 2016年2月29日 下午3:14:27
 * @version V1.0 
 */
public class RoleInfo {
	private Integer id;
	private String strName;
	private Integer ntype;
	private Integer nright;
	private Integer nowner;
	private String strComment;
	
	private String nownerName;
	private Integer userNum;
	
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
	
    public Integer getNtype() {
    	return ntype;
    }
	
    public void setNtype(Integer ntype) {
    	this.ntype = ntype;
    }
	
    public Integer getNright() {
    	return nright;
    }
	
    public void setNright(Integer nright) {
    	this.nright = nright;
    }
	
    public Integer getNowner() {
    	return nowner;
    }
	
    public void setNowner(Integer nowner) {
    	this.nowner = nowner;
    }
	
    public String getStrComment() {
    	return strComment;
    }
	
    public void setStrComment(String strComment) {
    	this.strComment = strComment;
    }
	
    public String getNownerName() {
    	return nownerName;
    }
	
    public void setNownerName(String nownerName) {
    	this.nownerName = nownerName;
    }
	
    public Integer getUserNum() {
    	return userNum;
    }
	
    public void setUserNum(Integer userNum) {
    	this.userNum = userNum;
    }
	



	

	
	
}
