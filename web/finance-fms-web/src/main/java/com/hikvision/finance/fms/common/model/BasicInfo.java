/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月22日 下午2:39:58
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.common.model;


/**
 * @author jinxindong 2016年2月22日 下午2:39:58
 * @version V1.0
 */
public class BasicInfo {
	private Integer id;
	private String strName;
	/**
     * 创建一个新的实例BasicInfo.
     */
    public BasicInfo() {
	    super();
	    // TODO Auto-generated constructor stub
    }
	/**
     * 创建一个新的实例BasicInfo.
     * @param id
     * @param strName
     */
    public BasicInfo(Integer id, String strName) {
	    super();
	    this.id = id;
	    this.strName = strName;
    }
	
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
}
