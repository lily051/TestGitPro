#pragma once
#include "GlobalParam.h"
#include "../util/Singleton.h"
#include "../util/Guard.h"
#include <list>
#include <map>
#include "hcs_sdk.h"
#include "hcs_object_api.h"

const int WRITE_LENGTH   = 512 * 1024; //512K�ֽ�
const int MAX_THREAD_NUM = 100; //��󲢷��ϴ���

class CMultiFileUpload 
{
	SINGLETON_DECLARE(CMultiFileUpload)///<�����������
public:
    CMultiFileUpload(void);
    virtual ~CMultiFileUpload(void);
public:
    /**	@fn	    Init
	*	@brief	��ʼ��������ģ����Ҫ�Ĳ���Ӧ���ɴ˺����������ļ�����
	*	@param  [in] ��
	*	@param  [out] ��
	*	@return	BOOL
	*/
    BOOL Init();

    /**	@fn	    Fini
	*	@brief	����ʼ���������ͷ���Դ��
	*	@param  [in] ��
	*	@param  [out] ��
	*	@return	BOOL
	*/
    BOOL Fini();

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

	/**	@fn	    MultiFileUpload
	*	@brief	�ϴ�¼��
	*	@param  [in] ��
	*	@return	void
	*/
    void MultiFileUpload();

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
private:

	/**	@fn	    UploadStreamFile
	*	@brief	��ʽ�ϴ�
	*	@param  [in] struRecord -- ¼����Ϣ
	*	@param  [out] struRecord.strCameraID -- �ļ�key
	*	@return	BOOL
	*/
	BOOL UploadStreamFile(Struct_RecordInfo &struRecord);

	/**	@fn	    UploadNormalFile
	*	@brief	��ͨ�ϴ�
	*	@param  [in] struRecord -- ¼����Ϣ
	*	@param  [out] struRecord.strCameraID -- �ļ�key
	*	@return	BOOL
	*/
	BOOL UploadNormalFile(Struct_RecordInfo &struRecord);
private:
	bool m_bInit;  //<�Ƿ��Ѿ���ʼ��
	bool m_bStart; //<�Ƿ��Ѿ�����

private:
//     CRITICAL_SECTION m_csMapUploadPercent;          //������
//     std::map<std::string, int> m_mapUploadPercent;  //¼�������Ϣ
	std::list<Struct_RecordInfo> m_lstFile;         //�ļ���Ϣ����
	volatile bool m_bStopWorker;  //ֹͣ�����߳�
	HCSClient * m_client;
	char  fileData[WRITE_LENGTH + 1];
	HANDLE hThread[MAX_THREAD_NUM];  //�߳̾��
	CRITICAL_SECTION m_csFile;       //�ļ�������    
};
