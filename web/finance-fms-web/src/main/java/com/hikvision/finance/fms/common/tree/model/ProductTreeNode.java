/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月22日 下午12:36:36
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.common.tree.model;

import com.hikvision.finance.fwork.util.tree.TreeNode;


/**
 * <p>产品树节点</p>
 * @author jinchenwu 2016年4月22日 下午12:36:36
 * @version V1.0 
 */
public class ProductTreeNode extends TreeNode {
	
	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 5951907306397816823L;
	private String strCode;
	private String saveYears;
	private String guaranteeYears;
	private String strEndDate;
	private String strGrade;
	private String strRisklevel;
	
	public String getStrCode() {
		return strCode;
	}
	
	public void setStrCode(String strCode) {
		this.strCode = strCode;
	}
	
	public String getSaveYears() {
		return saveYears;
	}
	
	public void setSaveYears(String saveYears) {
		this.saveYears = saveYears;
	}
	
	public String getGuaranteeYears() {
		return guaranteeYears;
	}
	
	public void setGuaranteeYears(String guaranteeYears) {
		this.guaranteeYears = guaranteeYears;
	}
	
	public String getStrEndDate() {
		return strEndDate;
	}
	
	public void setStrEndDate(String strEndDate) {
		this.strEndDate = strEndDate;
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
}
