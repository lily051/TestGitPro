#pragma once
#include "storagebase.h"
#include "../../util/Guard.h"
#include <map>
#include "hcs_sdk.h"
#include "hcs_object_api.h"
#include "../../util/NetSpeedControl.h"

// typedef struct _Struct_Process
// {
//     void* pThis;        //ָ��
//     std::string strFileId; //¼����Ϣ
//     _Struct_Process()
//     {
//         pThis = NULL;
//     }
// }Struct_Process;

const int WRITE_LENGTH = 512 * 1024; //512K�ֽ�

class CObjectCloud :
    public CStorageBase
{
public:
    CObjectCloud(void);
    virtual ~CObjectCloud(void);
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

    /** @fn SetUploadPercent
    *  @brief  �����ϴ�����
    *  @param [in]fileId -- �ϴ��ļ���id
    *  @param [in]nPercent -- ����ֵ
    *  @return void
    */
    /*void SetUploadPercent(const std::string &fileId, const int &nPercent);*/

	/**	@fn	    GetUploadRecordInfo
    *	@brief	��ȡ�洢�豸�����ϴ���¼����Ϣ
    *	@param  [in] lLoginID -- ��¼���
	*	@param  [in] struRecord -- �ϴ�¼����Ϣ
    *	@param  [out] struRecordParam -- ���ϴ����洢�豸�е�¼����Ϣ
    *	@return	BOOL
    */
    BOOL GetUploadRecordInfo(const LONG &lLoginID, Struct_RecordInfo &struRecord, Struct_UploadRecordParam &struRecordParam);

private:

	/**	@fn	    UploadStreamFile
	*	@brief	��ʽ�ϴ�
	*	@param  [in] pClient    -- �ƴ洢��ʼ�����
	*	@param  [in] struRecord -- ¼����Ϣ
	*	@param  [out] struRecord.strCameraID -- �ļ�key
	*	@return	BOOL
	*/
	BOOL UploadStreamFile(HCSClient *pClient, Struct_RecordInfo &struRecord);

	/**	@fn	    UploadNormalFile
	*	@brief	��ͨ�ϴ�
	*	@param  [in] pClient    -- �ƴ洢��ʼ�����
	*	@param  [in] struRecord -- ¼����Ϣ
	*	@param  [out] struRecord.strCameraID -- �ļ�key
	*	@return	BOOL
	*/
	BOOL UploadNormalFile(HCSClient *pClient, Struct_RecordInfo &struRecord);

	/**	@fn	    GetRecordTime
	*	@brief	��ȡ���ϴ�¼��Ŀ�ʼ/����ʱ��
	*	@param  [in] pClient    -- �ƴ洢��ʼ�����
	*	@param  [in] strBucket -- ��Դ��
	*	@param  [in] strObjectKey -- �ļ�key
	*	@param  [out] strStartTime -- ¼��ʼʱ��
	*	@param  [out] strEndTime -- ¼�����ʱ��
	*	@return	BOOL
	*/
	BOOL GetRecordTime(HCSClient *pClient, std::string strBucket, std::string strObjectKey, std::string &strStartTime, std::string &strEndTime);

	/**	@fn	    GetTimeStamp
	*	@brief	�õ�ʱ���
	*	@param  [in] strTime -- ʱ���ִ�
	*	@return	time_t
	*/
	time_t GetTimeStamp(std::string strTime);

private:
//     CRITICAL_SECTION m_csMapUploadPercent;          //������
//     std::map<std::string, int> m_mapUploadPercent;  //¼�������Ϣ
	std::map<std::string, HCSClient *> m_mapHCSClient;    //�ƴ洢��ʼ�����map��key���ƴ洢IP
    char  fileData[WRITE_LENGTH + 1];
	CNetSpeedControl m_oSpeedControl;
};
