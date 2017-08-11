/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月21日 下午6:13:12
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.product.dto;

/**
 * <p>
 * </p>
 * @author jinxindong 2016年3月21日 下午6:13:12
 * @version V1.0
 */
public class ProducttypeInfo {
	
	private Integer id;
	private Integer pid;
	private String strPath;;
	private String strName;
	private Integer nsortId;
	private String strInterAreaCode;
	private Boolean status;
	private String nparentName;//父节点名称
	
	public Integer getId() {
		return id;
	}
	
	public void setId(Integer id) {
		this.id = id;
	}
	
	public Integer getPid() {
		return pid;
	}
	
	public void setPid(Integer pid) {
		this.pid = pid;
	}
	
    public String getStrPath() {
    	return strPath;
    }

	
    public void setStrPath(String strPath) {
    	this.strPath = strPath;
    }

	public String getStrName() {
		return strName;
	}
	
	public void setStrName(String strName) {
		this.strName = strName;
	}
	
	public Integer getNsortId() {
		return nsortId;
	}
	
	public void setNsortId(Integer nsortId) {
		this.nsortId = nsortId;
	}

	
    public String getStrInterAreaCode() {
    	return strInterAreaCode;
    }

	
    public void setStrInterAreaCode(String strInterAreaCode) {
    	this.strInterAreaCode = strInterAreaCode;
    }

	
    public Boolean getStatus() {
    	return status;
    }

	
    public void setStatus(Boolean status) {
    	this.status = status;
    }

	
    public String getNparentName() {
    	return nparentName;
    }

	
    public void setNparentName(String nparentName) {
    	this.nparentName = nparentName;
    }
	
}
