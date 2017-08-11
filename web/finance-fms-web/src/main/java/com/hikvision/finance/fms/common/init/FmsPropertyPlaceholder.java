/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年10月31日 下午2:12:53
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.common.init;

import java.util.Map;
import java.util.Properties;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.BeansException;
import org.springframework.beans.factory.BeanInitializationException;
import org.springframework.beans.factory.config.ConfigurableListableBeanFactory;

import com.hikvision.security.patch.context.ConfigContextLoader;
import com.hikvision.security.patch.context.SecurityContext;
import com.hikvision.security.patch.context.SecurityProperty;
import com.hikvision.security.patch.spring.SecurityPropertyPlaceHolderConfigurer;
import com.hikvision.security.patch.util.HexConvertUtil;
import com.hikvision.security.patch.util.PropertyUtil;

/**
 * <p>
 * </p>
 * 
 * @author fuqunqing 2016年10月31日 下午2:12:53
 * @version V1.0
 */
public class FmsPropertyPlaceholder extends SecurityPropertyPlaceHolderConfigurer {
	private static final Log log = LogFactory.getLog(FmsPropertyPlaceholder.class);

	private static  Properties propsLocal;

	protected void processProperties(ConfigurableListableBeanFactory beanFactoryToProcess, Properties props)
			throws BeansException {
		log.info("SecurityPropertyPlaceHolderConfigurer replace properties start...");
		String logKey = "";
		try {
			SecurityContext context = ConfigContextLoader.load(null, null);

			Map<String, SecurityProperty> map = context.getSecurityProperties();
			for (String filePath : map.keySet()) {
				for (String encryptKey : ((SecurityProperty) map.get(filePath)).getEncryptKeys()) {
					logKey = encryptKey;
					String propertyValue = props.getProperty(encryptKey);
					if ((propertyValue != null) && (!(PropertyUtil.canExecSecurity(propertyValue)))) {
						String encrypted;
						try {
							encrypted = HexConvertUtil.getInstance()._$$2(propertyValue);
						} catch (Exception e) {
							log.warn("解密失败", e);
							encrypted = propertyValue;
						}
						props.setProperty(encryptKey, encrypted);
					}
				}
			}
			log.info("SecurityPropertyPlaceHolderConfigurer replace properties completed.");
			propsLocal = props;
			super.processProperties(beanFactoryToProcess, props);
			log.info("SecurityPropertyPlaceHolderConfigurer replace properties finish.");
		} catch (Exception e) {
			log.error(String.format("SecurityPropertyPlaceHolderConfigurer replace properties error:[enctyptKey=(%s)]",
					new Object[] { logKey }), e);

			throw new BeanInitializationException(e.getMessage());
		}
	}
	
	//static method for accessing context properties
    public static String getProperty(String name) {
    	
        return propsLocal.get(name).toString();
    }
}
