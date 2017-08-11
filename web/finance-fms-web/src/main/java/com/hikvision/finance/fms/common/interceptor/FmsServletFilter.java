/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月22日 上午10:53:35
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.common.interceptor;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.net.InetAddress;
import java.net.URLEncoder;

import javax.servlet.FilterChain;
import javax.servlet.RequestDispatcher;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.hikvision.cms.cache.manager.DataLoadException;
import com.hikvision.cms.cache.manager.DataManager;
import com.hikvision.finance.core.util.StringUtils;
import com.hikvision.finance.core.util.cm.ConfigManager;
import com.hikvision.finance.fms.common.init.FmsPropertyPlaceholder;
import com.hikvision.finance.fms.common.model.UserSession;
import com.hikvision.finance.fms.common.util.Constants.CacheKeyPrefix;
import com.hikvision.finance.fms.common.util.Constants.SessionAttrbutes;
import com.hikvision.finance.fms.common.util.Constants.SysConfigType;
import com.hikvision.finance.fwork.core.ServiceLocator;
import com.hikvision.finance.fwork.util.ContentType;
import com.hikvision.finance.fwork.util.ServletUtil;
import com.hikvision.finance.fwork.util.errorcode.ErrorCode;
import com.hikvision.finance.fwork.util.errorcode.ErrorCodeUtil;
import com.hikvision.finance.fwork.www.filter.HikFilter;
import com.hikvision.finance.fwork.www.filter.ResultCode;
import com.hikvision.finance.fwork.www.session.HikSession;
import com.hikvision.finance.fwork.www.session.ISessionProcessor;

/**
 * <p>
 * 理财风控的过滤器，提供校验用户是否在线和当前url是否有访问权限。
 * </p>
 * @author jinxindong 2016年2月22日 上午10:53:35
 * @version V1.0
 */
public class FmsServletFilter extends HikFilter {
	
	Logger log = LoggerFactory.getLogger(FmsServletFilter.class);
	
	/**
	 * 校验当前url的工具类
	 */
	private IURLFilter urlFilter = null;
	/**
	 * session管理工具类
	 */
	private ISessionProcessor sessionProcessor = null;
	/**
	 * 缓存管理器
	 */
	private DataManager dataManager;
	
	/**
	 * 从spring容器中获取名称为“urlFilter”的bean，如果不存在，则返回null
	 * @author jinxindong 2016年2月22日 上午10:58:40
	 * @return
	 */
	public IURLFilter getUrlFilter() {
		if (urlFilter == null) {
			urlFilter = ServiceLocator.findService("urlFilter");
		}
		if (urlFilter == null) {
			log.error("the bean named 'urlFilter' is not defined");
		}
		return urlFilter;
	}
	
	/**
	 * 设置处理URL校验的工具类
	 * @author jinxindong 2016年2月22日 上午10:57:36
	 * @param urlFilter
	 */
	public void setUrlFilter(IURLFilter urlFilter) {
		this.urlFilter = urlFilter;
	}
	
	/**
	 * 从spring容器中获取名称为“sessionProcessor”的bean，如果不存在，则返回null
	 * @author jinxindong 2016年2月22日 上午10:59:24
	 * @return
	 */
	public ISessionProcessor getSessionProcessor() {
		if (sessionProcessor == null) {
			sessionProcessor = ServiceLocator.findService("sessionProcessor");
		}
		if (sessionProcessor == null) {
			log.error("the bean named 'sessionProcessor' is not defined");
		}
		return sessionProcessor;
	}
	
	/**
	 * 设置处理用户session的工具类
	 * @author jinxindong 2016年2月22日 上午11:00:00
	 * @param sessionProcessor
	 */
	public void setSessionProcessor(ISessionProcessor sessionProcessor) {
		this.sessionProcessor = sessionProcessor;
	}
	
	/**
	 * 从spring容器中获取名称为“dataManager”的bean，如果不存在，则返回null
	 * @author jinxindong 2016年2月22日 上午11:00:12
	 * @return
	 */
	public DataManager getDataManager() {
		if (dataManager == null) {
			dataManager = ServiceLocator.findService("dataManager");
		}
		if (dataManager == null) {
			log.error("the bean named 'dataManager' is not defined");
		}
		return dataManager;
	}
	
	/**
	 * 设置处理缓存管理器
	 * @author jinxindong 2016年2月22日 上午11:00:50
	 * @param dataManager
	 */
	public void setDataManager(DataManager dataManager) {
		this.dataManager = dataManager;
	}
	
	/**
	 * 获取当前请求的url，不包括contentpath
	 * @author jinxindong 2016年2月22日 上午11:00:50
	 * @param httpRequest
	 * @return
	 */
	protected String getRequestUrl(HttpServletRequest httpRequest) {
		String requesturl = httpRequest.getRequestURI();
		String contentPath = httpRequest.getContextPath();
		if (!contentPath.equals("/")) {
			requesturl = requesturl.replaceFirst(contentPath, "");
		}
		return requesturl;
	}
	
