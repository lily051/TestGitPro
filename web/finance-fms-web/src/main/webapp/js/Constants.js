/** 常量定义 */
var Constants = Constants || {};
/** 前缀 */
Constants.PREFIX = {
	ORG: 'org_',
	REGION: 'reg_',
	CHANNEL: 'channel_',
	VEHICLE: 'vehicle_',
	VEHICLECHANNEL: 'vehiclechannel_',
	VGRP: 'vgrp_',
	SGRP: 'sgrp_',
	GGRP: 'ggrp_',
	PRODUCTTYPE: 'productType_',
	PRODUCT: 'product_'
};
/** 角色类型 */
Constants.RoleType = {
	SYSTEM_MANAGER: 0,
	CUSTOMIZE_ROLE: 1
};
/** 服务类型 */
Constants.ServerType = {
	SERVICE_UPLOAD: 20004, // 上传服务,
	SERVICE_WEB: 20200,		//WEB服务
	SERVICE_TIMING: 20001, // 系统巡检服务
	SERVICE_RECGUARD: 20002  //录像守卫服务
};
/** 角色资源类型 */
Constants.RoleResType = {
	ORG: 1,
	VEHICLE: 2
};
/** GIS类型 */
Constants.GISTYPE = {
	POLYGON: 1,
	POLYLINE: 2
};
/**参数配置 */
Constants.Sysconfig = {
	videoBeforeRelated:0,//录像后关联
	relatedBeforeVideo:1//关联后录像
};
Constants.device = {
	DEV_TYPE_CVR:50011,//CVR
	DEV_TYPE_CLOUD_STORAGE:50015,//云存储
	DEV_TYPE_CLOUD_STORAGE_OBJECT_STORAGE:50060,//云存储-对象存储
	DEV_TYPE_KMS:50050,//KMS
	DEV_TYPE_DVR:50000,//DVR
	DEV_TYPE_NVR:50012//NVR
};
Constants.log = {
	LOGIN_IN:0,//登录
	LOGIN_OUT:1,//登出
	ADD:0,//新增
	DELETE:1,//删除
	UPDATE:2//修改

};
//录像文件类型
var RecordFileTypes=[
			[0,"Schedule"],
			[1,"Motion"],
			[2,"Alarm"],
			[3,"Alarm | Motion"],
			[4,"Alarm & Motion"],
			[5,"Command"],
			[6,"Manual"],
			[7,"Backup"],
			[8,"Trigger Backup"],
			[255,"All"]];

Constants.record = {
	START_RECORD:0,//开始记录
	RECOEND_RECORD:1,//结束记录（未关联)
	CONTRACTED:2,//已关联（未上传）
	UPLOADING:3,//上传中
	UPLOADED:4,//已上传
	REFETCH:5,//补录
	REFETCHING:6,//补录中
	REFETCH_FAILED:7,//补录失败
	END_ERROR_RECORD:8,//结束异常记录（未关联）
	UPLOAD_FAIL:9,//上传失败
	DELETED:100,//已删除
	CLIINPUT_TYPE_SIGN:0,//签约单
	CLIINPUT_TYPE_COMPLAIN:1,//投诉单
	picturePath:'C:\\FMS8000\\PV_Picture\\',
	recordPath:'C:\\FMS8000\\PV_Record\\',
	dataBase:1,//从数据库获取
	cache:2//从缓存获取
}

//产品密码等级
Constants.RiskLevel = {
	RISKLEVEL_ONE:'1R',
	RISKLEVEL_TWO:'2R',
	RISKLEVEL_THREE:'3R',
	RISKLEVEL_FOUR:'4R',
	RISKLEVEL_FIVE:'5R',
	RISKLEVEL_SIX:'6R',
	RISKLEVEL_NONE:'—'
}