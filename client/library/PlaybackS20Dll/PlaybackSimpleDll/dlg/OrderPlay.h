/**   @file		OrderPlay.h
 *    @note		HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief	�����ط�¼���ļ�������.
 *
 *    @author   zhoupeiqiang
 *    @date     2013/05/14
 *
 *    @note  
 *    @note 
 *    @note		��ʷ��¼��
 *    @note		V1.0.0  
 *
 *    @warning 
 */

#pragma once


class COrderPlay
{
public:
	COrderPlay(void);
	~COrderPlay(void);

private:
	// �ļ��б�.
	std::vector<PB_FILEINFO> m_vecFileInfo; 
	// ��ǰ�������.
	int m_nCurPlayIndex;

public:
	// ����ļ�.
	BOOL AddFile(const PB_FILEINFO& stFileInfo);
	// ����ļ�.
	void ClearFile();
	// �����ļ��б�.
	BOOL SetFileInfo(const std::vector<PB_FILEINFO>& vFileInfo);
	// ��ȡ�����ļ���Ϣ
	BOOL GetFileInfo(int nIndex, PB_FILEINFO& stFileInfo);
	// ��ȡ�ļ��б�.
	std::vector<PB_FILEINFO>& GetFileInfo();
	// ��ȡ��ǰ�������.
	BOOL GetCurPlayIndex(int& nPlayIndex);
	// ���õ�ǰ�������.
	void SetCurPlayIndex(int nPlayIndex);

};