	/**
	 * 跳转页面
	 * @author jinxindong 2016年2月22日 上午11:00:50
	 * @param request 请求
	 * @param response 响应
	 * @param resultURL 跳转url
	 * @param errorCode 错误码
	 */
	protected void excuteResult(HttpServletRequest request, HttpServletResponse response, String resultURL, String errorCode) {
		ErrorCode ec = ErrorCodeUtil.getGlobalErrorCode(errorCode);
		
		if (ec == null) {
			ec = ErrorCodeUtil.getGlobalErrorCode(ResultCode.GLOBAL_UNKNOW_ERROR);
		}
		
		response.setStatus(ResultCode.EXPECT_EXCEPTION_STATUS);
		if (ec == null) {
			ec = new ErrorCode();
			ec.setCode("0");
			ec.setName("未知错误");
		} else {
			response.setHeader("ErrorCode", ec.getCode());
			try {
				response.setHeader("ErrorMsg", URLEncoder.encode(ec.getName(), "utf-8"));
			} catch (UnsupportedEncodingException e) {
				
			}
		}
		// FIXME:待扩展
		response.setHeader("LogOut", "false");
		try {
			if (ServletUtil.isAjaxRequest(request)) {
				String result = "{\"success\":false,\"errorStatus\":\"error\",\"errorCode\":\"" + ec.getCode() + "\"}";
				response.setHeader("is-refresh", "1");
				response.setHeader("refresh-url", "/");
				ServletUtil.writeResponse(response, result, ContentType.JSON);
			} else {
				request.setAttribute("ErrorCode", ec.getCode());
				request.setAttribute("ErrorMsg", ec.getName());
				String redirectURL = resultURL + "?errorCode=" + ec.getCode();
				try {
					RequestDispatcher requestDispatcher = request.getRequestDispatcher(redirectURL);
					if (requestDispatcher != null) {
						requestDispatcher.forward(request, response);
					}
				} catch (Exception e) {
					log.error("forward " + resultURL + ",error", e);
				}
				// response.sendRedirect(redirectURL);
			}
		} catch (IOException e) {
			log.error("write response  error", e);
		}
	}
	
	/*
	 * (non-Javadoc)
	 * @see javax.servlet.Filter#doFilter(javax.servlet.ServletRequest, javax.servlet.ServletResponse, javax.servlet.FilterChain)
	 */
	@Override
	public void doFilter(ServletRequest request, ServletResponse response, FilterChain filterChain) throws IOException,
	        ServletException {
		HttpServletRequest httpRequest = (HttpServletRequest)request;
		HttpServletResponse httpResponse = (HttpServletResponse)response;
		String requesturl = "";
		try {
			requesturl = getRequestUrl(httpRequest);
			// 过滤不需要会话的url
			if (getUrlFilter().isIgnoreURL(requesturl)) {
				filterChain.doFilter(request, response);
				return;
			}

			// 解决AppScan扫描的跨站点请求伪造问题，根据referer判断
		    String referer = httpRequest.getHeader("referer");
		    log.info("referer:" + referer);
		    InetAddress addr = InetAddress.getLocalHost();
		    String localIp = addr.getHostAddress();
		    log.info("localIp:" + localIp);
			if (null != referer && !referer.startsWith("http://localhost") && !referer.startsWith("http://127.0.0.1")
			        && !referer.startsWith("http://" + localIp)) {
				log.error("请求不合法");
				excuteResult(httpRequest, httpResponse, getUrlFilter().getResultURL("referererror"), "referererror");
				return;
			}
			
			// 解决AppScan扫描的missing X-XSS-Protection header
			httpResponse.addHeader("X-XSS-Protection", "1");
			// 解决AppScan扫描的missing X-Content-Type-Options header
			httpResponse.addHeader("X-Content-Type-Options", "nosniff");
			// 解决AppScan扫描的missing Content-Security-Policy header
	    	httpResponse.addHeader("Content-Security-Policy", "frame-src *");// 为了消除AppScan警告，frame-src可以将chrome中页面加载影响降到比较小
			
			if (!ServletUtil.isAjaxRequest(httpRequest)) {
				setContextParam(httpRequest);
			}

			
			HikSession session = null;
			// 用户是否已经登录
			if (!getSessionProcessor().isSessionExist(httpRequest)) {
				log.info("user login timeout or user not login yet.");
				httpResponse.setHeader("is-refresh", "1");
				httpResponse.setHeader("'in-login'", "1");
				excuteResult(httpRequest, httpResponse, getUrlFilter().getResultURL("notlogin"), ResultCode.GLOBAL_NOT_LOGIN);
				return;
			} else {
				session = getSessionProcessor().getSession(httpRequest);
			}
			
			// 过滤掉不需要进行访问权限验证的url
			if (!getUrlFilter().canAccess(requesturl, (UserSession)session.getAttribute(SessionAttrbutes.USER_SESSION))) {
				log.info("Access Denied.url:" + requesturl);
				excuteResult(httpRequest, httpResponse, getUrlFilter().getResultURL("noauth"), ResultCode.GLOBAL_NO_AUTH);
				return;
			}
			
			// 访问成功，更新用户session
			String expire = ConfigManager.getConfiguration("web-session", "session.expire");
			if (StringUtils.isEmpty(expire)) {
				expire = "20";
			}
			getSessionProcessor().setSession(httpRequest, httpResponse, session, Integer.parseInt(expire.trim()) * 60 * 1000);
			filterChain.doFilter(request, response);
		} catch (Exception e) {
			log.error("filter error", e);
			excuteResult(httpRequest, httpResponse, getUrlFilter().getResultURL("error"), ResultCode.GLOBAL_UNKNOW_ERROR);
			return;
		}
		
	}

