#pragma once
#include "storagebase.h"
#include "../../util/Guard.h"
#include <map>

// typedef struct _Struct_Process
// {
//     void* pThis;        //ָ��
//     std::string strFileId; //¼����Ϣ
//     _Struct_Process()
//     {
//         pThis = NULL;
//     }
// }Struct_Process;

class CKMS :
    public CStorageBase
{
public:
    CKMS(void);
    virtual ~CKMS(void);
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
    /** @fn getHttpToken
    *  @brief  ��KMS��ȡToken��֤��Ϣ
    *  @param [in]struKMSInfo -- KMS��������Ϣ
    *  @param [out]strToken -- ��ȡ����token��Ϣ
    *  @return BOOL
    */
    BOOL getHttpToken(const Struct_StorageConfig &struKMSInfo, std::string &strToken);

    /** @fn uploadFile
    *  @brief  ��KMS�ϴ��ļ�
    *  @param [in]strToken -- KMS��token��֤��Ϣ
    *  @param [in]struKMSInfo -- KMS��������Ϣ
    *  @param [in]filePath -- �ϴ��ļ���·��
    *  @param [out]strFileId -- KMS���ص��ļ�id
    *  @return BOOL
    */
    BOOL uploadFile(const std::string &strToken, const Struct_StorageConfig &struKMSInfo, const std::string &filePath, std::string &strFileId);

    /** @fn getRtspPath
    *  @brief  ��ȡrtsp·��
    *  @param [in]strToken -- KMS��token��֤��Ϣ
    *  @param [in]struKMSInfo -- KMS��������Ϣ
    *  @param [in]strFileId -- �ϴ��ļ���id
    *  @param [out]strRtspPath -- ���ļ���RTSP����·��
    *  @return bool
    */
	BOOL getRtspPath(const std::string &strToken, const Struct_StorageConfig &struKMSInfo, const std::string &strFileId, std::string &strRtspPath, std::string &strStartTime, std::string &strEndTime);

    /** @fn getFileSize
    *  @brief  ��ȡ�ļ���С
    *  @param [in]strToken -- KMS��token��֤��Ϣ
    *  @param [in]struKMSInfo -- KMS��������Ϣ
    *  @param [in]strFileId -- �ϴ��ļ���id
    *  @param [out]nFileSize -- ���ļ���С
    *  @return bool
    */
    BOOL getFileSize(const std::string &strToken, const Struct_StorageConfig &struKMSInfo, const std::string &strFileId, int &nFileSize);

private:
//     CRITICAL_SECTION m_csMapUploadPercent;          //������
//     std::map<std::string, int> m_mapUploadPercent;  //¼�������Ϣ

};
