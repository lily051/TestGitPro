/**
 * @ProjectName: 金融业务平台软件
 * @Copyright: 2010 HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @address: http://www.hikvision.com
 * @date: 2016年2月22日 上午11:09:27
 * @Description: 本内容仅限于杭州海康威视数字技术系统公司内部使用，禁止转发.
 */
package com.hikvision.finance.fms.common.util;

/**
 * 系统静态变量汇总
 * @author jinxindong 2016年2月22日 上午11:09:27
 * @version V1.0
 */
public final class Constants {
	
	public static final Integer ROOT_REG_PARENT = 0;
	public static final Integer ROOT_REG_ID = 1;
	public static final String ROOT_REG_CODE = "000000000000000000";
	public static final String ROLE_NAME_NULL = "无";
	public static final String ROLE_ID_NULL = "";
	public static final Integer ROOT_PRO_PARENT = 0;
	public static final Integer ROOT_PRO_ID = 1;
	public static final Integer CONS_0 = 0;
	public static final Integer CONS_1 = 1;
	public static final String CONS_0_STR = "0";
	public static final String CONS_1_STR = "1";
	public static final Integer EXCEL_MAX_NUM = 5000;
	public static final long SOFT_TIME=600;  //登录时间与加密狗中版本时间间隔 单位秒
	public static final class SessionAttrbutes {
		
		public static final String SESSIONPRX = "fms-";
		public static final String SESSION = "session";
		public static final String USER_SESSION = "userSession";
		public static final String MENUS = "menus";
		public static final String CMENUS = "cmenus";
	}
	
	/** 组织前缀 */
	public static final class TreeConstants {
		
		public static final String PREFIX_REG = "reg_";
		public static final String PREFIX_DEV = "dev_";
		public static final String PREFIX_CHANNEL = "channel_";
		public static final String PREFIX_PRODUCTTYPE = "productType_";
		public static final String PREFIX_PRODUCT = "product_";
	}
	
	/**组织上限*/
	public static final class RegNum{ //组织上限
		public static final int REGION_NUM=10; //组织
	}
	
	/** 角色类型 */
	public static final class RoleType {
		
		public static final int SYSTEM_MANAGER = 0;// 系统管理员
		public static final int CUSTOMIZE_ROLE = 1;// 自定义
		public static final int COLLECT_MANAGER = 3;//采集经理
	}
	
	/** 性别类型 */
	public static final class SexType {
		
		public static final int SEX_MAN = 1;// 男
		public static final int SEX_WOMEN = 0;// 女
	}
	
	/**
	 * 角色权限资源类型
	 */
	public static final class RoleResType {
		
		/** 组织类型 */
		public static final int ORG = 1;
		/** 设备类型 */
		public static final int DEVICE = 2;
	}
	
	/** 设备类型, 大类型5，类型50 */
	public static final class DeviceType {
		public final static int DEV_TYPE_KMS                = 50050 ; // KMS
		public final static int DEV_TYPE_DVR                = 50000 ; // DVR
		public final static int DEV_TYPE_ALARM_HOST         = 50001 ; //报警主机
		public final static int DEV_TYPE_GATE               = 50002 ; //门禁
		public final static int DEV_TYPE_TALK               = 50003 ; //对讲
		public final static int DEV_TYPE_DECODER            = 50004 ; //解码器
		public final static int DEV_TYPE_VIP                = 50005 ; //视频综合平台
		public final static int DEV_TYPE_SC                 = 50006 ; //防护舱(Safety Cabin)
		public final static int DEV_TYPE_IPC                = 50007 ; // IPC
		public final static int DEV_TYPE_DVS                = 50008 ; // DVS
		public final static int DEV_TYPE_PCDVR              = 50009 ; // PCDVR
		public final static int DEV_TYPE_AH_NETWORK_MODULE  = 50010 ; //报警主机网络模块
		public final static int DEV_TYPE_CVR                = 50011 ; // CVR
		public final static int DEV_TYPE_NVR                = 50012 ; // NVR
		public final static int DEV_TYPE_ENVIRONMENT        = 50013 ; // 环境量
		public final static int DEV_TYPE_FIRE               = 50014 ; // 消防
		public final static int DEV_TYPE_CLOUD_STORAGE      = 50015 ; // 云存储
		public final static int DEV_TYPE_CLOUD_STORAGE_OBJECT_STORAGE = 50060;//云存储-对象存储
	}
	
