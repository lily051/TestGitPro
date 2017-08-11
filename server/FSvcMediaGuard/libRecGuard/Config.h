/**	@file DBDataMgr.h
*	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief ����������
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
    SINGLETON_DECLARE(CConfig)///<�����������

private:
    CConfig(void);
    virtual ~CConfig(void);

public:
    /**	@fn	    LoadConfigFile
    *	@brief	���������ļ�
    *	@param  [in] ��
    *	@param  [out] ��
    *	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
    */
    int LoadConfigFile(void);

    /**	@fn	    LoadWebConfigXML
    *	@brief	����WEB�ͻ������ɵ�XML
    *	@param  [in] ��
    *	@param  [out] ��
    *	@return	
    */
    void LoadWebConfigXML(const std::string &sConfigXML);

    /**	@fn	    UpdateATSvrInfo
    *	@brief	����ϵͳѲ���������Ϣ
    *	@param  [in] sATSvrIP -- Ѳ�������IP 
    *	@param  [in] nATPort -- Ѳ��������˿�
    *	@param  [out] ��
    *	@return	void
    */
    void UpdateATSvrInfo(const std::string &sATSvrIP, const int &nATPort);

private:
    /**	@fn	    GetAppRunPath
    *	@brief	��ȡ��������·��
    *	@param  [in] ��
    *	@param  [out] sPathName ����·��
    *	@return	
    */
    void GetAppRunPath(std::string &sPathName);

public:
    std::string m_sFileVersion;     //�ļ��汾��Ϣ
    std::string m_sFileDescription; //�ļ�������Ϣ

    std::string m_strLocalIP;       //����IP
    std::string m_sLoginName;       //��¼�û���
    std::string m_sLoginPwd;        //��¼�û�����

    std::string m_strCenterIP;      //����DCIP
    int         m_nCenterPort;      //����DC�˿�

    std::string m_strATServerIP;    //ϵͳѲ�����IP
    int         m_nATServerPort;    //ϵͳѲ�����˿�

    // ����֮���ʱ����,��λ����
    int         m_nTaskInteval;
	int         m_nHaveCenterStorage;//�������Ĵ洢��0�ޣ�1�У�

    //¼����������ʱ��
    std::string m_strStartTime;     //¼��������ʼʱ��
    std::string m_strStopTime;      //¼����������ʱ��

    //int         m_nUpdateDataTime;  //��������ʱ��
};
