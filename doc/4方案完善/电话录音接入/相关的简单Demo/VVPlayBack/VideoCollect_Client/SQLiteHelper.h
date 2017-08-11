#ifndef GUARD_SQLITEHELPER_H
#define GUARD_SQLITEHELPER_H

/**	@file    SQLiteHelper.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   sqlite���ݿ����
 *
 *	@author	 xuezhihu
 *	@date	 2013/4/18
 *
 *	@note    �����note��warningΪ��ѡ��Ŀ
 *	@note    ������д���ļ�����ϸ����������ע��
 *	@note    ��ʷ��¼��
 *	@note    V0.0.1  ����
 *
 *	@warning ������д���ļ���صľ�����Ϣ
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
        // ����ȡֵ����
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
	sqlite3 *m_hDB;//���ݿ���
    std::_tstring m_fileName;

public:
    /**	@fn	    Connect
     *	@brief	�������ݿ�
     *	@param  [in]dbInfo, ���ݿ�������Ϣ����host�ֶν���Ϊsqlite�����ݿ��ļ�
     *	@param  [in]failNotExists, ���ݿ��ļ�������ʱ�Ĵ���ʽ��Ĭ��ʧ��
     *	@return	bool
     */
	bool Connect(const db_conn_info_t &dbInfo, bool failNotExists = true);
    //�Ͽ����ݿ�����
	bool Disconnect();

    LPCTSTR GetFileName(void) const
    {
        return m_fileName.c_str();
    }

public:
    //��Ҫ����������¼���ļ���ѯ
    bool QueryVideoNotes(NET_DVR_FINDDATA_V30& VideoFile);
    //�����ѹ���¼���¼
    bool InsertVideoNotes(NET_DVR_FINDDATA_V30& VideoFile);
    //ɾ������¼���¼
    bool DeleteVideoNotes(NET_DVR_FINDDATA_V30& VideoFile);
private:
    // �ӽ�����������һ���������ļ���¼
    void ExtractFileToTrans(fileToTrans_t &fileInfo, CMyRecordSet &recordSet);

private:
    //ִ��SQL����
    bool Exec(const TCHAR *sql);
    bool Exec(const TCHAR *sql, CMyRecordSet &resultSet);

    /**	@fn	    IsAnException
     *	@brief	�ж�һ�����Ƿ���һ�����������һ���쳣
     *	@param  [in]errMsg, ������Ϣ
     *	@return	void
     */
    void IsAnException(char const * const errMsg);

    //������ؽӿ�
    bool BeginTransaction();
    bool CommitTransaction();
    bool RollBackTransaction();
private:
    static size_t const MAX_SQL_LEN = 4 * 1024;
};

#endif // end of GUARD_SQLITEHELPER_H