	/** 码流类型 */
	public static final class StreamType{   
		public static final int STREAMTYPE_MAINSTREAM=0;  //主码流
		public static final int	STREAMTYPE_SUBSTREAM=1;    //子码流
	}
	
	/** 录像计划是否下发 */
	public static final class RecorPlan{   
		public static final int PLAN_0=0;  //未下发
		public static final int	PLAN_1=1;    //已下发
	}
	
	
	/** 错误类型 */
	public static final class FailureType {
		
		public static final String NOT_FETCH_IDS = "NOT_FETCH_IDS"; // 未获取ids
		public static final String EXIST_RELATION_DATA = "EXIST_RELATION_DATA"; // 存在关联数据
		public static final String PASSWORD_UNFRESH = "PASSWORD_UNFRESH"; // 密码不新鲜
		public static final String PASSWORD_DEFAULT = "PASSWORD_DEFAULT"; // 密码默认
	}
	
	/** 用户状态 */
	public static final class UserStatus {
		
		public static final int NORMAL = 0; // 正常
		public static final int DISABLE = 1; // 禁用
		public static final int EXPIRED = 2; // 过期
		public static final int ONLINE = 3; // 在线
		public static final int NOTONLINE = 4; // 离线
	}
	
	/** 服务状态 */
	public static final class ServerStatus {
		
		public static final int ONLINE = 1; // 在线
		public static final int NOTONLINE = 0; // 离线
		public static final String ON_LINE = "在线"; // 在线
		public static final String NOT_ONLINE = "离线"; // 离线
	}
	
	/*** RspServerData 消息应答 */
	public static final class RSP_RESULT {
		public static final Integer SUCCESS = 0; // 成功
		public static final Integer FAIL = -1; // 失败
	}
	
	/** 产品风险等级 */
	public static final class ProductRiskLevel {
		
		public static final int HIGH = 2; // 高
		public static final int MIDDLE = 1; // 中
		public static final int LOW = 0; // 低
	}
	
	/** 产品录像保存保障时间类型*/
	public static final class ProductKeepType {
		public static final int DAY = 2; // 日
		public static final int MONTH= 1; // 月
		public static final int YEAR = 0; // 年
	}
	
	/** 字典参数 */
	public static final class SysDictionary {
		
		/** 字典启用-1表示 */
		public static final int enable = 1;
		/** 字典不启用-0表示 */
		public static final int disable = 0;
		
		/** 服务器定义 */
		public final static class ServerType { // 服务器类型
		
			/**字典大类型*/
			public static final class DictBigType{
				public static final int DICT_B_PRODUCT				 =0;  //产品大类型
				public static final int DICT_B_EVENT           		 =1;  //报警事件大类型
				public static final int DICT_B_SERVER				 =2;  //服务器大类型
			}
			/**字典小类型*/
			public static final class DictType{
				public static final int DICT_S_SERVER			    =20; //服务器类型
			}
			
			// public final static int MT_SERVICE_VRM = 20000; // VRM服务
			public final static int MT_SERVICE_TIMING = 20001; // 校时服务
			public final static int MT_SERVICE_GUARD = 20002; // 录像守卫服务
			public final static int MT_SERVICE_ALARM = 20003; // 报警服务(理财暂时没有报警服务)
			public final static int MT_SERVICE_UPLOAD = 20004; // 上传服务
			public final static int MT_WEB = 20200; // WEB客户端
		}
	}
	
	/**缓存主键前缀定义*/
	public static final class CacheKeyPrefix{
		public static final String PREFIX_SYSCONFIG = "Sysconfig_";
		public static final String PREFIX_ROLE = "Role_";
		public static final String PREFIX_USER = "User_";
		
	}
	
	/**文件上传保存根路径</br> <strong>严禁使用其他目录</strong> */
	public static final String FILEUPLOAD = "fileupload/";
	
	/** 系统参数配置 */
	public static final class SysConfigType {
		
		/**系统参数大类型*/
		public static final class SysBigType{
			public static final int SYS_B_LOGO				 =60;  //平台参数大类型
			public static final int SYS_B_VIDEO				 =61;  //录像参数大类型
			public static final int SYS_B_MODE               =62;  //模式参数大类型
			public static final int SYS_B_PWD				 =63;  //安全策略大类型
			public static final int SYS_B_LOG				 =64;  //日志保存大类型
			public static final int SYS_B_BUSINESS			 =65;  //业务参数大类型
			public static final int SYS_B_NTP			 	 =66;  //NTP参数大类型
			public static final int SYS_B_CSCLIENT			 =67;  //CS客户端参数大类型
		}
		
