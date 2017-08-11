/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月22日 下午2:04:26
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.common.util;

import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.Random;

import com.hikvision.finance.core.util.StringUtils;

/**
 * <p>
 * </p>
 * @author jinxindong 2016年2月22日 下午2:04:26
 * @version V1.0
 */
public class StringUtilExpand extends StringUtils {
	
	/**
	 * 字符串转换成整型数组
	 * @author jinxindong 2016年2月22日 下午2:05:01
	 * @param value 以逗号隔开的数字字符串
	 * @return
	 */
	public static Integer[] strToIntArray(String value) {
		String[] strArr = value.split(",");
		Integer[] mIds = new Integer[strArr.length];
		for (int i = 0; i < strArr.length; i++) {
			mIds[i] = Integer.valueOf(strArr[i]);
		}
		return mIds;
	}
	
	/**
	 * 获取对应长度的字符串，不够补0;
	 * @author jinxindong 2016年2月22日 下午2:05:18
	 * @param str
	 * @param length
	 * @return
	 */
	public static String getLengthStr(String str, int length) {
		if (str == null) {
			return "100000";
		}
		if (str.length() >= length) {
			return str.substring(0, length);
		}
		return str + String.format("%0" + (length - str.length()) + "d", 0);
	}
	
	/**
	 * Integer转String，各位数前一位补0
	 * @author jinxindong 2016年2月22日 下午2:05:27
	 * @param i
	 * @return
	 */
	public static String integer2Str(Integer i) {
		if (i <= 0) {
			return "0";
		} else if (i < 10) {
			return "0" + i.toString();
		} else {
			return i.toString();
		}
	}
	
	// 生成六位随机数
	public static String getSixRandomSeril() {
		Random r = new Random();
		Integer index = 0;
		while (index < 100000) {
			index = r.nextInt(999999);
		}
		return index.toString();
	}
	
	/**
	 * sql模糊匹配特殊字符转义
	 * @author jinchenwu 2016年4月13日 下午3:54:41
	 * @param likeStr
	 * @return
	 */
	public static String escapeSQLLike(String likeStr) {
        String str = StringUtils.replace(likeStr, "/", "//");
        str = StringUtils.replace(str, "_",    "/_");
        str = StringUtils.replace(str, "%",    "/%");
        return str;
    }
	
	public static boolean integerIsNotNull(Integer inte){
		if(null!=inte){
			return true;
		}else{
			return false;
		}
	}
	
	/**
	 * 返回本机的ip
	 * @author xujiangfei 2016-5-17 下午12:50:52
	 * @return
	 */
	public static String getLocalhostIp(){
		String strip = null;
		try {
			InetAddress netAddress = InetAddress.getLocalHost();
			strip = netAddress.getHostAddress();
		} catch (UnknownHostException e) {
			e.printStackTrace();
		}
		return strip;
	}
}
