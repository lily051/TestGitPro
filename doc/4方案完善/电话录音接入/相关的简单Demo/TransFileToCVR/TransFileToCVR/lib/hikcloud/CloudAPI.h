/** @file   Cloud_API.h
*  @note	HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
*  @brief	API Interface H File
*  @author weiwenjuan@hikvision.com.cn
*  @date	2013-01-17
*
*  @change log
*
*/
//lint -library
#ifndef __CLOUD_API_H__
#define __CLOUD_API_H__

#include "CloudAPIDefine.h"

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

	/*****************************************************************************
	*                                                                            *
	*                             ��Ƶ¼�񲿷�                                   *
	*                                                                            *
	******************************************************************************/


	/** @fn CLOUD_API int API_RecordStart(unsigned int loginID,const char* CameraID,
	*                  int RecordType,RecordDataType DataType,RecordAttachedType AttachedType,
	*					int FrameSupport,unsigned int PoolID,const char* HeadBuff,int HeadSize)
	*  @brief ��ʼ¼��
	*  @param loginID[in]        �ɹ���½�ƴ洢���صľ��
	*  @param CameraID[in]       ������ID
	*  @param RecordType[in]     ¼������
	*  @param Data_Type[in]      ¼����������
	*  @param Attached_Type[in]  ¼�񸽼�����
	*  @param FrameSupport[in]   �Ƿ��Զ���I֡ [0: ���Զ���I֡  1: �Զ���I֡]
	*  @param PoolID[in]         ����¼��سɹ����ص�ID
	*  @param HeadBuff[in]       ��Ƶ����ͷbuf(���磺�������ļ�ͷ)
	*  @param HeadSize[in]       ��Ƶ����ͷ��С
	*  @@return �ɹ�>=0 :¼����
	            ʧ��<0 :������
	*/
	CLOUD_API int API_RecordStart(unsigned int loginID, const char* CameraID, int RecordType,
	                              RecordDataType DataType, RecordAttachedType AttachedType, int FrameSupport,
								  unsigned int PoolID, const char* HeadBuff, int HeadSize);


	/** @fn CLOUD_API int API_RecordData(unsigned int loginID, int RecordHandle,FrameType Type ,unsigned int BeginTime,
	*                                    unsigned int EndTime ,const char* Buffer,unsigned int Bufflen)
	*                                       
	*  @brief ¼������
	*  @param loginID[in]       �ɹ���½�ƴ洢���صľ��
	*  @param RecordHandle[in]  �ɹ�����¼�񷵻صľ��
	*  @param Type[in]          ֡����
	*  @param BeginTime[in]     ¼��ʼʱ��
	*  @param EndTime[in]       ¼�����ʱ��
	*  @param Buffer[in]        ����
	*  @param Bufflen[in]       ���ݳ���
	*  @@return �ɹ�=0
	            ʧ��<0 :������
	*/
	CLOUD_API int API_RecordData(unsigned int loginID, int RecordHandle, FrameType Type,
		                         unsigned int BeginTime, unsigned int EndTime, const char* Buffer, unsigned int Bufflen);


	/** @fn CLOUD_API int API_SetRecordType(unsigned int loginID,int RecordHandle,int NewRecordType)
	*  @brief �޸�¼������
	*  @param loginID[in]       �ɹ���½�ƴ洢���صľ��
	*  @param RecordHandle[in]  �ɹ�����¼�񷵻صľ��
	*  @param NewRecordType[in] ��¼������
	*  @@return �ɹ�=0
	            ʧ��<0 :������
	*/
	CLOUD_API int API_SetRecordType(unsigned int loginID, int RecordHandle, int NewRecordType);


