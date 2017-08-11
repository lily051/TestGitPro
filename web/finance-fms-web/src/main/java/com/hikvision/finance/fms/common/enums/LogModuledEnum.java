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
 * <p>日志模块的枚举类</p>
 * @author jinxindong 2016年2月29日 下午1:54:48
 * @version V1.0
 */
public enum LogModuledEnum {
	REGION("100", "组织管理"),
	SERVER("101", "服务管理"),
	AUTH("102", "权限管理 "),
	DEVICE("103", "资源管理"),
	PRODUCT("104", "产品管理"),
	CONFIG("105", "参数配置"),
	CARD("105001", "参数配置-证件管理"),
	RECORD("106", "理财录像"),
	SALE("107", "销售查询"),
	COUNT("108", "统计分析"),
	ALARM("109", "报警管理"),
	DUERECORD("110", "到期管理"),
	LOG("111", "日志查询");
	//String holder_rights = EnumStockRight.HSDICT_1210_f.getCode();
	private String code;
	private String describe;
	
	private LogModuledEnum(String code, String describe) {
		this.code = code;
		this.describe = describe;
	}
	
	public static Map<String, String> getMap() {
		Map<String, String> map = new HashMap<String, String>();
		for (LogModuledEnum item : LogModuledEnum.values()) {
			map.put(item.getCode(), item.getDescribe());
		}
		return map;
	}
	
	public static List<LogModuledEnum> getList() {
		List<LogModuledEnum> stateList = new ArrayList<LogModuledEnum>();
		for (LogModuledEnum item : LogModuledEnum.values()) {
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
