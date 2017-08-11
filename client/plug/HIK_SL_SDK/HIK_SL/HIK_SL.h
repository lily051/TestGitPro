//HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
#ifdef HIK_SL_EXPORTS
#define HIK_SL_API EXTERN_C __declspec(dllexport)
#else
#define HIK_SL_API EXTERN_C __declspec(dllimport)
#endif

#define NAME_LEN 64
#define SN_LEN 64

typedef struct tagHIK_SL_Time
{
    DWORD           dwYear;             /*��*/
    DWORD           dwMonth;            /*��*/
    DWORD           dwDay;              /*��*/
    DWORD           dwHour;             /*ʱ*/
    DWORD           dwMinute;           /*��*/
    DWORD           dwSecond;           /*��*/
}HIK_SL_TIME, *LPHIK_SL_TIME;

typedef struct tagHIK_SL_CameraInfo
{
    char szCameraSN[SN_LEN];//��ص�Ψһ���
    char szName[NAME_LEN];//��ص�����
    char szBranchSN[SN_LEN];//��ص���������Ψһ���
    char szBranchName[NAME_LEN];//��ص�������������
    char szDevSN[SN_LEN];//��ص������豸Ψһ���
    char szDevName[NAME_LEN];//��ص������豸����
}HIK_SL_CAMERA_INFO,*LPHIK_SL_CAMERAINFO;




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
                            const char* szPwd);

/**	@fn	    HIK_SL_Logout
*	@brief	�ǳ�˫¼ϵͳ
*   @param [in]nUid,HIK_SL_Login��¼���ص�ֵ
*	@return	��
*/
HIK_SL_API void HIK_SL_Logout(int nUid);

/**	@fn	    HIK_SL_StartRealPlay
*	@brief	ʵʱԤ��
*   @param [in]nUid,HIK_SL_Login��¼���ص�ֵ
*   @param [in]szCameraSN,��ص�Ψһ���
*   @param [in]hPlayWnd,���Ŵ��ڵľ��
*	@return	int���ɹ�����Ԥ��ID(>=0), ʧ�ܷ��ش�����(<0)
*/
HIK_SL_API int HIK_SL_StartRealPlay(int nUid,
                                    const char* szCameraSN,
                                    HWND hPlayWnd);

/**	@fn	    HIK_SL_StopRealPlay
*	@brief	ֹͣʵʱԤ��
*   @param [in]nUid,HIK_SL_Login�ķ���ֵ
*   @param [in]nRealPlayHandle,IK_SL_StartRealPlay�ķ���ֵ
*	@return	int���ɹ�����ID(>=0), ʧ�ܷ��ش�����(<0)
*/
HIK_SL_API int HIK_SL_StopRealPlay(int nUid,
                                   int nRealPlayHandle);

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
                                    HWND hPlayWnd         );
/**	@fn	    HIK_SL_StopPlayback
*	@brief	ֹͣ�ط�
*   @param [in]hPlayWnd,HIK_SL_Login�ķ���ֵ
*   @param [in]hPlayWnd,HIK_SL_StartPlaybackByTime�ķ���ֵ
*	@return	int���ɹ�����ID(>=0), ʧ�ܷ��ش�����(<0)
*/
HIK_SL_API int HIK_SL_StopPlayback(int nUid,           
                                    int nPlayBackHandle);

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
                               const char *sSavedFileName);

/**	@fn	    HIK_SL_StopDownload
*	@brief	ֹͣ����
*   @param [in]nUid,HIK_SL_Login�ķ���ֵ
*   @param [in]nDownLoadHandle,HIK_SL_StartDownloadByTime�ķ���ֵ
*	@return	int���ɹ�����ID(>=0), ʧ�ܷ��ش�����(<0)
*/
HIK_SL_API int HIK_SL_StopDownload(int nUid,          
                                   int nDownLoadHandle);

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
                                    int nCameraInfoArrayCount);
