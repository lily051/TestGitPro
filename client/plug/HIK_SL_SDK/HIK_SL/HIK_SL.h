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
    DWORD           dwYear;             /*年*/
    DWORD           dwMonth;            /*月*/
    DWORD           dwDay;              /*日*/
    DWORD           dwHour;             /*时*/
    DWORD           dwMinute;           /*分*/
    DWORD           dwSecond;           /*秒*/
}HIK_SL_TIME, *LPHIK_SL_TIME;

typedef struct tagHIK_SL_CameraInfo
{
    char szCameraSN[SN_LEN];//监控点唯一编号
    char szName[NAME_LEN];//监控点名称
    char szBranchSN[SN_LEN];//监控点所属网点唯一编号
    char szBranchName[NAME_LEN];//监控点所属网点名称
    char szDevSN[SN_LEN];//监控点所属设备唯一编号
    char szDevName[NAME_LEN];//监控点所属设备名称
}HIK_SL_CAMERA_INFO,*LPHIK_SL_CAMERAINFO;




/**	@fn	    HIK_SL_Login
*	@brief	登录双录系统
*   @param [in]szIp,双录系统IP地址
*   @param [in]uPort,双录系统端口
*   @param [in]szUser,双录系统访问用户名
*   @param [in]szPwd,双录系统访问密码
*	@return	int，成功返回用户ID(>=0), 失败返回错误码(<0)
*/
HIK_SL_API int HIK_SL_Login(const char* szIp,
                            int uPort,
                            const char* szUser,
                            const char* szPwd);

/**	@fn	    HIK_SL_Logout
*	@brief	登出双录系统
*   @param [in]nUid,HIK_SL_Login登录返回的值
*	@return	无
*/
HIK_SL_API void HIK_SL_Logout(int nUid);

/**	@fn	    HIK_SL_StartRealPlay
*	@brief	实时预览
*   @param [in]nUid,HIK_SL_Login登录返回的值
*   @param [in]szCameraSN,监控点唯一编号
*   @param [in]hPlayWnd,播放窗口的句柄
*	@return	int，成功返回预览ID(>=0), 失败返回错误码(<0)
*/
HIK_SL_API int HIK_SL_StartRealPlay(int nUid,
                                    const char* szCameraSN,
                                    HWND hPlayWnd);

/**	@fn	    HIK_SL_StopRealPlay
*	@brief	停止实时预览
*   @param [in]nUid,HIK_SL_Login的返回值
*   @param [in]nRealPlayHandle,IK_SL_StartRealPlay的返回值
*	@return	int，成功返回ID(>=0), 失败返回错误码(<0)
*/
HIK_SL_API int HIK_SL_StopRealPlay(int nUid,
                                   int nRealPlayHandle);

/**	@fn	    HIK_SL_StartPlaybackByTime
*	@brief  回放
*   @param [in]nUid,HIK_SL_Login的返回值
*   @param [in]szCameraSN,监控点唯一编号
*   @param [in]hPlayWnd,播放窗口的句柄
*   @param [in]struBTime,录像开始时间
*   @param [in]struETime,录像结束时间
*   @param [in]hPlayWnd,播放窗口的句柄
*	@return	int，成功返回ID(>=0), 失败返回错误码(<0)
*/
HIK_SL_API int HIK_SL_StartPlaybackByTime(int nUid,        
                                    const char* szCameraSN,
                                    HIK_SL_TIME struBTime,
                                    HIK_SL_TIME struETime,
                                    HWND hPlayWnd         );
/**	@fn	    HIK_SL_StopPlayback
*	@brief	停止回放
*   @param [in]hPlayWnd,HIK_SL_Login的返回值
*   @param [in]hPlayWnd,HIK_SL_StartPlaybackByTime的返回值
*	@return	int，成功返回ID(>=0), 失败返回错误码(<0)
*/
HIK_SL_API int HIK_SL_StopPlayback(int nUid,           
                                    int nPlayBackHandle);

/**	@fn	    HIK_SL_StartDownloadByTime
*	@brief	按时间下载
*   @param [in]nUid,HIK_SL_Login的返回值
*   @param [in]szCameraSN,监控点唯一编号
*   @param [in]struBTime,录像开始时间
*   @param [in]struETime,录像结束时间
*   @param [in]sSavedFileName,下载后保存到PC机的文件路径，需为绝对路径（包括文件名）
*	@return	int，成功返回ID(>=0), 失败返回错误码(<0)
*/
HIK_SL_API int HIK_SL_StartDownloadByTime(int nUid,  
                               const char* szCameraSN,
                               HIK_SL_TIME struBTime,
                               HIK_SL_TIME struETime,
                               const char *sSavedFileName);

/**	@fn	    HIK_SL_StopDownload
*	@brief	停止下载
*   @param [in]nUid,HIK_SL_Login的返回值
*   @param [in]nDownLoadHandle,HIK_SL_StartDownloadByTime的返回值
*	@return	int，成功返回ID(>=0), 失败返回错误码(<0)
*/
HIK_SL_API int HIK_SL_StopDownload(int nUid,          
                                   int nDownLoadHandle);

/**	@fn	    HIK_SL_GetCameraInfo
*	@brief	获取监控点信息
*   @param [in]nUid,HIK_SL_Login登录返回的值
*   @param [in]szBranchSN,监控点所属的网点编号
*   @param [in]pCameraInfoArray,监控点信息数组的指针
*   @param [in]nCameraInfoArrayCount,能容纳下的信息个数
*	@return	成功返回个数(>=0), 失败返回错误码(<0)
*/
HIK_SL_API int HIK_SL_GetCameraInfo(int nUid,
                                    const char * szBranchSN,
                                    HIK_SL_CAMERA_INFO ** pCameraInfoArray,
                                    int nCameraInfoArrayCount);
