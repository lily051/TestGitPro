/**	@file    Config.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   配置类
 *
 *	@author	 chenxiaoyan3
 *	@date	 2014/11/13
 *
 *	@note    下面的note和warning为可选项目
 *	@note    这里填写本文件的详细功能描述和注解
 *	@note    历史记录：
 *	@note    V0.0.1  创建
 *
 *	@warning 这里填写本文件相关的警告信息
 */
#ifndef GUARD_CONFIG_H
#define GUARD_CONFIG_H

#include <HPR_Singleton.h>
#include "Common.h"
#include "verify/RspServerLogin.pb.h"

namespace {
const int INVALID_POLICY_MODE = -1;
} //~ end of anonymous namespace

class CConfig : public singleton<CConfig>
{
    friend class singleton<CConfig>;
public:
    CConfig(void);
    ~CConfig(void);

public:

    /** @fn    GetConfigInfo
    *  @brief  获取策略配置信息
    *  @param  [out]stConfigInfo, 配置信息
    *  @return bool
    */
    bool GetPolicyInfo(config_info_t &stConfigInfo);

	/**	@fn	    LoadLocalCfgInfo
	*	@brief	加载本地配置文件
	*	@param  [in] 无
	*	@param  [out] 无
	*	@return	void
	*/
	void LoadLocalCfg(void);

	/**	@fn	    GetAppRunPath
	*	@brief	获取程序运行路径
	*	@param  [in] 无
	*	@param  [out] sPathName 程序路径
	*	@return	
	*/
	void GetAppRunPath(std::string &sPathName);

	/**	@fn	    LoadConfigFile
	*	@brief	加载配置文件
	*	@param  [in] 无
	*	@param  [out] 无
	*	@return	int，0 表示没有错误，其他数值表示出现了错误
	*/
	int LoadConfigFile(void);

	/**	@fn	    LoadWebConfig
    *	@brief	加载WEB端配置
    *	@param  [in] serverLoginRsp 登陆返回配置
    *	@param  [out] 无
    *	@return	bool
    */
	bool LoadWebConfig(cms_8120::RspServerLogin &serverLoginRsp);

private:

private:

    //校时策略
    int m_iAdjustMode;

    //每日定时校时时分秒
    int m_uiTimingHour;
    int m_uiTimingMin;
    int m_uiTimingSec;

    //dvr和本地允许自动校时的最大时间差 单位分钟
    int m_uiMaxTimeLag;

    //校时时间间隔
    int m_uiInterval;

public:
	std::string m_sFileVersion;     //文件版本信息
	std::string m_sFileDescription; //文件描述信息

	std::string m_strLocalIP;       //本地IP

	std::string m_strCenterIP;      //中心DCIP
	int         m_nCenterPort;      //中心DC端口

	std::string m_strATServerIP;    //系统巡检服务IP
	int         m_nATServerPort;    //系统巡检服务端口

	bool        m_bLocalConfig;     //是否启用本地配置

	int         m_nHaveCenterStorage;//有无中心存储（0无，1有）

};
inline CConfig* GetConfigPtr(void)
{
    return CConfig::instance();
}

#endif //GUARD_CONFIG_H
