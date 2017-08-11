package com.hikvision.finance.example.util;

import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLEncoder;
import java.util.Date;
import com.hikvision.finance.example.model.Cloud;
import com.hikvision.ivms.hikcstor.service.HttpCommonService;
import com.hikvision.ivms.hikcstor.service.JBucketService;
import com.hikvision.ivms.hikcstor.service.JObjectService;
import com.hikvision.ivms.hikcstor.utils.XmlParser;

/**
 * 标准云对象存储接入工具类
 * @author gengqiaolei
 */
public class CloudUtil {
	/**
	 * 获取JBucketService
	 * @author gengqiaolei
	 * @param ip
	 * @param port
	 * @param accesskey
	 * @param secretkey
	 */
	public static JBucketService getJBucketService(String ip, String port, String accesskey, String secretkey) {
		String host = "http://" + ip + ":" + port;
		return new JBucketService(host, accesskey, secretkey);
	}
	
	/**
	 * 获取JBucketService
	 * @author gengqiaolei
	 * @param cloud
	 */
	public static JBucketService getJBucketService(Cloud cloud) {
		return getJBucketService(cloud.getIp(), cloud.getPort(), cloud.getAccesskey(), cloud.getSecretkey());
	}
	
	/**
	 * 获取JObjectService
	 * @author gengqiaolei
	 * @param ip
	 * @param port
	 * @param accesskey
	 * @param secretkey
	 */
	public static JObjectService getJObjectService(String ip, String port, String accesskey, String secretkey) {
		String host = "http://" + ip + ":" + port;
		return new JObjectService(host, accesskey, secretkey);
	}
	
	/**
	 * 获取JObjectService
	 * @author gengqiaolei
	 * @param cloud
	 */
	public static JObjectService getJObjectService(Cloud cloud) {
		return getJObjectService(cloud.getIp(), cloud.getPort(), cloud.getAccesskey(), cloud.getSecretkey());
	}
	
	/**
	 * 获取鉴权摘要
	 * @author gengqiaolei
	 * @param method
	 * @param date
	 * @param bucket
	 * @param fileName
	 * @param fileDisplayName
	 */
	@SuppressWarnings("deprecation")
	public static String getStringToSign(String method, Date date, String bucket, String fileName, String fileDisplayName) {
		StringBuffer sb = new StringBuffer();
		sb.append(method);
		sb.append("\n");
		sb.append("\n");
		sb.append("multipart/form-data");
		sb.append("\n");
		if(date != null) {
			sb.append(date.toGMTString());
		}
		sb.append("\n");
		sb.append(fileDisplayName);
		sb.append("\n");
		sb.append("/" + bucket);
		sb.append("\n");
		sb.append(fileName);
		sb.append("\n");
		sb.append("\n");
		return sb.toString();
	}
	
	/**
	 * 获取签名
	 * @author gengqiaolei
	 * @param stringToSign
	 * @param secretKey
	 */
	public static String getSignature(String stringToSign, String secretKey) throws Exception {
		return new String(HmacSHAUtil.base64Encode(HmacSHAUtil.HmacSHA1Encrypt(stringToSign, secretKey)));
	}
	
	/**
	 * 获取下载信息
	 * @author gengqiaolei
	 * @param fileName
	 * @param fileDisplayName
	 * @param bucketName
	 * @param ip
	 * @param port
	 * @param accesskey
	 * @param secretkey
	 */
	@SuppressWarnings("deprecation")
	public static String[] getDownLoadUrl(String fileName, String fileDisplayName, String bucketName, String ip, String port, String accesskey, String secretkey) throws Exception {
		String urlString = bucketName + "/" + fileName;
		URL url = new URL("http://" + ip + ":" + port + "/" + urlString);
		Date date = new Date();
		HttpURLConnection conn = (HttpURLConnection)url.openConnection();
		conn.setInstanceFollowRedirects(false);
		conn.setRequestMethod("GET");
		conn.setRequestProperty("Range", "bytes= 0-");
		conn.setRequestProperty("Date", date.toGMTString());
		String authorization = getAuthorization("GET", date, bucketName, urlString, accesskey, secretkey);
		conn.setRequestProperty("Authorization", authorization);
		conn.connect();
		String redirectUrl = conn.getHeaderField("Location");
		httpErrorProcess(conn, "requestDownLoadString");
		// 1，计算urlStr
		int startdot = redirectUrl.indexOf("//");
		String redirectTmepHost = redirectUrl.substring(startdot + 2);
		int dot = redirectTmepHost.indexOf("/");
		String hostIp = redirectTmepHost.substring(0, dot);
		String urlStr = "http://" + hostIp + "/" + bucketName + "/" + fileName;
		// 2，计算token
		String fdn = (URLEncoder.encode(fileDisplayName, "utf-8")).replace("+", " ");
		String auth = URLEncoder.encode(fdn, "utf-8");
		String tokenUrl = bucketName + "/" + fileName + "?filename=" + auth;
		if(auth == null) {
			tokenUrl = bucketName + "/" + fileName;
		}
		String token = getAuthorization("GET", null, bucketName, tokenUrl, accesskey, secretkey);
		token = new String(HmacSHAUtil.base64Encode(token.getBytes()));
		String[] strArray = new String[3];
		strArray[0] = urlStr;
		strArray[1] = token;
		strArray[2] = fdn;
		return strArray;
	}
	
