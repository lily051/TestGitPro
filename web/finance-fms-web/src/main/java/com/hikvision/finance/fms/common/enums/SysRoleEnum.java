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
 * <p>
 * 系统类型的枚举类
 * </p>
 * @author jinxindong 2016年2月29日 下午1:54:48
 * @version V1.0
 */
public enum SysRoleEnum {
	CENTRY_ADMIN_0("0", "中心管理员"), 
	FINANC_MANAGER_1("1", "理财经理");
	//String holder_rights = EnumStockRight.HSDICT_1210_f.getCode();
	private String code;
	private String describe;
	
	private SysRoleEnum(String code, String describe) {
		this.code = code;
		this.describe = describe;
	}
	
	public static Map<String, String> getMap() {
		Map<String, String> map = new HashMap<String, String>();
		for (SysRoleEnum item : SysRoleEnum.values()) {
			map.put(item.getCode(), item.getDescribe());
		}
		return map;
	}
	
	public static List<SysRoleEnum> getList() {
		List<SysRoleEnum> stateList = new ArrayList<SysRoleEnum>();
		for (SysRoleEnum item : SysRoleEnum.values()) {
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
