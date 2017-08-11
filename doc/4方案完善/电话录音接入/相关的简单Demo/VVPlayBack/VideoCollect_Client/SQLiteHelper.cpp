#include "stdafx.h"
#include <algorithm>
#include <sstream>
#include <fcTchar.h>
#include <HPR_FileSystem.h>
#include <HPR_String.h>
#include "SQLiteHelper.h"
#include "hlogConfig.h"
//#include <hak.h>

using namespace std;

#pragma comment(lib, "sqlite3.lib")

namespace {
HPR_INT32 const FILESTAT_READY_FOR_TRANSPORT = 2;
HPR_INT32 const FILESTAT_TRANSPORTING = 3;
HPR_INT32 const FILESTAT_TRANSOVER = 4;

HPR_INT32 const COLLECTOR_DEFAULT_PORT = 8000;
int const BUSY_SLEEP_PER_TIME = 100; // 每次100ms
int const BUSY_SLEEP_TOTAL_TIME = 2000; // 一共2000ms
int const BUSY_SLEEP_TOTAL_TIMES = BUSY_SLEEP_TOTAL_TIME / BUSY_SLEEP_PER_TIME;
int my_busy_handler(void* param, int nTimes);
} //~ end of anonymous namespace

CSQLiteHelper::CMyRecordSet::CMyRecordSet()
: m_result(NULL)
, m_nRow(0)
, m_nColumn(0)
, m_nCurrentRow(0)
{
	m_mapIndex.clear();
}
CSQLiteHelper::CMyRecordSet::~CMyRecordSet()
{
    try
    {
        m_mapIndex.clear();
        if (NULL != m_result)
        {
            sqlite3_free_table((char**)m_result);
            m_result = NULL;
        }
    }
    catch (...)
    {
    
    }
}
bool CSQLiteHelper::CMyRecordSet::MoveNext(void)
{
    if (IsEOF())
    {
        return false;
    }
    ++m_nCurrentRow;
    return true;
}
HPR_INT32 CSQLiteHelper::CMyRecordSet::GetInt32Item(
    const char *colName,
    HPR_INT32 const defaultValue/* = 0*/) const
{
    const char * pValue = GetItem(colName);
    if (NULL == pValue)
    {
        return defaultValue;
    }

    return HPR_Atoi32(pValue);
}
HPR_INT64 CSQLiteHelper::CMyRecordSet::GetInt64Item(
    const char *colName,
    HPR_INT64 const defaultValue/* = 0*/) const
{
    const char * pValue = GetItem(colName);
    if (NULL == pValue)
    {
        return defaultValue;
    }

    return HPR_Atoi64(pValue);
}
const char *CSQLiteHelper::CMyRecordSet::GetStringItem(
    const char *colName,
    char * const defaultValue/* = ""*/) const
{
    const char * pValue = GetItem(colName);
    if (NULL == pValue)
    {
        return defaultValue;
    }

    return pValue;
}
const char* CSQLiteHelper::CMyRecordSet::GetItem(const char *colName) const
{
    assert(m_nCurrentRow > 0);
    assert(m_nCurrentRow <= m_nRow);
    for (vector<string>::size_type i = 0; i < m_mapIndex.size(); ++i)
    {
        if (HPR_OK == HPR_Strcasecmp(colName, m_mapIndex[i].c_str()))
        {
            return GetItem_(i);
        }
    }

    throw "colName not found!";
}
const char* CSQLiteHelper::CMyRecordSet::GetItem(const int colIndex) const
{
    assert(m_nCurrentRow > 0);
    assert(m_nCurrentRow <= m_nRow);
    if (colIndex < 0 || colIndex >= m_nColumn)
    {
        throw "colIndex out of range!";
    }
    return GetItem_(colIndex);
}
const char* CSQLiteHelper::CMyRecordSet::GetItem_(const int colIndex) const
{
    assert(m_result != NULL);
    return m_result[m_nCurrentRow * m_nColumn + colIndex];
}
void CSQLiteHelper::CMyRecordSet::SetData(char **result, int nRow, int nColumn)
{
    assert(result != NULL);
    assert(nRow >= 0);
    assert(nColumn >= 0);
    if (NULL == result)
    {
        return;
    }

    m_result = result;
    m_nRow = nRow;
    m_nColumn = nColumn;
    m_nCurrentRow = 1;
    for (int i = 0; i < nColumn; ++i)
    {
        m_mapIndex.push_back((const char*)fcU2A(result[i]));
    }
}


CSQLiteHelper::CSQLiteHelper(void)
{
    m_hDB=NULL;
}

CSQLiteHelper::~CSQLiteHelper(void)
{
    try
    {
	    if (NULL != m_hDB)
	    {
	        Disconnect();
            m_hDB = NULL;
	    }
    }
    catch (...)
    {
    }
}

