/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年4月21日 上午10:08:18
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.common.enums;

import java.util.HashMap;
import java.util.Map;


/**
 * <p>与prosaverecord表对应的枚举类</p>
 * @author jinchenwu 2016年4月21日 上午10:08:18
 * @version V1.0 
 */
public class ProSaveRecordEnum {
	
	/** 录像上传状态 */
	public static enum VideoUploadStatus {
		
		/** 未上传 */
		NOT_UPLOAD(0, "未上传"),
		/** 已上传 */
		UPLOADED(1, "已上传");
		
		private VideoUploadStatus(Integer nType, String strType) {
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
			for (VideoUploadStatus singleElem : VideoUploadStatus.values()) {
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
	
	/** 合规检查状态 */
	public static enum RecordCheckStatus {
		
		/** 未检查 */
		NOT_CHECK(0, "未检查"),
		/** 不合规 */
		NOT_CONFORMED(1, "不合规"),
		/** 合规 */
		CONFORMED(2, "合规"),
		/** 存在投诉 */
		HAS_COMPLAIN(3, "存在投诉"),
		/** 投诉关闭 */
		COMPLAIN_CLOSED(4, "投诉关闭");
		
		private RecordCheckStatus(Integer nType, String strType) {
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
			for (RecordCheckStatus singleElem : RecordCheckStatus.values()) {
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
