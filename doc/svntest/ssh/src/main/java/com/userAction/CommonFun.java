package com.userAction;


import org.apache.commons.lang3.StringUtils;

import java.io.UnsupportedEncodingException;
import java.net.Inet4Address;
import java.net.InetAddress;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;


public class CommonFun {
	

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

}
