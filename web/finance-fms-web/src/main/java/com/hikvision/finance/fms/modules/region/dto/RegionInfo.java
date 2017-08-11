/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月25日 下午6:16:20
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.region.dto;

import java.util.List;

/**
 * @author jinxindong 2016年2月25日 下午6:16:20
 * @version V1.0
 */
public class RegionInfo {
	
	private Integer id;
	private String strName;
	private Integer nparentId;
	private Integer nrealPlay;
	private Integer nplayBack;
	private Integer nmultId;
	private Integer nmultVer;
	private Integer nmultSysId;
	private String strCode;
	private String strAddress;
	private String strPrincipal;
	private String strSafetyOfficer;
	private String strTxt;
	private Integer nsortId;
	private Integer nlevel;
	private String strPath;
	private String strInterAreaCode;
	private List<Integer>  ids;
	private String nparentName;//父节点名称
	private Integer isShow;
	private Boolean status;
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
	
    public Integer getNparentId() {
    	return nparentId;
    }
	
    public void setNparentId(Integer nparentId) {
    	this.nparentId = nparentId;
    }
	
    public Integer getNrealPlay() {
    	return nrealPlay;
    }
	
    public void setNrealPlay(Integer nrealPlay) {
    	this.nrealPlay = nrealPlay;
    }
	
    public Integer getNplayBack() {
    	return nplayBack;
    }
	
    public void setNplayBack(Integer nplayBack) {
    	this.nplayBack = nplayBack;
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
	
    public String getStrCode() {
    	return strCode;
    }
	
    public void setStrCode(String strCode) {
    	this.strCode = strCode;
    }
	
    public String getStrAddress() {
    	return strAddress;
    }
	
    public void setStrAddress(String strAddress) {
    	this.strAddress = strAddress;
    }
	
    public String getStrPrincipal() {
    	return strPrincipal;
    }
	
    public void setStrPrincipal(String strPrincipal) {
    	this.strPrincipal = strPrincipal;
    }
	
    public String getStrSafetyOfficer() {
    	return strSafetyOfficer;
    }
	
    public void setStrSafetyOfficer(String strSafetyOfficer) {
    	this.strSafetyOfficer = strSafetyOfficer;
    }
	
    public String getStrTxt() {
    	return strTxt;
    }
	
    public void setStrTxt(String strTxt) {
    	this.strTxt = strTxt;
    }
	
    public Integer getNsortId() {
    	return nsortId;
    }
	
    public void setNsortId(Integer nsortId) {
    	this.nsortId = nsortId;
    }
	
    public Integer getNlevel() {
    	return nlevel;
    }
	
    public void setNlevel(Integer nlevel) {
    	this.nlevel = nlevel;
    }
	
    public String getStrPath() {
    	return strPath;
    }
	
    public void setStrPath(String strPath) {
    	this.strPath = strPath;
    }
	
    public String getStrInterAreaCode() {
    	return strInterAreaCode;
    }
	
    public void setStrInterAreaCode(String strInterAreaCode) {
    	this.strInterAreaCode = strInterAreaCode;
    }

	
    public List<Integer> getIds() {
    	return ids;
    }

	
    public void setIds(List<Integer> ids) {
    	this.ids = ids;
    }

	
    public String getNparentName() {
    	return nparentName;
    }

	
    public void setNparentName(String nparentName) {
    	this.nparentName = nparentName;
    }

	
    public Integer getIsShow() {
    	return isShow;
    }

	
    public void setIsShow(Integer isShow) {
    	this.isShow = isShow;
    }

	
    public Boolean getStatus() {
    	return status;
    }

	
    public void setStatus(Boolean status) {
    	this.status = status;
    }

	

	
	
	
	
	
}
