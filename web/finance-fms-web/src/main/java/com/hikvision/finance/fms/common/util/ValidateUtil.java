
package com.hikvision.finance.fms.common.util;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.servlet.http.HttpServletRequest;

import com.google.code.kaptcha.Constants;
import com.hikvision.finance.core.util.StringUtils;

/**
 * 验证工具类
 * @author jinxindong 2016年4月26日 下午3:12:11
 * @version V1.0
 */
public class ValidateUtil {

	/**
	 * 验证码 验证
	 * 
	 * @param request
	 * @param randomCode
	 * @return
	 */
	public static boolean isValidateCode(HttpServletRequest request, String randomCode) {
		String kaptchaCode = (String)request.getSession().getAttribute(Constants.KAPTCHA_SESSION_KEY);
		return (StringUtils.isEmpty(kaptchaCode) || !kaptchaCode.equalsIgnoreCase(randomCode));// 有误则返回
	}
	
	/**
	 * 判断时间是否合法
	 * @author jinxindong 2016年6月15日 下午3:17:01
	 * @param strDate
	 * @return
	 */
	public static boolean validateDate(String strDate) {
		
		boolean convertSuccess = false;
		SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd");// 指定日期格式为四位年/两位月份/两位日期，注意yyyy/MM/dd区分大小写；
		try {
			format.setLenient(false);// 设置lenient为false. 否则SimpleDateFormat会比较宽松地验证日期，比如2007/02/29会被接受，并转换成2007/03/01
			format.parse(strDate);
			convertSuccess = true;
		} catch (ParseException e) {
			// e.printStackTrace();
			// 如果throw java.text.ParseException或者NullPointerException，就说明格式不对
			convertSuccess = false;
		}
		return convertSuccess;
		
	}
	

	/**
	 * 校验Ip的合法性
	 * @author jinxindong 2016年6月15日 下午3:19:17
	 * @param strIp
	 * @return
	 */
	public static boolean validateIp(String strIp) {
		if (strIp != null && !strIp.isEmpty()) {
			// 定义正则表达式
			String regex = "^(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|[1-9])\\." + "(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)\\."
			        + "(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)\\." + "(1\\d{2}|2[0-4]\\d|25[0-5]|[1-9]\\d|\\d)$";
			// 判断ip地址是否与正则表达式匹配
			if (strIp.matches(regex)) {
				return true;
			} else {
				return false;
			}
		}
		return false;
	}
	
	/**
	 * 校验是否含有特殊字符
	 * @author jinxindong 2016年6月15日 下午4:35:48
	 * @param strChar
	 * @return
	 */
	public static boolean validateChar(String strChar) {
		if(StringUtils.isBlank(strChar)){
			return false;
		}
		String regEx = "[\\/:*?\"<|'%>&/]";
		Pattern p = Pattern.compile(regEx);
		Matcher m = p.matcher(strChar);
		boolean match = m.find();
		if (match) {
			return true;
		}
		return false;
	}
	
	public static boolean validateEamil(String strEmail) {
		if(StringUtils.isBlank(strEmail)){
			return true;
		}
		String regEx = "^([a-z0-9A-Z]+[-|_|\\.]?)+[a-z0-9A-Z]@([a-z0-9A-Z]+(-[a-z0-9A-Z]+)?\\.)+[a-zA-Z]{2,}$";
		Pattern p = Pattern.compile(regEx);
		Matcher m = p.matcher(strEmail);
		boolean match = m.find();
		if (match) {
			return true;
		}
		return false;
	}

	
	
	/**
	 * 判断是否含有数字
	 * @author jinxindong 2016年6月30日 上午11:16:21
	 * @param str
	 * @return
	 */
	public static boolean isNumeric(String str){ 

	    Pattern pattern = Pattern.compile(".*[0-9]+.*"); 

	    return pattern.matcher(str).matches();    

	} 
	
	/**
	 * 判断是否含有大写字母
	 * @author jinxindong 2016年6月30日 上午11:18:35
	 * @param str
	 * @return
	 */
	public static boolean isLetter(String str){ 
		
		Pattern pattern = Pattern.compile(".*[A-Z].*"); 
		
		return pattern.matcher(str).matches();    
		
	} 
	
	/**
	 * 判断是否有小写字母
	 * @author jinxindong 2016年6月30日 上午11:23:51
	 * @param str
	 * @return
	 */
	public static boolean isLowerCase(String str){ 
		
		Pattern pattern = Pattern.compile(".*[a-z].*"); 
		
		return pattern.matcher(str).matches();    
		
	} 
	
	/**
	 * 判断是否含有汉字
	 * @author jinxindong 2016年6月30日 下午12:58:49
	 * @param str
	 * @return
	 */
	public static boolean isZHCase(String str){ 
		
		Pattern pattern = Pattern.compile(".*[\\u4e00-\\u9faf].*"); 	
		return pattern.matcher(str).matches();    
		
	} 
	
	/**
	 * 判断是否含有大小写字母和字符串
	 * @author jinxindong 2016年6月30日 上午11:24:01
	 * @param str
	 * @return
	 */
	public static boolean isNumaAndCase(String str){ 
		boolean flag = false;
		if(isNumeric(str)&&isLetter(str)&&isLowerCase(str)){
			flag=true;
		}		
		return flag;    
		
	} 
	
	public static int getPwdRank(String szPwd, String szUser) {
		int iRank = 0;
		char[] ch = szUser.toCharArray();
		StringBuffer sb = new StringBuffer();
		for (int i = ch.length - 1; i >= 0; i--) {
			sb.append(ch[i]);
		}
		String reverse = sb.toString();
		// 判断是否含有小写
		if (isLowerCase(szPwd)) {
			iRank++;
		}
		// 判断是否含有大写
		if (isLetter(szPwd)) {
			iRank++;
		}
		// 判断是否含有数字
		if (isNumeric(szPwd)) {
			iRank++;
		}
		// 判断是否含有数字字母
		if (isNumaAndCase(szPwd)) {
			iRank++;
		}
		iRank = (iRank > 3?3:iRank);
		
		if (szPwd.length() < 8 || iRank == 1 || szUser.equals(szPwd) || reverse.equals(szPwd)) {
			iRank = 0;
		}
		if (iRank == 2) {
			if ((isNumeric(szPwd) && isLowerCase(szPwd)) || isNumeric(szPwd) && isNumaAndCase(szPwd)) {
				iRank = 1;
			}
		}
		return iRank;
	}

	
	
}
