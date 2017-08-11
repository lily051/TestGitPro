/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016-1-22 上午11:24:59
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.common.excel.util;

import java.lang.reflect.InvocationTargetException;
import java.util.Date;
import java.util.Map;

import org.apache.commons.beanutils.ConvertUtils;
import org.apache.commons.beanutils.converters.DateTimeConverter;


/**
 * <p>Excel导入数据时进行bean的复制</p>
 * 为Date类型注册类型转换器
 * @author jinxindong 2016年4月21日 下午1:11:28
 * @version V1.0
 */
public class ExcelBeanUtils extends  org.apache.commons.beanutils.BeanUtils{
	
	private static final String CONTEXT_KEY_FORMAT_DATE_VALUE = "yyyy-MM-dd";
	private static final String CONTEXT_KEY_FORMAT_DATETIME_VALUE = "yyyy-MM-dd HH:mm:ss";
	private static final String CONTEXT_KEY_FORMAT_TIME_VALUE = "HH:mm:ss";
	
	static {
		DateConverter dateConverter = new DateConverter(null);  
		dateConverter.setUseLocaleFormat(true);  
		dateConverter.setPatterns(new String[]{CONTEXT_KEY_FORMAT_DATE_VALUE, CONTEXT_KEY_FORMAT_DATETIME_VALUE, CONTEXT_KEY_FORMAT_TIME_VALUE});  
		ConvertUtils.register(dateConverter, Date.class);  
	}
	
	public static class DateConverter extends DateTimeConverter {  
		  
	    public DateConverter() { 
	   
	    }  
	   
	    public DateConverter(Object defaultValue) {  
	        super(defaultValue);  
	    }  
	  
	    @SuppressWarnings("rawtypes")  
	    protected Class getDefaultType() {  
	        return Date.class;  
	    }  
	  
	    @SuppressWarnings("rawtypes")  
	    @Override  
	    protected Object convertToType(Class type, Object obj) throws Exception {  
	        if (obj == null) {  
	            return null;  
	        }  
	        String value = obj.toString().trim();  
	        if (value.length() == 0) {  
	            return null;  
	        }  
	        return super.convertToType(type, obj);  
	    }  
	}  
	
	public static void populateBean(Object bean, Map<?, ?> properties) throws IllegalAccessException, InvocationTargetException{
		populate(bean, properties);
	}
}
