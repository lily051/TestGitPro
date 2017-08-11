/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月29日 下午1:54:48
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.common.enums;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * <p>报警类型的枚举类</p>
 * @author jinxindong 2016年2月29日 下午1:54:48
 * @version V1.0
 */
public enum AlarmTypeEnum {
	 //WORK_TYPE_CENTTER_OFFLINE_60001("60001", "中心不在线"),           
	// WORK_TYPE_TRANSPORT_SVC_OFFLINE_61001("61001", "上传服务不在线"),  
	 WORK_TYPE_TRANSPORT_SVC_FAIL_61002("61002", "上传服务失败"),         
	// WORK_TYPE_GUARD_SVC_OFFLINE_62001("62001", "录像守卫服务不在线"),         
	 WORK_TYPE_GUARD_SVC_FAIL_62002("62002", "录像守卫服务失败"),             
	// WORK_TYPE_ADJUST_SVC_OFFLINE_63001("63001", "巡检服务不在线"),
	 WORK_TYPE_ADJUST_SVC_FAIL_63002("63002", "系统巡检服务失败"),         
	 //WORK_TYPE_PATROL_SVC_FAIL_64001("64001", "DVR巡检失败"),            
	 WORK_TYPE_DVR_OFFLINE_64002("64002", "采集设备不在线"),                
	 WORK_TYPE_DVR_TIME_ERROR_64003("64003", "采集设备时间和本地时间差很大"), 
	 WORK_TYPE_DVR_VI_LOST_64004("64004", "采集设备视频信号丢失"),
	 WORK_TYPE_DVR_HD_EXCEPTION_64005("64005", "采集设备硬盘异常"),
	 //WORK_TYPE_DVR_OPERATION_FAIL_64006("64006", "DVR某些操作失败"),
	 WORK_TYPE_DVR_NEARLLY_FULL_64007("64007", "采集设备磁盘满"),
	 WORK_TYPE_CVR_OFFLINE_65001("65001", "CVR不在线"),
	 //WORK_TYPE_CVR_CANNOT_LOGIN_65002("65002", "CVR登录失败"),
	 WORK_TYPE_CVR_NEARLLY_FULL_65003("65003", "CVR磁盘满"),
	 //WORK_TYPE_CVR_OPERATION_FAIL_65004("65004", "CVR某些操作失败"),    
	 WORK_TYPE_CLOUD_OFFLINE_66001("66001", "Cloud不在线"),           
	 //WORK_TYPE_CLOUD_CANNOT_LOGIN_66002("66002", "Cloud登录失败"), 
	// WORK_TYPE_CLOUD_GET_POOLLIST_FAIL_66003("66003", "获取录像池超时"),
	 WORK_TYPE_CLOUD_NEARLLY_FULL_66004("66004", "Cloud磁盘满"),
	// WORK_TYPE_CLOUD_OPERATION_FAIL_66005("66005", "Cloud某些操作失败"), 
	 WORK_TYPE_KMS_OFFLINE_67001("67001", "KMS不在线"),
	 WORK_TYPE_KMS_NEARLLY_FULL_67002("67002", "KMS磁盘满"),
	 WORK_TYPE_CLOUD_STORAGE_OBJECT_STORAGE_OFFLINE_69001("69001", "云存储-对象存储不在线"),
	 WORK_TYPE_CLOUD_STORAGE_OBJECT_STORAGE_NEARLLY_FULL_69002("69002", "云存储-对象存储磁盘满"); 
	//String holder_rights = EnumStockRight.HSDICT_1210_f.getCode();
	private String code;
	private String describe;
	
	private AlarmTypeEnum(String code, String describe) {
		this.code = code;
		this.describe = describe;
	}
	
	public static Map<String, String> getMap() {
		Map<String, String> map = new HashMap<String, String>();
		for (AlarmTypeEnum item : AlarmTypeEnum.values()) {
			map.put(item.getCode(), item.getDescribe());
		}
		return map;
	}
	
	public static List<AlarmTypeEnum> getList() {
		List<AlarmTypeEnum> stateList = new ArrayList<AlarmTypeEnum>();
		for (AlarmTypeEnum item : AlarmTypeEnum.values()) {
			stateList.add(item);
		}
		return stateList;
	}
	

	
	public String getCode() {
		return code;
	}
	
	public void setCode(String Code) {
		this.code = Code;
	}
	
	public String getDescribe() {
		return describe;
	}
	
	public void setDescribe(String Describe) {
		this.describe = Describe;
	}
}
