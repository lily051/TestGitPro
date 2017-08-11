/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月30日 下午7:17:09
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.modules.sysconfig.dto;


/**
 * <p>日志保存天数包装类</p>
 * @author jinxindong 2016年3月30日 下午7:17:09
 * @version V1.0 
 */
public class LogParamInfo {

	private  String  configLog; //配置日志
	private String  systemLog; //系统日志
	private String  ndeleteLog; //删除日志
	private String  reviewLog; //回放日志
	private String  alarmLog; //报警日志
	private String  operaLog; //操作日志
	
    public String getConfigLog() {
    	return configLog;
    }
	
    public void setConfigLog(String configLog) {
    	this.configLog = configLog;
    }
	
    public String getSystemLog() {
    	return systemLog;
    }
	
    public void setSystemLog(String systemLog) {
    	this.systemLog = systemLog;
    }
	
    public String getNdeleteLog() {
    	return ndeleteLog;
    }
	
    public void setNdeleteLog(String ndeleteLog) {
    	this.ndeleteLog = ndeleteLog;
    }
	
    public String getReviewLog() {
    	return reviewLog;
    }
	
    public void setReviewLog(String reviewLog) {
    	this.reviewLog = reviewLog;
    }

	
    public String getAlarmLog() {
    	return alarmLog;
    }

	
    public void setAlarmLog(String alarmLog) {
    	this.alarmLog = alarmLog;
    }

	
    public String getOperaLog() {
    	return operaLog;
    }

	
    public void setOperaLog(String operaLog) {
    	this.operaLog = operaLog;
    }
	
    
	
	
}
