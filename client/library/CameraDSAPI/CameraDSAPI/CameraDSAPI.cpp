/**	@file    CameraDSAPI.cpp
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   获取usb摄像头海康ps流
 *
 *	@author	 chenxiaoyan3
 *	@date	 2015/07/23
 *
 *	@note    下面的note和warning为可选项目
 *	@note    这里填写本文件的详细功能描述和注解
 *	@note    历史记录：
 *	@note    V0.0.1  创建
 *
 *	@warning 这里填写本文件相关的警告信息
 */

#include "CameraDSAPI.h"
#include "CameraDS.h"
#include "hlogConfig.h"

/**	@fn	    CameraDSAPI_Init
 *	@brief	初始化函数，模块需要的参数应该由此函数从配置文件读入
 *          上层应用不需要处理
 *	@param  [in] 无
 *	@param  [out] 无
 *	@return	int，大于等于零 表示没有错误，小于零表示出现了错误
  */
int WINAPI CameraDSAPI_Init(void **pCameraDS)
{
    //return CCameraDS::Instance()->Init();
    *pCameraDS = new CCameraDS;
    if (NULL == *pCameraDS)
    {
        TPLOG_ERROR("new CCameraDSa failed");
        return -1;
    }

    return ((CCameraDS*)(*pCameraDS))->Init();
}

/**	@fn	    CameraDSAPI_Fini
 *	@brief	反初始化函数，负责资源释放
 *	@param  [in] 无
 *	@param  [out] 无
 *	@return	int，大于等于零 表示没有错误，小于零表示出现了错误
*/
int WINAPI CameraDSAPI_Fini(void **pCameraDS)
{
    //long lRet = CCameraDS::Instance()->Fini();
    //CCameraDS* pInstance = ((CCameraDS*)(*pCameraDS));
    ((CCameraDS*)(*pCameraDS))->Fini();
    delete (*pCameraDS);
    (*pCameraDS) = NULL;

    return 0;
}

/**	@fn	    CameraDSAPI_InitCamera
 *	@brief	获取usb摄像头个数以及名字
 *	@param  [out] lstCameraInfo 摄像头信息列表
 *	@param  [out] lstMicInfo 麦克风信息列表
 *	@return	int，大于等于零 表示没有错误，小于零表示出现了错误
*/
int WINAPI CameraDSAPI_GetAllCameraInfo(std::list<CAMERA_INFO_ST> &lstCameraInfo, std::list<MICOR_INFO_ST> &lstMicInfo)
{
    return CCameraDS::GetAllCameraInfo(lstCameraInfo, lstMicInfo);
}

/**	@fn	    CameraDSAPI_InitCamera
 *	@brief	初始化摄像头函数
 *	@param  [in] uiCamIndex 摄像头索引号
 *	@param  [in] uiMicIndex 麦克风索引号
 *	@param  [in] strMicName 麦克风名字
 *	@param  [in] stProperty 图像属性  当前支持分辨率只有1920*1080 1280*720 640*480 320*240
 *	@param  [out] 无
 *	@return	int，大于等于零 表示没有错误，小于零表示出现了错误
*/
int WINAPI CameraDSAPI_InitCamera(void *pCameraDS,
                                  unsigned int uiCamIndex,
                                  unsigned int uiMicIndex,
                                  const std::string &strMicName,
                                  const PICTURE_PROPERTY_ST &stProperty
                                  )
{
    return ((CCameraDS*)pCameraDS)->OpenCamera(uiCamIndex, uiMicIndex, strMicName, stProperty);
    //return CCameraDS::Instance()->OpenCamera(uiCamIndex, uiMicIndex, strMicName, stProperty);
}

/**	@fn	    CameraDSAPI_FiniCamera
 *	@brief	反初始化摄像头函数
 *	@param  [in] 无
 *	@param  [out] 无
 *	@return	int，大于等于零 表示没有错误，小于零表示出现了错误
*/
int WINAPI CameraDSAPI_FiniCamera(void *pCameraDS)
{
    //CCameraDS::Instance()->CloseCamera();
    ((CCameraDS*)pCameraDS)->CloseCamera();
    return 0;
}

