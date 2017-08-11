/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月22日 下午3:00:59
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.common.util;

import java.util.HashMap;
import java.util.Map;


/**
 * 
 * @author jinxindong 2016年2月22日 下午3:00:59
 * @version V1.0
 */
public class ThreadLocalHolder {
	
    private static final ThreadLocal<Map<String, Object>> map = new InheritableThreadLocal<Map<String, Object>>();

	private static <E> Map<String, Object> getMap(){
		Map<String, Object> m=map.get();
		if(m==null){
			m=new HashMap<String, Object>();
		map.set(m);
		}
		return m;
	}
	
	public static void setAttr(String key, Object value){
		getMap().put(key, value);
	}
	
	public static Object getAttr(String key){
		return getMap().get(key);
	}
	
	public static void clear(){
		getMap().clear();
	}

}
