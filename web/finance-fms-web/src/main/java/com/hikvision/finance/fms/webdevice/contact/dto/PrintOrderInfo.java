/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-4-25 下午4:57:50
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.webdevice.contact.dto;

import java.util.Date;

/**
 * <p>业务单的打印信息</p>
 * @author xujiangfei 2016-4-25 下午4:57:50
 * @version V1.0 
 */
public class PrintOrderInfo {
	private String name;//客户姓名
	private String creditname;//证件名
	private String creditCode;//证件号
	private String nproId;//业务单号
	private Date businessTime;//交易时间
	private String strGrade;//产品发行机构
	private String productName;//产品名称
	private String productType;//产品类型
	private String proName;//业务员姓名
	private String proUserNo;//业务员编号

	private String pictureStrart;//截图的开始路径
	private String pictureEnd;//截图的结束路径
	
	private Integer prosaverecordId;
	
	
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getNproId() {
		return nproId;
	}
	public void setNproId(String nproId) {
		this.nproId = nproId;
	}
	public Date getBusinessTime() {
		return businessTime;
	}
	public void setBusinessTime(Date businessTime) {
		this.businessTime = businessTime;
	}
	public String getStrGrade() {
		return strGrade;
	}
	public void setStrGrade(String strGrade) {
		this.strGrade = strGrade;
	}
	public String getProductName() {
		return productName;
	}
	public void setProductName(String productName) {
		this.productName = productName;
	}
	public String getProName() {
		return proName;
	}
	public void setProName(String proName) {
		this.proName = proName;
	}
	public String getProUserNo() {
		return proUserNo;
	}
	public void setProUserNo(String proUserNo) {
		this.proUserNo = proUserNo;
	}
	public String getCreditname() {
		return creditname;
	}
	public void setCreditname(String creditname) {
		this.creditname = creditname;
	}
	
	public String getPictureStrart() {
		return pictureStrart;
	}
	public void setPictureStrart(String pictureStrart) {
		this.pictureStrart = pictureStrart;
	}
	public String getPictureEnd() {
		return pictureEnd;
	}
	public void setPictureEnd(String pictureEnd) {
		this.pictureEnd = pictureEnd;
	}
	public String getProductType() {
		return productType;
	}
	public void setProductType(String productType) {
		this.productType = productType;
	}
	
    public String getCreditCode() {
    	return creditCode;
    }
	
    public void setCreditCode(String creditCode) {
    	this.creditCode = creditCode;
    }
	public Integer getProsaverecordId() {
		return prosaverecordId;
	}
	public void setProsaverecordId(Integer prosaverecordId) {
		this.prosaverecordId = prosaverecordId;
	}
	
}
