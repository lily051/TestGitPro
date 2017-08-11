// HIK_SL.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "HIK_SL.h"

/**	@fn	    HIK_SL_Login
*	@brief	��¼˫¼ϵͳ
*   @param [in]szIp,˫¼ϵͳIP��ַ
*   @param [in]uPort,˫¼ϵͳ�˿�
*   @param [in]szUser,˫¼ϵͳ�����û���
*   @param [in]szPwd,˫¼ϵͳ��������
*	@return	int���ɹ������û�ID(>=0), ʧ�ܷ��ش�����(<0)
*/
HIK_SL_API int HIK_SL_Login(const char* szIp,
                            int uPort,
                            const char* szUser,
                            const char* szPwd)
{
    return -1;
}

/**	@fn	    HIK_SL_Logout
*	@brief	�ǳ�˫¼ϵͳ
*   @param [in]nUid,HIK_SL_Login��¼���ص�ֵ
*	@return	��
*/
HIK_SL_API void HIK_SL_Logout(int nUid)
{
}

/**	@fn	    HIK_SL_StartRealPlay
*	@brief	ʵʱԤ��
*   @param [in]nUid,HIK_SL_Login��¼���ص�ֵ
*   @param [in]szCameraSN,��ص�Ψһ���
*   @param [in]hPlayWnd,���Ŵ��ڵľ��
*	@return	int���ɹ�����Ԥ��ID(>=0), ʧ�ܷ��ش�����(<0)
*/
HIK_SL_API int HIK_SL_StartRealPlay(int nUid,
                                    const char* szCameraSN,
                                    HWND hPlayWnd)
{
    return -1;
}

/**	@fn	    HIK_SL_StopRealPlay
*	@brief	ֹͣʵʱԤ��
*   @param [in]nUid,HIK_SL_Login�ķ���ֵ
*   @param [in]nRealPlayHandle,IK_SL_StartRealPlay�ķ���ֵ
*	@return	int���ɹ�����ID(>=0), ʧ�ܷ��ش�����(<0)
*/
HIK_SL_API int HIK_SL_StopRealPlay(int nUid,
                                   int nRealPlayHandle)
{
    return -1;
}

/**	@fn	    HIK_SL_StartPlaybackByTime
*	@brief  �ط�
*   @param [in]nUid,HIK_SL_Login�ķ���ֵ
*   @param [in]szCameraSN,��ص�Ψһ���
*   @param [in]hPlayWnd,���Ŵ��ڵľ��
*   @param [in]struBTime,¼��ʼʱ��
*   @param [in]struETime,¼�����ʱ��
*   @param [in]hPlayWnd,���Ŵ��ڵľ��
*	@return	int���ɹ�����ID(>=0), ʧ�ܷ��ش�����(<0)
*/
HIK_SL_API int HIK_SL_StartPlaybackByTime(int nUid,        
                                          const char* szCameraSN,
                                          HIK_SL_TIME struBTime,
                                          HIK_SL_TIME struETime,
                                          HWND hPlayWnd         )
{
    return -1;
}
/**	@fn	    HIK_SL_StopPlayback
*	@brief	ֹͣ�ط�
*   @param [in]hPlayWnd,HIK_SL_Login�ķ���ֵ
*   @param [in]hPlayWnd,HIK_SL_StartPlaybackByTime�ķ���ֵ
*	@return	int���ɹ�����ID(>=0), ʧ�ܷ��ش�����(<0)
*/
HIK_SL_API int HIK_SL_StopPlayback(int nUid,           
                                   int nPlayBackHandle)
{
    return -1;
}

/**	@fn	    HIK_SL_StartDownloadByTime
*	@brief	��ʱ������
*   @param [in]nUid,HIK_SL_Login�ķ���ֵ
*   @param [in]szCameraSN,��ص�Ψһ���
*   @param [in]struBTime,¼��ʼʱ��
*   @param [in]struETime,¼�����ʱ��
*   @param [in]sSavedFileName,���غ󱣴浽PC�����ļ�·������Ϊ����·���������ļ�����
*	@return	int���ɹ�����ID(>=0), ʧ�ܷ��ش�����(<0)
*/
HIK_SL_API int HIK_SL_StartDownloadByTime(int nUid,  
                                          const char* szCameraSN,
                                          HIK_SL_TIME struBTime,
                                          HIK_SL_TIME struETime,
                                          const char *sSavedFileName)
{
    return -1;
}

/**	@fn	    HIK_SL_StopDownload
*	@brief	ֹͣ����
*   @param [in]nUid,HIK_SL_Login�ķ���ֵ
*   @param [in]nDownLoadHandle,HIK_SL_StartDownloadByTime�ķ���ֵ
*	@return	int���ɹ�����ID(>=0), ʧ�ܷ��ش�����(<0)
*/
HIK_SL_API int HIK_SL_StopDownload(int nUid,          
                                   int nDownLoadHandle)
{
    return -1;
}

/**	@fn	    HIK_SL_GetCameraInfo
*	@brief	��ȡ��ص���Ϣ
*   @param [in]nUid,HIK_SL_Login��¼���ص�ֵ
*   @param [in]szBranchSN,��ص�������������
*   @param [in]pCameraInfoArray,��ص���Ϣ�����ָ��
*   @param [in]nCameraInfoArrayCount,�������µ���Ϣ����
*	@return	�ɹ����ظ���(>=0), ʧ�ܷ��ش�����(<0)
*/
HIK_SL_API int HIK_SL_GetCameraInfo(int nUid,
                                    const char * szBranchSN,
                                    HIK_SL_CAMERA_INFO ** pCameraInfoArray,
                                    int nCameraInfoArrayCount)
{
    return -1;
}
