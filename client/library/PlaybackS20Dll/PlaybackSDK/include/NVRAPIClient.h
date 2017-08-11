#ifndef NVRAPICLIENT_H
#define NVRAPICLIENT_H


#include "Header.h"



#ifndef _WIN32
#define NVR_LINUX
#endif

#ifdef __cplusplus
extern "C"
{
#endif
/************************************************************************/
/* 登陆和退出                                                                     */
/************************************************************************/
unsigned int BnNVR_Login(const char *ip,int port,const char *username,const char *passwd,MsgCallback pPlayMsgCB,NVR_DWORD pUserData);
int BnNVR_Logout(unsigned int LoginID);
/************************************************************************/
/* 开始录像和结束录像                                                                     */
/************************************************************************/
unsigned int BnNVR_StartRecord(unsigned int logInID,const char *CamerID,const char *Alias,int Recordtype,int rvID,const char *Header,unsigned int HeadLen,const char *describe,int describelen);
int BnNVR_Record(unsigned int RecordHandle,const char *data ,int datalen,unsigned int  nBeginTime,unsigned int nEndTime);
int BnNVR_StopRecord(unsigned int RecordHandle);
/************************************************************************/
/* 查询录像                                                                     */
/************************************************************************/
int BnNVR_SearchRecord(unsigned int logInID,const char *CamerID,const char*RecordTypde,unsigned int Begintime,unsigned int Endtime,SearchResult_CallBack pSeach,NVR_DWORD pSearchData);
/************************************************************************/
/* 下载录像和停止下载录像                                                                     */
/************************************************************************/
unsigned int BnNVR_GetStream(unsigned int logInID,const char *CamerID,int recordTypde,unsigned int Begintime,unsigned int Endtime,GetData_CallBack pGetData,NVR_DWORD pUserCallData);
int BnNVR_StopGetStream(unsigned int PlayHandle);

/*************************************************************************
 **新增倒序取流和停止倒序取流  fanlidong 2009-12-01
 ************************************************************************/
unsigned int BnNVR_ReverseGetStrem(unsigned int logInID,const char *CamerID,int recordTypde,unsigned int BeginTime,unsigned int EndTime,GetData_CallBack pGetData,NVR_DWORD pUserCallData);
unsigned int BnNVR_ReverseStopGetStream(unsigned int PlayHandle);

/***************************************************************************
 **新增获取录像文件头的函数 fanlidong 2009-12-01
 ***************************************************************************/
 int BnNVR_GetReadHeader(unsigned int logInID,const char *CamerID,int recordTypde,unsigned int BeginTime,unsigned int EndTime,char *RecvBuffer,unsigned int *RecvLen);
/************************************************************************/
/* 查询录像卷                                                                     */
/************************************************************************/
int BnNVR_GetRecordVolume(unsigned int logInID,GetGroup_CallBack pGroupCB,NVR_DWORD pUserData);
/************************************************************************/
/*  查询摄像头是否在录像                                                                     */
/************************************************************************/
int BnNVR_GetState(unsigned int LogInID,const char *CamerID,int *status);
/************************************************************************/
/* 得到最近的发生的错误                                                                     */
/************************************************************************/
unsigned long BnNVR_GetLastError();
 
/************************************************************************/
/* 修改录像的描述信息                                                                    */
/************************************************************************/
int BnNVR_Modify(unsigned int LoginID,const char *CamerID,unsigned int BeginTime,unsigned int EndTime,const char *desc,int desclen);

/************************************************************************
 *获取录像的大小
 ************************************************************************/
int BnNVR_GetRecordSize(unsigned int LoginID,const char *CameraID,unsigned int BeginTime,unsigned int EndTime,unsigned int *RecordSizeHigh,unsigned int *RecordSizeLow);

/*************************************************************************
 *删除录像
 *************************************************************************/
 
int BnNVR_DeleteRecord(unsigned int LoginID,const char *CameraID,unsigned int BeginTime,unsigned int EndTime,const char *RecordType);

/************************************************************************/
/* 得到lib库的版本                                                                     */
/************************************************************************/
int  BnNVR_GetLibVersion(char *version,char *time);


 /*得到服务器的版本*/

int BnNVR_GetServerVersion(unsigned int LogInID,char *version,char *time);





#ifdef __cplusplus
}
#endif
#endif // !defined(AFX_NVRAPICLIENT_H__6C5D73C6_3F1F_4AA4_BAAE_AC1A5F3BDCCA__INCLUDED_)
