/**	@file DBDataMgr.h
*	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief 服务配置类
*	@author		chenbin5
*	@date		2016-3-5
*	@note 
*/
#pragma once
#include "../util/Singleton.h"
#include "center/Collectconfig.pb.h"

class CConfig
{
    SINGLETON_DECLARE(CConfig)///<单例相关声明

private:
    CConfig(void);
    virtual ~CConfig(void);
// public:
//     /**	@fn	    Init
// 	*	@brief	初始化函数
// 	*	@param  [in] 
// 	*	@return	int，0 表示没有错误，其他数值表示出现了错误
// 	*/
// 	int Init();
// 
// 	/**	@fn	    Fini
// 	*	@brief	反初始化函数，负责资源释放
// 	*	@param  [in] 无
// 	*	@param  [out] 无
// 	*	@return	int，0 表示没有错误，其他数值表示出现了错误
// 	*/
// 	int Fini();
// 
//     /**	@fn	    Start
//     *	@brief	开始函数，启动此模块的核心功能，一般这里应该开启某个线程进行实际的工作
//     *          此函数应该尽快返回，不应该占用太多时间，必须小于30s
//     *	@param  [in] 无
//     *	@param  [out] 无
//     *	@return	int，0 表示没有错误，其他数值表示出现了错误
//     */
//     int Start();
// 
//     /**	@fn	    Stop
//     *	@brief	停止函数
//     *	@param  [in] 无
//     *	@param  [out] 无
//     *	@return	int，0 表示没有错误，其他数值表示出现了错误
//     */
//     int Stop();

public:
    /**	@fn	    Deciphering
    *	@brief	解密操作
    *	@param  [in] strPwd -- 密文密码
    *	@param  [out] strPwd -- 明文密码
    *	@return	void
    */
    void Deciphering(std::string& strPwd);

    /**	@fn	    LoadConfigFile
    *	@brief	加载配置文件
    *	@param  [in] 无
    *	@param  [out] 无
    *	@return	int，0 表示没有错误，其他数值表示出现了错误
    */
    int LoadConfigFile(void);

    /**	@fn	    LoadLocalCfgInfo
    *	@brief	加载本地配置文件
    *	@param  [in] 无
    *	@param  [out] 无
    *	@return	void
    */
    void LoadLocalCfgInfo(void);

    /**	@fn	    LoadWebConfigXML
    *	@brief	加载WEB客户端生成的XML
    *	@param  [in] 无
    *	@param  [out] 无
    *	@return	
    */
    void LoadWebConfigXML(const std::string &sConfigXML);

    /**	@fn	    UpdateATSvrInfo
    *	@brief	更新系统巡检服务器信息
    *	@param  [in] sATSvrIP -- 巡检服务器IP 
    *	@param  [in] nATPort -- 巡检服务器端口
    *	@param  [out] 无
    *	@return	void
    */
    void UpdateATSvrInfo(const std::string &sATSvrIP, const int &nATPort);

    /**	@fn	    UpdateCollectInfo
    *	@brief	更新采集平台信息
    *	@param  [in] rspCollect -- 采集平台信息
    *	@param  [out] 无
    *	@return	int，0 表示没有错误，其他数值表示出现了错误
    */
    //void UpdateCollectInfo(const cms_8120::RspCollectconfig &rspCollect);

    /**	@fn	    InitTransInterval
    *	@brief	初始化上传时间间隔
    *	@param  [in] strConfig -- 配置文件名
    *	@param  [out] 无
    *	@return	void
    */
    /*void InitTransInterval(const std::string strConfig);*/

    /**	@fn	    InitReservedTime
    *	@brief	初始化预留时间
    *	@param  [in] strConfig -- 配置文件名
    *	@param  [out] 无
    *	@return	void
    */
    /*void InitReservedTime(const std::string strConfig);*/

private:
    /**	@fn	    GetAppRunPath
    *	@brief	获取程序运行路径
    *	@param  [in] 无
    *	@param  [out] sPathName 程序路径
    *	@return	
    */
    void GetAppRunPath(std::string &sPathName);

public:
    std::string m_sFileVersion;     //文件版本信息
    std::string m_sFileDescription; //文件描述信息

    std::string m_strLocalIP;       //本地IP
    std::string m_sLoginName;       //登录用户名
    std::string m_sLoginPwd;        //登录用户密码

    std::string m_strCenterIP;      //中心DCIP
    int         m_nCenterPort;      //中心DC端口

//     int         m_nCollectID;       //采集平台ID
//     std::string m_strCollectIP;     //采集平台IP
//     int         m_nCollectPort;     //采集平台端口

    std::string m_strATServerIP;    //系统巡检服务IP
    int         m_nATServerPort;    //系统巡检服务端口
   
    bool        m_isAutoDelRecord;  //是否自动删除records里的记录  
    bool        m_isAutoDelFile;    //是否自动删除录像文件 
//     bool        m_isUpdatePercent;  //更新上传百分比
//     int         m_npercentStep;     //上传进度步长
    int         m_nTaskCount;       //上传任务数量
	int         m_nHaveCenterStorage;//有无中心存储（0无，1有）
    int         m_nMode;            //上传模式（1为IPC，2为USB）
    bool        m_bLocalConfig;     //是否启用本地配置获取采集平台信息

    // 上传任务之间的时间间隔,单位秒
    int         m_nUploadInteval;
    // 状态更新时间间隔,单位秒
    int         m_nStatusInteval;

    //上传工作时间
    std::string m_strStartTime;     //录像上传开始时间
    std::string m_strStopTime;      //录像上传结束时间

    int         m_nUpdateDataTime;  //更新数据时间(更新录像信息)
	int         m_nUploadSpeed;    //上传速度，单位:Kbps

	// 开始时间最大偏差
	int         m_nOffsetStartTime;
	// 结束时间最大偏差
	int         m_nOffsetStopTime;
};
