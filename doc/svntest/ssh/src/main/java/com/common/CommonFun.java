package com.common;

import org.apache.commons.lang.StringUtils;

import java.io.UnsupportedEncodingException;
import java.net.Inet4Address;
import java.net.InetAddress;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;


public class CommonFun {
	
	/**
	 * 获取当前时间字符串，格式："yyyy-MM-dd HH:mm:ss"
	 * @return 时间字符串
	 */
	public static String GetCurrentDatetimeString() {
		SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		Date curDate = new Date(System.currentTimeMillis()); // 获取当前时间
		return formatter.format(curDate);
	}
	
	/**
	 * 把8859-1编码转换成UTF-8编码
	 * @param str
	 * @return
	 */
	public static String convertStringEncoding(String str) {
		try {
			return new String(str.getBytes("iso8859-1"), "UTF-8");
		} catch (UnsupportedEncodingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return "Error";
		}
	}
	
	public static String DateToStr(Date date) {
		if (date == null) {
			return "";
		}
		SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd");
		return format.format(date);
	}
	
	public static String DateTimeToStr(Date date) {
		if (date == null) {
			return "";
		}
		SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		return format.format(date);
	}
	
	public static Date StrToDate(String date) {
		if (date == null) {
			return new Date(System.currentTimeMillis());
		}
		SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		try {
			return format.parse(date);
		} catch (ParseException e) {
			
		}
		return new Date(System.currentTimeMillis());
	}
	
	public static Calendar strToCalendar(String date) {
		Calendar happenTime = null;
		if (StringUtils.isNotBlank(date)) {
			date.replace("T", " ");
			happenTime = Calendar.getInstance();
			happenTime.setTime(StrToDate(date));
		}
		return happenTime;
	}
	/**
	 * @Description: 判断基准值是否位于开始值与结束值区间
	 * @author hansong
	 * @Date 2010-10-15
	 */
	public static boolean between(final int base, final int begin, final int end) {
		return base >= begin && base <= end;
	}
	
	/**
	 * @Description: 判断基准值是否位于开始值与结束值区间
	 * @author hansong
	 * @Date 2010-10-15
	 */
	public static String fetchLocIp() {
		String localAddr = "127.0.0.1";
		InetAddress[] localIpArray = null;
		try {
			localIpArray = InetAddress.getAllByName(InetAddress.getLocalHost().getHostName());
		} catch (Exception e) {
		}
		if (localIpArray != null && localIpArray.length > 0) {
			for (InetAddress localIp : localIpArray) {
				if (localIp instanceof Inet4Address) {
					localAddr = localIpArray[0].getHostAddress();
					break;
				}
			}
		}
		return localAddr;
	}
	/**
	 * 将字符串格式时间转换为Calendar
	 * @param date
	 * @param isEnd 如果字符串格式为 yyyy-MM-dd HH:mm:ss，此参数无用。如果格式为yyyy-MM-dd，isEnd:true=补上“23:59:59”， isEnd:false=补上“00:00:00”
	 * @return
	 */
//	public static Calendar strToCalendar(String date, boolean isEnd) {
//		Calendar happenTime = null;
//		if (StringUtil.isNotNullOrEmpty(date)) {
//			date = date.replaceAll("T", " ");
//			happenTime = Calendar.getInstance();
//			happenTime.setTime(StrToDate(date, isEnd));
//		}
//		return happenTime;
//	}
	/**
	 * 将字符串格式时间转换为Date
	 * @param date
	 * @param isEnd 如果字符串格式为 yyyy-MM-dd HH:mm:ss，此参数无用。如果格式为yyyy-MM-dd，isEnd:true=补上“23:59:59”， isEnd:false=补上“00:00:00”
	 * @return
	 */
	public static Date StrToDate(String date, boolean isEnd){
		if (date == null) {
			return new Date(System.currentTimeMillis());
		}
		//判断date 格式是否符合 yyyy-MM-dd HH:mm:ss
		if (date.indexOf(" ") == -1) {
			if(isEnd){
				date += " 23:59:59";
			}else{
				date += " 00:00:00";
			}
		}else{//判断 若date 为 yyyy-MM-dd 00:00:00
			if(isEnd){
				int nIndex = date.indexOf(" ");
				date = date.substring(0, nIndex);
				date += " 23:59:59";
			}
		}
		
		SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		try {
			return format.parse(date);
		} catch (ParseException e) {
			e.printStackTrace();
		}
		return new Date(System.currentTimeMillis());
	}
}