		/**平台参数*/
		public static final int PLATFORM_DISCRIBE = 6000;		//平台描述文字
		public static final int PLATFORM_COPYRIGHT = 6001;		//版权描述文字
		public static final int IMG_LOGO = 6002;				//登录页LOGO图标
		public static final int IMG_BAR = 6003;					//导航栏
		public static final int IMG_LOGIN = 6004;				//登录页图片
		
		/**录像参数*/
		public static final int  MAXIMUM_VIDEO_LENGTH = 6100; //最大录像时长
		public static final int  PLAYBACK_VIDEO_NUM = 6101; //同时回放录像数
		public static final int  VIDEO_PLAYBACK_BANDWIDTH = 6102; //录像回放带宽
		public static final int  VIDEO_UPLOAD_BANDWIDTH = 6103; //上传录像带宽
		public static final int  UPLOAD_TIME_START = 6104;  //守卫服务工作时间段开始时间
		public static final int  UPLOAD_TIME_END = 6105;  //守卫服务工作时间段结束时间
		public static final int  VIDEO_DELETE_DAYS = 6106;  //录像删除提示天数
		public static final int  UPLOAD_START_TIME = 6190;  //录像上传开始时间(与CS客户端协调, DB中没有对应数据)
		public static final int  UPLOAD_END_TIME = 6191;  //录像上传结束时间(与CS客户端协调, DB中没有对应数据)
		
		/**模式参数*/
		public static final int  VERIFICATION_CODE = 6200; //启用禁用验证码
		public static final int  CHOICE_IPC_USB = 6201; //选则usb还是ipc  ipc
		public static final int  PRODUCT_REVIEW = 6202; //启用禁用产品复核
		public static final int  OVERLAY_OSD  =6203; //启用禁用OSD叠加
		public static final int  CARD_READER_VIDEO = 6204; //启用禁用身份证读卡器刷卡开启录像
		public static final int  INPUT_MODE = 6205; //业务单录入方式
		public static final int  NIP_lIMIT = 6206; //IP限制
		public static final int  NON_CENTER_STORAGE = 6207;//无中心存储
		public static final int  LOG_REPORT = 6208;//c/s客户端日志上报
		public static final int  LOG_REPORT_METHOD = 6209;//c/s客户端日志上报方式：ftp/http
		
		/** 安全策略 */
		/** 密码保鲜天数 */
		public static final int PWD_FRESH_DAYS = 6300;
		/** 密码修改提示天数 */
		public static final int PWD_MODFIY_DAYS = 6301;
		/** 平台初始密码 */
		public static final int DEFAULT_PASSWORD = 6302;
		/** 密码安全等级 */
		public static final int PASSWORD_SECRUITY_LEVEL = 6303;
		/** 登录次数 */
		public static final int LOGIN_TIMES = 6304;
		/** 登录锁定时间 */
		public static final int LOCK_TIME = 6305;
		
		/**日志保存天数*/
		public static final int  CONFIG_LOG = 6400; //配置日志
		public static final int  SYSTEM_LOG = 6401; //系统日志
		public static final int  DELETE_LOG = 6402; //删除日志
		public static final int  REVIEW_LOG = 6403; //回放日志
		public static final int  ALARM_LOG = 6404; //报警日志
		public static final int  OPERA_LOG = 6405; //操作日志
		
		/**业务参数*/
		public static final int  TRANSACTION_ZONE_NUMBER = 6500;//交易区域号
		public static final int  TRADE_AREA_NAME = 6501;//交易区域名称
		public static final int  BUSINESS_SERIAL_NUMBER = 6502;//业务单流水号
		public static final int  CUSTOMER_NAME = 6503;//客户姓名
		public static final int  CUSTOMER_CARDTYPE = 6504;//客户证件类型（证件类型编码）
		public static final int  CUSTOMER_CARDNO = 6505;//客户证件号	
		public static final int  CUSTOMER_TEL = 6506;//客户电话
		public static final int  CUSTOMER_BANKNO = 6507;//客户银行卡号
		public static final int  PRODUCT_NAME = 6508;//产品名称
		public static final int  PRODUCT_CODE = 6509;//产品代码
		public static final int  PRODUCT_ORG = 6510;//产品发行机构
		public static final int  PRODUCT_SAVEYEAR = 6511;//产品保障期限
		public static final int  PRODUCT_ENDDATE = 6512;//产品到期日
		public static final int  VIDEO_SAVEYEAR = 6513;//视频保存时间（视频到期日=产品到期日（保障期限）+视频保存时间）
		public static final int  PRODUCT_RISKLEVEL = 6514;//产品风险等级
		public static final int  PURCHASE_AMOUNT = 6515;//购买金额
		public static final int  CLERK_NUMBER = 6516;//业务员编号
		public static final int  HANDLING_TIME = 6517;//办理时间 前端获取本机时间
		public static final int  TRANSACTION_REMARK = 6518;//备注
		
