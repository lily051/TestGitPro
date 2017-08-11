/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月22日 下午1:52:17
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.common.interceptor;

import java.io.IOException;
import java.net.URLEncoder;
import java.sql.SQLException;
import java.util.Locale;
import java.util.Map;
import java.util.Set;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts2.ServletActionContext;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.hikvision.finance.core.util.CollectionUtils;
import com.hikvision.finance.core.util.JsonUtils;
import com.hikvision.finance.core.util.StringUtils;
import com.hikvision.finance.fms.common.util.StringUtilExpand;
import com.hikvision.finance.fwork.core.notify.NotifyManager;
import com.hikvision.finance.fwork.exception.ExpectedException;
import com.hikvision.finance.fwork.exception.ProgramException;
import com.hikvision.finance.fwork.util.ContentType;
import com.hikvision.finance.fwork.util.ServletUtil;
import com.hikvision.finance.fwork.util.errorcode.ErrorCode;
import com.hikvision.finance.fwork.util.errorcode.ErrorCodeUtil;
import com.hikvision.finance.fwork.www.filter.ResultCode;
import com.hikvision.finance.fwork.www.interceptor.ValidationResult;
import com.opensymphony.xwork2.ActionContext;
import com.opensymphony.xwork2.ActionInvocation;
import com.opensymphony.xwork2.interceptor.AbstractInterceptor;
import com.opensymphony.xwork2.util.LocalizedTextUtil;

/**
 * <p>
 * </p>
 * @author jinxindong 2016年2月22日 下午1:52:17
 * @version V1.0
 */
public class HikExceptionInterceptor extends AbstractInterceptor {
	
	/**
     * 序列化ID
     */
    private static final long serialVersionUID = 8099483666234204683L;

	private final static Logger log = LoggerFactory.getLogger(HikExceptionInterceptor.class);
	
	private final static String ERRORCODE_PRE = "errorcode.";
	
	/*
	 * (non-Javadoc)
	 * @see com.opensymphony.xwork2.interceptor.AbstractInterceptor#intercept(com.opensymphony.xwork2.ActionInvocation)
	 */
	@Override
	public String intercept(ActionInvocation invocation) throws Exception {
		String result = null;
		try {
			result = invocation.invoke();
		} catch (Exception e) {
			String actionName = invocation.getAction().getClass().getName();
			String method = invocation.getProxy().getMethod();
			// 记录文本错误日志
			String errorCode = logException(e, actionName, method, e);
			if (StringUtils.isEmpty(errorCode)) {
				errorCode = "0";
			}
			String errorInfo = findGlobalErrorInfo(e);
			String errorField = "";
			HttpServletResponse response = ServletActionContext.getResponse();
			HttpServletRequest request = ServletActionContext.getRequest();
			if (ServletUtil.isAjaxRequest(request)) {
				boolean success = false;
				if (e instanceof ExpectedException) {
					success = true;
					errorInfo = ((ExpectedException)e).getMessage();
					if (StringUtils.isEmpty(errorInfo)) {
						errorInfo = findErrorInfo((ProgramException)e);
					}
					response.setStatus(ResultCode.EXPECT_EXCEPTION_STATUS);
					response.setHeader("ErrorCode", "0");
					String message = URLEncoder.encode(errorInfo, "utf-8");
					// 后台将空格编码为'+'，但前台仅将'%20'解码为空格
					response.setHeader("ErrorMsg", StringUtilExpand.replace(message, "+", "%20"));
					// FIXME:待扩展
					response.setHeader("LogOut", "false");
				} else if (e instanceof ProgramException) {// 如果请求时ajax，则直接写错误数据，然后不返回任何页面
					ProgramException ex = (ProgramException)e;
					errorInfo = findErrorInfo(ex);
					errorField = findErrorField(ex);
					success = true;
					writeAjaxResponse(success, errorCode, errorInfo, errorField);
				} else {
					success = true;
					response.setStatus(ResultCode.SYSTEM_EXCEPTION_STATUS);
					response.setHeader("ErrorCode", errorCode);
					response.setHeader("ErrorMsg", StringUtilExpand.replace(URLEncoder.encode(errorInfo, "utf-8"), "+", "%20"));
					// FIXME:待扩展
					response.setHeader("LogOut", "false");
				}
				return null;
			} else {
				request.setAttribute("ErrorCode", errorCode);
				if (e instanceof ExpectedException) {
					errorInfo = ((ExpectedException)e).getMessage();
					if (StringUtils.isEmpty(errorInfo)) {
						errorInfo = findErrorInfo((ProgramException)e);
					}
					response.setStatus(ResultCode.EXPECT_EXCEPTION_STATUS);
				} else if (e instanceof ProgramException) {// 如果请求时ajax，则直接写错误数据，然后不返回任何页面
					ProgramException ex = (ProgramException)e;
					errorInfo = findErrorInfo(ex);
					errorField = findErrorField(ex);
				}
				request.setAttribute("ErrorMsg", errorInfo);
				request.setAttribute("ErrorField", errorField);
				// FIXME:待扩展
				response.setHeader("LogOut", "false");
			}
			throw e;
		} finally {
			// 清除当前线程的所有通知消息
			NotifyManager.cancelNotify();
		}
		return result;
	}
	
