/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-3-18 上午10:49:57
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.license.util;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.hikvision.finance.extend.hibernate.template.PageHibernateTemplate;
import com.hikvision.finance.fwork.core.ServiceLocator;
import com.hikvision.rpc.factory.CmsHppFactory;
import com.ivms6.core.hpp.exception.HppInitException;



/**
 * <p></p>
 * @author xujiangfei 2016-3-18 上午10:49:57
 * @version V1.0 
 */
public class RununingStateUtil {

	private static Logger logger = LoggerFactory.getLogger(RununingStateUtil.class);
	
	/**
	 * 是否已连接数据库
	 * @author zhangzhenbin 2015-3-23 下午04:51:20
	 * @return
	 */
	public static Boolean isDBConnected() {
		Boolean connected = true;
		try {
			PageHibernateTemplate ht = ServiceLocator.findService("pageHibernateTemplate");
			ht.getSessionFactory();
		} catch (Exception e) {
			connected = false;
			logger.error(e.getMessage(), e);
		}
		return connected;
	}
	
	/**
	 * MINA服务是否正在运行
	 * @author zhangzhenbin 2015-3-23 下午06:44:42
	 * @return
	 */
	public static Boolean isMINAServer() {
		Boolean servicing = true;
		try {
			CmsHppFactory.getInstance().getServer();
		} catch (HppInitException e) {
			servicing = false;
			logger.error(e.getMessage(), e);
		}
		
		return servicing;
	}
	
	
	
	
	
	
	
	
}
