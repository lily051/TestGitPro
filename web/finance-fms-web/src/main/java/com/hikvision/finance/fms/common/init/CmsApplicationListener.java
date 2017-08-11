/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-3-11 下午2:44:33
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.common.init;

import java.io.IOException;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.ApplicationListener;
import org.springframework.context.event.ContextRefreshedEvent;

import com.hikvision.rpc.factory.CmsHppFactory;
import com.ivms6.core.hpp.exception.HppInitException;

/**
 * <p>spring容器启动成功后执行</p>
 * @author xujiangfei 2016-3-11 下午2:44:33
 * @version V1.0 
 */
public class CmsApplicationListener implements ApplicationListener<ContextRefreshedEvent>{
	private static Logger logger = LoggerFactory.getLogger("CmsApplicationListener");
	
	/* (non-Javadoc)
	 * @see org.springframework.context.ApplicationListener#onApplicationEvent(org.springframework.context.ApplicationEvent)
	 */
	@Override
	public void onApplicationEvent(ContextRefreshedEvent event) {
		
		if (event.getApplicationContext().getParent() == null) {
    		//1、启动mina客户端
    		logger.info("启动mina客户端");
    		try {
    			CmsHppFactory hppFactory;
	            hppFactory = CmsHppFactory.getInstance();
	            hppFactory.startClient();
            } catch (HppInitException e) {
            	logger.error("启动mina客户端失败"+e.getMessage(),e);
            	throw new RuntimeException(e.getCause());
            } catch (IOException e) {
            	logger.error("启动mina客户端失败"+e.getMessage(),e);
            	throw new RuntimeException(e.getCause());
            }
    	}
		
	}

}