	/**
	 * 将异常记录到日志中,并返回errorcode
	 * @author jinxindong 2016年2月22日 下午1:58:52
	 * @param e 异常
	 * @param actionName 发生异常的action名称
	 * @param method  发生异常的方法名
	 * @param rootException 根异常
	 * @return
	 */
	private String logException(Throwable e, String actionName, String method, Throwable rootException) {
		try {
			if (e instanceof ExpectedException) {
				return appendLog(actionName, method, rootException, e.getClass().getSimpleName());
			}
			if (e.getCause() != null) {
				Throwable causeException = e.getCause();
				if (causeException instanceof SQLException) {// 数据库异常
					return appendLog(actionName, method, rootException, "database");
				} else {
					return logException(causeException, actionName, method, rootException);
				}
			} else {// 其他异常
				return appendLog(actionName, method, rootException, null);
			}
		} catch (Throwable exx) {
			return "";
		}
	}
	
	/**
	 * 全局错误信息
	 * @author jinxindong 2016年2月22日 下午1:56:04
	 * @param e
	 * @return
	 */
	private String findGlobalErrorInfo(Throwable e) {
		String errorInfo = "操作失败";
		if (e.getCause() != null) {
			Throwable causeException = e.getCause();
			if (causeException instanceof SQLException) {// 数据库异常
				ErrorCode eCode = ErrorCodeUtil.getGlobalErrorCode("database");
				if (eCode != null) {
					return eCode.getName();
				}
			} else {
				String eInfo = findGlobalErrorInfo(causeException);
				if (!StringUtils.isEmpty(eInfo)) {
					return eInfo;
				}
			}
		}
		return errorInfo;
	}
	
	/**
	 * 用ajax的方式将数据返回到前台
	 * @author jinxindong 2016年2月22日 下午1:56:19
	 * @param success
	 * @param errorCode  错误码
	 * @param errorMsg   错误信息
	 * @param errorField  错误对应的field名称
	 * @throws IOException
	 */
	private void writeAjaxResponse(boolean success, String errorCode, String errorMsg, String errorField) throws IOException {
		ValidationResult result = new ValidationResult();
		result.setSuccess(success);
		result.setErrorCode(errorCode);
		if (StringUtils.isEmpty(errorMsg)) {
			errorMsg = "";
		}
		if (StringUtils.isEmpty(errorField)) {
			result.addActionError(errorMsg);
		} else {
			result.addFieldError(errorField, errorMsg);
		}
		ServletUtil.writeResponse(ServletActionContext.getResponse(), JsonUtils.object2Json(result), ContentType.HTML);
	}
	
	/**
	 * 重组异常信息
	 * @author jinxindong 2016年2月22日 下午1:57:41
	 * @param logInfo  异常信息
	 * @param errorCode 错误码
	 * @return
	 */
	@SuppressWarnings("unchecked")
	private String buildLogInfoForAction(String logInfo, ErrorCode errorCode) {
		HttpServletRequest request = ServletActionContext.getRequest();
		Map<String, String[]> parameterMap = request.getParameterMap();
		Set<Map.Entry<String, String[]>> entrySet = parameterMap.entrySet();
		String[] parameterName = null;
		Object[] parameterValue = null;
		if (!CollectionUtils.isEmpty(entrySet)) {
			parameterName = new String[entrySet.size()];
			parameterValue = new Object[entrySet.size()];
			int count = 0;
			for (Map.Entry<String, String[]> entry : entrySet) {
				parameterName[count] = entry.getKey();
				StringBuilder values = new StringBuilder();
				if (entry.getValue() == null) {
					parameterValue[count++] = null;
				} else {
					for (String value : entry.getValue()) {
						values.append(value);
					}
					parameterValue[count++] = String.valueOf(values);
				}
			}
		}
		String bussiness = errorCode.findBussiness();
		String module = errorCode.findModule();
		String logString = buildLogInfo(errorCode.getAllCode(), logInfo, bussiness, module, parameterName, parameterValue);
		return logString;
	}
	