		public static final int  NTP_ADDRESS = 6600;//NTP服务地址
		public static final int  NTP_PORT = 6601;//NTP端口
		public static final int  NTP_INTERVAL = 6602;//NTP校时间隔
		
		public static final int  LOGUPLOAD_URL = 6700;//CSClient日志上传路径
		public static final int  VERSION_URL = 6701;//CSClient版本更新路径
		public static final int  VERSION_NO = 6702;//CSClient最新版本号
		public static final int  FORCE_UPDATE = 6703;//CSClient强制升级(1:启用 0:禁用)
		
		/** 监控单号，取自授权文件 */
		public final static int DOG_NOTES_ID = 6800;
		
		/** LicenseNo，取自授权文件 */
		public final static int DOG_LICENSE_NO = 6801;
		
		public final static int FAIL_COUNT = 6304; //登陆错误最大次数
		public final static int DISTANCE_MINUTES = 6305; //账户锁定的分钟数
		
		public final static int BEFORE_DAYS = 7000; //USB删除n天前的视频
		
		public static final String Default_NAVBAR_URL="/images/logo_main.png";		//导航栏logo地址
		public static final String Default_LOGO_URL="/images/login_logo.png";		//登录页logo地址 
		public static final String Default_LOGIN_URL="/images/login_bg.png";		//登录页背景图片
		
		/** 系统通道类型*/
		public static final class ChannelType{
			public static final int IPC_CVR	= 1;  // IPC-CVR
			public static final int USB_KMS = 2;  // USB_KMS
			public static final int USB_CLOUD_STORAGE_OBJECT_STORAGE = 3;// USB_CLOUD_STORAGE_OBJECT_STORAGE
		}
	}
	
	/**登录0 登出1*/
	public static final class LoginStatus {
		
		public static final int LOGIN_IN = 0; // 登录
		public static final int LOGIN_OUT = 1; // 登出
	}
	
	/**操作状态*/
	public static final class OperStatus {
		public static final int ADD = 0; // 添加
		public static final int DELETE = 1; // 删除
		public static final int UPDATE = 2; // 修改
		public static final int IMPORT = 3; // 导入
		public static final int EXPORT = 4; // 导出
	
	}
	
	/** 统计报表类型 */
	public static final class EchatsType {
		/** 按年 */
		public static final int ORDER_YERA = 2;
		/** 按季度 */
		public static final int ORDER_QUARTER = 1;
		/** 按月 */
		public static final int ORDER_MONTH = 0;
	}
	
	/** 录像类型 */
	public static final class RecordType{   
		public static final int RECORD_SIGN=1;    //签约录像
		public static final int	RECORD_COMPLAINNT=2;    //投诉录像
	}
	
	public static final class StrRecordType{   
		public static final String STR_RECORD_SIGN= "signvideo";    //签约录像
		public static final String STR_RECORD_COMPLAINNT = "complainvideo";    //投诉录像
	}
	
	/** 业务单状态 */
	public static final class ClientStatus{   
		public static final int CLIENT_SIGN=0;    //0-签约
		public static final int	CLIENT_COMPLAINNT=1;    //1-投诉
	}
	
	/** 报警处理状态 */
	public static final class AlarmLogStatus {
		
		public static final int PROCESSED = 1; // 处理
		public static final int UNPROCESSED = 0; // 未处理
	}
	
	/** 报警类型 */
	public static final class AlarmLogType {
		
