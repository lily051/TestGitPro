/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月20日 下午2:58:52
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.common.enums;

import java.util.HashMap;
import java.util.Map;


/**
 * <p>与clientinput表对应的枚举类</p>
 * @author jinchenwu 2016年4月20日 下午2:58:52
 * @version V1.0 
 */
public class ClientInputEnum {
	
	/** 业务单纠纷状态 */
	public static enum BusiComplainStatus {
		
		/** 无纠纷 */
		NOT_COMPLAIN(0, "无纠纷"),
		/** 有纠纷 */
		COMPLAINED(1, "有纠纷");
		
		private BusiComplainStatus(Integer nType, String strType) {
			this.nType = nType;
			this.strType = strType;
		}
		
		private Integer nType;
		private String strType;
		
		public Integer getNType() {
			return nType;
		}
		
		public String getStrType() {
			return strType;
		}
		
		private static final Map<Integer, String> NTOSTRMAP = new HashMap<Integer, String>();
		
		static {
			for (BusiComplainStatus singleElem : BusiComplainStatus.values()) {
				NTOSTRMAP.put(singleElem.getNType(), singleElem.getStrType());
			}
		}
		
		public static String getStrByN(Integer ntype) {
			if (NTOSTRMAP.containsKey(ntype)) {
				return NTOSTRMAP.get(ntype);
			} else {
				return "未知";
			}
		}
	}
	
	/** 业务单和录像是否到期 */
	public static enum VideoDueStatus {
		
		/** 未到期 */
		NOT_DUE(0, "未到期"),
		/** 已到期 */
		IS_DUE(1, "已到期");
		
		private VideoDueStatus(Integer nType, String strType) {
			this.nType = nType;
			this.strType = strType;
		}
		
		private Integer nType;
		private String strType;
		
		public Integer getNType() {
			return nType;
		}
		
		public String getStrType() {
			return strType;
		}
		
		private static final Map<Integer, String> NTOSTRMAP = new HashMap<Integer, String>();
		
		static {
			for (VideoDueStatus singleElem : VideoDueStatus.values()) {
				NTOSTRMAP.put(singleElem.getNType(), singleElem.getStrType());
			}
		}
		
		public static String getStrByN(Integer ntype) {
			if (NTOSTRMAP.containsKey(ntype)) {
				return NTOSTRMAP.get(ntype);
			} else {
				return "未知";
			}
		}
	}
	
	/** 到期业务单和录像是否已读 */
	public static enum BusiReadStatus {
		
		/** 未到期 */
		NOT_READ(0, "未读"),
		/** 已到期 */
		READED(1, "已读");
		
		private BusiReadStatus(Integer nType, String strType) {
			this.nType = nType;
			this.strType = strType;
		}
		
		private Integer nType;
		private String strType;
		
		public Integer getNType() {
			return nType;
		}
		
		public String getStrType() {
			return strType;
		}
		
		private static final Map<Integer, String> NTOSTRMAP = new HashMap<Integer, String>();
		
		static {
			for (BusiReadStatus singleElem : BusiReadStatus.values()) {
				NTOSTRMAP.put(singleElem.getNType(), singleElem.getStrType());
			}
		}
		
		public static String getStrByN(Integer ntype) {
			if (NTOSTRMAP.containsKey(ntype)) {
				return NTOSTRMAP.get(ntype);
			} else {
				return "未知";
			}
		}
	}
}
