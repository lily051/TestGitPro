package com.hikvision.finance.fms.common.interceptor;

import java.io.Serializable;
import java.util.Date;
import java.util.StringTokenizer;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.hibernate.EmptyInterceptor;
import org.hibernate.dialect.MySQLDialect;
import org.hibernate.type.Type;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.hikvision.finance.core.util.StringUtils;
import com.hikvision.finance.fms.common.model.BaseEntity;
import com.hikvision.finance.fwork.core.log.OperationLogUtil;

/**
 * 统一处理提交到数据库前的sql语句。 目前处理了mysql的order by语句
 * @author jinxindong 2016年2月22日 下午2:12:46
 * @version V1.0
 */
public class HikModBaseEntityInterceptor extends EmptyInterceptor {
	
	/**
	 * 序列化ID
	 */
	private static final long serialVersionUID = 1574901156112897670L;
	
	private static final Logger LOG = LoggerFactory.getLogger(HikModBaseEntityInterceptor.class);
	private static final Pattern ORDER_PATTERN_ALL = Pattern.compile(
	        "order\\s*by\\s*[(]?[\\w|\\.]*[)]?\\s*(desc|asc)?((,\\s*[(]?[\\w|\\.]*[)]?)*\\s*(desc|asc)?)*",
	        Pattern.CASE_INSENSITIVE);
	private static final Pattern ORDER_PATTERN_HEAD = Pattern.compile("order\\s*by", Pattern.CASE_INSENSITIVE);
	private static final Pattern FROM_PATTERN_HEAD = Pattern.compile("from\\s*.*\\s*where", Pattern.CASE_INSENSITIVE);
	private static final String[] IGNORE_COLUMN = new String[] {"c_create_time", "c_creator", "c_update_time"};
	private String dialectName;
	private String encoding = "gbk";
	
	/*
	 * (non-Javadoc)
	 * @see org.hibernate.EmptyInterceptor#onPrepareStatement(java.lang.String)
	 */
	@Override
	public String onPrepareStatement(String sql) {
		String localSql = super.onPrepareStatement(sql);
		if ("mysql".equals(dialectName)) {
			localSql = rePrepareSQL(dialectName, sql);
		} else {
			localSql = addDefaultOrder2BySQL(dialectName, sql);
		}
		return localSql;
	}
	
	/*
	 * (non-Javadoc)
	 * @see org.hibernate.EmptyInterceptor#onSave(java.lang.Object, java.io.Serializable, java.lang.Object[], java.lang.String[], org.hibernate.type.Type[])
	 */
	@Override
	public boolean onSave(Object entity, Serializable id, Object[] state, String[] propertyNames, Type[] types) {
		if (entity instanceof BaseEntity) {
			for (int i = 0; i < propertyNames.length; i++) {
				if ("dtCreateTime".equals(propertyNames[i])) {
					state[i] = new Date();
				}
				if ("ncreatorId".equals(propertyNames[i])) {
					if (OperationLogUtil.getOperationLogBo() != null) {
						Integer createrId = OperationLogUtil.getOperationLogBo().getCreatorId();
						state[i] = createrId;
					}
				}
			}
			return true;
		}
		return false;
	}
	
	/*
	 * (non-Javadoc)
	 * @see org.hibernate.EmptyInterceptor#onFlushDirty(java.lang.Object, java.io.Serializable, java.lang.Object[], java.lang.Object[], java.lang.String[],
	 * org.hibernate.type.Type[])
	 */
	@Override
	public boolean onFlushDirty(Object entity, Serializable id, Object[] currentState, Object[] previousState,
	        String[] propertyNames, Type[] types) {
		if (entity instanceof BaseEntity) {
			for (int i = 0; i < propertyNames.length; i++) {
				if ("dtUpdateTime".equals(propertyNames[i])) {
					currentState[i] = new Date();
					return true;
				}
			}
		}
		return false;
	}
	
	protected String addDefaultOrder2BySQL(String dialectName, String sql) {
		if (hasJoin(sql)) {
			return sql;
		}
		Matcher m = ORDER_PATTERN_ALL.matcher(sql);
		StringBuffer sb = new StringBuffer();
		while (m.find()) {
			String replaceMent = sql.substring(m.start(), m.end());
			m.appendReplacement(sb, replaceMent);
		}
		m.appendTail(sb);
		return sb.toString();
		
	}
	
	private boolean hasJoin(String sql) {
		if (sql.indexOf(" join ") > 0) {
			return true;
		}
		Matcher m = FROM_PATTERN_HEAD.matcher(sql);
		while (m.find()) {
			String matchStr = sql.substring(m.start(), m.end());
			if (matchStr.indexOf(",") > 0) {
				return true;
			}
		}
		return false;
	}
	
