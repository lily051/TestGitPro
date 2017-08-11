#ifndef GUARD_FCMYSQL_H
#define GUARD_FCMYSQL_H

/**	@file    fcMySQL.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   MYSQL CAPI ��װ
 *
 *	@author	 xuezhihu
 *	@date	 2013/4/11
 *
 *	@note    �����note��warningΪ��ѡ��Ŀ
 *	@note    ������д���ļ�����ϸ����������ע��
 *	@note    ��ʷ��¼��
 *	@note    V0.0.1  ����
 *
 *	@warning ������д���ļ���صľ�����Ϣ
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
    // ����ȡֵ����
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

// Ŀǰû���Ƕ��̷߳��ʵ����飬ʹ��ʱ��Ҫע��
// ������
class CMySQLConnection
{
public:
    CMySQLConnection();
    virtual ~CMySQLConnection();
    //�������ݿ�
	bool Connect(const db_conn_info_t &dbInfo, bool bTimeout = true);

    //�Ͽ����ݿ�����
	bool Disconnect();

    //ִ��SQL����
    bool Exec(const TCHAR *sql);
    bool Exec(const TCHAR *sql, CMySQLRecordSet &resultSet);

	//ִ��SQL����
	bool ExecSQL(const TCHAR *sql);
	bool ExecSQL(const TCHAR *sql, CMySQLRecordSet &resultSet);

    /**	@fn	    IsAnException
     *	@brief	�ж�һ�����Ƿ���һ�����������һ���쳣
     *	@param  [in]errMsg, ������Ϣ
     *	@return	void
     */
    void IsAnException(const char * const errMsg);

    //������ؽӿ�
    bool BeginTransaction();
    bool CommitTransaction();
    bool RollBackTransaction();

protected:
	MYSQL *m_pConn;

	////���Ӷ���m_pConn����
	//HPR_Mutex m_mutexConn;

private:
    db_conn_info_t m_dbInfo;
    bool m_bConnected;
};

#endif // end of GUARD_FCMYSQL_H

