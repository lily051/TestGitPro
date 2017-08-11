/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年3月11日 上午11:21:32
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.common.enums;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;


/**
 * <p>服务类型的枚举类</p>
 * @author jinxindong 2016年3月11日 上午11:21:32
 * @version V1.0 
 */
public enum ServerTypeEnum {
	MT_WEB_40200("40200","中心WEB服务"),
	MT_SERVICE_VRM_20000("20000", "VRM服务"),
	MT_SERVICE_TIMING_20001("20001", "校时服务"), 
	MT_SERVICE_GUARD_20002("20002", "录像守卫服务"),
	MT_SERVICE_ALARM_20003("20003", "报警服务"),
	MT_SERVICE_UPLOAD_20004("20004", "上传服务");
	//String holder_rights = EnumStockRight.HSDICT_1210_f.getCode();
	private String code;
	private String describe;
	
	private ServerTypeEnum(String code, String describe) {
		this.code = code;
		this.describe = describe;
	}
	
	public static Map<String, String> getMap() {
		Map<String, String> map = new HashMap<String, String>();
		for (ServerTypeEnum item : ServerTypeEnum.values()) {
			map.put(item.getCode(), item.getDescribe());
		}
		return map;
	}
	
	public static List<ServerTypeEnum> getList() {
		List<ServerTypeEnum> stateList = new ArrayList<ServerTypeEnum>();
		for (ServerTypeEnum item : ServerTypeEnum.values()) {
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