bool CSQLiteHelper::Connect(const db_conn_info_t &dbInfo, bool failNotExists/* = true*/)
{
    if (NULL != m_hDB)
    {
        if (dbInfo.host == m_fileName)
        {
            return true;
        }
        else
        {
            Disconnect();
        }
    }

    m_fileName = dbInfo.host;
    if (failNotExists
        && HPR_OK != HPR_AccessFile(fcT2A(m_fileName.c_str()), HPR_F_OK))
    {
        TPLOG_ERROR("dbfile %s not exists",
            (const char*)fcT2A(m_fileName.c_str()));
        return false;
    }
    //打开指定的数据库文件,如果不存在将创建一个同名的数据库文件
	int rc = sqlite3_open(fcT2U(m_fileName.c_str()), &m_hDB); 
	if(SQLITE_OK != rc)
	{
		sqlite3_close(m_hDB);
        m_hDB = NULL;
		return false;
	}

    // 设置busy
    sqlite3_busy_handler(m_hDB, my_busy_handler, this);
    //sqlite3_busy_timeout(m_hDB, BUSY_SLEEP_TOTAL_TIME);

	return true;  
}
bool CSQLiteHelper::Exec(const TCHAR *sql)
{
    assert(sql != NULL);
    assert(m_hDB);
    char* errMsg = NULL;
    int rc = sqlite3_exec(m_hDB, fcT2U(sql), 0, 0, &errMsg);
    if(SQLITE_OK == rc)
    {
        return true;
    }

    if(errMsg != NULL)
    {
        IsAnException(errMsg);

        TPLOG_ERROR("exec %s failed-%s", (const char*)fcT2A(sql),
            (const char*)fcU2A(errMsg));
        sqlite3_free(errMsg);
        //errMsg = NULL;
    }

	return false;
}
bool CSQLiteHelper::Exec(const TCHAR *sql, CMyRecordSet &resultSet)
{
    assert(m_hDB != NULL);

    char **result = NULL; //二维数组存放结果
    int nRow = -1;
    int nColumn = -1;
    char* errMsg = NULL;
    if(SQLITE_OK != sqlite3_get_table(m_hDB, fcT2U(sql), &result, &nRow, &nColumn, &errMsg))
    {
        if(errMsg != NULL)
        {
            IsAnException(errMsg);

            TPLOG_ERROR("exec %s failed-%s", (const char*)fcT2A(sql),
                (const char*)fcU2A(errMsg));
            sqlite3_free(errMsg);
            //errMsg = NULL;
        }
        else
        {
            TPLOG_ERROR("exec %s failed", (const char*)fcT2A(sql));
        }
        return false;
    }
    resultSet.SetData(result, nRow, nColumn);
    return true;
}

bool CSQLiteHelper::Disconnect()
{
    if (NULL == m_hDB)
    {
        TPLOG_WARN("bad connection");
        return true;
    }
    //关闭数据库
	if(sqlite3_close(m_hDB) == SQLITE_OK)
    {
        m_hDB = NULL;
		return true;
	}

	return false;
}

bool CSQLiteHelper::BeginTransaction()
{
    return Exec(_T("begin transaction"));
}

bool CSQLiteHelper::CommitTransaction()
{
    return Exec(_T("commit transaction"));
}

bool CSQLiteHelper::RollBackTransaction()
{
    return Exec(_T("rollback transaction"));
}

// errMsg是一个utf8的字符串，但是都是英文的，所以不转码了
void CSQLiteHelper::IsAnException(char const * const errMsg)
{
    assert(NULL != errMsg);

    if (NULL != HPR_Strstr(errMsg, "no such table:"))
    {
        throw errMsg;
    }   

    if (NULL != HPR_Strstr(errMsg, "no such column:"))
    {
        throw errMsg;
    }    

    if (NULL != HPR_Strstr(errMsg, "syntax error"))
    {
        throw errMsg;
    }

    TPLOG_ERROR("db op fail:%s", errMsg);
}


namespace {
int my_busy_handler(void* param, int nTimes)
{
    assert(NULL != param);
    if (nTimes > BUSY_SLEEP_TOTAL_TIMES)
    {
        return 0; // busy等待结束了，返回busy_timeout
    }
    
    CSQLiteHelper *pHelper = reinterpret_cast<CSQLiteHelper *>(param);
    if (NULL != pHelper && nTimes % 10 == 0)
    {
	    TPLOG_INFO("%s busy handler %d",
	        (LPCSTR)fcT2A(pHelper->GetFileName()),
	        nTimes);
    }
    sqlite3_sleep(BUSY_SLEEP_PER_TIME);
    return 1;
}
}


//
bool CSQLiteHelper::QueryVideoNotes(NET_DVR_FINDDATA_V30& VideoFile)
{
    USES_CONVERSION;
    TCHAR sqlStmt[MAX_SQL_LEN] = {0};
    _stprintf_s(sqlStmt, _countof(sqlStmt),
        _T("SELECT * FROM MyRelateVideo WHERE strVideoName = '%s' AND nState = %d;"),
           A2T((VideoFile).sFileName),1);

    CMyRecordSet recordSet;
    if (!Exec(sqlStmt,recordSet))
    {
        return false;
    }
    if(!recordSet.IsEOF())
    {
        if(1 != recordSet.GetRowCount())
        {
            TPLOG_ERROR("recordSet.GetRowCount() != 1(FileName:%s)",A2T((VideoFile).sFileName));
            //return false;
        }
    }
    else
        return false;
    
}
bool CSQLiteHelper::InsertVideoNotes(NET_DVR_FINDDATA_V30& VideoFile)
{
    USES_CONVERSION;
    TCHAR sqlStmt[MAX_SQL_LEN] = {0};
    _stprintf_s(sqlStmt, _countof(sqlStmt),
        _T("INSERT INTO MyRelateVideo(strVideoName,nState) VALUES('%s',%d);"),
           A2T((VideoFile).sFileName),1);

    if (!Exec(sqlStmt))
    {
        return false;
    }

    return true;
}

bool CSQLiteHelper::DeleteVideoNotes(NET_DVR_FINDDATA_V30& VideoFile)
{
    USES_CONVERSION;
    TCHAR sqlStmt[MAX_SQL_LEN] = {0};
    _stprintf_s(sqlStmt, _countof(sqlStmt),
        _T("DELETE FROM MyRelateVideo WHERE strVideoName = '%s' AND nState = %d;"),
        A2T((VideoFile).sFileName),1);

    if (!Exec(sqlStmt))
    {
        return false;
    }

    return true;
}