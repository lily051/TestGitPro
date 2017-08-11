#include "stdafx.h"
#include "PlaybackBase.h"


CPlaybackBase::CPlaybackBase()
{
	m_lChannel = 0;
	m_lPlaybackID = -1;
	m_lDownloadID = -1;
}

CPlaybackBase::~CPlaybackBase()
{

}

int CPlaybackBase::ChangeFileType(int nFileType)
{
	return nFileType;
}

void CPlaybackBase::PBTimeToCTime(const PB_TIME& stPBTime, CTime& odtTime)
{
	CTime ctTemp(
		stPBTime.dwYear,
		stPBTime.dwMonth,
		stPBTime.dwDay,
		stPBTime.dwHour,
		stPBTime.dwMinute,
		stPBTime.dwSecond);

	odtTime = ctTemp;
}

/**  @fn          IsCardNumAllowed
*    @brief   	  是否支持按卡号查询.
*    @param[in]   
*    @param[in]   
*    @return      TRUE-支持,FALSE-不支持.
*/
BOOL CPlaybackBase::IsCardNumAllowed()
{
	return FALSE;
}

/**  @fn          IsEventAllowed
*    @brief   	  是否支持按事件查询.
*    @param[in]   
*    @param[in]   
*    @return      TRUE-支持,FALSE-不支持.
*/
BOOL CPlaybackBase::IsEventAllowed()
{
    return FALSE;
}

/**  @fn          GetStreamData
*    @brief   	  获取回放流数据.
*    @param[in]   
*    @param[in]   
*    @return      TRUE-支持,FALSE-不支持.
*/
BOOL CPlaybackBase::StartGetStreamData(PbStreamDataCB pStreamData, DWORD dwUser)
{
	return FALSE;
}

/**   @fn          StopGetStreamData
*    @brief   	   停止获取回放流数据.
*    @param[in]   hPlayback:回放句柄.
*    @return     
*/
void CPlaybackBase::StopGetStreamData()
{

}

/**   @fn          GetCompressCfgAbility
*    @brief   	   获取压缩参数能力集.
*    @param[out]   stCompresscfgAbility:能力集结构体.
*    @return     
*/
BOOL CPlaybackBase::GetCompressCfgAbility( char* pOutbuf )
{
	return FALSE;
}

/**  @fn          SmartSearch
*    @brief   	  智能检索.
*    @param[in]   
*    @param[in]   
*    @return      TRUE-支持,FALSE-不支持.
*/
BOOL CPlaybackBase::SmartSearch( SMARTSEARCH_PARAM& stSmartSearchParam, LPB_FILEINFO& lpFileList, int& nFileCount )
{
	return FALSE;
}

/**   @fn          GetVideoFormat
*    @brief   	   获取视频制式.
*    @param[out]   nVideoFormat:视频制式.
*    @return     
*/
BOOL CPlaybackBase::GetVideoFormat( int& nVideoFormat )
{
	return FALSE;
}

/**   @fn         EnableZoomOut
*    @brief   	  开启回放解码放大.
*    @param[in]   rcShow: 显示区域
*    @param[in]   nRegionNum: 显示区域序号，为0时忽略hWnd，bEnable参数
*    @param[in]   hWnd: 目标窗口
*    @param[in]   bEnable: 打开关闭显示区域.
*    @return     
*/
BOOL CPlaybackBase::EnableZoomOut( RECT rcShow, DWORD nRegionNum /*= 0*/, HWND hWnd /*= NULL*/, BOOL bEnable /*= FALSE*/ )
{
	return FALSE;
}

BOOL CPlaybackBase::GetPictureSize( LONG* lWidth, LONG* lHeight )
{
	return FALSE;
}

BOOL CPlaybackBase::GetKeyFrameData(const PB_TIME& stFrameTime, PbKeyFrameDataCB pFunGetKeyFrame, DWORD dwUser )
{
    return FALSE;
}

BOOL CPlaybackBase::StopGetKeyFrameData()
{
    return FALSE;
}
