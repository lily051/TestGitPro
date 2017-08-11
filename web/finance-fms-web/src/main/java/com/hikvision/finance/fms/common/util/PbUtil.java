package com.hikvision.finance.fms.common.util;

/**
 * protobuf 工具类
 * 针对protobuf中设置null报空指针异常
 * @author jinxindong 2016年2月22日 下午2:57:07
 * @version V1.0
 */
public class PbUtil {
	
	/**
	 * 防止空指针异常
	 * @author jinxindong 2016年2月22日 下午2:57:19
	 * @param str
	 * @return
	 */
	public static String parseString(String str) {
		return str == null ? "" : str;
	}
	
	/**
	 * 防止空指针异常
	 * @author jinxindong 2016年2月22日 下午2:57:25
	 * @param a
	 * @return
	 */
	public static Integer parseInteger(Integer a) {
		return a == null ? 0 : a;
	}
	
}