/**	@fn	    CameraDSAPI_Start
 *	@brief	开始取流函数
 *	@param  [in] 无
 *	@param  [out] 无
 *	@return	int，大于等于零 表示没有错误，小于零表示出现了错误
*/
int WINAPI CameraDSAPI_Start(void *pCameraDS)
{
    //return CCameraDS::Instance()->Run();
    return ((CCameraDS*)pCameraDS)->Run();
}

/**	@fn	    CameraDSAPI_Pause
 *	@brief	暂停取流函数
 *	@param  [in] 无
 *	@param  [out] 无
 *	@return	int，大于等于零 表示没有错误，小于零表示出现了错误
*/
int WINAPI CameraDSAPI_Pause(void *pCameraDS)
{
    //return CCameraDS::Instance()->Pause();
    return ((CCameraDS*)pCameraDS)->Pause();
}

/**	@fn	    CameraDSAPI_Stop
 *	@brief	停止取流函数
 *	@param  [in] 无
 *	@param  [out] 无
 *	@return	int，大于等于零 表示没有错误，小于零表示出现了错误
*/
int WINAPI CameraDSAPI_Stop(void *pCameraDS)
{
    //return CCameraDS::Instance()->Stop();
    return ((CCameraDS*)pCameraDS)->Stop();
}

/**	@fn	    CameraDSAPI_GetPictureInfo
 *	@brief	获取输出图像属性
 *	@param  [in] stProperty 图像属性
 *	@param  [out] stProperty 图像属性
 *	@return	int，大于等于零 表示没有错误，小于零表示出现了错误
*/
int WINAPI CameraDSAPI_GetPictureInfo(void *pCameraDS, PICTURE_PROPERTY_ST &stProperty)
{
    //return CCameraDS::Instance()->GetPictureInfo(stProperty);
    return ((CCameraDS*)pCameraDS)->GetPictureInfo(stProperty);
}

/**	@fn	    CameraDSAPI_SetDataCallback
 *	@brief	数据回调函数
 *	@param  [in] const pCameraDSDataBack datafun, 数据回调函数
 *	@param  [in] pUser, 用户对象指针
 *	@param  [out] 无
 *	@return	int，大于等于零 表示没有错误，小于零表示出现了错误
*/
int WINAPI CameraDSAPI_SetDataCallback(void *pCameraDS, const pCameraDSDataBack datafun, void *pUser)
{
    //return CCameraDS::Instance()->SetDataCallback(datafun, pUser);
    return ((CCameraDS*)pCameraDS)->SetDataCallback(datafun, pUser);
}
//void CALLBACK PsDataCallBack(OUTPUTDATA_INFO* pDataInfo, void* pUser)
//{
//    static FILE *jpgFile = fopen("d:\\PS", "wb+");
//    fwrite(pDataInfo->pData, pDataInfo->dwDataLen, 1, jpgFile);
//    //fclose(jpgFile);
//}
//void main()
//{
//    int iRet = 0;
//    void *pCameraDS1 = NULL;
//    iRet = CameraDSAPI_Init(&pCameraDS1);
//    std::list<CAMERA_INFO_ST> lstCameraInfo;
//    std::list<MICOR_INFO_ST> lstMicInfo;
//    CameraDSAPI_GetAllCameraInfo(lstCameraInfo, lstMicInfo);
//    PICTURE_PROPERTY_ST stProperty;
//    stProperty.uiWidth = 1280;
//    stProperty.uiHeight = 720;
//    iRet = CameraDSAPI_InitCamera(pCameraDS1, 0,0,"test",stProperty);
//    if (iRet < 0)
//    {
//        return;
//    }
//    iRet = CameraDSAPI_SetDataCallback(pCameraDS1, PsDataCallBack,NULL);
//    if (iRet < 0)
//    {
//        return;
//    }
//    iRet = CameraDSAPI_Start(pCameraDS1);
//    if (iRet < 0)
//    {
//        return;
//    }
//    while (1)
//    {
//        Sleep(1000);
//    }
//    CameraDSAPI_Stop(pCameraDS1);
//    CameraDSAPI_Fini(&pCameraDS1);
//}
