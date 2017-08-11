#ifndef  CLOUD_API_PUBLIC_H_H
#define  CLOUD_API_PUBLIC_H_H
//lint -library
#include <string.h>
#define CAMERAID_LEN         72    // ����������ַ�����
#define IPLength             20    // ����IP��ַ��Ϣ����
#define DeviceIDLength       48    // �����豸ID����
#define MAX_IP_LEN				64

/**  @enum PlayType  [CloudApiDefine.h]
*  @brief ¼��طŵ�����ö����
*/
enum PlayType
{
	PlayNormal = 1,                          // ˳��ط�
	PlayNormalIFrame = 3,                    // ����I֡�ط�
	PlayReindex = 5,                         // ����ط�
};



/**  @enum RecordLockType  [CloudApiDefine.h]
*  @brief ¼����������
*
*/
enum RecordLockType
{  
	TypeAll = 0,	                          // ȫ��������������¼���û��������¼��
	TypeUnlock = 1,                         // û������(���ԶԵ�ǰ¼��ִ�����в���)
	TypeLock = 2,	                          // ����(��ǰ¼�������ǣ������Իطš����غ�ɾ������)

};

/** @struct tagCloudRecordInfo [CloudApiDefine.h]
* @brief ¼�����Ϣ���ݽṹ����
*/
typedef struct tagCloudPoolInfo
{
	tagCloudPoolInfo()
	{
		memset(PoolName,0,sizeof(PoolName));
		PoolType =0;
		PoolTimeCycle=0;
		PoolSize=0;
		Disperse_Type=0;
		CoverType=0;
		LockLimit=0;
		PoolRate=0;
	}

	char  PoolName[100] ;	                // ¼������� [��ע: ��֧������]
	int   PoolType ;			            // ¼������� [0: ͨ�ô洢��1: ��Ƶ��2: ���Ӷ���Ϣ��3: ͼƬ��Ϣ]
	unsigned int PoolTimeCycle;             // ¼��ظ������� [��λ: �졢��Χ: ����0]
	unsigned int PoolSize ;                 // ¼��ش�С [��λ: MB]
	int   Disperse_Type;                    // ��ɢ���� [0: ���С�1: ��ȫ��ɢ]
	int   CoverType;                        // ���ǲ������� [0: �����ǡ�1:���������ǡ�2:���ڸ���]
	int   LockLimit;                        // ����ʱ��,�ӵ�ǰʱ����¼�����ʱ��ʱ����[��λ: ��]
	int   PoolRate;			                // ����[��λ: mb/s]
}CloudPoolInfo;

/** @struct tagCloudRecordInfo [CloudApiDefine.h]
* @brief ¼�����Ϣ���ݽṹ����
*/
typedef struct tagCloudPoolInfoEx
{
	tagCloudPoolInfoEx()
	{
		PoolID = 0 ;
		FreeSize=0;
		memset(&PoolInfo,0,sizeof(PoolInfo));
	}

	unsigned int PoolID;	                  // ¼���ID
	unsigned int FreeSize ;                   // ¼���ʣ��ռ��С[��λ: MB]
	unsigned int BeginTime;	                  // ����ʱ���
	unsigned int CoverTime;                   // ����ʱ���
	CloudPoolInfo PoolInfo ;                  // ¼��ػ�����Ϣ	
}CloudPoolInfoEx;


/**  @struct tagLableInfo  [CloudApiDefine.h]
*  @brief ��ǩ��Ϣ
*
*   ������ǩ�ڡ�����ͷ����ʼʱ�䡢����ʱ��ͱ�ǩ����
*/
typedef struct tagLabelInfo
{
	tagLabelInfo()
	{
		BeginTime = 0 ;
		EndTime = 0 ;
		memset(Camera,0,sizeof(Camera));
		memset(LableContext,0,sizeof(LableContext));
	}

	tagLabelInfo(tagLabelInfo& tag)
	{
		memcpy(Camera,tag.Camera,sizeof(tag.Camera));
		BeginTime = tag.BeginTime;
		EndTime = tag.EndTime ;
		memcpy(LableContext,tag.LableContext,sizeof(tag.LableContext));
	}

	tagLabelInfo& operator=(tagLabelInfo& tag)
	{
		memcpy(Camera,tag.Camera,sizeof(tag.Camera));
		BeginTime = tag.BeginTime;
		EndTime = tag.EndTime ;
		memcpy(LableContext,tag.LableContext,sizeof(tag.LableContext));
		return *this;
	}

	char Camera[CAMERAID_LEN];		            // ����������
	unsigned int BeginTime;                     // ��ʼʱ��
	unsigned int EndTime ;                      // ����ʱ��
	char LableContext[1024];                    // ��ǩ����
}LabelInfo;


