/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月23日 上午11:26:55
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.webdevice.contact.dto;

import java.util.Date;

import org.apache.struts2.json.annotations.JSON;

/**
 * <p>理财经理选择产品编码专用，只包含基本产品字段，提高性能(话术模板可能包含数千字符，排除在外)</p>
 * @author jinchenwu 2016年4月23日 上午11:26:55
 * @version V1.0 
 */
public class BasicProductInfo {
	
	private Integer id;// 产品Id
	private String strCode;// 产品编码
	private String strName;// 产品名称
	private Date strEndDate;// 产品到期日
	private Integer guaranteeYears;// 产品保障年期
	private Integer guaranteeType;//保障期限类型，年月日
	private Integer saveYears;// 视频保存时间
	private Integer saveType; //保存期限类型 ，年月日
	private String strGrade;// 发行者
	private String strRisklevel;// 风险等级
	private Integer pid;// 父节点(产品类型)Id
	public Integer getId() {
		return id;
	}
	
	public void setId(Integer id) {
		this.id = id;
	}
	
	public String getStrCode() {
		return strCode;
	}
	
	public void setStrCode(String strCode) {
		this.strCode = strCode;
	}
	
	@JSON(format="yyyy-MM-dd")
	public Date getStrEndDate() {
		return strEndDate;
	}
	
	public void setStrEndDate(Date strEndDate) {
		this.strEndDate = strEndDate;
	}
	
	public Integer getGuaranteeYears() {
		return guaranteeYears;
	}
	
	public void setGuaranteeYears(Integer guaranteeYears) {
		this.guaranteeYears = guaranteeYears;
	}
	
	public Integer getSaveYears() {
		return saveYears;
	}
	
	public void setSaveYears(Integer saveYears) {
		this.saveYears = saveYears;
	}
	
	public String getStrGrade() {
		return strGrade;
	}
	
	public void setStrGrade(String strGrade) {
		this.strGrade = strGrade;
	}
	
	public String getStrRisklevel() {
		return strRisklevel;
	}
	
	public void setStrRisklevel(String strRisklevel) {
		this.strRisklevel = strRisklevel;
	}
	
	public Integer getPid() {
		return pid;
	}
	
	public void setPid(Integer pid) {
		this.pid = pid;
	}
	
	public String getStrName() {
		return strName;
	}
	
	public void setStrName(String strName) {
		this.strName = strName;
	}

	public Integer getGuaranteeType() {
	    return guaranteeType;
    }

	public void setGuaranteeType(Integer guaranteeType) {
	    this.guaranteeType = guaranteeType;
    }

	public Integer getSaveType() {
	    return saveType;
    }

	public void setSaveType(Integer saveType) {
	    this.saveType = saveType;
    }
}
