#include "StdAfx.h"
#include <cassert>
#include <stdexcept>
#include "fcMySQL.h"

#pragma comment(lib, "libmysql.lib")

using std::vector;
using std::string;

namespace {
int const MYSQL_NO_ERROR = 0;
}

bool fcDB_MySQL_Init(void)
{
    if (MYSQL_NO_ERROR == mysql_library_init(0, NULL, NULL))
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool fcDB_MySQL_Fini(void)
{
    mysql_library_end();
    return true;
}

CMySQLRecordSet::CMySQLRecordSet()
: m_pResult(NULL)
, m_nRow(0)
, m_nColumn(0)
, m_nCurrentRow(0)
, m_pConn(NULL)
, m_row(NULL)
{
}
CMySQLRecordSet::~CMySQLRecordSet()
{
    try
    {
        if (NULL != m_pResult)
        {
            mysql_free_result(m_pResult);
            m_pResult = NULL;
        }
        //if (NULL != m_pConn)
        //{
        //    mysql_close(m_pConn);
        //    m_pConn = NULL;
        //}
    }
    catch (...)
    {
    }
}
bool CMySQLRecordSet::MoveNext(void)
{
    assert(NULL != m_pResult);
    if (m_nCurrentRow > m_nRow)
    {
        return false;
    }
    m_row = mysql_fetch_row(m_pResult);
    if (NULL == m_row)
    {
        return false;
    }
    ++m_nCurrentRow;
    return true;
}
INT32 CMySQLRecordSet::GetInt32Item(
    const char *colName,
    INT32 const defaultValue/* = 0*/) const
{
    const char * pValue = GetItem(colName);
    if (NULL == pValue)
    {
        return defaultValue;
    }

    return ::atoi(pValue);
}
INT64 CMySQLRecordSet::GetInt64Item(
    const char *colName,
    INT64 const defaultValue/* = 0*/) const
{
    const char * pValue = GetItem(colName);
    if (NULL == pValue)
    {
        return defaultValue;
    }

    return ::_atoi64(pValue);
}
const char *CMySQLRecordSet::GetStringItem(
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
const char* CMySQLRecordSet::GetItem(const char *colName) const
{
    assert(m_nCurrentRow > 0);
    assert(m_nCurrentRow <= m_nRow);
    assert(NULL != m_row);
    for (vector<string>::size_type i = 0; i < m_mapIndex.size(); ++i)
    {
        if (0 == _stricmp(colName, m_mapIndex[i].c_str()))
        {
            return GetItem_(i);
        }
    }

    throw "colName not found!";
}
const char* CMySQLRecordSet::GetItem(const int colIndex) const
{
    assert(m_nCurrentRow > 0);
    assert(m_nCurrentRow <= m_nRow);
    assert(NULL != m_row);
    if (colIndex < 0 || colIndex >= m_nColumn)
    {
        throw "colIndex out of range!";
    }
    return GetItem_(colIndex);
}
const char* CMySQLRecordSet::GetItem_(const int colIndex) const
{
    assert(m_pResult != NULL);
    assert(NULL != m_row);
    if (NULL == m_row)
    {
        return NULL;
    }
    return m_row[colIndex];
}
bool CMySQLRecordSet::SetData(void)
{
    assert(NULL != m_pConn);
	if (NULL != m_pResult)
	{
		mysql_free_result(m_pResult);
		m_pResult = NULL;
	}
	assert(NULL == m_pResult);

    // 因为知道结果集不大而且这样处理简单所以不使用mysql_use_result
    m_pResult = mysql_store_result(m_pConn);
    // 在某些情况下m_pResult会为NULL，如内存不足，conn突然断了
    assert(NULL != m_pResult); 
    if (NULL == m_pResult)
    {
        VIEWMSGLOG_ERROR("get result set failed");
        m_nColumn = mysql_field_count(m_pConn);
        m_nRow = 0;
        m_nCurrentRow = 1;
        return false;
    }

    m_nRow = static_cast<int>(mysql_num_rows(m_pResult));
    m_nColumn = mysql_num_fields(m_pResult);
    m_row = mysql_fetch_row(m_pResult);
    m_nCurrentRow = 1;

	m_mapIndex.clear();
    MYSQL_FIELD *field = NULL;
    while(NULL != (field = mysql_fetch_field(m_pResult)))
    {
        m_mapIndex.push_back((const char*)fcU2A(field->name));
    }
    assert(m_mapIndex.size() == static_cast<size_t>(m_nColumn));

    return true;
}


CMySQLConnection::CMySQLConnection()
: m_bConnected(false)
, m_pConn(NULL)
{

}
CMySQLConnection::~CMySQLConnection()
{
    try
    {
	    if (m_bConnected)
	    {
	        Disconnect();
	    }
		m_pConn = NULL;
    }
    catch (...)
    {
    	
    }
}

//连接数据库
bool CMySQLConnection::Connect(const db_conn_info_t &dbInfo, bool bTimeout)
{
	m_dbInfo = dbInfo;
	if (!bTimeout)
	{
		if (m_bConnected)
		{
			return true;
		}

		m_pConn = mysql_init(NULL);
		if (NULL == m_pConn)
		{
			VIEWMSGLOG_ERROR("init mysql failed");
			return false;
		}    
		mysql_options(m_pConn, MYSQL_SET_CHARSET_NAME, "utf8");
		mysql_options(m_pConn, MYSQL_OPT_RECONNECT, "1");
		if (NULL == mysql_real_connect(
			m_pConn,
			fcT2U(m_dbInfo.host.c_str()), 
			fcT2U(m_dbInfo.username.c_str()),
			fcT2U(m_dbInfo.password.c_str()), 
			fcT2U(m_dbInfo.dbschema.c_str()),
			_tstoi(m_dbInfo.port.c_str()), 
			NULL, 
			CLIENT_MULTI_QUERIES))
		{
			const char *errMsg = mysql_error(m_pConn);
			IsAnException(errMsg);
			VIEWMSGLOG_ERROR("connect %s:%s failed-%s",
				(const char*)fcT2A(m_dbInfo.host.c_str()),
				(const char*)fcT2A(m_dbInfo.port.c_str()),
				(const char*)fcU2A(errMsg));
			return false;
		}
	}

	m_bConnected = true;
	return true;
}

//断开数据库连接
bool CMySQLConnection::Disconnect()
{
	HPR_Guard guard(&m_mutexConn);

	if (!m_bConnected)
	{
		return true;
	}

	if (NULL != m_pConn)
	{
		mysql_close(m_pConn);
		m_pConn = NULL;
	}

	m_bConnected = false;
	return true;
}

bool CMySQLConnection::Exec(const TCHAR *sql)
{
    assert(sql != NULL);

	m_pConn = mysql_init(NULL);
	if (NULL == m_pConn)
	{
		VIEWMSGLOG_ERROR("init mysql failed");
		return false;
	} 
    mysql_options(m_pConn, MYSQL_SET_CHARSET_NAME, "utf8");
    if (NULL == mysql_real_connect(m_pConn, fcT2U(m_dbInfo.host.c_str()), fcT2U(m_dbInfo.username.c_str()),
        fcT2U(m_dbInfo.password.c_str()), fcT2U(m_dbInfo.dbschema.c_str()),
        _tstoi(m_dbInfo.port.c_str()), NULL,0))
    {
        const char *errMsg = mysql_error(m_pConn);
        IsAnException(errMsg);
        VIEWMSGLOG_ERROR("connect %s:%s failed-%s",
            (const char*)fcT2A(m_dbInfo.host.c_str()),
            (const char*)fcT2A(m_dbInfo.port.c_str()),
            (const char*)fcU2A(errMsg));
        return false;
    }

    string usqlstmt = fcT2U(sql);
    if (MYSQL_NO_ERROR != mysql_real_query(m_pConn, usqlstmt.c_str(),
        usqlstmt.size()))
    {
        const char *errMsg = mysql_error(m_pConn);
        IsAnException(errMsg);
        VIEWMSGLOG_ERROR("exec %s failed-%s", (const char*)fcT2A(sql),
            (const char*)fcU2A(errMsg));
        mysql_close(m_pConn);
        return false;
    }

    mysql_close(m_pConn);
    return true;
}

bool CMySQLConnection::Exec(const TCHAR *sql, CMySQLRecordSet &resultSet)
{
    assert(sql != NULL);

    resultSet.ConnPtrRef() = mysql_init(m_pConn);
    if (NULL == resultSet.ConnPtrRef())
    {
        VIEWMSGLOG_ERROR("init mysql failed");
        return false;
    }
    mysql_options(resultSet.ConnPtrRef(), MYSQL_SET_CHARSET_NAME, "utf8");

    if (NULL == mysql_real_connect(resultSet.ConnPtrRef(), fcT2U(m_dbInfo.host.c_str()), fcT2U(m_dbInfo.username.c_str()),
        fcT2U(m_dbInfo.password.c_str()), fcT2U(m_dbInfo.dbschema.c_str()),
        _tstoi(m_dbInfo.port.c_str()), NULL,0))
    {
        const char *errMsg = mysql_error(resultSet.ConnPtrRef());
        IsAnException(errMsg);
        VIEWMSGLOG_ERROR("connect %s:%s failed-%s",
            (const char*)fcT2A(m_dbInfo.host.c_str()),
            (const char*)fcT2A(m_dbInfo.port.c_str()),
            (const char*)fcU2A(errMsg));
        return false;
    }

    string usqlstmt = fcT2U(sql);
    if (MYSQL_NO_ERROR != mysql_real_query(resultSet.ConnPtrRef(), usqlstmt.c_str(),
        usqlstmt.size()))
    {
        const char *errMsg = mysql_error(resultSet.ConnPtrRef());
        IsAnException(errMsg);
        VIEWMSGLOG_ERROR("exec %s failed-%s", (const char*)fcT2A(sql),
            (const char*)fcU2A(errMsg));
        //mysql_close(resultSet.ConnPtrRef());
        return false;
    }

    return resultSet.SetData();
}

bool CMySQLConnection::ExecSQL(const TCHAR *sql)
{
	HPR_Guard guard(&m_mutexConn);

	assert(sql != NULL);
	if (NULL == m_pConn)
	{
		VIEWMSGLOG_ERROR("exec %s failed, connection failed ", 
			(const char*)fcT2A(sql));
		return false;
	}

	string usqlstmt = fcT2U(sql);
	if (MYSQL_NO_ERROR != mysql_real_query(m_pConn, usqlstmt.c_str(),
		usqlstmt.size()))
	{
		const char *errMsg = mysql_error(m_pConn);
		IsAnException(errMsg);
		VIEWMSGLOG_ERROR("exec %s failed-%s", (const char*)fcT2A(sql),
			(const char*)fcU2A(errMsg));
		//mysql_close(m_pConn);
		return false;
	}

	//mysql_close(m_pConn);
	return true;
}

bool CMySQLConnection::ExecSQL(const TCHAR *sql, CMySQLRecordSet &resultSet)
{
	HPR_Guard guard(&m_mutexConn);

	assert(sql != NULL);
	if (NULL == m_pConn)
	{
		VIEWMSGLOG_ERROR("exec %s failed, connection failed ", 
			(const char*)fcT2A(sql));
		return false;
	}

	resultSet.ConnPtrRef() = m_pConn;
	string usqlstmt = fcT2U(sql);
	if (MYSQL_NO_ERROR != mysql_real_query(resultSet.ConnPtrRef(), usqlstmt.c_str(),
		usqlstmt.size()))
	{
		const char *errMsg = mysql_error(resultSet.ConnPtrRef());
		IsAnException(errMsg);
		VIEWMSGLOG_ERROR("exec %s failed-%s", (const char*)fcT2A(sql),
			(const char*)fcU2A(errMsg));
		return false;
	}

	return resultSet.SetData();
}

bool CMySQLConnection::BeginTransaction()
{
    return Exec(_T("begin transaction"));
}

bool CMySQLConnection::CommitTransaction()
{
    return Exec(_T("commit transaction"));
}

bool CMySQLConnection::RollBackTransaction()
{
    return Exec(_T("rollback transaction"));
}

// errMsg是一个utf8的字符串，但是都是英文的，所以不转码了
void CMySQLConnection::IsAnException(const char * const errMsg)
{
    assert(NULL != errMsg);
    if (NULL == errMsg)
    {
        return;
    }

    if (NULL != ::strstr(errMsg, "Table")
        && NULL != ::strstr(errMsg, "doesn't exist"))
    {
        throw std::logic_error(errMsg);
    }

    if (NULL != ::strstr(errMsg, "Unknown column"))
    {
        throw std::runtime_error(errMsg);
    }    

    if (NULL != ::strstr(errMsg, "syntax error"))
    {
        throw std::logic_error(errMsg);
    }

    VIEWMSGLOG_ERROR("db op fail:%s", errMsg);
}
