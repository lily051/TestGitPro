/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2015年8月7日 上午11:08:15
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.common.init;

import javax.servlet.ServletContext;
import javax.servlet.ServletContextEvent;
import javax.servlet.ServletContextListener;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.ApplicationContext;
import org.springframework.web.context.support.WebApplicationContextUtils;

import com.hikvision.finance.fms.common.timetask.MemoryTimeTask;
import com.hikvision.finance.fms.license.task.LicenseTask;

/**
 * <p>Web应用监听器，监听Web应用启动和销毁</p>
 * @author jinxindong 2016年3月31日 下午3:44:47
 * @version V1.0
 */
public class AppInitContextListener implements ServletContextListener {
	
	private static Logger logger = LoggerFactory.getLogger(AppInitContextListener.class);
	private ApplicationContext appContext;
	
	/*
	 * (non-Javadoc)
	 * @see javax.servlet.ServletContextListener#contextInitialized(javax.servlet.ServletContextEvent)
	 */
	@Override
	public void contextInitialized(ServletContextEvent sce) {
		logger.info("初始化应用数据开始");
		ServletContext sc = sce.getServletContext();
		appContext = WebApplicationContextUtils.getWebApplicationContext(sc);
		setSysParam();// 设置系统参数到缓存中
		getAuthorizationInfo();// 获取并写入授权信息
		logger.info("初始化应用数据结束");
	}
	
	/*
	 * (non-Javadoc)
	 * @see javax.servlet.ServletContextListener#contextDestroyed(javax.servlet.ServletContextEvent)
	 */
	@Override
	public void contextDestroyed(ServletContextEvent sce) {
		logger.info("Web application destroyed ...");
	}
	
	/**
	 * 设置系统参数到缓存中
	 * @author jinchenwu 2016年11月11日 上午10:49:45
	 */
	private void setSysParam() {
		if (null != appContext) {// coverity检测出需要添加空指针判断
			MemoryTimeTask memoryTimeTask = (MemoryTimeTask)appContext.getBean("memoryTimeTask");
			memoryTimeTask.run();
		} else {
			logger.error("appContext为空，未执行设置系统参数到缓存中的操作");
		}
	}
	
	/**
	 * 获取并写入授权信息
	 * @author jinchenwu 2016年11月8日 下午3:31:55
	 */
	private void getAuthorizationInfo() {
		if (null != appContext) {// coverity检测出需要添加空指针判断
			LicenseTask licenseTask = (LicenseTask)appContext.getBean("licenseTimeTask");
			licenseTask.run();
		} else {
			logger.error("appContext为空，未执行写入授权信息操作");
		}
	}
}
