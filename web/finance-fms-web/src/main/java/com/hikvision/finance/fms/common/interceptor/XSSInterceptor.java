/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月22日 下午1:45:25
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.common.interceptor;

import java.util.Iterator;
import java.util.Map;
import java.util.Map.Entry;

import javax.servlet.http.HttpServletRequest;

import org.apache.struts2.ServletActionContext;
import org.apache.struts2.dispatcher.multipart.MultiPartRequestWrapper;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.hikvision.finance.core.util.StringUtils;
import com.opensymphony.xwork2.ActionInvocation;
import com.opensymphony.xwork2.interceptor.MethodFilterInterceptor;

/**
 * XSS防御拦截器
 * @author jinxindong 2016年2月22日 下午1:45:25
 * @version V1.0
 */
public class XSSInterceptor extends MethodFilterInterceptor {
	
	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 3764092812641881178L;
	private static Logger logger = LoggerFactory.getLogger(XSSInterceptor.class);
	private String excludedParams;
	private String[] excludedParamArray;
	
	/*
	 * (non-Javadoc)
	 * @see com.opensymphony.xwork2.interceptor.MethodFilterInterceptor#doIntercept(com.opensymphony.xwork2.ActionInvocation)
	 */
	@Override
	protected String doIntercept(ActionInvocation invocation) throws Exception {
		dataInitialize(); // 白名单初始化
		HttpServletRequest request = ServletActionContext.getRequest();
		// 验证是否 文件上传, 含文件上传的请求xss自行处理
		if (request instanceof MultiPartRequestWrapper) {
			return invocation.invoke();
		}		
		Map<String, Object> map = invocation.getInvocationContext().getParameters();
		if (map != null && map.size() > 0) {
			for (Iterator<Entry<String, Object>> it = map.entrySet().iterator(); it.hasNext();) {
				Map.Entry<String, Object> entry = it.next();
				String name = entry.getKey();
				Object obj = entry.getValue();
				if (obj.getClass().getSimpleName().equals("String[]")) {
					for (int i = 0; i < ((String[])obj).length; i++) {
						((String[])obj)[i] = htmlEncode(name, ((String[])obj)[i]);
					}
				} else {
					// 记录请求中的特殊类型于日志，用于下一次优化
					if (obj.getClass().getSimpleName().equals("File[]")) {
						logger.warn("XSSInterceptor:请求中存在非字符串数组/非文件数组的类型，是：" + name + ":" + obj.getClass().getSimpleName());
					}
				}
			}
		}
		return invocation.invoke();
	}
	
	/**
	 * Html编码
	 * @author jinxindong 2016年2月22日 下午1:48:07
	 * @param name
	 * @param str
	 * @return
	 */
	private String htmlEncode(String name, String str) {
		if (str == null || str.trim().equals("") || inExcludedParamArray(name))
			return str;
		StringBuilder encodeStrBuilder = new StringBuilder();
		for (int i = 0; i < str.length(); i++) {
			encodeStrBuilder.append(htmlEncode(str.charAt(i)));
		}
		return encodeStrBuilder.toString();
	}
	
	/**
	 * Html编码
	 * @author jinxindong 2016年2月22日 下午1:48:27
	 * @param c
	 * @return
	 */
	private String htmlEncode(char c) {
		switch (c) {
			case '&':
				return "&amp;";
			case '<':
				return "&lt;";
			case '>':
				return "&gt;";
			case '\"':
				return "&quot;";
			case '\'':
				return "&apos;";
			default:
				return c + "";
		}
	}
	
	/**
	 * 白名单初始化
	 * @author jinxindong 2016年2月22日 下午1:49:06
	 */
	private void dataInitialize() {
		if (excludedParamArray == null) {
			if (StringUtils.isNotBlank(excludedParams)) {
				excludedParamArray = excludedParams.split(",");
				for (int i = 0; i < excludedParamArray.length; i++) {
					excludedParamArray[i] = excludedParamArray[i].trim();
				}
			} else {
				excludedParamArray = new String[0];
			}
		}
	}
	
	/**
	 * 判断参数是否存在于白名单
	 * @author jinxindong 2016年2月22日 下午1:49:13
	 * @param param
	 * @return
	 */
	private boolean inExcludedParamArray(String param) {
		if (excludedParamArray != null && excludedParamArray.length > 0) {
			for (String str : excludedParamArray) {
				if (str.equals(param)) {
					return true;
				}
			}
		}
		return false;
	}
	
	public String getExcludedParams() {
		return excludedParams;
	}
	
	public void setExcludedParams(String excludedParams) {
		this.excludedParams = excludedParams;
	}
}
