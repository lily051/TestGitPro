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

class CConfig
{
    SINGLETON_DECLARE(CConfig)///<�����������

private:
    CConfig(void);
    virtual ~CConfig(void);
// public:
//     /**	@fn	    Init
// 	*	@brief	��ʼ������
// 	*	@param  [in] 
// 	*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
// 	*/
// 	int Init();
// 
// 	/**	@fn	    Fini
// 	*	@brief	����ʼ��������������Դ�ͷ�
// 	*	@param  [in] ��
// 	*	@param  [out] ��
// 	*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
// 	*/
// 	int Fini();
// 
//     /**	@fn	    Start
//     *	@brief	��ʼ������������ģ��ĺ��Ĺ��ܣ�һ������Ӧ�ÿ���ĳ���߳̽���ʵ�ʵĹ���
//     *          �˺���Ӧ�þ��췵�أ���Ӧ��ռ��̫��ʱ�䣬����С��30s
//     *	@param  [in] ��
//     *	@param  [out] ��
//     *	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
//     */
//     int Start();
// 
//     /**	@fn	    Stop
//     *	@brief	ֹͣ����
//     *	@param  [in] ��
//     *	@param  [out] ��
//     *	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
//     */
//     int Stop();

public:
    /**	@fn	    Deciphering
    *	@brief	���ܲ���
    *	@param  [in] strPwd -- ��������
    *	@param  [out] strPwd -- ��������
    *	@return	void
    */
    void Deciphering(std::string& strPwd);

    /**	@fn	    LoadConfigFile
    *	@brief	���������ļ�
    *	@param  [in] ��
    *	@param  [out] ��
    *	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
    */
    int LoadConfigFile(void);

    /**	@fn	    LoadLocalCfgInfo
    *	@brief	���ر��������ļ�
    *	@param  [in] ��
    *	@param  [out] ��
    *	@return	void
    */
    void LoadLocalCfgInfo(void);

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

    /**	@fn	    UpdateCollectInfo
    *	@brief	���²ɼ�ƽ̨��Ϣ
    *	@param  [in] rspCollect -- �ɼ�ƽ̨��Ϣ
    *	@param  [out] ��
    *	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
    */
    //void UpdateCollectInfo(const cms_8120::RspCollectconfig &rspCollect);

    /**	@fn	    InitTransInterval
    *	@brief	��ʼ���ϴ�ʱ����
    *	@param  [in] strConfig -- �����ļ���
    *	@param  [out] ��
    *	@return	void
    */
    /*void InitTransInterval(const std::string strConfig);*/

    /**	@fn	    InitReservedTime
    *	@brief	��ʼ��Ԥ��ʱ��
    *	@param  [in] strConfig -- �����ļ���
    *	@param  [out] ��
    *	@return	void
    */
    /*void InitReservedTime(const std::string strConfig);*/

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

//     int         m_nCollectID;       //�ɼ�ƽ̨ID
//     std::string m_strCollectIP;     //�ɼ�ƽ̨IP
//     int         m_nCollectPort;     //�ɼ�ƽ̨�˿�

    std::string m_strATServerIP;    //ϵͳѲ�����IP
    int         m_nATServerPort;    //ϵͳѲ�����˿�
   
    bool        m_isAutoDelRecord;  //�Ƿ��Զ�ɾ��records��ļ�¼  
    bool        m_isAutoDelFile;    //�Ƿ��Զ�ɾ��¼���ļ� 
//     bool        m_isUpdatePercent;  //�����ϴ��ٷֱ�
//     int         m_npercentStep;     //�ϴ����Ȳ���
    int         m_nTaskCount;       //�ϴ���������
	int         m_nHaveCenterStorage;//�������Ĵ洢��0�ޣ�1�У�
    int         m_nMode;            //�ϴ�ģʽ��1ΪIPC��2ΪUSB��
    bool        m_bLocalConfig;     //�Ƿ����ñ������û�ȡ�ɼ�ƽ̨��Ϣ

    // �ϴ�����֮���ʱ����,��λ��
    int         m_nUploadInteval;
    // ״̬����ʱ����,��λ��
    int         m_nStatusInteval;

    //�ϴ�����ʱ��
    std::string m_strStartTime;     //¼���ϴ���ʼʱ��
    std::string m_strStopTime;      //¼���ϴ�����ʱ��

    int         m_nUpdateDataTime;  //��������ʱ��(����¼����Ϣ)
	int         m_nUploadSpeed;    //�ϴ��ٶȣ���λ:Kbps

	// ��ʼʱ�����ƫ��
	int         m_nOffsetStartTime;
	// ����ʱ�����ƫ��
	int         m_nOffsetStopTime;
};