		public static final int WORK_TYPE_CENTTER_OFFLINE           = 60001; // 中心不在线
		public static final int WORK_TYPE_TRANSPORT_SVC_OFFLINE     = 61001; // 上传服务不在线
		public static final int WORK_TYPE_TRANSPORT_SVC_FAIL        = 61002; // 上传服务失败
		public static final int WORK_TYPE_GUARD_SVC_OFFLINE         = 62001; // 录像守卫服务不在线
		public static final int WORK_TYPE_GUARD_SVC_FAIL            = 62002; // 录像守卫服务失败
		public static final int WORK_TYPE_ADJUST_SVC_OFFLINE        = 63001; // 巡检服务不在线
		public static final int WORK_TYPE_ADJUST_SVC_FAIL           = 63002; // 巡检服务失败
		public static final int WORK_TYPE_PATROL_SVC_FAIL           = 64001; // DVR巡检失败
		public static final int WORK_TYPE_DVR_OFFLINE               = 64002; // DVR不在线
		public static final int WORK_TYPE_DVR_TIME_ERROR            = 64003; // 巡检服务DVR和本地时间差很大
		public static final int WORK_TYPE_DVR_VI_LOST               = 64004; // DVR视频信号丢失
		public static final int WORK_TYPE_DVR_HD_EXCEPTION          = 64005; // DVR硬盘异常
		public static final int WORK_TYPE_DVR_OPERATION_FAIL        = 64006; // DVR某些操作失败
		public static final int WORK_TYPE_CVR_OFFLINE               = 65001; // CVR不在线
		public static final int WORK_TYPE_CVR_CANNOT_LOGIN          = 65002; // CVR登录失败
		public static final int WORK_TYPE_CVR_NEARLLY_FULL          = 65003; // CVR磁盘满
		public static final int WORK_TYPE_CVR_OPERATION_FAIL        = 65004; // CVR某些操作失败
		public static final int WORK_TYPE_CLOUD_OFFLINE             = 66001; // Cloud不在线
		public static final int WORK_TYPE_CLOUD_CANNOT_LOGIN        = 66002; // Cloud登录失败
		public static final int WORK_TYPE_CLOUD_GET_POOLLIST_FAIL   = 66003; // 获取录像池超时
		public static final int WORK_TYPE_CLOUD_NEARLLY_FULL        = 66004; // Cloud磁盘满
		public static final int WORK_TYPE_CLOUD_OPERATION_FAIL      = 66005; // Cloud某些操作失败
		public static final int WORK_TYPE_KMS_OFFLINE               = 67001; // KMS不在线
		public static final int WORK_TYPE_KMS_NEARLLY_FULL          = 67002; // KMS磁盘满
		public static final int WORK_TYPE_KMS_OPERATION_FAIL        = 67003; // KMS某些操作失败
		public static final int WORK_TYPE_VRM_SVC_OFFLINE           = 68001; // VRM服务不在线
		
	}
	
	/** 树节点读取策略 */
	public static final class TreeReadStrategy {
		/** 只读取节点本身 */
		public static final int ONLY_SELF = 0;
		/** 包含子节点 */
		public static final int CONTAIN_CHILD = 1;		
	}
	
	/** 录像到期状态 */
	public static final class VideoDueStatus {
		/** 未到期 */
		public static final int NOT_DUE = 0;
		/** 已到期 */
		public static final int IS_DUE = 1;
	}
	
	/** 业务单读状态 */
	public static final class BusiReadStatus {
		/** 未读 */
		public static final int NOT_READ = 0;
		/** 已读 */
		public static final int READED = 1;
	}
	
	/** 录像上传状态 */
	public static final class VideoUploadStatus {
		/** 未上传 */
		public static final int NOT_UPLOAD = 0;
		/** 已上传 */
		public static final int UPLOADED = 1;
	}
	
	/** 业务单纠纷状态 */
	public static final class BusiComplainStatus {
		/** 没有纠纷 */
		public static final int NOT_COMPLAIN = 0;
		/** 有纠纷 */
		public static final int COMPLAINED = 1;
	}
	
	/** 合规检查状态 */
	public static final class RecordCheckStatus {
		/** 未检查 */
		public static final int NOT_CHECK = 0;
		/** 不合规 */
		public static final int NOT_CONFORMED = 1;
		/** 合规 */
		public static final int CONFORMED = 2;
		/** 存在投诉 */
		public static final int HAS_COMPLAIN = 3;
		/** 投诉关闭 */
		public static final int COMPLAIN_CLOSED = 4;
	}
	
	/** 录像状态 */
	public static final class RecordStatus {
		
		/** 开始记录 */
		public static final int START_RECORD = 0;
		/** 结束记录（未关联） */
		public static final int END_RECORD = 1;
		/** 已关联（未上传） */
		public static final int CONTRACTED = 2;
		/** 上传中 */
		public static final int UPLOADING = 3;
		/** 已上传 */
		public static final int UPLOADED = 4;
		/** 异常结束记录（未关联） */
		public static final int END_ERROR_RECORD = 8;
		/** 上传失败 */
		public static final int UPLOAD_FAIL = 9;
		/** 已删除 */
		public static final int DELETED = 100;
		
	}
	
