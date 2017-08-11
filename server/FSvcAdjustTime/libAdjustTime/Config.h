/**	@file    Config.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   ������
 *
 *	@author	 chenxiaoyan3
 *	@date	 2014/11/13
 *
 *	@note    �����note��warningΪ��ѡ��Ŀ
 *	@note    ������д���ļ�����ϸ����������ע��
 *	@note    ��ʷ��¼��
 *	@note    V0.0.1  ����
 *
 *	@warning ������д���ļ���صľ�����Ϣ
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
    *  @brief  ��ȡ����������Ϣ
    *  @param  [out]stConfigInfo, ������Ϣ
    *  @return bool
    */
    bool GetPolicyInfo(config_info_t &stConfigInfo);

	/**	@fn	    LoadLocalCfgInfo
	*	@brief	���ر��������ļ�
	*	@param  [in] ��
	*	@param  [out] ��
	*	@return	void
	*/
	void LoadLocalCfg(void);

	/**	@fn	    GetAppRunPath
	*	@brief	��ȡ��������·��
	*	@param  [in] ��
	*	@param  [out] sPathName ����·��
	*	@return	
	*/
	void GetAppRunPath(std::string &sPathName);

	/**	@fn	    LoadConfigFile
	*	@brief	���������ļ�
	*	@param  [in] ��
	*	@param  [out] ��
	*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
	*/
	int LoadConfigFile(void);

	/**	@fn	    LoadWebConfig
    *	@brief	����WEB������
    *	@param  [in] serverLoginRsp ��½��������
    *	@param  [out] ��
    *	@return	bool
    */
	bool LoadWebConfig(cms_8120::RspServerLogin &serverLoginRsp);

private:

private:

    //Уʱ����
    int m_iAdjustMode;

    //ÿ�ն�ʱУʱʱ����
    int m_uiTimingHour;
    int m_uiTimingMin;
    int m_uiTimingSec;

    //dvr�ͱ��������Զ�Уʱ�����ʱ��� ��λ����
    int m_uiMaxTimeLag;

    //Уʱʱ����
    int m_uiInterval;

public:
	std::string m_sFileVersion;     //�ļ��汾��Ϣ
	std::string m_sFileDescription; //�ļ�������Ϣ

	std::string m_strLocalIP;       //����IP

	std::string m_strCenterIP;      //����DCIP
	int         m_nCenterPort;      //����DC�˿�

	std::string m_strATServerIP;    //ϵͳѲ�����IP
	int         m_nATServerPort;    //ϵͳѲ�����˿�

	bool        m_bLocalConfig;     //�Ƿ����ñ�������

	int         m_nHaveCenterStorage;//�������Ĵ洢��0�ޣ�1�У�

};
inline CConfig* GetConfigPtr(void)
{
    return CConfig::instance();
}

#endif //GUARD_CONFIG_H