	/**
	 * 获取下载信息
	 * @author gengqiaolei
	 * @param objectName
	 * @param fileName
	 * @param bucketName
	 * @param cloud
	 */
	public static String[] getDownLoadUrl(String objectName, String fileName, String bucketName, Cloud cloud) throws Exception {
		return getDownLoadUrl(objectName, fileName, bucketName, cloud.getIp(), cloud.getPort(), cloud.getAccesskey(), cloud.getSecretkey());
	}
	
	/** ------------------------------ 以下私有方法 ---------------------------- **/
	/**
	 * 获取Authorization
	 * @author gengqiaolei
	 * @param method
	 * @param date
	 * @param bucketName
	 * @param uri
	 * @param accesskey
	 * @param secretkey
	 */
	private static String getAuthorization(String method, Date date, String bucketName, String uri, String accesskey, String secretkey) throws Exception {
		StringBuffer sb = new StringBuffer();
		sb.append("hikcstor");
		sb.append(" ");
		sb.append(accesskey);
		sb.append(":");
		sb.append(new String(HmacSHAUtil.base64Encode(HmacSHAUtil.HmacSHA1Encrypt(getCreateBucketStrSign(method, bucketName, date, uri), secretkey))));
		return sb.toString();
	}
	
	/**
	 * 获取摘要
	 * @author gengqiaolei
	 * @param method
	 * @param bucketName
	 * @param date
	 * @param uri
	 */
	@SuppressWarnings("deprecation")
	private static String getCreateBucketStrSign(String method, String bucketName, Date date, String uri) {
		StringBuffer sb = new StringBuffer();
		sb.append(method);
		sb.append("\n");
		sb.append("\n");
		sb.append("\n");
		if(null != date) {
			sb.append(date.toGMTString());
		}
		sb.append("\n");
		sb.append("\n");
		if(null != uri) {
			sb.append("/" + uri);
		} else {
			sb.append("/");
		}
		return sb.toString();
	}
	
	/**
	 * 服务端返回异常处理
	 * @author gengqiaolei
	 * @param conn
	 * @param type
	 */
	private static String httpErrorProcess(HttpURLConnection conn, String type) throws Exception {
		InputStream inputStream = null;
		try {
			if(conn.getResponseCode() >= 400) {
				throw new Exception(XmlParser.getErrorMessage(conn.getErrorStream()));
			} else {
				inputStream = conn.getInputStream();
				if("openStream".equals(type)) {
					String v = XmlParser.getNodeValueByXpath(inputStream, "//OpenStreamResult/StreamID/text()");
					if(null == v) {
						throw new Exception("java-api通讯内部异常");
					}
					return v;
				} else if("getStreamListByTimeSegment".equals(type) || "queryTaskList".equals(type)) {
					String responseStr = HttpCommonService.inputStreamToString(inputStream);
					return responseStr;
				} else if("queryTaskStatus".equals(type)) {
					String v = XmlParser.getNodeValueByXpath(inputStream, "//QueryTaskStatus/Status/text()");
					if(null == v) {
						throw new Exception("java-api通讯内部异常");
					}
					return v;
				}
			}
		} finally {
			if(null != inputStream) {
				inputStream.close();
			}
			if(null != conn) {
				conn.disconnect();
			}
		}
		return null;
	}
}