	/** records表常量 */
	public static final class Records {
		public static final class recordType{
			/** 正常录像 */
			public static final int NORMAL_RECORD = 1;
			/** 补录录像 */
			public static final int REPAIR_RECORD = 2;
		}
		public static final class recordFileType{
			/** DVR/NVR录像 */
			public static final int DVR_RECORD = 1;
			/** 本地录像文件 */
			public static final int LOCAL_RECORD = 2;
			/** 本地录像上传对象云 */
			public static final int LOCAL_CLOUD_RECORD = 3;
		}
	}
	
	/** 操作日志使用软件类型 */
	public static final class OperLogClientType {
		
		public static final int BROWSER = 1;
		public static final int CLIENT = 2;
	}
	
	/** 操作日志操作类型 */
	public static final class OperLogType{
		/** 开始录像 */
		public static final int OperStartVideo = 1;
		/** 结束录像 */
		public static final int OperEndVideo = 2;
		/** 视频补录 */
		public static final int OperRepairVideo = 3;
		/** 添加本地离线录像 */
		public static final int OperAddLocalVideo = 4;
		/** 编辑录像名 */
		public static final int OperEditVideo = 5;
		/** 删除录像*/
		public static final int OperDelVideo = 6;
		/** 处理异常录像*/
		public static final int OperHandleAbnormalVideo = 7;
		/** 录单 */
		public static final int OperOrder = 11;
		/** 关联业务单*/
		public static final int OperRel = 12;
		/** 编辑业务单*/
		public static final int OperEditOrder = 13;
		/** 删除业务单*/
		public static final int OperDelOrder = 14;
	}
	
	public static final class ProductEnable {
		public static final int FORBIDDEN = 0; // 禁用
		public static final int STARTUSING = 1; // 启用
	}
	
	/**DC接口常量--BEG */
	
	/**接口执行后的状态 */
	public static final class ResultStatus{
		/**失败 */
		public static final int RESULT_FAILE = -1;
		/**成功 */
		public static final int RESULT_SUCCESS = 0;
		
	}
	
	/**DC接口常量--END */
	
	/**业务记录审批状态 */
	public static final class LockClient {
		public static final int NO_AUDIT = 0;//待审批
		public static final int PASS = 1; // 审批通过
		public static final int SEND_BACK = 2; // 审批退回
	}
	
	/**登录的类型  */
	public static final class ModuleType {
		public static final int CENTER = 0;//中心端
		public static final int COLLECT = 1; // 采集页面
		public static final int CSCLIENT = 2; // CS客户端
	}
	
	/**是否为初始化密码  */
	public static final class PasswordType {
		public static final int PERSONAL_PASSWORD = 0;//不是初始化密码
		public static final int INITAL_PASSWORD = 1; // 是初始化密码
	}
	
	/**DC接口常量 */
	/**c/s客户端日志上报功能禁用0  c/s客户端日志上报功能启用1*/
	public static final class LogReportStatus {
		public static final int FORBIDDEN = 0;//禁用
		public static final int ENABLED = 1; //启用
	}
	
	/**DC接口常量 */
	/**c/s客户端日志上报功能启用后，日志上报方式：http-0 ftp-1*/
	public static final class LogReportMethodStatus {
		public static final int HTTP = 0;//http上传方式
		public static final int FTP = 1; //ftp上传方式
	}
	
	/**密码等级  */
	public static final class PasswordSecurityLevel {
		public static final String RISK_PWD = "0";//风险密码
		public static final String WEAK_PWD = "1";//弱密码
		public static final String MIDDLE_PWD = "2";//中密码
		public static final String STRONG_PWD = "3";//强密码
		public static final String NO_PWD = "4";//无密码
	}
	
	/**通道方式  */
	public static final class ModeType {
		public static final String IPC = "1"; // ipc模式
		public static final String USB = "2"; // usb模式
	}
	
	/**产品密码等级  */
	public static final class RiskLevel {
		public static final String RISKLEVEL_ONE ="1R";
		public static final String RISKLEVEL_TWO = "2R";
		public static final String RISKLEVEL_THREE = "3R";
		public static final String RISKLEVEL_FOUR = "4R";
		public static final String RISKLEVEL_FIVE = "5R";
		public static final String RISKLEVEL_SIX = "6R";
		public static final String RISKLEVEL_NONE = "—";
	}
	
}
