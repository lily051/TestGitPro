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
*    @brief   	  �Ƿ�֧�ְ����Ų�ѯ.
*    @param[in]   
*    @param[in]   
*    @return      TRUE-֧��,FALSE-��֧��.
*/
BOOL CPlaybackBase::IsCardNumAllowed()
{
	return FALSE;
}

/**  @fn          IsEventAllowed
*    @brief   	  �Ƿ�֧�ְ��¼���ѯ.
*    @param[in]   
*    @param[in]   
*    @return      TRUE-֧��,FALSE-��֧��.
*/
BOOL CPlaybackBase::IsEventAllowed()
{
    return FALSE;
}

/**  @fn          GetStreamData
*    @brief   	  ��ȡ�ط�������.
*    @param[in]   
*    @param[in]   
*    @return      TRUE-֧��,FALSE-��֧��.
*/
BOOL CPlaybackBase::StartGetStreamData(PbStreamDataCB pStreamData, DWORD dwUser)
{
	return FALSE;
}

/**   @fn          StopGetStreamData
*    @brief   	   ֹͣ��ȡ�ط�������.
*    @param[in]   hPlayback:�طž��.
*    @return     
*/
void CPlaybackBase::StopGetStreamData()
{

}

/**   @fn          GetCompressCfgAbility
*    @brief   	   ��ȡѹ������������.
*    @param[out]   stCompresscfgAbility:�������ṹ��.
*    @return     
*/
BOOL CPlaybackBase::GetCompressCfgAbility( char* pOutbuf )
{
	return FALSE;
}

/**  @fn          SmartSearch
*    @brief   	  ���ܼ���.
*    @param[in]   
*    @param[in]   
*    @return      TRUE-֧��,FALSE-��֧��.
*/
BOOL CPlaybackBase::SmartSearch( SMARTSEARCH_PARAM& stSmartSearchParam, LPB_FILEINFO& lpFileList, int& nFileCount )
{
	return FALSE;
}

/**   @fn          GetVideoFormat
*    @brief   	   ��ȡ��Ƶ��ʽ.
*    @param[out]   nVideoFormat:��Ƶ��ʽ.
*    @return     
*/
BOOL CPlaybackBase::GetVideoFormat( int& nVideoFormat )
{
	return FALSE;
}

/**   @fn         EnableZoomOut
*    @brief   	  �����طŽ���Ŵ�.
*    @param[in]   rcShow: ��ʾ����
*    @param[in]   nRegionNum: ��ʾ������ţ�Ϊ0ʱ����hWnd��bEnable����
*    @param[in]   hWnd: Ŀ�괰��
*    @param[in]   bEnable: �򿪹ر���ʾ����.
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
