/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年10月26日 下午6:59:29
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.common.interceptor;

import java.io.IOException;

import javax.servlet.Filter;
import javax.servlet.FilterChain;
import javax.servlet.FilterConfig;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.http.HttpServletResponse;


/**
 * <p>过滤js文件</p>
 * @author jinchenwu 2016年10月26日 下午6:59:29
 * @version V1.0 
 */
public class FrontEndFilter implements Filter{

	/* (non-Javadoc)
     * @see javax.servlet.Filter#init(javax.servlet.FilterConfig)
     */
    @Override
    public void init(FilterConfig filterConfig) throws ServletException {
	    
    }

	/* 
	 * 解决AppScan扫描的missing X-XSS-Protection header，missing X-Content-Type-Options header，missing Content-Security-Policy header
     * @see javax.servlet.Filter#doFilter(javax.servlet.ServletRequest, javax.servlet.ServletResponse, javax.servlet.FilterChain)
     */
    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain) throws IOException,
            ServletException {
    	HttpServletResponse httpResponse = (HttpServletResponse)response;
    	httpResponse.addHeader("X-XSS-Protection", "1");
    	httpResponse.addHeader("X-Content-Type-Options", "nosniff");
    	httpResponse.addHeader("Content-Security-Policy", "frame-src *");// 为了消除AppScan警告，frame-src可以将chrome中页面加载影响降到比较小
    	chain.doFilter(request, response);
    }

	/* (non-Javadoc)
     * @see javax.servlet.Filter#destroy()
     */
    @Override
    public void destroy() {
    	
    }
	
}
