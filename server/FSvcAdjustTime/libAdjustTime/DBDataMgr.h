/**	@file DBDataMgr.h
*	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief DC数据交互类
*	@author		chenbin5
*	@date		2016-3-4
*	@note 
*/
#pragma once
#include <HPR_Singleton.h>
#include "verify/RspServerLogin.pb.h"
#include <fclib/fcTchar.h>
//状态消息
typedef struct _work_state_t {
	int           nKey;            //报警类型
	int           nParentKey;      //报警模块
	int           nMsgType;        //报警消息类型
	int           nState;          //1 报警 0 非报警
	int           nUserID;         //用户
	int           nChannel;        //通道
	int           nRev;            //导致报警的操作类型
	int           nBeginOccureTime; //第一次发生时间
	int           nLastOccureTime; //上一次发生时间
	int           nOccureTime;     //发生时间
	__int64       nAlarmID;        //0 新报警，1已存在报警
	int           nRegionID;       //报警区域
	std::_tstring strKeyValue;     //报警类型中文名称
	std::_tstring strOccureTime;   //发生时间字符串
	std::_tstring strInfo;         //报警信息
	std::_tstring strHost;         //发报警的主机ip
	_work_state_t()
		: nKey(0)
		, nParentKey(0)
		, nMsgType(0)
		, nState(0)
		, nUserID(0)
		, nChannel(0)
		, nRev(0)
		, nBeginOccureTime(0)
		, nLastOccureTime(0)
		, nOccureTime(0)
		, nAlarmID(0)
		, nRegionID(1)
		, strKeyValue(_T(""))
		, strOccureTime(_T(""))
		, strInfo(_T(""))
		, strHost(_T(""))
	{}
}work_state_t;

//状态消息
typedef struct _Struct_AlarmLog {
	int           nID;             //报警日志流水号
	int           nState;          //1 报警 0 非报警
	std::string   strHost;         //报警主机IP
	_Struct_AlarmLog()
		: nID(-1)
		, nState(-1)
	{}
}Struct_AlarmLog;

class CDBDataMgr: public singleton<CDBDataMgr>
{
	friend class singleton<CDBDataMgr>;

public:
    CDBDataMgr(void);
    virtual ~CDBDataMgr(void);
public:
    /**	@fn	    Init
	*	@brief	初始化函数
	*	@param  [in] 
	*	@return	int，0 表示没有错误，其他数值表示出现了错误
	*/
	int Init();

	/**	@fn	    Fini
	*	@brief	反初始化函数，负责资源释放
	*	@param  [in] 无
	*	@param  [out] 无
	*	@return	int，0 表示没有错误，其他数值表示出现了错误
	*/
	int Fini();

    /**	@fn	    Start
    *	@brief	开始函数，启动此模块的核心功能，一般这里应该开启某个线程进行实际的工作
    *          此函数应该尽快返回，不应该占用太多时间，必须小于30s
    *	@param  [in] 无
    *	@param  [out] 无
    *	@return	int，0 表示没有错误，其他数值表示出现了错误
    */
    int Start();

    /**	@fn	    Stop
    *	@brief	停止函数
    *	@param  [in] 无
    *	@param  [out] 无
    *	@return	int，0 表示没有错误，其他数值表示出现了错误
    */
    int Stop();
public:
    /**	@fn	    Login
    *	@brief	登录
    *	@param  [in] 无
    *	@return	ENUM_LOGIN_RESULT，LOGIN_RESULT_OK,LOGIN_RESULT_FAIL
    */
    ENUM_LOGIN_RESULT Login();

    /**	@fn	    Logout
    *	@brief	登出
    *	@param  [in] 无
    *	@param  [out] 无
    *	@return	int，0 表示没有错误，其他数值表示出现了错误
    */
    int Logout();

    /**	@fn	    SendLoginReq
    *	@brief	发送登录消息
    *	@return	int，0 表示没有错误，其他数值表示出现了错误
    */
    int SendLoginReq();

    /**	@fn	    SendLogoutReq
    *	@brief	发送退出消息
    *	@param  [in]无
    *	@return	
    */
    void SendLogoutReq();

	/**	@fn	    SendLoginRsp
    *	@brief	获得登陆响应消息
    *	@param  [in] serLoginRsp 登陆响应信息
    *	@return	
    */
    void SendLoginRsp(const cms_8120::RspServerLogin & serLoginRsp);

	/**	@fn	    GetAllCollectDeviceReq
    *	@brief	请求获取采集设备
    *	@param  [in]  无
    *	@param  [out] 无
    *	@return	int，0 表示没有错误，其他数值表示出现了错误
    */
    int GetAllCollectDeviceReq();

    /**	@fn	    GetStorageDevice
    *	@brief	请求获取存储设备
    *	@param  [in]  无
    *	@param  [out] 无
    *	@return	int，0 表示没有错误，其他数值表示出现了错误
    */
    int GetAllStorageDeviceReq();

	/**	@fn	    GetAlarmLogReq
	*	@brief	获得报警处理状态
	*	@param  [in] workstate -- workstate信息
	*	@return	int，0 表示没有错误，其他数值表示出现了错误
	*/
	int GetAlarmLogReq(const work_state_t &workstate);

	/** @fn    AddCenterAlarmReq
	*  @brief  中心报警信息表添加报警
	*  @param  [in]workstate, workstate信息
	*  @param  [in]ulCheckType, 100 dvr,102 采集器服务，200中心服务
	*  @return bool
	*/
	int AddCenterAlarmReq(const work_state_t &workstate, DWORD ulCheckType);

	/** @fn     UpdateCenterAlarmReq
     *  @brief  中心报警信息更新报警
     *  @param  [in]workstate, workstate信息
     *  @param  [in]nID 报警日志流水号
     *  @return int 0表示无错误
     */
    int UpdateCenterAlarmReq(const work_state_t &workstate, int nID);

   /** @fn     UpdateDeviceOnlineStateReq
	*  @brief  更新设备在线状态
	*  @param  [in]Device_Type 设备类型
	*  @param  [in]strIP       设备IP地址
	*  @param  [in]nPort       设备端口号
	*  @param  [in]iOnline,    0不在线 1在线
	*  @return int 0表示无错误
	*/
	int UpdateDeviceOnlineStateReq(int Device_Type, const std::string &strIP, int nPort, int iOnline);

   /** @fn    UpdateStorageCapacityReq
	*  @brief  更新存储设备容量
	*  @param  [in]Storage_Type 存储设备类型
	*  @param  [in]strIP        存储设备IP地址
	*  @param  [in]nPort        存储设备端口号
	*  @param  [in]iTotalSpace, 总容量
	*  @param  [in]iFreeSpace,  剩余容量
	*  @return int 0表示无错误
	*/
	int UpdateStorageCapacityReq(int Storage_Type, const std::string &strIP, int nPort, unsigned long long iTotalSpace, unsigned long long iFreeSpace);

	/** @fn    UpdateServerHeartBeartReq
	*  @brief  向中心服务表更新巡检服务心跳
	*  @return int 0表示无错误
	*/
	int UpdateServerHeartBeartReq();

private:
    bool m_bInit;                                       //是否被初始化过
    bool m_bStart;                                      //是否启动过

    HANDLE m_hLoginRetEvent;                            //登录返回信号
public:
    cms_8120::RspServerLogin m_ServerLoginRsp;          //服务器登陆响应信息
};
inline CDBDataMgr* GetDBDataMgrPtr(void)
{
	return CDBDataMgr::instance();
}