	/**
	 * 设置系统参数
	 * @author jinxindong 2016年2月22日 下午2:31:08
	 * @param request
	 */
	private void setContextParam(ServletRequest request) {
    	try {
    		//全局参数
	        request.setAttribute(CacheKeyPrefix.PREFIX_SYSCONFIG+SysConfigType.PLATFORM_DISCRIBE, getDataManager().get(CacheKeyPrefix.PREFIX_SYSCONFIG+SysConfigType.PLATFORM_DISCRIBE));
	        request.setAttribute(CacheKeyPrefix.PREFIX_SYSCONFIG+SysConfigType.PLATFORM_COPYRIGHT, getDataManager().get(CacheKeyPrefix.PREFIX_SYSCONFIG+SysConfigType.PLATFORM_COPYRIGHT));
	        request.setAttribute(CacheKeyPrefix.PREFIX_SYSCONFIG+SysConfigType.IMG_LOGO, getDataManager().get(CacheKeyPrefix.PREFIX_SYSCONFIG+SysConfigType.IMG_LOGO));
	        request.setAttribute(CacheKeyPrefix.PREFIX_SYSCONFIG+SysConfigType.IMG_LOGIN, getDataManager().get(CacheKeyPrefix.PREFIX_SYSCONFIG+SysConfigType.IMG_LOGIN));
	        request.setAttribute(CacheKeyPrefix.PREFIX_SYSCONFIG+SysConfigType.IMG_BAR, getDataManager().get(CacheKeyPrefix.PREFIX_SYSCONFIG+SysConfigType.IMG_BAR));
	        //模式参数（Sysconfig_6000）
	        request.setAttribute(CacheKeyPrefix.PREFIX_SYSCONFIG+SysConfigType.PRODUCT_REVIEW, getDataManager().get(CacheKeyPrefix.PREFIX_SYSCONFIG+SysConfigType.PRODUCT_REVIEW));
	        request.setAttribute(CacheKeyPrefix.PREFIX_SYSCONFIG+SysConfigType.CHOICE_IPC_USB, getDataManager().get(CacheKeyPrefix.PREFIX_SYSCONFIG+SysConfigType.CHOICE_IPC_USB));
	        request.setAttribute(CacheKeyPrefix.PREFIX_SYSCONFIG+SysConfigType.OVERLAY_OSD, getDataManager().get(CacheKeyPrefix.PREFIX_SYSCONFIG+SysConfigType.OVERLAY_OSD));
	        request.setAttribute(CacheKeyPrefix.PREFIX_SYSCONFIG+SysConfigType.MAXIMUM_VIDEO_LENGTH, getDataManager().get(CacheKeyPrefix.PREFIX_SYSCONFIG+SysConfigType.MAXIMUM_VIDEO_LENGTH));
	        request.setAttribute(CacheKeyPrefix.PREFIX_SYSCONFIG+SysConfigType.VIDEO_PLAYBACK_BANDWIDTH, getDataManager().get(CacheKeyPrefix.PREFIX_SYSCONFIG+SysConfigType.VIDEO_PLAYBACK_BANDWIDTH));
	        request.setAttribute(CacheKeyPrefix.PREFIX_SYSCONFIG+SysConfigType.NON_CENTER_STORAGE, FmsPropertyPlaceholder.getProperty("nonCenterStorage"));
	        request.setAttribute(CacheKeyPrefix.PREFIX_SYSCONFIG+SysConfigType.NIP_lIMIT, FmsPropertyPlaceholder.getProperty("channelIpBind"));
	        request.setAttribute(CacheKeyPrefix.PREFIX_SYSCONFIG+SysConfigType.LOG_REPORT, FmsPropertyPlaceholder.getProperty("logReport"));
	        request.setAttribute(CacheKeyPrefix.PREFIX_SYSCONFIG+SysConfigType.CHOICE_IPC_USB, FmsPropertyPlaceholder.getProperty("choiceUsbIpc"));
	        request.setAttribute(CacheKeyPrefix.PREFIX_SYSCONFIG+SysConfigType.LOG_REPORT_METHOD, FmsPropertyPlaceholder.getProperty("logReportMethod"));
    	} catch (DataLoadException e) {
	        log.error("Load Data Error",e.getMessage());
        }
	}
}
