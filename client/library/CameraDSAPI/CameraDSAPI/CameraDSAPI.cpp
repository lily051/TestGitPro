/**	@file    CameraDSAPI.cpp
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   ��ȡusb����ͷ����ps��
 *
 *	@author	 chenxiaoyan3
 *	@date	 2015/07/23
 *
 *	@note    �����note��warningΪ��ѡ��Ŀ
 *	@note    ������д���ļ�����ϸ����������ע��
 *	@note    ��ʷ��¼��
 *	@note    V0.0.1  ����
 *
 *	@warning ������д���ļ���صľ�����Ϣ
 */

#include "CameraDSAPI.h"
#include "CameraDS.h"
#include "hlogConfig.h"

/**	@fn	    CameraDSAPI_Init
 *	@brief	��ʼ��������ģ����Ҫ�Ĳ���Ӧ���ɴ˺����������ļ�����
 *          �ϲ�Ӧ�ò���Ҫ����
 *	@param  [in] ��
 *	@param  [out] ��
 *	@return	int�����ڵ����� ��ʾû�д���С�����ʾ�����˴���
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
 *	@brief	����ʼ��������������Դ�ͷ�
 *	@param  [in] ��
 *	@param  [out] ��
 *	@return	int�����ڵ����� ��ʾû�д���С�����ʾ�����˴���
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
 *	@brief	��ȡusb����ͷ�����Լ�����
 *	@param  [out] lstCameraInfo ����ͷ��Ϣ�б�
 *	@param  [out] lstMicInfo ��˷���Ϣ�б�
 *	@return	int�����ڵ����� ��ʾû�д���С�����ʾ�����˴���
*/
int WINAPI CameraDSAPI_GetAllCameraInfo(std::list<CAMERA_INFO_ST> &lstCameraInfo, std::list<MICOR_INFO_ST> &lstMicInfo)
{
    return CCameraDS::GetAllCameraInfo(lstCameraInfo, lstMicInfo);
}

/**	@fn	    CameraDSAPI_InitCamera
 *	@brief	��ʼ������ͷ����
 *	@param  [in] uiCamIndex ����ͷ������
 *	@param  [in] uiMicIndex ��˷�������
 *	@param  [in] strMicName ��˷�����
 *	@param  [in] stProperty ͼ������  ��ǰ֧�ֱַ���ֻ��1920*1080 1280*720 640*480 320*240
 *	@param  [out] ��
 *	@return	int�����ڵ����� ��ʾû�д���С�����ʾ�����˴���
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
 *	@brief	����ʼ������ͷ����
 *	@param  [in] ��
 *	@param  [out] ��
 *	@return	int�����ڵ����� ��ʾû�д���С�����ʾ�����˴���
*/
int WINAPI CameraDSAPI_FiniCamera(void *pCameraDS)
{
    //CCameraDS::Instance()->CloseCamera();
    ((CCameraDS*)pCameraDS)->CloseCamera();
    return 0;
}

/**	@fn	    CameraDSAPI_Start
 *	@brief	��ʼȡ������
 *	@param  [in] ��
 *	@param  [out] ��
 *	@return	int�����ڵ����� ��ʾû�д���С�����ʾ�����˴���
*/
int WINAPI CameraDSAPI_Start(void *pCameraDS)
{
    //return CCameraDS::Instance()->Run();
    return ((CCameraDS*)pCameraDS)->Run();
}

/**	@fn	    CameraDSAPI_Pause
 *	@brief	��ͣȡ������
 *	@param  [in] ��
 *	@param  [out] ��
 *	@return	int�����ڵ����� ��ʾû�д���С�����ʾ�����˴���
*/
int WINAPI CameraDSAPI_Pause(void *pCameraDS)
{
    //return CCameraDS::Instance()->Pause();
    return ((CCameraDS*)pCameraDS)->Pause();
}

/**	@fn	    CameraDSAPI_Stop
 *	@brief	ֹͣȡ������
 *	@param  [in] ��
 *	@param  [out] ��
 *	@return	int�����ڵ����� ��ʾû�д���С�����ʾ�����˴���
*/
int WINAPI CameraDSAPI_Stop(void *pCameraDS)
{
    //return CCameraDS::Instance()->Stop();
    return ((CCameraDS*)pCameraDS)->Stop();
}

/**	@fn	    CameraDSAPI_GetPictureInfo
 *	@brief	��ȡ���ͼ������
 *	@param  [in] stProperty ͼ������
 *	@param  [out] stProperty ͼ������
 *	@return	int�����ڵ����� ��ʾû�д���С�����ʾ�����˴���
*/
int WINAPI CameraDSAPI_GetPictureInfo(void *pCameraDS, PICTURE_PROPERTY_ST &stProperty)
{
    //return CCameraDS::Instance()->GetPictureInfo(stProperty);
    return ((CCameraDS*)pCameraDS)->GetPictureInfo(stProperty);
}

/**	@fn	    CameraDSAPI_SetDataCallback
 *	@brief	���ݻص�����
 *	@param  [in] const pCameraDSDataBack datafun, ���ݻص�����
 *	@param  [in] pUser, �û�����ָ��
 *	@param  [out] ��
 *	@return	int�����ڵ����� ��ʾû�д���С�����ʾ�����˴���
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
