/**   @file		OrderPlay.h
 *    @note		HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *    @brief	连续回放录像文件控制类.
 *
 *    @author   zhoupeiqiang
 *    @date     2013/05/14
 *
 *    @note  
 *    @note 
 *    @note		历史记录：
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
	// 文件列表.
	std::vector<PB_FILEINFO> m_vecFileInfo; 
	// 当前播放序号.
	int m_nCurPlayIndex;

public:
	// 添加文件.
	BOOL AddFile(const PB_FILEINFO& stFileInfo);
	// 清空文件.
	void ClearFile();
	// 设置文件列表.
	BOOL SetFileInfo(const std::vector<PB_FILEINFO>& vFileInfo);
	// 获取单个文件信息
	BOOL GetFileInfo(int nIndex, PB_FILEINFO& stFileInfo);
	// 获取文件列表.
	std::vector<PB_FILEINFO>& GetFileInfo();
	// 获取当前播放序号.
	BOOL GetCurPlayIndex(int& nPlayIndex);
	// 设置当前播放序号.
	void SetCurPlayIndex(int nPlayIndex);

};
