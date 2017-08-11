#ifndef GUARD_FCMYSQL_H
#define GUARD_FCMYSQL_H

/**	@file    fcMySQL.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   MYSQL CAPI 封装
 *
 *	@author	 xuezhihu
 *	@date	 2013/4/11
 *
 *	@note    下面的note和warning为可选项目
 *	@note    这里填写本文件的详细功能描述和注解
 *	@note    历史记录：
 *	@note    V0.0.1  创建
 *
 *	@warning 这里填写本文件相关的警告信息
 */

#include <vector>
#include <mysql/mysql_5_1_45/include/mysql.h>
#include "fcTchar.h"
#include "fcDBCommon.h"
//#include <HPR_Guard.h>

bool fcDB_MySQL_Init(void);
bool fcDB_MySQL_Fini(void);

class CMySQLRecordSet
{
public:
    CMySQLRecordSet();
    ~CMySQLRecordSet();
    inline bool IsEmpty(void) const
    {
        return 0 == m_nRow;
    }
    inline bool IsEOF(void) const
    {
        return m_nCurrentRow > m_nRow
            || NULL == m_row;
    }
    bool MoveNext(void);
    // 具体取值函数
    INT32 GetInt32Item(const char *colName, INT32 const defaultValue = 0) const;
    INT64 GetInt64Item(const char *colName, INT64 const defaultValue = 0) const;
    const char *GetStringItem(const char *colName, char * const defaultValue = "") const;

    bool SetData(void);
    inline int GetColumnCount(void) const
    {
        return m_nColumn;
    }
    inline int GetRowCount(void) const
    {
        return m_nRow;
    }
    inline MYSQL* &ConnPtrRef(void)
    {
        return m_pConn;
    }
public:
    MYSQL *m_pConn;
private:
    const char* GetItem(const char *colName) const;
    const char* GetItem(const int colIndex) const;
    const char* GetItem_(const int colIndex) const;
    CMySQLRecordSet(const CMySQLRecordSet &);
    CMySQLRecordSet& operator=(const CMySQLRecordSet &);
    MYSQL_RES *m_pResult;
    MYSQL_ROW m_row;
    int m_nRow;           /* Number of result rows written here */
    int m_nColumn;        /* Number of result columns written here */
    int m_nCurrentRow;
    std::vector<std::string> m_mapIndex;
};

// 目前没考虑多线程访问的事情，使用时需要注意
// 增加锁
class CMySQLConnection
{
public:
    CMySQLConnection();
    virtual ~CMySQLConnection();
    //连接数据库
	bool Connect(const db_conn_info_t &dbInfo, bool bTimeout = true);

    //断开数据库连接
	bool Disconnect();

    //执行SQL命令
    bool Exec(const TCHAR *sql);
    bool Exec(const TCHAR *sql, CMySQLRecordSet &resultSet);

	//执行SQL命令
	bool ExecSQL(const TCHAR *sql);
	bool ExecSQL(const TCHAR *sql, CMySQLRecordSet &resultSet);

    /**	@fn	    IsAnException
     *	@brief	判断一下这是否是一个代码错误是一个异常
     *	@param  [in]errMsg, 错误消息
     *	@return	void
     */
    void IsAnException(const char * const errMsg);

    //事务相关接口
    bool BeginTransaction();
    bool CommitTransaction();
    bool RollBackTransaction();

protected:
	MYSQL *m_pConn;

	////连接对象m_pConn的锁
	//HPR_Mutex m_mutexConn;

private:
    db_conn_info_t m_dbInfo;
    bool m_bConnected;
};

#endif // end of GUARD_FCMYSQL_H

