/**	@file DBDataMgr.h
*	@note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*	@brief DC���ݽ�����
*	@author		chenbin5
*	@date		2016-3-4
*	@note 
*/
#pragma once
#include <HPR_Singleton.h>
#include "verify/RspServerLogin.pb.h"
#include <fclib/fcTchar.h>
//״̬��Ϣ
typedef struct _work_state_t {
	int           nKey;            //��������
	int           nParentKey;      //����ģ��
	int           nMsgType;        //������Ϣ����
	int           nState;          //1 ���� 0 �Ǳ���
	int           nUserID;         //�û�
	int           nChannel;        //ͨ��
	int           nRev;            //���±����Ĳ�������
	int           nBeginOccureTime; //��һ�η���ʱ��
	int           nLastOccureTime; //��һ�η���ʱ��
	int           nOccureTime;     //����ʱ��
	__int64       nAlarmID;        //0 �±�����1�Ѵ��ڱ���
	int           nRegionID;       //��������
	std::_tstring strKeyValue;     //����������������
	std::_tstring strOccureTime;   //����ʱ���ַ���
	std::_tstring strInfo;         //������Ϣ
	std::_tstring strHost;         //������������ip
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

//״̬��Ϣ
typedef struct _Struct_AlarmLog {
	int           nID;             //������־��ˮ��
	int           nState;          //1 ���� 0 �Ǳ���
	std::string   strHost;         //��������IP
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
	*	@brief	��ʼ������
	*	@param  [in] 
	*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
	*/
	int Init();

	/**	@fn	    Fini
	*	@brief	����ʼ��������������Դ�ͷ�
	*	@param  [in] ��
	*	@param  [out] ��
	*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
	*/
	int Fini();

    /**	@fn	    Start
    *	@brief	��ʼ������������ģ��ĺ��Ĺ��ܣ�һ������Ӧ�ÿ���ĳ���߳̽���ʵ�ʵĹ���
    *          �˺���Ӧ�þ��췵�أ���Ӧ��ռ��̫��ʱ�䣬����С��30s
    *	@param  [in] ��
    *	@param  [out] ��
    *	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
    */
    int Start();

    /**	@fn	    Stop
    *	@brief	ֹͣ����
    *	@param  [in] ��
    *	@param  [out] ��
    *	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
    */
    int Stop();
public:
    /**	@fn	    Login
    *	@brief	��¼
    *	@param  [in] ��
    *	@return	ENUM_LOGIN_RESULT��LOGIN_RESULT_OK,LOGIN_RESULT_FAIL
    */
    ENUM_LOGIN_RESULT Login();

    /**	@fn	    Logout
    *	@brief	�ǳ�
    *	@param  [in] ��
    *	@param  [out] ��
    *	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
    */
    int Logout();

    /**	@fn	    SendLoginReq
    *	@brief	���͵�¼��Ϣ
    *	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
    */
    int SendLoginReq();

    /**	@fn	    SendLogoutReq
    *	@brief	�����˳���Ϣ
    *	@param  [in]��
    *	@return	
    */
    void SendLogoutReq();

	/**	@fn	    SendLoginRsp
    *	@brief	��õ�½��Ӧ��Ϣ
    *	@param  [in] serLoginRsp ��½��Ӧ��Ϣ
    *	@return	
    */
    void SendLoginRsp(const cms_8120::RspServerLogin & serLoginRsp);

	/**	@fn	    GetAllCollectDeviceReq
    *	@brief	�����ȡ�ɼ��豸
    *	@param  [in]  ��
    *	@param  [out] ��
    *	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
    */
    int GetAllCollectDeviceReq();

    /**	@fn	    GetStorageDevice
    *	@brief	�����ȡ�洢�豸
    *	@param  [in]  ��
    *	@param  [out] ��
    *	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
    */
    int GetAllStorageDeviceReq();

	/**	@fn	    GetAlarmLogReq
	*	@brief	��ñ�������״̬
	*	@param  [in] workstate -- workstate��Ϣ
	*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
	*/
	int GetAlarmLogReq(const work_state_t &workstate);

	/** @fn    AddCenterAlarmReq
	*  @brief  ���ı�����Ϣ����ӱ���
	*  @param  [in]workstate, workstate��Ϣ
	*  @param  [in]ulCheckType, 100 dvr,102 �ɼ�������200���ķ���
	*  @return bool
	*/
	int AddCenterAlarmReq(const work_state_t &workstate, DWORD ulCheckType);

	/** @fn     UpdateCenterAlarmReq
     *  @brief  ���ı�����Ϣ���±���
     *  @param  [in]workstate, workstate��Ϣ
     *  @param  [in]nID ������־��ˮ��
     *  @return int 0��ʾ�޴���
     */
    int UpdateCenterAlarmReq(const work_state_t &workstate, int nID);

   /** @fn     UpdateDeviceOnlineStateReq
	*  @brief  �����豸����״̬
	*  @param  [in]Device_Type �豸����
	*  @param  [in]strIP       �豸IP��ַ
	*  @param  [in]nPort       �豸�˿ں�
	*  @param  [in]iOnline,    0������ 1����
	*  @return int 0��ʾ�޴���
	*/
	int UpdateDeviceOnlineStateReq(int Device_Type, const std::string &strIP, int nPort, int iOnline);

   /** @fn    UpdateStorageCapacityReq
	*  @brief  ���´洢�豸����
	*  @param  [in]Storage_Type �洢�豸����
	*  @param  [in]strIP        �洢�豸IP��ַ
	*  @param  [in]nPort        �洢�豸�˿ں�
	*  @param  [in]iTotalSpace, ������
	*  @param  [in]iFreeSpace,  ʣ������
	*  @return int 0��ʾ�޴���
	*/
	int UpdateStorageCapacityReq(int Storage_Type, const std::string &strIP, int nPort, unsigned long long iTotalSpace, unsigned long long iFreeSpace);

	/** @fn    UpdateServerHeartBeartReq
	*  @brief  �����ķ�������Ѳ���������
	*  @return int 0��ʾ�޴���
	*/
	int UpdateServerHeartBeartReq();

private:
    bool m_bInit;                                       //�Ƿ񱻳�ʼ����
    bool m_bStart;                                      //�Ƿ�������

    HANDLE m_hLoginRetEvent;                            //��¼�����ź�
public:
    cms_8120::RspServerLogin m_ServerLoginRsp;          //��������½��Ӧ��Ϣ
};
inline CDBDataMgr* GetDBDataMgrPtr(void)
{
	return CDBDataMgr::instance();
}