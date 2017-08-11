/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-1-21 上午10:40:42
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.common.excel.model;


/**
 * Excel列信息
 * @author jinxindong 2016年4月21日 下午1:02:23
 * @version V1.0
 */
public class ExcelColumn {
	/**
	 * 列索引
	 */
	private Integer index;
	/**
	 * 实际字段名称
	 */
	private String fieldName;
	/**
	 * 表格中的显示名称
	 */
	private String fieldDispName;
	/**
	 * 字段类型。数字类型还是日期等
	 */
	private Integer type;
	
	public ExcelColumn() {

	}

	public ExcelColumn(int index, String fieldName, String fieldDispName) {
	    this.index = index;
	    this.fieldName = fieldName;
	    this.fieldDispName = fieldDispName;
	}

	public ExcelColumn(int index, String fieldName, String fieldDispName, int type) {
	    this.index = index;
	    this.fieldName = fieldName;
	    this.fieldDispName = fieldDispName;
	    this.type = type;
	}

    public Integer getIndex() {
    	return index;
    }
	
    public void setIndex(Integer index) {
    	this.index = index;
    }

    public String getFieldName() {
    	return fieldName;
    }

    public void setFieldName(String fieldName) {
    	this.fieldName = fieldName;
    }

    public String getFieldDispName() {
    	return fieldDispName;
    }

    public void setFieldDispName(String fieldDispName) {
    	this.fieldDispName = fieldDispName;
    }
	
    public Integer getType() {
    	return type;
    }
	
    public void setType(Integer type) {
    	this.type = type;
    }
	
}
