package com.common;

import org.ini4j.Ini;
import org.ini4j.Profile.Section;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.InputStream;
import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

/**
 * @ClassName: ConfigManager
 * @Description: 全局系统配置。
 * @author dongying
 * @date 2016-8-11
 * 
 */
public class ConfigManager {

	protected final static Logger log = LoggerFactory.getLogger(ConfigManager.class);
	/**
	 * 配置文件的内存处理类
	 */
	private static Ini ini = null;

	static {
		try {
			ClassLoader loader = Thread.currentThread().getContextClassLoader();
			InputStream is = loader.getResourceAsStream("system.properties");
			ini = new Ini();
			ini.load(is);
		} catch (Exception e) {
			log.warn("classpath:system.properties loadFromPath error, ", e);
		}
	}

	/**
	 * @Title: getConfiguration
	 * @Description: 查找配置。
	 * @author dongying
	 * @param sectionName
	 * @param propertyName
	 * @return 设定文件
	 * @return String 返回类型
	 * @throws
	 */
	public static String getConfiguration(String sectionName, String propertyName) {
		Section section = ini.get(sectionName);
		return section.get(propertyName);
	}

	/**
	 * @Title: getSection
	 * @Description: 查找配置。
	 * @author dongying
	 * @param sectionName
	 * @return 设定文件
	 * @return Map<String,String> 返回类型
	 * @throws
	 */
	public static Map<String, String> getSection(String sectionName) {
		Map<String, String> map = new HashMap<String, String>();
		Set<Entry<String, Section>> set = ini.entrySet();
		for (Entry<String, Section> entry : set) {
			String sn = entry.getKey();
			if (sn.equals(sectionName)) {
				Section section = entry.getValue();
				for (Entry<String, String> e : section.entrySet()) {
					map.put(e.getKey(), e.getValue());
				}
			}
		}
		return map;
	}
}
