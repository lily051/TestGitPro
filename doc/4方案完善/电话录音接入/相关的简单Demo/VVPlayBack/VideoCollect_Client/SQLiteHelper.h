#ifndef GUARD_SQLITEHELPER_H
#define GUARD_SQLITEHELPER_H

/**	@file    SQLiteHelper.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   sqlite数据库操作
 *
 *	@author	 xuezhihu
 *	@date	 2013/4/18
 *
 *	@note    下面的note和warning为可选项目
 *	@note    这里填写本文件的详细功能描述和注解
 *	@note    历史记录：
 *	@note    V0.0.1  创建
 *
 *	@warning 这里填写本文件相关的警告信息
 */

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <vector>
#include <list>
#include <sqlite/sqlite3/sqlite3.h>
#include <string>
#include "Udd.h"
#include <fcDBCommon.h>
#include "HCNetSDK.h"
using namespace std;

#define MAX_REINFORCE_RECORD_ID 10000

class CSQLiteHelper
{
private:
    class CMyRecordSet {
    public:
        CMyRecordSet();
        ~CMyRecordSet();
        inline bool IsEmpty(void) const
        {
            return 0 == m_nRow;
        }
        inline bool IsEOF(void) const
        {
            return m_nCurrentRow > m_nRow;
        }
        bool MoveNext(void);
        // 具体取值函数
        HPR_INT32 GetInt32Item(const char *colName, HPR_INT32 const defaultValue = 0) const;
        HPR_INT64 GetInt64Item(const char *colName, HPR_INT64 const defaultValue = 0) const;
        const char *GetStringItem(const char *colName, char * const defaultValue = "") const;

        void SetData(char **result, int nRow, int nColumn);
        inline int GetColumnCount(void) const
        {
            return m_nColumn;
        }
        inline int GetRowCount(void) const
        {
            return m_nRow;
        }
    private:
        const char* GetItem(const char *colName) const;
        const char* GetItem(const int colIndex) const;
        const char* GetItem_(const int colIndex) const;
        CMyRecordSet(const CMyRecordSet &);
        CMyRecordSet& operator=(const CMyRecordSet &);
        char const * const *m_result;
        int m_nRow;           /* Number of result rows written here */
        int m_nColumn;        /* Number of result columns written here */
        int m_nCurrentRow;
        std::vector<std::string> m_mapIndex;
    };
public:
	CSQLiteHelper(void);
	~CSQLiteHelper(void);
private:
	sqlite3 *m_hDB;//数据库句柄
    std::_tstring m_fileName;

public:
    /**	@fn	    Connect
     *	@brief	连接数据库
     *	@param  [in]dbInfo, 数据库连接信息，其host字段解释为sqlite的数据库文件
     *	@param  [in]failNotExists, 数据库文件不存在时的处理方式，默认失败
     *	@return	bool
     */
	bool Connect(const db_conn_info_t &dbInfo, bool failNotExists = true);
    //断开数据库连接
	bool Disconnect();

    LPCTSTR GetFileName(void) const
    {
        return m_fileName.c_str();
    }

public:
    //需要关联的这条录像文件查询
    bool QueryVideoNotes(NET_DVR_FINDDATA_V30& VideoFile);
    //插入已关联录像记录
    bool InsertVideoNotes(NET_DVR_FINDDATA_V30& VideoFile);
    //删除关联录像记录
    bool DeleteVideoNotes(NET_DVR_FINDDATA_V30& VideoFile);
private:
    // 从结果集里解析出一条待传输文件记录
    void ExtractFileToTrans(fileToTrans_t &fileInfo, CMyRecordSet &recordSet);

private:
    //执行SQL命令
    bool Exec(const TCHAR *sql);
    bool Exec(const TCHAR *sql, CMyRecordSet &resultSet);

    /**	@fn	    IsAnException
     *	@brief	判断一下这是否是一个代码错误是一个异常
     *	@param  [in]errMsg, 错误消息
     *	@return	void
     */
    void IsAnException(char const * const errMsg);

    //事务相关接口
    bool BeginTransaction();
    bool CommitTransaction();
    bool RollBackTransaction();
private:
    static size_t const MAX_SQL_LEN = 4 * 1024;
};

#endif // end of GUARD_SQLITEHELPER_H