/** @struct tagCloudRecordInfo [CloudApiDefine.h]
* @brief ¼����Ϣ���ݽṹ����
*/
typedef struct  tagCloudRecordInfo
{
	tagCloudRecordInfo()
	{
		memset(Camera,0,sizeof(Camera) );
		BeginTime=0;
		EndTime=0;
		Type=0;
		Lock=0;
		Size=0;
		status = 0;
        Backup = 0;
	}

	tagCloudRecordInfo(tagCloudRecordInfo&tag)
	{
		memcpy(Camera,tag.Camera,sizeof(tag.Camera));
		BeginTime=tag.BeginTime;
		EndTime=tag.EndTime;
		Type=tag.Type;
		Lock=tag.Lock;
		Size=tag.Size;
		memcpy(VSIP,tag.VSIP,sizeof(tag.VSIP));
		memcpy(DeviceID,tag.DeviceID,sizeof(tag.DeviceID));
		BlockID=tag.BlockID;
		SegmentID=tag.SegmentID;
		status = tag.status;
        Backup = tag.Backup;
	}

	tagCloudRecordInfo& operator=(tagCloudRecordInfo& tag)
	{
		memcpy(Camera,tag.Camera,sizeof(tag.Camera));
		BeginTime=tag.BeginTime;
		EndTime=tag.EndTime;
		Type=tag.Type;
		Lock=tag.Lock;
		Size=tag.Size;
		memcpy(VSIP,tag.VSIP,sizeof(tag.VSIP));
		memcpy(DeviceID,tag.DeviceID,sizeof(tag.DeviceID));
		BlockID=tag.BlockID;
		SegmentID=tag.SegmentID;
		status = tag.status;
        Backup = tag.Backup;
		return *this;
	}

	char  Camera[CAMERAID_LEN];	          // ������ID
	unsigned int  BeginTime ;	          // ¼��ʼʱ��
	unsigned int  EndTime;	              // ¼�����ʱ��
	int   Type;	                          // ¼������
	int	  Lock;	                          // ¼������״̬ [0: ȫ������(����������¼���û��������¼��)��1: û��������2: ����]
	int	  Size;                           // ¼���С[��λ: �ֽ�]
	char  VSIP[IPLength];		          // ¼�������IP (VSIP)
	char  DeviceID[DeviceIDLength];       // �洢�豸ID
	unsigned int BlockID;		          // ��ID
	unsigned int SegmentID;		          // ¼�����к�
	int status;                           // ��ǰ¼�����״̬[0: ������1: VS�����ߡ�2: �豸�����á�3: �豸������]
    unsigned int Backup;                 // ���ݱ�־[0 �C ԭʼ���ݣ�1 �C ��������]
}CloudRecordInfo;

//ʱ���(�ӽṹ)
typedef struct _sdk_schedtime
{
    //��ʼʱ��
    unsigned char byStartHour;
    unsigned char byStartMin;

    //����ʱ��
    unsigned char byStopHour;
    unsigned char byStopMin;
}SDK_SCHEDTIME, *LPSDK_SCHEDTIME;

/*�·���������*/
typedef struct strBackupPlan
{
    unsigned int BeginTime; 	// ���ݵĿ�ʼʱ��
    unsigned int EndTime; 		// ���ݵĽ���ʱ��
    int BlockType;				// �������� 1��¼��2��������3��ͼƬ
    char LocalHost[MAX_IP_LEN];	// ����IP
    int LocalPort1; 				// ���ض˿�
    int LocalPort2; 				// ���ض˿�
    char RemoteHost[MAX_IP_LEN]; 	// Զ��IP
    int RemotePort1; 				// Զ�˶˿�
    int RemotePort2; 				// Զ�˶˿�
    int PoolID;					// ��Ҫ���ݵ�ָ��¼���
    int PoolType;					// ��Ҫ���ݵ�ָ��¼��ص�����
    _sdk_schedtime SchedTime;	// ����ִ�д�����Ŀ�ʼʱ�䣬0-23
}SDK_BACKUPPLAN, *LPSDK_BACKUPPLAN;


