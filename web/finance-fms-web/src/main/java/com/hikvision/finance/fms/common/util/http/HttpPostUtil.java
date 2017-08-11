/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月8日 下午6:03:32
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.common.util.http;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.URL;
import java.net.URLConnection;
import java.util.List;
import java.util.Map;

import org.apache.log4j.Logger;

import com.hikvision.finance.fwork.exception.ExpectedException;

/**
 * <p>设备请求http工具类</p>
 * @author jinxindong 2016年4月8日 下午6:03:32
 * @version V1.0
 */
public class HttpPostUtil {
	
	private static Logger logger = Logger.getLogger(HttpPostUtil.class);
	
	/**
	 * GET请求发送数据
	 * @author jinxindong 2016年4月8日 下午6:03:32
	 * @version V1.0
	 */
	public static String sendGet(String url, String param) {
		String result = "";// 访问返回结果
		BufferedReader in = null;// 读取访问结果
		try {
			String urlNameString = url + "?" + param;// 创建url
			URL realUrl = new URL(urlNameString);
			URLConnection connection = realUrl.openConnection();// 打开连接
			connection.setConnectTimeout(10 * 1000);//设置连接主机超时（单位：毫秒）
			connection.setReadTimeout(10 * 1000);//设置从主机读取数据超时（单位：毫秒）
			// 设置通用的请求属性
			connection.setRequestProperty("accept", "*/*");
			connection.setRequestProperty("connection", "Keep-Alive");
			connection.setRequestProperty("user-agent", "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1;SV1)");
			connection.connect();// 建立连接
			
			// 获取所有响应头字段
			Map<String, List<String>> map = connection.getHeaderFields();
			// 遍历所有的响应头字段，获取到cookies等
			for (String key : map.keySet()) {
				logger.info(key + "--->" + map.get(key));
			}
			// 定义 BufferedReader输入流来读取URL的响应
			logger.info("***********************开始发送GET请求数据********************" );
			in = new BufferedReader(new InputStreamReader(connection.getInputStream(),"UTF-8"));
			String line;
			while ((line = in.readLine()) != null) {
				result += line;
			}
		} catch (Exception e) {
			logger.info("发送GET请求出现异常！", e);
			//throw new ExpectedException("", "发送GET请求出现异常！" + e +" ");
			throw new ExpectedException("", "获取kms信息失败");//2016-5-20 17:17  曹工要求改成这个提示，我的意见是网络异常，请检查网络信息
		} finally {
			try {
				if (in != null) {
					in.close();
				}
			} catch (Exception e2) {
				logger.info("GET请求关闭流异常！" + e2);
				e2.printStackTrace();
			}
		}
		logger.info("***********************GET请求数据成功************************" );
		return result;
	}
	

	
	// 提供主方法，测试发送GET请求和POST请求
	public static void main(String args[]) {
		// 发送GET请求
		//String s1 = HttpPostUtil.sendGet("http://10.10.48.101:8080/kms/services/rest/operationService/getTokenInfo", "username=admin&password=hik12345_");
		//String s1 = HttpPostUtil.sendGet("http://10.10.48.101:8080/kms/services/rest/CRL/getSpaceInfo", "token=7a57a5a70000015360ecfe43f56d21d4d9");
		//System.out.println(s1);
		int[] a={1,3,9,6,5,2,4};
		java.util.Arrays.sort(a);
		/*for(int i=0;i<a.length;i++)
		System.out.println(a[i]);*/
	}

}
