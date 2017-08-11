#pragma once
#include "../stdafx.h"
#include "StorageBase.h"
#include "../../util/Singleton.h"
#include "../../util/Guard.h"
#include <time.h>
#include <map>

#define    INTERNAL_5_MINUTE  300   //��¼ʱ����

//�洢�豸��¼��Ϣ
typedef struct _Struct_LoginInfo
{
    LONG lLoginID;//��Чʱ��30����
    int nCount;//����ʹ�ü���
    time_t loginTime;//��½ʱ��
    int nError;//�����������
    ENUM_STORAGE_TYPE nManType; //����
    _Struct_LoginInfo()
        :lLoginID(STORAGE_LOGIN_FAILED)
        ,nCount(0)
        ,loginTime(time(0))
        ,nError(0)
        ,nManType(STORAGE_TYPE_MAX)
    {

    }
}Struct_LoginInfo;

class CStorageMgr
{
    SINGLETON_DECLARE(CStorageMgr)///<�����������

private:
    CStorageMgr(void);
    virtual ~CStorageMgr(void);
public:
    /**	@fn	    Init
	*	@brief	��ʼ������
	*	@param  [in] 
	*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
	*/
	int Init(ENUM_STORAGE_TYPE enumStorageType);

	/**	@fn	    Fini
	*	@brief	����ʼ��������������Դ�ͷ�
	*	@param  [in] ��
	*	@param  [out] ��
	*	@return	int��0 ��ʾû�д���������ֵ��ʾ�����˴���
	*/
	int Fini();

    /**	@fn	    Login
	*	@brief	��¼
	*	@param  [in] struStorage -- �洢����Ϣ
	*	@param  [out] struStorage.lLoginID -- ��¼���
	*	@return	LONG
	*/
    LONG Login(const Struct_StorageConfig &struStorage);

    /**	@fn	    Logout
	*	@brief	�ǳ�
	*	@param  [in] lLoginID -- ��¼���
	*	@return	BOOL
	*/
    BOOL Logout(const ENUM_STORAGE_TYPE &storageType, const LONG &lLoginID);

    /**	@fn	    Upload
	*	@brief	�ϴ�¼��
    *	@param  [in] lLoginID -- ��¼���
	*	@param  [in] struStorage -- �洢����Ϣ
	*	@param  [in] struRecord -- ¼����Ϣ
	*	@return	BOOL
	*/
    BOOL Upload(const LONG &lLoginID, Struct_RecordInfo &struRecord);

    /**	@fn	    GetUploadStatus
    *	@brief	��ȡ�ϴ�״̬
    *	@param  [in] lLoginID -- ��¼���
    *	@param  [in] struRecord -- ¼����Ϣ
    *	@param  [out] struRecord.recordStatus -- ¼��״̬
    *	@return	BOOL
    */
    BOOL GetUploadStatus(const LONG &lLoginID, Struct_RecordInfo &struRecord);

	/**	@fn	    GetUploadRecordInfo
    *	@brief	��ȡ�洢�豸�����ϴ���¼����Ϣ
    *	@param  [in] lLoginID -- ��¼���
	*	@param  [in] struRecord -- �ϴ�¼����Ϣ
    *	@param  [out] struRecordParam -- ���ϴ����洢�豸�е�¼����Ϣ
    *	@return	BOOL
    */
    BOOL GetUploadRecordInfo(const LONG &lLoginID, Struct_RecordInfo &struRecord, Struct_UploadRecordParam &struRecordParam);

    /**	@fn	    LoginManage
    *	@brief	��¼����
    *	@param  [in] struStorage -- �洢�豸��Ϣ
    *	@return	LONG -- ��¼���
    */
    LONG LoginManage(const Struct_StorageConfig &struStorage);

    /**	@fn	    LogoutManage
    *	@brief	�ǳ�����
    *	@param  [in] struStorage -- �洢�豸��Ϣ
    *	@param  [in] lLoginID -- ��¼���
    *   @param  [in] bError -- �Ƿ��д�����
    *	@return	void
    */
    void LogoutManage(const Struct_StorageConfig &struStorage, BOOL bError);
private:
    CRITICAL_SECTION m_csMapLoginInfo;                                  //�洢�豸��¼��
    std::map<std::string, Struct_LoginInfo> m_mapStorageLoginInfo;  //�洢�豸��¼��Ϣ

    CStorageBase* m_pStorageMgr[STORAGE_TYPE_MAX];
};
