#pragma once
#define STORAGE_LOGIN_FAILED -1

class CStorageBase
{
public:
    CStorageBase(void);
    virtual ~CStorageBase(void);
public:
    /**	@fn	    Init
	*	@brief	��ʼ��������ģ����Ҫ�Ĳ���Ӧ���ɴ˺����������ļ�����
	*	@param  [in] ��
	*	@param  [out] ��
	*	@return	BOOL
	*/
    virtual BOOL Init() = 0;

    /**	@fn	    Cleanup
	*	@brief	�ͷ���Դ
	*	@param  [in] ��
	*	@param  [out] ��
	*	@return	BOOL
	*/
    virtual BOOL Cleanup() = 0;

    /**	@fn	    Login
	*	@brief	��¼
	*	@param  [in] struStorage -- �洢����Ϣ
	*	@return	LONG -- ��¼���
	*/
    virtual LONG Login(const Struct_StorageConfig &struStorage) = 0;

    /**	@fn	    Logout
	*	@brief	�ǳ�
	*	@param  [in] lLoginID -- ��¼���
	*	@return	BOOL
	*/
    virtual BOOL Logout(LONG lLoginID) = 0;

    /**	@fn	    Upload
	*	@brief	�ϴ�¼��
    *	@param  [in] lLoginID -- ��¼���
	*	@param  [in] struRecord -- ¼����Ϣ
	*	@return	BOOL
	*/
    virtual BOOL Upload(const LONG &lLoginID, Struct_RecordInfo &struRecord) = 0;

    /**	@fn	    GetUploadStatus
	*	@brief	��ȡ�ϴ�״̬
    *	@param  [in] lLoginID -- ��¼���
    *	@param  [in] struRecord -- ¼����Ϣ
    *	@param  [out] struRecord.recordStatus -- ¼��״̬
	*	@return	BOOL
	*/
    virtual BOOL GetUploadStatus(const LONG &lLoginID, Struct_RecordInfo &struRecord) = 0;

	/**	@fn	    GetUploadRecordInfo
    *	@brief	��ȡ�洢�豸�����ϴ���¼����Ϣ
    *	@param  [in] lLoginID -- ��¼���
	*	@param  [in] struRecord -- �ϴ�¼����Ϣ
    *	@param  [out] struRecordParam -- ���ϴ����洢�豸�е�¼����Ϣ
    *	@return	BOOL
    */
    virtual BOOL GetUploadRecordInfo(const LONG &lLoginID, Struct_RecordInfo &struRecord, Struct_UploadRecordParam &struRecordParam) = 0;
};