#ifdef _WIN32
#ifdef CLOUD_API_EXPORTS
#define CLOUD_API __declspec(dllexport)
#else
#define CLOUD_API __declspec(dllimport)
#endif
#else
#define CLOUD_API
#endif
extern "C"
{

	/** 
	*  @brief API�¼��ص���������.�����������,�����쳣,��Ϣ�ϱ�
	*  @param LogicType[in] API��ģ����� 0:��֤,1:¼��(������ע),2:ͼƬ,3:������Ϣ,4:¼��ع���
	*  @param ErrorCode[in] ������
	*  @param pUser[in] �û��Զ������� 
	*  @@return =0:�����Ѵ���;<0:����δ����
	*/
	typedef int (*CUserMessage)(int LogicType,int ErrorCode,void* pUser);


	/** 
	*  @brief ¼����б��ѯ����ص���������
	*  @param info[out] ¼�����Ϣ 
	*  @param Count[out] ¼��ز�ѯ���������
	*  @param Flat[out]  ʣ��¼������� [����0: ��ʾ¼���ȫ�����ء� С��0: Ϊ�����롢����0: ��ʾʣ���¼�������]
	*  @param pUser[out] �û��Զ������� 
	*  @@return ����0:�����Ѵ���С��0:����δ����
	
	
	*  ��ע����Flat��ʾ��Ϊ0��Count��ʾ����Ϊ0ʱ����ʾ��ȡ��ǰ�ƴ洢ϵͳ��¼�����Ϣ����
	*        ��Flat��ʾ��Ϊ0��Count��ʾ��ҲΪ0ʱ����ʾ�ص�ֹͣ
	*/
	typedef int (*CPoolListDataCb)(CloudPoolInfoEx info, int Count, int Flat, void* pUser);


	/** 
	*  @brief ¼���ѯ����ص���������
	*  @param info[out] ¼����Ϣ 
	*  @param Count[out] ��ѯ¼����������
	*  @param Flat[out]  ʣ��¼����Ϣ���� [����0: ��ʾ¼����Ϣȫ�����ء� С��0: Ϊ�����롢����0: ��ʾʣ���¼������]
	*  @param pUser[out] �û��Զ������� 
	*  @@return ����0:�����Ѵ���; С��0:����δ����
	
	*  ��ע����Flat��ʾ��Ϊ0��Count��ʾ����Ϊ0ʱ����ʾ��ȡ��ǰ��������Ӧ��¼����Ϣ����
	*        ��Flat��ʾ��Ϊ0��Count��ʾ��ҲΪ0ʱ����ʾ�ص�ֹͣ
	*/
	typedef int (*CSearchDataCb)(CloudRecordInfo info, int Count, int Flat, void* pUser);


	/** 
	*  @brief ¼���������ݻص���������
	*  @param buff[in] ¼������
	*  @param bufflen[in] ���ݳ��� 
	*  @param Flat[in]  =0:���ؽ��� <0Ϊ������
	*  @param pUser[in] �û��Զ������� 
	*  @@return =0:�����Ѵ���;<0:����δ����
	
	*  ��ע����Flat��ʾ��Ϊ0��Count��ʾ����Ϊ0ʱ����ʾ���ص�ǰ��������Ӧ��¼�����
	*        ��Flat��ʾ��Ϊ0��Count��ʾ��ҲΪ0ʱ����ʾ�ص�ֹͣ
	*/
	typedef int (*CDownloadDataCb)(const char* buff ,int bufflen,int Flat,void* pUser);



	/** 
	*  @brief ��ǩ��Ϣ��ѯ�ص���������
	*  @param Info[in]  ��ǩ��Ϣ 
	*  @param Count[in] ��ѯ���ı�ǩ������
	*  @param Flat[in]  ʣ���ǩ����[С�ڵ���0:��ʾ������ С��0:Ϊ������]
	*  @param pUser[in] �û��Զ������� 
	*  @@return =0:�����Ѵ���; <0:����δ����
	*/
	typedef int (*CLabelCb)(LabelInfo Info,int Count,int Status,void* pUser);

    /** 
    *  @brief ��������Ļص���������
    *  @param nLoginID[out] ��½ID
    *  @param CameraID[out] ����������
    *  @param nBeginTime[out] �ɹ����ݵĿ�ʼʱ�� 
    *  @param nEndTime[out] �ɹ����ݵĽ���ʱ��
    *  @param nFlag[out]  �ж����񷵻��Ƿ���� [����0: ��ʾ���λص������� С��0: Ϊ�����롢����0: ��ʾ�����������]
    *  @param pUser[out] �û��Զ������� 
    *  @@
    */
    typedef void(*BackupDataCallBack) (unsigned int nLoginID, const char *CameraID, unsigned int nBeginTime, unsigned int nEndTime, unsigned int nFlag, void*pUserData);

	/** @fn CLOUD_API int API_Login(const char* CloudIP, unsigned int CloudPort, const char* Username,
	*	const char* Password, CUserMessage UserMsg, void* pUser, unsigned int& logiID);
	*  @brief ��¼�ƴ洢
	*  @param CloudIP[in]   VMIP��ַ
	*  @param CloudPort[in] VM��¼�˿�
	*  @param Username[in]  �ƴ洢�û��� ���32�ַ�
	*  @param Password[in]  �ƴ洢���� ���32�ַ�
	*  @param UserMsg[in]   �û���Ϣ�ص�����
	*  @param pUser[in]     �û��Զ�������
	*  @param logiID[out]   ���صĵ�¼���
	*  @@return �ɹ�=0 :��¼�ɹ� 
	ʧ��<0 :������
	*/
	CLOUD_API int API_Login(const char* CloudIP, unsigned int CloudPort, const char* Username,
		const char* Password, CUserMessage UserMsg, void* pUser, unsigned int& logiID);



	/** @fn CLOUD_API unsigned int API_LogOut(unsigned int logiID)
	*  @brief �˳��ƴ洢
	*  @param loginID[in] �ɹ���½�ƴ洢���صľ��
	*  @@return �ɹ�=0 
	ʧ��<0 :������
	*/
	CLOUD_API int API_LogOut(unsigned int logiID);



	/******************************************************************************
	*                                                                             *
	*                                         ¼��ز���                          *
	*                                                                             *
	*******************************************************************************/


	/** @fn CLOUD_API int API_PoolCreate(unsigned int loginID, CloudPoolInfo info)
	*  @brief ¼��ش���
	*  @param loginID[in] �ɹ���½�ƴ洢���صľ��
	*  @param info[in] ¼�����Ϣ
	*  @param PoolID[out] ����¼��سɹ��󷵻ص�ID
	*  @@return  �ɹ�=0 :����¼��سɹ� 
	             ʧ��<0 :������

	* ��ע��Ŀǰ����¼��ص����Ʋ�֧������
	*/
	CLOUD_API int API_PoolCreate(unsigned int loginID, CloudPoolInfo info, unsigned int &PoolID);


	/** @fn CLOUD_API int API_PoolDestroy(unsigned int loginID,unsigned int PoolID )
	*  @brief ¼���ɾ��
	*  @param loginID[in] �ɹ���½�ƴ洢���صľ��
	*  @param PoolID[in] ����¼��سɹ��󷵻ص�ID
	*  @@return �ɹ�=0
	            ʧ��<0 :������
	*/
	CLOUD_API int API_PoolDestroy(unsigned int loginID, unsigned int PoolID );


	/** @fn CLOUD_API int API_PoolEdit(unsigned int loginID,unsigned int PoolID,CloudPoolInfo info)
	*  @brief ¼����޸�
	*  @param loginID[in] �ɹ���½�ƴ洢���صľ��
	*  @param PoolID[in] ����¼��سɹ��󷵻ص�ID
	*  @param info[in] ¼�����Ϣ
	*  @@return �ɹ�=0
	            ʧ��<0 :������
	*/
	CLOUD_API int API_PoolEdit(unsigned int loginID, unsigned int PoolID, CloudPoolInfo info);


	/** @fn CLOUD_API int API_PoolSize(unsigned int loginID,unsigned int PoolID,int PoolSize)
	*  @brief �޸�¼�������
	*  @param loginID[in] �ɹ���½�ƴ洢���صľ��
	*  @param PoolID[in] ����¼��سɹ��󷵻ص�ID
	*  @param PoolSize[in] ¼�������
	*  @@return �ɹ�=0
	            ʧ��<0 :������
	*/
	CLOUD_API int API_PoolSize(unsigned int loginID, unsigned int PoolID, unsigned int PoolSize);


	/** @fn CLOUD_API int API_PoolInfo(unsigned int loginID, unsigned int PoolID , CloudPoolInfoEx& loudPoolInfoEx)
	*  @brief ¼��ز�ѯ.��ѯ����¼�����Ϣ
	*  @param loginID[in] �ɹ���½�ƴ洢���صľ��
	*  @param PoolID[in] ����¼��سɹ��󷵻ص�ID
	*  @param loudPoolInfoEx[out] ¼�����Ϣ
	*  @@return �ɹ�=0
	            ʧ��<0 :������
	*/
	CLOUD_API int API_PoolInfo(unsigned int loginID, unsigned int PoolID , CloudPoolInfoEx& loudPoolInfoEx);


	/** @fn CLOUD_API int API_PoolList(unsigned int loginID,unsigned int& Count,CPoolListDataCb PoolListDataCb,void* pUser)
	*  @brief ��ѯ����¼�����Ϣ
	*  @param loginID[in] �ɹ���½�ƴ洢���صľ��
	*  @param Count[out]  ��ѯ����¼���������
	*  @param PoolListDataCb[in] ¼�����Ϣ�ص�����
	*  @param pUser[in]   �û��Զ�������
	*  @@return �ɹ�=0
	            ʧ��<0 :������
	*/
	CLOUD_API int API_PoolList(unsigned int loginID, unsigned int& Count, CPoolListDataCb PoolListDataCb, void* pUser);






	/***********************************************************************************
	*                                                                                  *
	*                                     ��ǩ��Ϣ����                                 *
	*                                                                                  *
	************************************************************************************/


	/** @fn CLOUD_API int API_LabelAdd(unsigned int loginID,const char* CameraID
	*                             ,unsigned int BeginTime,unsigned int EndTime ,const char* content)
	*  @brief ��ǩ���
	*  @param loginID[in]              �ɹ���½�ƴ洢���صľ��
	*  @param CameraID[in]             ������ID ���64�ַ�
	*  @param BeginTime[in]            ��ӱ�ǩ�Ŀ�ʼʱ��
	*  @param EndTime[in]              ��ӱ�ǩ�Ľ���ʱ��
	*  @param content[in]              ��ǩ���ݣ����֧��1024�ַ�
	*  @@return �ɹ�=0
	            ʧ��<0 :������
	*/
	CLOUD_API int API_LabelAdd(unsigned int loginID, const char* CameraID, unsigned int BeginTime,
		unsigned int EndTime, const char* content);


	/** @fn CLOUD_API int API_LabelSearch(unsigned int loginID,const char* CameraID,
	*                                unsigned int BeginTime,unsigned int EndTime ,unsigned int&Count,CLabelCb LabelCb,void* pUser)
	*  @brief ��ǩ����
	*  @param loginID[in]              �ɹ���½�ƴ洢���صľ��
	*  @param CameraID[in]             ������ID ���64�ַ� 
	*  @param BeginTime[in]            ������ǩ�Ŀ�ʼʱ��
	*  @param EndTime[in]              ������ǩ�Ľ���ʱ��
	*  @param LabelKeyword[in]         ������ǩ�Ĺؼ���
	*  @param Count[out]               ��ѯ����ǩ��������
	*  @param LabelCb[in]              ���ݻص�����
	*  @param pUser[in]                �û��Զ�������
	*  @@return �ɹ�=0
	            ʧ��<0 :������
	*/
	CLOUD_API int API_LabelSearch(unsigned int loginID, const char* CameraID, unsigned int BeginTime,
		unsigned int EndTime, const char* LabelKeyword, unsigned int& Count, CLabelCb LabelCb, void* pUser);


	/** @fn CLOUD_API int API_LabelDel(unsigned int loginID,const char* CameraID,unsigned int BeginTime,unsigned int EndTime )
	*  @brief ��ǩɾ��
	*  @param loginID[in]              �ɹ���½�ƴ洢���صľ��
	*  @param CameraID[in]             ������ID ���64�ַ�
	*  @param BeginTime[in]            ɾ����ǩ�Ŀ�ʼʱ��
	*  @param EndTime[in]              ɾ����ǩ�Ľ���ʱ��
	*  @@return �ɹ�=0
	            ʧ��<0 :������
	*/
	CLOUD_API int API_LabelDel(unsigned int loginID, const char* CameraID, unsigned int BeginTime, unsigned int EndTime );


	/** @fn CLOUD_API int API_LabelEdit(unsigned int loginID,const char* CameraID,unsigned int BeginTime,unsigned int EndTime ,const char* newcontent)
	*  @brief ��ǩ�޸�
	*  @param loginID[in]              �ɹ���½�ƴ洢���صľ��
	*  @param CameraID[in]             ������ID ���64�ַ�
	*  @param BeginTime[in]            �޸ı�ǩ�Ŀ�ʼʱ��
	*  @param EndTime[in]              �޸ı�ǩ�Ľ���ʱ��
	*  @param newcontent[in]           �±�ǩ���� ���֧��1024�ַ�
	*  @@return �ɹ�=0
	            ʧ��<0 :������
	*/
	CLOUD_API int API_LabelEdit(unsigned int loginID, const char* CameraID, unsigned int BeginTime, unsigned int EndTime, const char* newcontent);




	/**************************************************************************
	*                                                                         *
	*                             ¼��طŲ���                                *
	*                                                                         *
	***************************************************************************/


	/** @fn CLOUD_API int API_GetRecordHead(unsigned int loginID,const char* CameraID
	*                        ,unsigned int BeginTime,unsigned int EndTime,char*& Headbuff,int& HeadSize)
	*  @brief ��ȡ��Ƶͷ�ļ�
	*  @param loginID[in]              �ɹ���½�ƴ洢���صľ��
	*  @param CameraID[in]             ������ID ���64�ַ�
	*  @param BeginTime[in]            ¼��ʼʱ��
	*  @param EndTime[in]              ¼�����ʱ��
	*  @param Headbuff[out]            ��ȡ��¼������
	*  @param headSize[out]            ��ȡ��¼�����ݴ�С
	*  @@return �ɹ�=0
	            ʧ��<0 :������
	*/
	CLOUD_API int API_GetRecordHead(unsigned int loginID, const char* CameraID, unsigned int BeginTime,
		unsigned int EndTime, char* Headbuff, int& HeadSize);

	/** @fn CLOUD_API int API_PlayStart(unsigned int loginID,const char* CameraID
	*                                ,unsigned int BeginTime,unsigned int EndTime,PlayType type,const char* RecordType)
	*  @brief ��ʼ�ط�
	*  @param loginID[in]              �ɹ���½�ƴ洢���صľ��
	*  @param CameraID[in]             ������ID ���64�ַ�
	*  @param BeginTime[in]            �ط�¼��Ŀ�ʼʱ��
	*  @param EndTime[in]              �ط�¼��Ľ���ʱ��
	*  @param type[in]                 �ط�����[�μ�PlayType����]
	*  @param RecordType[in]           ¼������(����ʹ���������)
	1.RecordType֧�ֱ�������ϲ�ѯ
	��������ʽ��1,2,3 
	2.��Ҫ�����Ʊ������Ͳ�ѯ����ʹ�ÿմ� ""
	3.���������ִ��У�����0��-1�ĳ��֣�0��ʾ���ԣ���ʱ��¼��-1��ʾ�ֶ�������¼�񣬲���0��-1ͬ�����Ժ�1��2��3�ȱ���������ϳɸ��ָ��ӵ�¼���ѯ����
	*  @@return �ɹ�>=0 :�طž��
	            ʧ��<0 :������
	*/
	CLOUD_API int API_PlayStart(unsigned int loginID, const char* CameraID, unsigned int BeginTime,
		unsigned int EndTime, PlayType type, const char* RecordType);


	/** @fn CLOUD_API int API_PlayData(unsigned int loginID,int PlayHandle,char* PlayBuff,int Bufflen)
	*  @brief ¼��ط�����:�˽ӿ�Ϊ�����ȡ���ݽӿڣ���ȡ���ݹ����г����κ��쳣��������һ����Ƶ���ݽ�����һ����Ƶ���ݵ�ȡ��
	*  @param loginID[in]              �ɹ���½�ƴ洢���صľ��
	*  @param PlayHandle[in]           �ɹ������طŷ��صľ��
	*  @param PlayBuff[in]             �ط�����
	*  @param Bufflen[in]              ���ݳ��� 
	*  @@return �ɹ�>=0: ��ȡ���ݴ�С[��λ���ֽ�]
	            ʧ��<0 : ������
	            ������Ϊ-50014ʱ�򣬻ط���ɣ����Խ���ֹͣ�ط�
				      ������Ϊ-50015ʱ���Ѿ�ֹͣ�ط�
	*/
	CLOUD_API int API_PlayData(unsigned int loginID, int PlayHandle, char* PlayBuff, int Bufflen);

    /** @fn CLOUD_API int API_PlayData_resuming(unsigned int loginID,int PlayHandle,char* PlayBuff,int Bufflen)
    *  @brief ¼��ط����ݣ��˽ӿڵ������κ��쳣�����ظ������ߣ������ٴε��ô˽ӿڣ����ݽ��ӳ����쳣��λ�ü���ȡ��
    *  @param loginID[in]              �ɹ���½�ƴ洢���صľ��
    *  @param PlayHandle[in]           �ɹ������طŷ��صľ��
    *  @param PlayBuff[in]             �ط�����
    *  @param Bufflen[in]              ���ݳ��� 
    *  @@return �ɹ�>=0: ��ȡ���ݴ�С[��λ���ֽ�]
    ʧ��<0 : ������
    ������Ϊ-50014ʱ�򣬻ط���ɣ����Խ���ֹͣ�ط�
    ������Ϊ-50015ʱ���Ѿ�ֹͣ�ط�
    */
    CLOUD_API int API_PlayData_resuming(unsigned int loginID, int PlayHandle, char* PlayBuff, int Bufflen);


	/** @fn CLOUD_API int API_PlayStop(unsigned int loginID,int PlayID)
	*  @brief ֹͣ�ط�
	*  @param loginID[in]              �ɹ���½�ƴ洢���صľ��
	*  @param PlayHandle[in]           �ɹ������طŷ��صľ��
	*  @@return �ɹ�=0
	            ʧ��<0 :������
	*/
	CLOUD_API int API_PlayStop(unsigned int loginID, int PlayHandle);


	/** @fn CLOUD_API int API_PlaySeek(unsigned int loginID,int PlayHandle,unsigned int PosTime)
	*  @brief �طŶ�λ
	*  @param loginID[in]              �ɹ���½�ƴ洢���صľ��
	*  @param PlayHandle[in]           �ɹ������طŷ��صľ��
	*  @param PosTime[in]              ��λʱ��[ʱ���]
	*  @@return �ɹ�>=0
	            ʧ��<0 :������
	*/
	CLOUD_API int API_PlaySeek(unsigned int loginID, int PlayHandle, unsigned int PosTime);


	/** @fn CLOUD_API int API_RecordSearch(unsigned int loginID,const char* CameraID,
	*                                    unsigned int BeginTime,unsigned int EndTime,const char* SearchRecordType,
	*                                    RecordLockType bLock,CSearchDataCb SearchDataCb,void* pUser)
	*  @brief ¼������
	*  @param loginID[in]       �ɹ���½�ƴ洢���صľ��
	*  @param CameraID[in]      ������ID ���64�ַ�
	*  @param Type[in]          ֡����
	*  @param BeginTime[in]     ¼��ʼʱ��
	*  @param EndTime[in]       ¼�����ʱ��
	*  @param SearchRecordType[in] ��ѯ���������
	1.SearchRecordType֧�ֱ�������ϲ�ѯ
	��������ʽ��1,2,3 
	2.��Ҫ�����Ʊ������Ͳ�ѯ����ʹ�ÿմ� ""
	3.���������ִ��У�����0��-1�ĳ��֣�0��ʾ���ԣ���ʱ��¼��-1��ʾ�ֶ�������¼�񣬲���0��-1ͬ�����Ժ�1��2��3�ȱ���������ϳɸ��ָ��ӵ�¼���ѯ����
	*  @param bLock[in]          ¼���������� [��ѯ¼���������ͣ�0: ȫ������(����������¼���û��������¼��)��1: û��������2: ����]
	*  @param Count[out]         ��ѯ����¼���������
	*  @param SearchDataCb[in]   ¼��ص�����
	*  @param pUser[in]          �û��Զ�������
	*  @@return �ɹ�=0 
	            ʧ��<0  : ������
	*/
	CLOUD_API int API_RecordSearch(unsigned int loginID, const char* CameraID, unsigned int BeginTime,
		                           unsigned int EndTime, const char* SearchRecordType, RecordLockType bLock,
								   unsigned int &Count, CSearchDataCb SearchDataCb,void* pUser);


    /** @fn CLOUD_API int API_RecordSearch(unsigned int loginID,const char* CameraID,
    *                                    unsigned int BeginTime,unsigned int EndTime,const char* SearchRecordType,
    *                                    RecordLockType bLock,CSearchDataCb SearchDataCb,void* pUser)
    *  @brief ¼����������չ�ӿڣ��ɲ�ѯ����¼��
    *  @param loginID[in]       �ɹ���½�ƴ洢���صľ��
    *  @param CameraID[in]      ������ID ���64�ַ�
    *  @param Type[in]          ֡����
    *  @param BeginTime[in]     ¼��ʼʱ��
    *  @param EndTime[in]       ¼�����ʱ��
    *  @param SearchRecordType[in] ��ѯ���������
    1.SearchRecordType֧�ֱ�������ϲ�ѯ
    ��������ʽ��1,2,3 
    2.��Ҫ�����Ʊ������Ͳ�ѯ����ʹ�ÿմ� ""
    3.���������ִ��У�����0��-1�ĳ��֣�0��ʾ���ԣ���ʱ��¼��-1��ʾ�ֶ�������¼�񣬲���0��-1ͬ�����Ժ�1��2��3�ȱ���������ϳɸ��ָ��ӵ�¼���ѯ����
    *  @param bLock[in]          ¼���������� [0: ȫ������(����������¼���û��������¼��)��1: û��������2: ������]
    *  @param nBackup[in]        ��ѯ������Ϣ��־[0 �C ��ѯ����¼����Ϣ��1 �C ��ѯ����¼����Ϣ]
    *  @param Count[out]         ��ѯ����¼���������
    *  @param SearchDataCb[in]   ¼��ص�����
    *  @param pUser[in]          �û��Զ�������
    *  @@return �ɹ�=0 
    ʧ��<0  : ������
    */
    CLOUD_API int API_RecordSearchEx(unsigned int loginID, const char* CameraID, unsigned int BeginTime, 
                        unsigned int EndTime, const char* SearchRecordType, RecordLockType bLock,
                        unsigned int nBackup, unsigned int &Count, CSearchDataCb SearchDataCb, void* pUser);

	/** @fn CLOUD_API int API_RecordSearchByTag(unsigned int loginID,
	*                         const char* CameraID,const char* tagInfo,CSearchDataCb SearchDataCb,void* pUser)
	*  @brief ����ǩ����¼��
	*  @param loginID[in]         �ɹ���½�ƴ洢���صľ��
	*  @param CameraID[in]        ������ID ���64�ַ�
	*  @param tagInfo[in]         ��ǩ��Ϣ  	���Ϊ���ַ�("") ��ʾ��ѯ�����б�ע��¼��
	*  @param Count[out]          ��ѯ����¼���������
	*  @param SearchDataCb[in]    ��ǩ�ص�����
	*  @param pUser[in]           �û��Զ�������
	*  @@return �ɹ�=0
	            ʧ��<0 :������
	*/
	CLOUD_API int API_RecordSearchByTag(unsigned int loginID, const char* CameraID, const char* tagInfo,
		                                unsigned int &Count, CSearchDataCb SearchDataCb, void* pUser);


	/** @fn CLOUD_API int API_RecordLock(unsigned int loginID,const char* CameraID
	*                                       ,unsigned int BeginTime,unsigned int EndTime ,unsigned int LockTime)
	*  @brief ¼������
	*  @param loginID[in]         �ɹ���½�ƴ洢���صľ��
	*  @param CameraID[in]        ������ID ���64�ַ�
	*  @param BeginTime[in]       ¼��ʼʱ��
	*  @param EndTime[in]         ¼�����ʱ��
	*  @param LockTime[in]        ����ʱ��[0: ��ʾ������������0: ��ʾ������ʱ��(�ӵ�ǰʱ�䵽��Ҫ����ʱ����ʱ��) ��λ:��]
	*  @@return �ɹ�=0
	            ʧ��<0 :������
	*/
	CLOUD_API int API_RecordLock(unsigned int loginID, const char* CameraID, unsigned int BeginTime,
		                         unsigned int EndTime, unsigned int LockTime, const char* RecordType);


	/** @fn CLOUD_API int API_RecordDelete(unsigned int loginID,const char* CameraID,
	*                                          const char* RecordType,unsigned int BeginTime,unsigned int EndTime)
	*  @brief ¼��ɾ��
	*  @param loginID[in]         �ɹ���½�ƴ洢���صľ��
	*  @param CameraID[in]        ������ID ���64�ַ�
	*  @param RecordType[in]      ɾ��¼�������(����ʹ���������),����ɾ����Ӧ�ĸ�����Ϣ�����
	*  @param BeginTime[in]       ��Ҫɾ��¼��Ŀ�ʼʱ��
	*  @param EndTime[in]         ��Ҫɾ��¼��Ľ���ʱ��
	*  @@return �ɹ�=0
	            ʧ��<0 :������
	*/
	CLOUD_API int API_RecordDelete(unsigned int loginID, const char* CameraID, const char* RecordType,
		                           unsigned int BeginTime, unsigned int EndTime);


	/** @fn CLOUD_API int API_RecordDownloadStart(unsigned int loginID,
	*                                    const char* CameraID,unsigned int BeginTime,unsigned int EndTime,
	*                                    const char* RecordType,int DownloadOffset,CDownloadDataCb DownloadDataCb,void* pUser)
	*  @brief ¼��ʼ����
	*  @param loginID[in]         �ɹ���½�ƴ洢���صľ��
	*  @param CameraID[in]        ������ID ���64�ַ�
	*  @param BeginTime[in]       ����¼��Ŀ�ʼʱ��
	*  @param EndTime[in]         ����¼��Ľ���ʱ��
	*  @param RecordType[in]       ��Ҫ���ص�¼������(����ʹ���������)
	1.RecordType֧�ֱ�������ϲ�ѯ
	��������ʽ��1,2,3 
	2.��Ҫ�����Ʊ������Ͳ�ѯ����ʹ�ÿմ� ""
	3.���������ִ��У�����0��-1�ĳ��֣�0��ʾ���ԣ���ʱ��¼��-1��ʾ�ֶ�������¼�񣬲���0��-1ͬ�����Ժ�1��2��3�ȱ���������ϳɸ��ָ��ӵ�¼���ѯ����
	*  @param DownloadOffset[in]   �������ֽ��� ֧�ֶϵ�����
	*  @param DownloadDataCb[in]   �������ݻص�
	*  @param pUser[in]            �û��Զ�������
	*  @@return �ɹ�=>0:���ؾ��
	            ʧ��<0 :������
	*/
	CLOUD_API int API_RecordDownloadStart(unsigned int loginID, const char* CameraID, unsigned int BeginTime, unsigned int EndTime,
		                                  const char* RecordType, int DownloadOffset, CDownloadDataCb DownloadDataCb, void* pUser);


	/** @fn CLOUD_API int API_RecordDownloadStop(unsigned int loginID,int RecordDownloadHandle)
	*  @brief ¼��ֹͣ����
	*  @param loginID[in]              �ɹ���½�ƴ洢���صľ��
	*  @param RecordDownloadHandle[in] �������سɹ����صľ��
	*  @@return �ɹ�=0
	            ʧ��<0 :������
	*/
	CLOUD_API int API_RecordDownloadStop(unsigned int loginID, int RecordDownloadHandle);


	/** @fn CLOUD_API int API_RecordMove(unsigned int loginID,const char* CameraID,
	*                           const char* RecordType,BOOL iFrameOnly,unsigned int BeginTime,unsigned int EndTime)
	*  @brief ¼��ת���Ǩ��
	*  @param loginID[in]              �ɹ���½�ƴ洢���صľ��
	*  @param CameraID[in]             ������ID ���64�ַ�
	*  @param RecordType[in]           ¼������(����ʹ���������)
	1.RecordType֧�ֱ�������ϲ�ѯ
	��������ʽ��1,2,3 
	2.��Ҫ�����Ʊ������Ͳ�ѯ����ʹ�ÿմ� ""
	3.���������ִ��У�����0��-1�ĳ��֣�0��ʾ���ԣ���ʱ��¼��-1��ʾ�ֶ�������¼�񣬲���0��-1ͬ�����Ժ�1��2��3�ȱ���������ϳɸ��ָ��ӵ�¼���ѯ����
	*  @param MoveType[in]             Ǩ������ [0: Ĭ�Ϸ�ʽǨ������֡�������ݡ�1:ֻǨ��I֡����]
	*  @param BeginTime[in]            ¼��ʼʱ��
	*  @param EndTime[in]              ¼�����ʱ��
	*  @@return �ɹ�=0
	            ʧ��<0 :������
	*/
	CLOUD_API int API_RecordMove(unsigned int loginID, const char* CameraID, const char* RecordType,
		                         int MoveType, unsigned int BeginTime, unsigned int EndTime);


    /** @fn CLOUD_API int API_backup ( unsigned int loginID, const char *CameraID,const strBackupPlan *pBackupPlan, unsigned int BackupType, unsigned int Type)
    *  @brief ���ƴ洢ϵͳ�·���ǰ�������ı��ݺ�����ƻ���Ϣ
    *  @param loginID[in]       �ɹ���½�ƴ洢���صľ��
    *  @param CameraID[in]  �ɹ�����¼�񷵻صľ��
    *  @param strBackupPlan
    *  @param BackupType     �������ͣ�1:���Ʊ��ݣ�2:���Ʊ��ݣ�IPͨ����3:���Ʊ��ݣ�IP��ͨ��
    *  @param Type            1:�趨����2:ȡ������
    *  @@return �ɹ�=0
    ʧ��<0 :������
    */
    CLOUD_API int API_backup ( unsigned int loginID, const char *CameraID, 
        const strBackupPlan *pBackupPlan, unsigned int BackupType, unsigned int Type);

    /** @fn CLOUD_API int SetBackupCallBack(BackupDataCallBack  pBackupDataCallBak, void* pUserData)
    *  @brief ע�ᱸ������Ļص�����
    *  @param BackupDataCallBack[in]       �û�ע��Ļص�����
    *  @param handle[in]                   �û�����
    *  @@return �ɹ�=0
    ʧ��<0 :������
    */
    CLOUD_API int SetBackupCallBack(BackupDataCallBack  pBackupDataCallBak, void* pUserData);

};//extern "C"

#endif