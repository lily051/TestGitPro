#include "StdAfx.h"
#include "OrderPlay.h"
//lint -library
COrderPlay::COrderPlay(void)
: m_nCurPlayIndex(0)
{
}

COrderPlay::~COrderPlay(void)
{
}

/**   @fn          AddFile
 *    @brief   	   添加文件.
 *    @param[in]   stFileInfo:文件信息.
 *    @param[in]   
 *    @return      TRUE-成功,FALSE-失败.
 */
BOOL COrderPlay::AddFile(const PB_FILEINFO& stFileInfo)
{
	m_vecFileInfo.push_back(stFileInfo);

	return TRUE;
}

/**   @fn          ClearFile
 *    @brief   	   清空文件.
 *    @param[in]   
 *    @param[in]   
 *    @return      
 */
void COrderPlay::ClearFile()
{
	m_nCurPlayIndex = 0;
	m_vecFileInfo.clear();
}

/**   @fn          SetFileInfo
 *    @brief   	   设置文件列表.
 *    @param[in]   vFileInfo:文件列表.
 *    @param[in]   
 *    @return      TRUE-成功,FALSE-失败.
 */
BOOL COrderPlay::SetFileInfo(const std::vector<PB_FILEINFO>& vFileInfo)
{
	m_vecFileInfo.clear();

	for (std::vector<PB_FILEINFO>::const_iterator ite = vFileInfo.begin();
		ite != vFileInfo.end();
		ite++)
	{
		m_vecFileInfo.push_back(*ite);
	}

	return TRUE;
}

/**   @fn          GetFileInfo
 *    @brief   	   获取单个文件信息.
 *    @param[in]   nIndex:文件索引.
 *    @param[out]  stFileInfo:文件信息引用. 
 *    @return      TRUE-成功,FALSE-失败.
 */
BOOL COrderPlay::GetFileInfo(int nIndex, PB_FILEINFO& stFileInfo)
{
	if (nIndex < 0  ||  nIndex >= m_vecFileInfo.size())
	{
		return FALSE;
	}
	stFileInfo = m_vecFileInfo[nIndex];

	return TRUE;
}

/**   @fn          GetFileInfo
 *    @brief   	   获取文件列表引用.
 *    @param[in]   
 *    @param[in]   
 *    @return      文件列表的引用.
 */
std::vector<PB_FILEINFO>& COrderPlay::GetFileInfo()
{
	return m_vecFileInfo;
}

/**   @fn          GetCurPlayIndex
 *    @brief   	   获取当前播放序号.
 *    @param[in]   nPlayIndex:当前播放序号.
 *    @param[in]   
 *    @return      TRUE-成功,FALSE-失败.
 */
BOOL COrderPlay::GetCurPlayIndex(int& nPlayIndex)
{
	if (m_vecFileInfo.empty())
	{
		return FALSE;
	}

	nPlayIndex = m_nCurPlayIndex;
	return TRUE;
}

/**   @fn          SetCurPlayIndex
 *    @brief   	   设置当前播放序号.
 *    @param[in]   nPlayIndex:播放序号.
 *    @param[in]   
 *    @return      
 */
void COrderPlay::SetCurPlayIndex(int nPlayIndex)
{
	m_nCurPlayIndex = nPlayIndex;
}