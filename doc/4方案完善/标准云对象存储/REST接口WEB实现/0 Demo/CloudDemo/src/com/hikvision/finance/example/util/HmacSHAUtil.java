package com.hikvision.finance.example.util;

import javax.crypto.Mac;
import javax.crypto.SecretKey;
import javax.crypto.spec.SecretKeySpec;

/**
 * HmacSHA1加密工具类
 * @author gengqiaolei
 */
public class HmacSHAUtil {
	 private static final String MAC_NAME = "HmacSHA1";
	 private static final String ENCODING = "UTF-8";
	 
	 /**
	  * Base64编码
	  * @author gengqiaolei
	  * @param data
	  */
	public static char[] base64Encode(byte[] data) {
		char[] alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=".toCharArray();
		char[] out = new char[((data.length+2)/3)*4];
		for(int i = 0, index = 0; i < data.length; i += 3, index += 4) {
			boolean quad = false;
			boolean trip = false;
			int val = (0xFF & (int)data[i]);
			val <<= 8;
			if((i + 1) < data.length) {
				val |= (0xFF & (int)data[i+1]);
				trip = true;
			}
			val <<= 8;
			if((i + 2) < data.length) {
				val |= (0xFF & (int)data[i+2]);
				quad = true;
			}
			out[index+3] = alphabet[(quad ? (val & 0x3F) : 64)];
			val >>= 6;
			out[index+2] = alphabet[(trip ? (val & 0x3F) : 64)];
			val >>= 6;
			out[index+1] = alphabet[val & 0x3F];
			val >>= 6;
			out[index+0] = alphabet[val & 0x3F];
		}
		return out;
	}

	/**
	 * HmacSHA1加密
	 * @author gengqiaolei
	 * @param encryptText
	 * @param encryptKey
	 */
	public static byte[] HmacSHA1Encrypt(String encryptText, String encryptKey) throws Exception {
		byte[] data = encryptKey.getBytes(ENCODING);
		SecretKey secretKey = new SecretKeySpec(data, MAC_NAME);
		Mac mac = Mac.getInstance(MAC_NAME);
		mac.init(secretKey);
		byte[] text = encryptText.getBytes(ENCODING);
		return mac.doFinal(text);
	}
}