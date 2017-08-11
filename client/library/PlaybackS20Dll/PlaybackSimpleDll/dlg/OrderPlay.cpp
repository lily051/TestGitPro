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
 *    @brief   	   ����ļ�.
 *    @param[in]   stFileInfo:�ļ���Ϣ.
 *    @param[in]   
 *    @return      TRUE-�ɹ�,FALSE-ʧ��.
 */
BOOL COrderPlay::AddFile(const PB_FILEINFO& stFileInfo)
{
	m_vecFileInfo.push_back(stFileInfo);

	return TRUE;
}

/**   @fn          ClearFile
 *    @brief   	   ����ļ�.
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
 *    @brief   	   �����ļ��б�.
 *    @param[in]   vFileInfo:�ļ��б�.
 *    @param[in]   
 *    @return      TRUE-�ɹ�,FALSE-ʧ��.
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
 *    @brief   	   ��ȡ�����ļ���Ϣ.
 *    @param[in]   nIndex:�ļ�����.
 *    @param[out]  stFileInfo:�ļ���Ϣ����. 
 *    @return      TRUE-�ɹ�,FALSE-ʧ��.
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
 *    @brief   	   ��ȡ�ļ��б�����.
 *    @param[in]   
 *    @param[in]   
 *    @return      �ļ��б������.
 */
std::vector<PB_FILEINFO>& COrderPlay::GetFileInfo()
{
	return m_vecFileInfo;
}

/**   @fn          GetCurPlayIndex
 *    @brief   	   ��ȡ��ǰ�������.
 *    @param[in]   nPlayIndex:��ǰ�������.
 *    @param[in]   
 *    @return      TRUE-�ɹ�,FALSE-ʧ��.
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
 *    @brief   	   ���õ�ǰ�������.
 *    @param[in]   nPlayIndex:�������.
 *    @param[in]   
 *    @return      
 */
void COrderPlay::SetCurPlayIndex(int nPlayIndex)
{
	m_nCurPlayIndex = nPlayIndex;
}