	/**
	 * 错误码补齐0
	 * @author jinxindong 2016年2月22日 下午1:57:52
	 * @param length 错误码长度
	 * @param src   错误码
	 * @return
	 */
	private String preFixZero(int length, String src) {
		int srcLength = src.length();
		if (length <= srcLength) {
			return src;
		}
		StringBuffer preStr = new StringBuffer();
		for (int i = 0; i < length - srcLength; i++) {
			preStr.append("0");
		}
		return preStr + src;
	}
	
	/**
	 * 根据错误发生的位置组建错误信息
	 * @author jinxindong 2016年2月22日 下午1:58:06
	 * @param errorCode  错误码
	 * @param logInfo    日志信息
	 * @param bussiness  出现错误的业务
	 * @param module     出现错误模块
	 * @param parameterName  参数名称
	 * @param parameterValue  参数值
	 * @return
	 */
	public String buildLogInfo(String errorCode, String logInfo, String bussiness, String module, String[] parameterName,
	        Object[] parameterValue) {
		StringBuilder sb = new StringBuilder();
		sb.append("[hik_errcode=").append(preFixZero(32, errorCode)).append("] ");
		sb.append(logInfo).append(",").append(module).append(",").append(bussiness);
		if (parameterName != null && parameterValue != null && parameterName.length == parameterValue.length) {
			int parameterCount = parameterName.length;
			for (int i = 0; i < parameterCount; i++) {
				if (parameterName[i] == null) {
					continue;
				}
				if (parameterValue[i] == null) {
					parameterValue[i] = "";
				}
				if (i == 0) {
					sb.append(",");
				}
				sb.append(parameterName[i]).append("=").append(parameterValue[i]);
				if (i != parameterCount - 1) {
					sb.append(",");
				}
			}
		}
		return sb.toString();
	}
	
	/**
	 * 记录错误日志
	 * @author jinxindong 2016年2月22日 下午2:01:03
	 * @param actionName 发生错误的action名称
	 * @param method 发生错误的方法
	 * @param e 异常
	 * @param globalId 异常的全局ID
	 * @return
	 */
	public String appendLog(String actionName, String method, Throwable e, String globalId) {
		ErrorCode errorCode = null;
		if (StringUtils.isEmpty(globalId)) {
			errorCode = ErrorCodeUtil.getErrorCode(actionName, method);
		} else {
			errorCode = ErrorCodeUtil.getGlobalErrorCode(globalId);
		}
		String errorCodeStr = "-1";
		if (errorCode != null) {
			errorCodeStr = errorCode.getAllCode();
			String logInfo = errorCode.getName();
			if (StringUtils.isEmpty(logInfo)) {
				logInfo = "未注册错误";
			}
			if (e instanceof ProgramException) {
				logServiceErrorInfo(errorCode, e);
			} else if (e instanceof ProgramException) {
				String logString = buildLogInfoForAction(logInfo, errorCode);
				log.error(logString, e.getMessage());
			} else {
				String logString = buildLogInfoForAction(logInfo, errorCode);
				log.error(logString, e);
			}
		} else {
			log.error("expetion not regist.", e);
		}
		return errorCodeStr;
	}
	
	private void logServiceErrorInfo(ErrorCode errorCode, Throwable e) {
		ProgramException ex = (ProgramException)e;
		String logInfo = findErrorInfo(ex);
		String logString = buildLogInfoForAction(logInfo, errorCode);
		log.error(logString, e.getMessage());
	}
	
	/**
	 * 从资源配置文件中获取错误信息
	 * @author jinxindong 2016年2月22日 下午2:01:49
	 * @param ex
	 * @return
	 */
	private String findErrorInfo(ProgramException ex) {
		Locale locale = ActionContext.getContext().getLocale();
		String logInfo = LocalizedTextUtil.findDefaultText(ERRORCODE_PRE + ex.getCode(), locale);
		if (logInfo == null) {
			ErrorCode ec = ErrorCodeUtil.getGlobalErrorCode(ex.getClass().getSimpleName());
			logInfo = ec.getName();
			if (logInfo == null) {
				logInfo = "";
			}
			return logInfo;
		} else if (ex.getArgs() == null || ex.getArgs().length <= 0) {
			return logInfo;
		} else {
			int index = 0;
			for (Object o : ex.getArgs()) {
				logInfo = logInfo.replace("{" + index++ + "}", String.valueOf(o));
			}
		}
		return String.format(logInfo, ex.getArgs());
	}
	
	/**
	 * 从配置文件中去获取错误信息对应的field名称
	 * @author jinxindong 2016年2月22日 下午2:02:16
	 * @param ex
	 * @return
	 */
	private String findErrorField(ProgramException ex) {
		Locale locale = ActionContext.getContext().getLocale();
		String field = LocalizedTextUtil.findDefaultText(ERRORCODE_PRE + ex.getCode() + ".field", locale);
		return field;
	}
	
}
