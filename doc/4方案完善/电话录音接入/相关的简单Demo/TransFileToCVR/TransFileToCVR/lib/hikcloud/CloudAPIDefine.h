/** @file   CloudApiDefine.h
*  @note   HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*  @brief  API Interface H File
*  @author weiwenjuan@hikvision.com.cn
*  @date   2013-01-17
*
*  @change log
*
*/
//lint -library
#ifndef __CLOUD_API_DEFINE_H__
#define __CLOUD_API_DEFINE_H__

#include "CloudAPIPublic.h"

//#define CAMERAID_LEN         72    // ����������ַ�����
//#define IPLength             20    // ����IP��ַ��Ϣ����
//#define DeviceIDLength       48    // �����豸ID����


//typedef unsigned char       BYTE;
//typedef unsigned long       DWORD;

extern "C"
{
	/**  @enum  Record_Data_Type [CloudApiDefine.h]
	*  @brief ¼����������
	*
	*/
	enum RecordDataType
	{
		Record_Normal = 1,                         // ��ͨ¼��
		Record_Patch = 2,                          // ��¼
	};


	/**  @enum Record_Attached_Type [CloudApiDefine.h]
	*  @brief ¼������Ϣ����
	*
	*/
	enum RecordAttachedType
	{
		RecordNULL = 0 ,                          // �޸��Ӳ���
		RecordBack = 1,		                      // ����
		RecordRedundancy = 3,                     // ����
	};


	/**  @enum FrameType[CloudApiDefine.h]
	*  @brief ¼��֡����
	*
	*/
	enum FrameType
	{
		FrameN = 1,                               // ��ͨ֡
		FrameI = 3,                               // �ؼ�֡
	};

	/** @struct tagCloudAttachedInfo [CloudApiDefine.h]
	* @brief ������Ϣ���ݽṹ����
	*/
	typedef struct  tagCloudAttachedInfo
	{
		tagCloudAttachedInfo()
		{
			memset(Camera,0,sizeof(Camera) );
			BeginTime=0;
			EndTime=0;
			Type=0;
			Lock=0;
			Size=0;
		}

		tagCloudAttachedInfo(tagCloudAttachedInfo&tag)
		{
			memcpy(Camera,tag.Camera,sizeof(tag.Camera));
			BeginTime=tag.BeginTime;
			EndTime=tag.EndTime;
			Type=tag.Type;
			Lock=tag.Lock;
			Size=tag.Size;
		}

		tagCloudAttachedInfo& operator=(tagCloudAttachedInfo& tag)
		{
			memcpy(Camera,tag.Camera,sizeof(tag.Camera));
			BeginTime=tag.BeginTime;
			EndTime=tag.EndTime;
			Type=tag.Type;
			Lock=tag.Lock;
			Size=tag.Size;
			return *this;
		}

		char  Camera[CAMERAID_LEN];	           // ������ID
		unsigned int  BeginTime ;	           // ������Ϣ��ʼʱ��
		unsigned int  EndTime;	               // ������Ϣ����ʱ��
		int   Type;	                           // ��������
		int	  Lock;	                           // ������Ϣ����״̬ [0: ȫ������(������δ����)��1: δ������2: ����]
		int	  Size;                            // ������С[��λ: �ֽ�]
	}CloudAttachedInfo;

	/** 
	*  @brief ������Ϣ��ѯ����ص���������
	*  @param info[out] ������Ϣ 
	*  @param Count[out] ��ѯ������Ϣ���������
	*  @param Flat[out]  ʣ�฽����Ϣ������ [����0: ��ʾ������Ϣȫ�����ء� С��0: Ϊ�����롢����0: ��ʾʣ��ĸ�����Ϣ����]
	*  @param pUser[out] �û��Զ������� 
	*  @@return ����0:�����Ѵ���С��0:����δ����
	*/
	typedef int (*CAttachedSearchDataCb)(CloudAttachedInfo info, int Count, int Flat, void* pUser);

	/** 
	*  @brief ¼���������ݻص���������
	*  @param buff[out] ¼������
	*  @param bufflen[out] ¼�����ݳ��� 
	*  @param Flat[out]  ������ɱ�ʶ [С�ڵ���0: ���ؽ����� С��0: Ϊ������]
	*  @param pUser[out] �û��Զ������� 
	*  @@return ����0:�����Ѵ���С��0:����δ����
	
	typedef int (*CDownloadDataCb)(const char* buff, int bufflen, int Flat, void* pUser);
*/

	/** 
	*  @brief ͼƬ���ػص���������
	*  @param buff[out] ͼƬ����
	*  @param bufflen[out] ͼƬ���ݳ���
	*  @param pUser[out] �û��Զ������� 
	*  @@return ����0:�����Ѵ���С��0:����δ����
	
	typedef int (*CPictureDownLoadCB)(const char* buff, int bufflen, void* pUser);
*/

	/** 
	*  @brief ����ͼƬ���ػص���������
	*  @param buff[out] ͼƬ����
	*  @param bufflen[out] ͼƬ���ݳ���
	*  @param Type[out] ͼƬ���� [1:JPG��2:BMP��3:PNG] 
	*  @param Count[out] ��ѯ����ͼƬ������
	*  @param Flat[out]  ʣ��ͼƬ������(�������ж�ֹͣ����) [����0: ��ʾ������ɡ� С��0: Ϊ�����롢����0: ��ʾ��Ҫ����ͼƬ������]
	*  @param pUser[out] �û��Զ������� 
	*  @@return ����0:�����Ѵ���С��0:����δ����
	*/
	typedef int (*CPictureDownLoadCBEx)(const char* buff, int bufflen, int Type, int Count, int Flat, void* pUser);


    /** 
	*  @brief �¼��ص���������.����������󡢳����쳣����Ϣ�ϱ�
	*  @param LogicType[out] API��ģ����� [0:��֤��1:¼��(������ע)��2:ͼƬ��3:������Ϣ��4:¼��ع���]
	*  @param ErrorCode[out] ������
	*  @param pUser[out] �û��Զ������� 
	*  @@return ����0:�����Ѵ���С��0:����δ����
	
	typedef int (*CUserMessage)(int LogicType, int ErrorCode, void* pUser);
*/
}//extern "C"

#endif  //__CLOUD_API_DEFINE_H__