#ifdef RECORD_REDUNDANCY
#endif
    /** @fn CLOUD_API int API_redundancy(unsigned int loginID, int handle, int poolID, unsigned int Type)
    *  @brief ¼������
    *  @param loginID[in]       �ɹ���½�ƴ洢���صľ��
    *  @param handle[in]        ����¼��ľ��
    *  @param poolID[in]        �����Ŀ��¼���ID
    *  @param Type[in]          �����������ͣ�1���趨����2��ȡ������
    *  @@return �ɹ�=0
                ʧ��<0 :������
    */
    CLOUD_API int API_redundancy(unsigned int loginID, int handle, unsigned int poolID, unsigned int Type);

	/** @fn CLOUD_API int API_RecordStop(unsigned int loginID,int RecordHandle)
	*  @brief ¼��ֹͣ
	*  @param loginID[in]       �ɹ���½�ƴ洢���صľ��
	*  @param RecordHandle[in]  �ɹ�����¼�񷵻صľ��
	*  @@return �ɹ�=0
	            ʧ��<0 :������
	*/
	CLOUD_API int API_RecordStop(unsigned int loginID, int RecordHandle);

	/***************************************************************************************
	*                                                                                      *
	*                                         ͼƬ����                                     *
	*                                                                                      *
	****************************************************************************************/


	/** @fn CLOUD_API int API_PictureStoreStart(unsigned int loginID,const char* CameraID , unsigned int PoolID)
	*  @brief �����洢ͼƬ
	*  @param loginID[in]              �ɹ���½�ƴ洢���صľ��
	*  @param CameraID[in]             ������ID [���64�ַ�]
	*  @param PoolID[in]               �ɹ�����¼��ط��ص�ID
	*  @@return �ɹ�>=0:ͼƬ�洢���
	            ʧ��<0 :������
	*/
	CLOUD_API int API_PictureStoreStart(unsigned int loginID, const char* CameraID, unsigned int PoolID);


	/** @fn CLOUD_API int API_PictureStoreData(unsigned int loginID,int PictureStoreHandle,unsigned int TimePos,unsigned short PictureType,const char* buff ,unsigned int bufflen,char* RetURL)
	*  @brief д��ͼƬ����
	*  @param loginID[in]              �ɹ���½�ƴ洢���صľ��
	*  @param PictureStoreHandle[in]   �ɹ�����ͼƬ�洢���صľ��
	*  @param TimePos[in]              ͼƬʱ���[�洢ͼƬ��ʱ���ǰ�˱�������ʱ��]
	*  @param PictureType[in]          ͼƬ���� [1:JPG��2:BMP��3:PNG] 
	*  @param buff[in]                 ͼƬ������Ϣ
	*  @param bufflen[in]              ͼƬ���ݳ���
	*  @param RetURL[out]              ���ر���ͼƬ��URL
	*  @@return �ɹ�=0
	            ʧ��<0 :������
	*/
	CLOUD_API int API_PictureStoreData(unsigned int loginID, int PictureStoreHandle, unsigned int TimePos,
		                               unsigned short PictureType, const char* buff, unsigned int bufflen, char* RetURL);


	/** @fn CLOUD_API int API_PictrueStoreStop(unsigned int loginID,int PictureStoreHandle)
	*  @brief ֹͣ�洢ͼƬ
	*  @param loginID[in]              �ɹ���½�ƴ洢���صľ��
	*  @param PictureStoreHandle[in]   �ɹ�����ͼƬ�洢���صľ��
	*  @@return �ɹ�=0
	            ʧ��<0 :������
	*/
	CLOUD_API int API_PictureStoreStop(unsigned int loginID, int PictureStoreHandle);


	/** @fn CLOUD_API int API_PictureStoreDataEx(unsigned int loginID,int PictureStoreHandle,unsigned int TimePos,unsigned short PictureType,const char* buff ,unsigned int bufflen,char* RetURL)
	*  @brief д��ͼƬ����(Ϊ��ҵ����)
	*  @param loginID[in]              �ɹ���½�ƴ洢���صľ��
	*  @param PictureStoreHandle[in]   �ɹ�����ͼƬ�洢���صľ��
	*  @param TimePos[in]              ͼƬʱ���[�洢ͼƬ��ʱ���ǰ�˱�������ʱ��]
	*  @param PictureType[in]          ͼƬ���� [1:JPG��2:BMP��3:PNG] 
	*  @param buff[in]                 �洢��ͼƬ����
	*  @param bufflen[in]              �洢���ݳ���
	*  @param Radom[int]               �����[��ǰʱ����ڴ洢��ͼƬ��ţ��ɵ��ó������ɵ�4λ�������1000 ��  9999]
	*  @@return �ɹ�=0
	            ʧ��<0 :������
	*/
	CLOUD_API int API_PictureStoreDataEx(unsigned int loginID, int PictureStoreHandle, unsigned int TimePos,
		                                 unsigned short PictureType,  const char* buff, unsigned int bufflen, unsigned int Radom);


	/** @fn CLOUD_API int API_PictrueStoreStop(unsigned int loginID,int PictureStoreHandle)
	*  @brief ֹͣͼƬ�洢(Ϊ��ҵ����)
	*  @param loginID[in]              �ɹ���½�ƴ洢���صľ��
	*  @param PictureStoreHandle[in]   �ɹ�����ͼƬ�洢���صľ��
	*  @@return �ɹ�=0
	            ʧ��<0 :������
	*/
	CLOUD_API int API_PictureStoreStopEx(unsigned int loginID,int PictureStoreHandle);


	/** @fn CLOUD_API int API_PictureDeleteByURL(unsigned int loginID,const char* URL)
	*  @brief ɾ����ǰRUL��ͼƬ
	*  @param loginID[in]              �ɹ���½�ƴ洢���صľ��
	*  @param URL[in]                  ����ͼƬ��URL 
	*  @@return �ɹ�=0
	            ʧ��<0 :������
	*/
	CLOUD_API int API_PictureDeleteByURL(unsigned int loginID,const char* URL);


	/** @fn CLOUD_API int API_PictureDeleteByTime(unsigned int loginID,const char* CameraID,unsigned int BeginTime ,unsigned int EndTime)
	*  @brief ɾ����ǰʱ��ε�ͼƬ
	*  @param loginID[in]              �ɹ���½�ƴ洢���صľ��
	*  @param CameraID[in]             ������ID [���64�ַ�]
	*  @param BeginTime[in]            ��ʼʱ��
	*  @param EndTime[in]              ����ʱ�� 
	*  @@return �ɹ�=0
	            ʧ��<0 :������
	*/
	CLOUD_API int API_PictureDeleteByTime(unsigned int loginID, const char* CameraID, unsigned int BeginTime , unsigned int EndTime);


	/** @fn CLOUD_API int API_PictureLockByURL(unsigned int loginID,const char* URL,unsigned int LockTime)
	*  @brief ������ǰURL��ͼƬ
	*  @param loginID[in]              �ɹ���½�ƴ洢���صľ��
	*  @param URL[in]                  ����ͼƬ��URL
	*  @param LockTime[in]             ����ʱ��[0: ��ʾ����������0: ��ʾ������ʱ��(�ӵ�ǰʱ�䵽��Ҫ����ʱ����ʱ��) ��λ:��]
	*  @@return �ɹ�=0
	            ʧ��<0 :������
	*/
	CLOUD_API int API_PictureLockByURL(unsigned int loginID, const char* URL, unsigned int LockTime);


	/** @fn CLOUD_API int API_PictureLockByTime(unsigned int loginID,const char* CameraID
	*                                       ,unsigned int BeginTime,unsigned int EndTime,unsigned int LockTime)
	*  @brief ������ǰʱ����ڵ�ͼƬ
	*  @param loginID[in]              �ɹ���½�ƴ洢���صľ��
	*  @param BeginTime[in]            ��ʼʱ��
	*  @param EndTime[in]              ����ʱ��
	*  @param LockTime[in]             ����ʱ��[0: ��ʾ������������0: ��ʾ������ʱ��(�ӵ�ǰʱ�䵽��Ҫ����ʱ����ʱ��) ��λ:��]
	*  @@return �ɹ�=0
	            ʧ��<0 :������
	*/
	CLOUD_API int API_PictureLockByTime(unsigned int loginID, const char* CameraID, unsigned int BeginTime,
		                                unsigned int EndTime, unsigned int LockTime);


	/** @fn CLOUD_API int API_PictureDownloadByURL(unsigned int loginID,const char* URL,unsigned int &Count,CPictureDownLoadCB BuffCB,void* pUser,float zoom )
	*  @brief ��URL����ͼƬ
	*  @param loginID[in]              �ɹ���½�ƴ洢���صľ��
	*  @param URL[in]                  ����ͼƬ��URL 
	*  @param ZoomType[in]             ��������[0:�����š�1:���ߴ����ű�����2:����С���ű���]
	*  @param ZoomValue[in]            ���ű���ֵ[ȡֵ1-99 ���ٷֱ�������]
	*  @param pUserData[in]            ����չ���� ����
	*  @param pBuf[in]                 ͼƬ�����ڴ�
	*  @param BufLen[out]              ͼƬ��С ��λ�ֽ�
	*  @param PictureType[out]         ͼƬ���� [1:JPG��2:BMP��3:PNG] 
	*  @@return �ɹ�=0
	ʧ��<0 :������
	*/
	CLOUD_API int API_PictureDownloadByURL(unsigned int loginID,const char* URL,int ZoomType,int ZoomValue,void* pUserData ,char* pBuf,int& BufLen,int&PictureType );


	/** @fn CLOUD_API int API_PictureDownloadByTime(unsigned int loginID,
	*                      const char* CameraID,const char* PictureType,unsigned int BeginTime,
	*                     unsigned int EndTime,unsigned int &Count,CPictureDownLoadCB BuffCB,void* pUser,float zoom)
	*  @brief ��ʱ������ͼƬ
	*  @param loginID[in]              �ɹ���½�ƴ洢���صľ��
	*  @param CameraID[in]             ������ID ���64�ַ�
	*  @param PictureType[in]          ��Ҫ���ص�ͼƬ����(����ʹ���������)
	*  @param BeginTime[in]            ��ʼʱ��
	*  @param EndTime[in]              ����ʱ��
	*  @param Count[out]               ��ѯ����ͼƬ������
	*  @param BuffCB[in]               ���ݻص�����
	*  @param pUser[in]                �û��Զ�������
	*  @param ZoomType[in]             ��������[0:�����š�1:���ߴ����ű�����2:����С���ű���]
	*  @param zoom[in]                 ���ű���ֵ [ȡֵ1-99 ���ٷֱ�������]
	*  @@return �ɹ�>=0:���ؾ��
	            ʧ��<0 :������
	*/
	CLOUD_API int API_PictureDownloadByTime(unsigned int loginID, const char* CameraID, const char* PictureType,
		                                    unsigned int BeginTime, unsigned int EndTime, unsigned int &Count,
											CPictureDownLoadCBEx BuffCB, void* pUser, int ZoomType, int ZoomValue);


	/** @fn CLOUD_API int API_PictureDownloadByTimeStop(unsigned int loginID,int PictureDownloadHandler)
	*  @brief ֹͣ��ʱ������ͼƬ
	*  @param loginID[in]                �ɹ���½�ƴ洢���صľ��
	*  @param PictureDownloadHandler[in] �ɹ�����ͼƬ���ط��صľ��
	*  @@return �ɹ�=0
	            ʧ��<0 :������
	*/
	CLOUD_API int API_PictureDownloadByTimeStop(unsigned int loginID, int PictureDownloadHandler);



	/**************************************************************************
	*                                                                         *
	*                                ������Ϣ����                             *
	*                                                                         *
	***************************************************************************/

	/** @fn CLOUD_API int API_AttachedInfoStart(unsigned int loginID,const char* CameraID,
	*                                             unsigned int PoolID,const char* HeadBuff,int HeadSize)
	*  @brief ��ʼ���渽����Ϣ
	*  @param loginID[in]                �ɹ���½�ƴ洢���صľ��
	*  @param CameraID[in]               ������ID ���64�ַ�
	*  @param PoolID[in]                 ¼���ID
	*  @param HeadBuff[in]               ������Ϣ����ͷ
	*  @param HeadSize[in]               ����ͷ��С
	*  @@return �ɹ�>0 :������Ϣ�洢���
	            ʧ��<0 :������
	*/
	CLOUD_API int API_AttachedInfoStart(unsigned int loginID, const char* CameraID, unsigned int PoolID,
		                                const char* HeadBuff, int HeadSize);


	/** @fn CLOUD_API int API_AttachedInfoData(unsigned int loginID,int AttachedInfoHandle
	*                             ,unsigned int BeginTime,unsigned int EndTime,const char* Buff,int Bufflen)
	*  @brief ������Ϣ���ݴ洢
	*  @param loginID[in]                �ɹ���½�ƴ洢���صľ��
	*  @param AttachedInfoHandle[in]     �ɹ�����д�븽����Ϣ���صľ��
	*  @param BeginTime[in]              ��ʼʱ��
	*  @param EndTime[in]                ����ʱ��
	*  @param Buff[in]                   ������Ϣ����
	*  @param Bufflen[in]                ������Ϣ���ݳ���
	*  @@return �ɹ�=0
	            ʧ��<0 :������
	*/
	CLOUD_API int API_AttachedInfoData(unsigned int loginID, int AttachedInfoHandle, unsigned int BeginTime,
		                               unsigned int EndTime, const char* Buff, int Bufflen);


	/** @fn CLOUD_API int API_AttachedInfoStop(unsigned int loginID,int AttachedInfoHandle)
	*  @brief ������Ϣֹͣ�洢
	*  @param loginID[in]                �ɹ���½�ƴ洢���صľ��
	*  @param AttachedInfoHandle[in]     �ɹ�����д�븽����Ϣ���صľ��
	*  @@return �ɹ�=0
	            ʧ��<0 :������
	*/
	CLOUD_API int API_AttachedInfoStop(unsigned int loginID,int AttachedInfoHandle);


	/** @fn CLOUD_API int API_AttachedInfoLock(unsigned int loginID,
	*                     const char* CameraID,unsigned int BeginTime,unsigned int EndTime,unsigned int LockTime)
	*  @brief ������Ϣ����
	*  @param loginID[in]                �ɹ���½�ƴ洢���صľ��
	*  @param CameraID[in]               ������ID ���64�ַ�
	*  @param BeginTime[in]              ��ʼʱ��
	*  @param EndTime[in]                ����ʱ��
	*  @param LockTime[in]               ����ʱ��[0: ��ʾ����������0: ��ʾ������ʱ��(�ӵ�ǰʱ�䵽��Ҫ����ʱ����ʱ��) ��λ:��]
	*  @@return �ɹ�=0
	ʧ��<0 :������
	*/
	CLOUD_API int API_AttachedInfoLock(unsigned int loginID, const char* CameraID, unsigned int BeginTime,
		                               unsigned int EndTime, unsigned int LockTime);

	/** @fn CLOUD_API int API_AttachedSearch(unsigned int loginID,const char* CameraID,unsigned int BeginTime,unsigned int EndTime,unsigned int &Count,CAttachedSearchDataCb AttachedSearchDataCb,void* pUser)
	*  @brief ������Ϣ����
	*  @param loginID[in]                �ɹ���½�ƴ洢���صľ��
	*  @param CameraID[in]               ������ID ���64�ַ�
	*  @param BeginTime[in]              ��ʼʱ��
	*  @param EndTime[in]                ����ʱ��
	*  @param Count[out]                 ��ѯ���ĸ�����Ϣ��������
	*  @param AttachedSearchDataCb[in]   �ص�����
	*  @param pUser[in]                  �û��Զ�������
	*  @@return �ɹ�=0
	            ʧ��<0 :������
	*/
	CLOUD_API int API_AttachedSearch(unsigned int loginID, const char* CameraID, unsigned int BeginTime,
		unsigned int EndTime, unsigned int &Count, CAttachedSearchDataCb AttachedSearchDataCb, void* pUser);


	/** @fn CLOUD_API int API_AttachedDownloadStart(unsigned int loginID,
	*                      const char* CameraID,unsigned int BeginTime,unsigned int EndTime,int Downloadoffset)
	*  @brief ������Ϣ��ʼ����
	*  @param loginID[in]                �ɹ���½�ƴ洢���صľ��
	*  @param CameraID[in]               ������ID ���64�ַ�
	*  @param BeginTime[in]              ��ʼʱ��
	*  @param EndTime[in]                ����ʱ��
	*  @param Downloadoffset[in]         �Ѿ����ص��ֽ���
	*  @@return �ɹ�>0 :������Ϣ���ؾ��
	            ʧ��<0 :������
	*/
	CLOUD_API int API_AttachedDownloadStart(unsigned int loginID, const char* CameraID,
		                                    unsigned int BeginTime, unsigned int EndTime, int Downloadoffset);


	/** @fn CLOUD_API int API_AttachedDownloadStop(unsigned int loginID,int AttachedDownloadHandle)
	*  @brief ֹͣ���� ������Ϣ(ֻ��ֹͣ��ǰ�û�������)
	*  @param loginID[in]                �ɹ���½�ƴ洢���صľ��
	*  @param AttachedDownloadHandle[in] �ɹ��������ظ�����Ϣ���صľ��
	*  @@return �ɹ�=0
	ʧ��<0 :������
	*/
	CLOUD_API int API_AttachedDownloadStop(unsigned int loginID,int AttachedDownloadHandle);


	/** @fn CLOUD_API int API_AttachedDel(unsigned int loginID,const char* CameraID,unsigned int BeginTime,unsigned int EndTime)
	*  @brief ɾ��������Ϣ
	*  @param loginID[in]                �ɹ���½�ƴ洢���صľ��
	*  @param CameraID[in]               �ɹ��������ظ�����Ϣ���صľ��
	*  @param BeginTime[in]              ��ʼʱ��
	*  @param EndTime[in]                ����ʱ��
	*  @@return �ɹ�=0
	            ʧ��<0 :������
	*/
	CLOUD_API int API_AttachedDel(unsigned int loginID,const char* CameraID, unsigned int BeginTime, unsigned int EndTime);


};//extern "C"

#endif //__CLOUD_API_H__
