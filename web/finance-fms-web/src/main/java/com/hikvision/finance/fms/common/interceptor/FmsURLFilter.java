/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月22日 下午2:18:13
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.common.interceptor;

import java.util.List;
import java.util.Map;
import java.util.Set;

import com.hikvision.finance.core.util.StringUtils;
import com.hikvision.finance.core.util.cm.ConfigManager;
import com.hikvision.finance.fms.common.model.UserSession;
import com.hikvision.finance.fwork.util.match.PatternMatcher;

/**
 * url校验的实现
 * @author jinxindong 2016年2月22日 下午2:18:13
 * @version V1.0
 */
public class FmsURLFilter implements IURLFilter {
	
	protected String SECTION_URL_AUTH = "url-auth";
	protected String AUTH_URL = "withoutauth";
	protected String SESSION_URL = "withoutlogin";
	protected String SECTION_GLOBAL_RESULT = "global-result";
	protected String KEY_GLOBAL_RESULT_PREFIX = "global.result.";
	protected String KEY_GLOBAL_RESULT_DEFAULT = "/error.jsp";
	
	private PatternMatcher hikMatcher;
	
	public PatternMatcher getHikMatcher() {
		return hikMatcher;
	}
	
	public void setHikMatcher(PatternMatcher hikMatcher) {
		this.hikMatcher = hikMatcher;
	}
	
	@Override
	public boolean canAccess(String url, String userId) {
		if (validateUrl(url, AUTH_URL)) {
			return true;
		}
		return false;
	}
	
	@Override
	public boolean isIgnoreURL(String url) {
		return validateUrl(url, SESSION_URL);
	}
	
	protected boolean validateUrl(String url, String auth) {
		Map<String, String> section = ConfigManager.getSection(SECTION_URL_AUTH);
		if (section == null) {
			return true;
		}
		Set<Map.Entry<String, String>> entryList = section.entrySet();
		for (Map.Entry<String, String> entry : entryList) {
			if (!StringUtils.isEmpty(entry.getValue()) && entry.getValue().contains(auth)) {
				if (hikMatcher.matches(entry.getKey(), url)) {
					return true;
				}
			}
		}
		return false;
	}
	
	@Override
	public String getResultURL(String resultName) {
		String globalReault = KEY_GLOBAL_RESULT_PREFIX + "default";
		Map<String, String> section = ConfigManager.getSection(SECTION_GLOBAL_RESULT);
		if (section == null) {
			return KEY_GLOBAL_RESULT_DEFAULT;
		}
		String result = section.get("global.result." + resultName);
		if (StringUtils.isEmpty(result)) {
			return section.get(globalReault);
		} else {
			return result;
		}
	}
	
	/*
	 * (non-Javadoc)
	 * @see com.hikvision.finance.cvtms.common.interceptor.IURLFilter#canAccess(java.lang.String, java.lang.String,com.hikvision.finance.cvtms.common.model.UserSession)
	 */
	@Override
	public boolean canAccess(String url, UserSession session) {
		//对home和index的处理 登录后采集不可访问中心主页(重定向到home在index()方法中)  中心不可访问采集主页(在此处拦截 返回false)
		if(session.getSysRole() == 1 && hikMatcher.matches("/web/index.action", url)){
		//	return false;
		}else if(session.getSysRole() == 0 && hikMatcher.matches("/web/home.action", url)){
			return false;
		}
		
    	if(session.isSystemRole()){
    		if(hikMatcher.matches("/web/index.jsp", url)){
    			return false;
    		}
    		//return true;
    	}
		if(validateUrl(url,AUTH_URL)){
			return true;
		}
		List<String> privileges = session.getPrivileges();
		for(String pri: privileges){
			if(hikMatcher.matches(pri, url)){
				return true;
			}
		}
		return false;
    }
	
}
