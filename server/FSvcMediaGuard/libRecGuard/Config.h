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

#define TOP_DIRECTORY_CONFIG_FILE   "ServiceConfig.xml"
#define NODE_ROOT			"ROOT"
#define NODE_LOCAL_PARAM	"LOCAL_PARAM"
#define NODE_DATA_CENTER	"DATA_CENTER"

#define NODE_PARAM_RECGUARD "PARAM_RECGUARD"
#define NODE_PARAM_ATSERVER "PARAM_ATSERVER"

class CConfig
{
    SINGLETON_DECLARE(CConfig)///<单例相关声明

private:
    CConfig(void);
    virtual ~CConfig(void);

public:
    /**	@fn	    LoadConfigFile
    *	@brief	加载配置文件
    *	@param  [in] 无
    *	@param  [out] 无
    *	@return	int，0 表示没有错误，其他数值表示出现了错误
    */
    int LoadConfigFile(void);

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

    std::string m_strATServerIP;    //系统巡检服务IP
    int         m_nATServerPort;    //系统巡检服务端口

    // 任务之间的时间间隔,单位毫秒
    int         m_nTaskInteval;
	int         m_nHaveCenterStorage;//有无中心存储（0无，1有）

    //录像守卫工作时间
    std::string m_strStartTime;     //录像守卫开始时间
    std::string m_strStopTime;      //录像守卫结束时间

    //int         m_nUpdateDataTime;  //更新数据时间
};
