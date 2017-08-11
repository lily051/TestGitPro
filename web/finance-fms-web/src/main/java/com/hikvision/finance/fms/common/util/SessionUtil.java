/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月22日 下午2:58:13
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.common.util;

import org.apache.struts2.ServletActionContext;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.hikvision.finance.core.util.StringUtils;
import com.hikvision.finance.core.util.cm.ConfigManager;
import com.hikvision.finance.fms.common.model.UserSession;
import com.hikvision.finance.fms.common.util.Constants.SessionAttrbutes;
import com.hikvision.finance.fwork.core.ServiceLocator;
import com.hikvision.finance.fwork.www.session.HikSession;
import com.hikvision.finance.fwork.www.session.HikWebSession;
import com.hikvision.finance.fwork.www.session.ISessionProcessor;

/**
 * <p>
 * session工具类，快速获取Session信息
 * </p>
 * <strong>依赖struts上下文环境</strong>
 * @author jinxindong 2016年2月22日 下午2:58:13
 * @version V1.0
 */
public class SessionUtil {
	
	private static final Logger log = LoggerFactory.getLogger(SessionUtil.class.getName());
	
	/**
	 * session管理工具类
	 */
	private static ISessionProcessor sessionProcessor = null;
	
	/**
	 * 获取用户Session信息
	 * @author jinxindong 2016年2月22日 下午2:58:54
	 * @return
	 */
	public static UserSession getUserSession() {
		UserSession userSession = null;
		HikSession hikSession = getHikSession();
		
		if (hikSession != null) {
			userSession = (UserSession)hikSession.getAttribute(SessionAttrbutes.USER_SESSION);
		}
		return userSession;
	}
	
	/**
	 * 获取当前session信息
	 * @author jinxindong 2016年2月22日 下午2:59:02
	 * @return
	 */
	public static HikSession getHikSession() {
		HikSession hikSession = getSessionProcessor().getSession(ServletActionContext.getRequest());
		return hikSession;
	}
	
	/**
	 * 设置session信息
	 * @author jinxindong 2016年2月22日 下午2:59:13
	 * @param hikSession
	 * @return
	 */
	public static boolean setHikSession(HikWebSession hikSession) {
		String expire = ConfigManager.getConfiguration("web-session", "session.expire");
		if (StringUtils.isEmpty(expire)) {
			expire = "20";
		}
		return getSessionProcessor().setSession(ServletActionContext.getRequest(), ServletActionContext.getResponse(),
		        hikSession, Integer.parseInt(expire.trim()) * 60 * 1000);
	}
	
	private static ISessionProcessor getSessionProcessor() {
		if (sessionProcessor == null) {
			sessionProcessor = ServiceLocator.findService("sessionProcessor");
		}
		if (sessionProcessor == null) {
			log.error("the bean named 'sessionProcessor' is not defined");
		}
		return sessionProcessor;
	}
	
}
