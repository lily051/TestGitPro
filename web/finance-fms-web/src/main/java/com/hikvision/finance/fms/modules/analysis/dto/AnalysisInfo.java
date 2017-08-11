/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月13日 下午6:16:48
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.analysis.dto;

import java.util.List;
import java.util.Map;


/**
 * <p></p>
 * @author jinxindong 2016年4月13日 下午6:16:48
 * @version V1.0 
 */
public class AnalysisInfo {
	private Integer ntype;//统计方式 0月报表  1季度报表  2年报表
	private Integer nregionId;//所选组织id
	private Integer nregionNum;//所选组织的直接子组织个数 1
	private String recordStart; //开始时间
	private String recordEnd; //结束时间

	private String orderMax; //签约订单总数
	private String complaintMax;//投诉总量
	private String complaintOrder;//投诉率
	
	private Integer checdRecord;//已检查录像
	private String rate;//合规率
	private String rateMaxRgeionName;//合规率最高区域
	private String rateMinRgeionName;//合规率最低区域
	
	//******柱状图表****************
	private Map<String,Double> columnMapOrder;//订单柱状<区域名-数据><x-y>
	private Map<String,Double> columnMapComplaint;//投诉柱状<区域名-数据><x-y>
	
	//******柱状图表****************
	private Map<String,Double> columnMap;//合规柱状<区域名-数据><x-y>
	private List<PieChat> peCheckList;//饼图 数据结构 检查未检查
	private List<PieChat> peList;//饼图 数据结构  合规不合规
	
	private List<PieChat> columnList;//饼状图List
	public Integer getNtype() {
    	return ntype;
    }
	
    public void setNtype(Integer ntype) {
    	this.ntype = ntype;
    }
	
    public Integer getNregionId() {
    	return nregionId;
    }
	
    public void setNregionId(Integer nregionId) {
    	this.nregionId = nregionId;
    }
	
    public Integer getNregionNum() {
    	return nregionNum;
    }
	
    public void setNregionNum(Integer nregionNum) {
    	this.nregionNum = nregionNum;
    }
	
    public String getRecordStart() {
    	return recordStart;
    }
	
    public void setRecordStart(String recordStart) {
    	this.recordStart = recordStart;
    }
	
    public String getRecordEnd() {
    	return recordEnd;
    }
	
    public void setRecordEnd(String recordEnd) {
    	this.recordEnd = recordEnd;
    }
	
    public String getOrderMax() {
    	return orderMax;
    }
	
    public void setOrderMax(String orderMax) {
    	this.orderMax = orderMax;
    }
	

	
    
    public String getComplaintMax() {
    	return complaintMax;
    }

	
    public void setComplaintMax(String complaintMax) {
    	this.complaintMax = complaintMax;
    }

	
    public String getComplaintOrder() {
    	return complaintOrder;
    }

	
    public void setComplaintOrder(String complaintOrder) {
    	this.complaintOrder = complaintOrder;
    }

	public Integer getChecdRecord() {
    	return checdRecord;
    }
	
    public void setChecdRecord(Integer checdRecord) {
    	this.checdRecord = checdRecord;
    }
	
    public String getRate() {
    	return rate;
    }
	
    public void setRate(String rate) {
    	this.rate = rate;
    }
	
    public String getRateMaxRgeionName() {
    	return rateMaxRgeionName;
    }
	
    public void setRateMaxRgeionName(String rateMaxRgeionName) {
    	this.rateMaxRgeionName = rateMaxRgeionName;
    }
	
    public String getRateMinRgeionName() {
    	return rateMinRgeionName;
    }
	
    public void setRateMinRgeionName(String rateMinRgeionName) {
    	this.rateMinRgeionName = rateMinRgeionName;
    }
	

    
    public Map<String, Double> getColumnMap() {
    	return columnMap;
    }

	
    public void setColumnMap(Map<String, Double> columnMap) {
    	this.columnMap = columnMap;
    }

	
    public Map<String, Double> getColumnMapOrder() {
    	return columnMapOrder;
    }

	
    public void setColumnMapOrder(Map<String, Double> columnMapOrder) {
    	this.columnMapOrder = columnMapOrder;
    }

	
    public Map<String, Double> getColumnMapComplaint() {
    	return columnMapComplaint;
    }

	
    public void setColumnMapComplaint(Map<String, Double> columnMapComplaint) {
    	this.columnMapComplaint = columnMapComplaint;
    }

	public List<PieChat> getPeCheckList() {
    	return peCheckList;
    }
	
    public void setPeCheckList(List<PieChat> peCheckList) {
    	this.peCheckList = peCheckList;
    }
	
    public List<PieChat> getPeList() {
    	return peList;
    }
	
    public void setPeList(List<PieChat> peList) {
    	this.peList = peList;
    }

	
    public List<PieChat> getColumnList() {
    	return columnList;
    }

	
    public void setColumnList(List<PieChat> columnList) {
    	this.columnList = columnList;
    }

	



	
   
	
	
}
