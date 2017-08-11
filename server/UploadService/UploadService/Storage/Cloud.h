#pragma once
#include "storagebase.h"

class CCloud :
    public CStorageBase
{
public:
    CCloud(void);
    virtual ~CCloud(void);
public:
    /**	@fn	    Init
	*	@brief	��ʼ��������ģ����Ҫ�Ĳ���Ӧ���ɴ˺����������ļ�����
	*	@param  [in] ��
	*	@param  [out] ��
	*	@return	BOOL
	*/
    BOOL Init();

    /**	@fn	    Cleanup
	*	@brief	�ͷ���Դ
	*	@param  [in] ��
	*	@param  [out] ��
	*	@return	BOOL
	*/
    BOOL Cleanup();

    /**	@fn	    Login
	*	@brief	��¼
	*	@param  [in] struStorage -- �洢����Ϣ
	*	@return	LONG -- ��¼���
	*/
    LONG Login(const Struct_StorageConfig &struStorage);

    /**	@fn	    Logout
	*	@brief	�ǳ�
	*	@param  [in] lLoginID -- ��¼���
	*	@return	BOOL
	*/
    BOOL Logout(LONG lLoginID);

    /**	@fn	    Upload
	*	@brief	�ϴ�¼��
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

};