	/**
	 * 处理mysql的order by语句，用convert 方法来解决中文配需的问题 如果是"i_"开头的字段，表示是数字，则不进行排序 如果是"c_create_time","c_creator","c_update_time"三个之一，也不进行排序
	 * @author huanghuafeng 2013-7-22 下午01:00:24
	 * @param dialectName
	 * @param sql
	 * @return
	 */
	protected String rePrepareSQL(String dialectName, String sql) {
		Matcher m = ORDER_PATTERN_ALL.matcher(sql);
		StringBuffer sb = new StringBuffer();
		while (m.find()) {
			String replaceMent = sql.substring(m.start(), m.end());
			replaceMent = replaceColumnName(replaceMent);
			m.appendReplacement(sb, replaceMent);
		}
		m.appendTail(sb);
		return sb.toString();
		
	}
	
	/**
	 * 转换排序字段
	 * @author jinxindong 2016年2月22日 下午2:12:16
	 * @param replaceMent
	 * @return
	 */
	private String replaceColumnName(String replaceMent) {
		replaceMent = replaceMent.toLowerCase().trim();
		boolean isEndWithQuot = false;
		if (replaceMent.endsWith(")")) {// 如果以）结尾需要特殊处理
			isEndWithQuot = true;
		}
		String end = "";
		if (replaceMent.endsWith(" desc")) {
			end = "desc";
			replaceMent = replaceMent.substring(0, replaceMent.length() - 5);
		}
		
		if (replaceMent.endsWith(" asc")) {
			end = "asc";
			replaceMent = replaceMent.substring(0, replaceMent.length() - 4);
		}
		
		StringBuffer sb = new StringBuffer();
		Matcher m = ORDER_PATTERN_HEAD.matcher(replaceMent);
		while (m.find()) {
			m.appendReplacement(sb, "");
		}
		m.appendTail(sb);
		replaceMent = sb.toString();
		// replaceMent = replaceMent.replaceAll("\\s*", "");
		int total = replaceMent.length();
		replaceMent = replaceMent.replaceAll("[(]", "");
		int newTotal = replaceMent.length();
		int leftQuot = total - newTotal;
		replaceMent = replaceMent.replaceAll("[)]", "");
		int rightQuot = newTotal - replaceMent.length();
		
		StringTokenizer st = new StringTokenizer(replaceMent, ",");
		sb.setLength(0);
		sb.append("order by ");
		boolean isNotFirst = false;
		while (st.hasMoreTokens()) {
			String orderByColumn = st.nextToken().trim();
			String append = "";
			if (orderByColumn.endsWith(" desc")) {
				append = "desc";
				orderByColumn = orderByColumn.substring(0, orderByColumn.length() - 5);
			}
			
			if (orderByColumn.endsWith(" asc")) {
				append = "asc";
				orderByColumn = orderByColumn.substring(0, orderByColumn.length() - 4);
			}
			
			if (isNotFirst) {
				sb.append(",");
			} else {
				isNotFirst = true;
			}
			if (isNeedConvert(orderByColumn)) {
				sb.append("convert(").append(orderByColumn.trim()).append(" using ").append(getEncoding()).append(") ");
			} else {
				sb.append(orderByColumn.trim()).append(" ");
			}
			sb.append(append);
		}
		if (isEndWithQuot && leftQuot != rightQuot) {
			sb.append(" )");
		}
		sb.append(end).append(" ");
		return sb.toString();
	}
	
	/**
	 * 判断该字段是否需要使用convert函数来进行排序
	 * @author jinxindong 2016年2月22日 下午2:12:07
	 * @param columnName
	 * @return
	 */
	private boolean isNeedConvert(String columnName) {
		int pos = columnName.indexOf(".");
		if (pos > 0) {
			columnName = columnName.substring(pos + 1, columnName.length());
		}
		if (!columnName.startsWith("c_")) {
			return false;
		}
		for (String column : IGNORE_COLUMN) {
			if (column.equals(columnName)) {
				return false;
			}
		}
		return true;
	}
	
	/**
	 * 获取数据库方言名称
	 * @author jinxindong 2016年2月22日 下午2:11:58
	 * @return
	 */
	public String getDialectName() {
		return dialectName;
	}
	
	/**
	 * 这里判断当前环境启动的时候使用什么方言，并放入dialectName属性中
	 * @author jinxindong 2016年2月22日 下午2:11:51
	 * @param dialectName
	 */
	public void setDialectName(String dialectName) {
		if (StringUtils.isBlank(dialectName)) {
			return;
		}
		try {
			Class<?> dialetClass = Class.forName(dialectName);
			while (dialetClass != Object.class) {
				if (dialetClass == MySQLDialect.class) {
					this.dialectName = "mysql";
					LOG.debug("use mysql dialect");
					break;
				}
				dialetClass = dialetClass.getSuperclass();
			}
			
		} catch (ClassNotFoundException e) {
			LOG.error("dialect name for bean hikbaseEntityInterceptor is not a class.", e);
		}
		
	}
	
	/**
	 * 获取encoding
	 * @author jinxindong 2016年2月22日 下午2:11:43
	 * @return
	 */
	public String getEncoding() {
		if (StringUtils.isBlank(encoding)) {
			return "gbk";
		}
		return encoding;
	}
	
	/**
	 * 设置encoding
	 * @author jinxindong 2016年2月22日 下午2:11:36
	 * @param encoding
	 */
	public void setEncoding(String encoding) {
		this.encoding = encoding;
	}
	
}
