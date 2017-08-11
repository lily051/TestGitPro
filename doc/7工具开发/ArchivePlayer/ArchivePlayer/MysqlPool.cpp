#include "StdAfx.h"
#include "MysqlPool.h"

//连接池的构造函数
MysqlPool::MysqlPool(int maxSize,
                     const std::string &name,
                     const std::string &ip,
                     const std::string &port,
                     const std::string &user,
                     const std::string &password)
{
   /* std::wstring pathname = _T("..\\config\\Config.ini");
    Config cfg(pathname);
    cfg.GetConfig(m_sql);*/
    m_Dbname = name;
    m_Port =  port;
    m_Ip = ip;
    m_Username = user;
    m_Password = password;
    m_curSize = 0;
    m_maxSize = maxSize;
    InitializeCriticalSection(&lock);
    this->InitConnection(maxSize / 2);
}

//连接池的析构函数
MysqlPool::~MysqlPool() {
    this->DestoryDataBasePool();
}

/*
//获取连接池对象
MysqlPool*MysqlPool::GetInstance()
{
    static MysqlPool DataBasePool(10);
    return &DataBasePool;
}*/

//初始化连接池，创建最大连接数的一半连接数量
void MysqlPool::InitConnection(int iInitialSize)
{
    MYSQL*conn;
    EnterCriticalSection(&lock);
    for (int i = 0; i < iInitialSize; i++)
    {
        conn = this->CreateConnection();
        if (conn)
        {
            if (!mysql_set_character_set(conn, "gbk"))
            {
                TRACE("New client character set: %s\n", mysql_character_set_name(conn));
            }
            else
            {
                //std::cout << "cannot set_character :" << mysql_error(conn) << std::endl;
            }
            connList.push_back(conn);
            ++(this->m_curSize);
        }
        else
        {
            this->driver = NULL;
        }
    }
    if (connList.empty())
    {
        isEmpty = true;
    }
    LeaveCriticalSection(&lock);
}


//创建连接,返回一个Connection
MYSQL* MysqlPool::CreateConnection()
{
    
    this->driver = mysql_init((MYSQL *)NULL);
    if (NULL == this->driver)
    {
        str_error = std::wstring(CString(mysql_error(this->driver)).GetBuffer());
        TRACE("%s\n",CString(mysql_error(this->driver)).GetBuffer());
        return NULL;
    }
    
    if (mysql_real_connect(this->driver,
        m_Ip.c_str(),
        m_Username.c_str(),
        m_Password.c_str(),
        m_Dbname.c_str(),
        atoi(m_Port.c_str()), NULL, 0))
    {
        return this->driver;
    }
    else
    {
        str_error = std::wstring(CString(mysql_error(this->driver)).GetBuffer());
        TRACE("%s\n",CString(mysql_error(this->driver)).GetBuffer());
        return NULL;
    }
  
}


//在连接池中获得一个连接
MYSQL*MysqlPool::GetConnection()
{
    MYSQL*con;
    EnterCriticalSection(&lock);

    if (connList.size() > 0)
    {	//连接池容器中还有连接
        con = connList.front(); //得到第一个连接
        connList.pop_front(); 	//移除第一个连接
        if (mysql_ping(con))
        {	//如果连接已经被关闭，删除后重新建立一个
            con = this->CreateConnection();
        }
        //如果连接为空，则创建连接出错
        if (con == NULL)
        {
            --m_curSize;
        }
        LeaveCriticalSection(&lock);
        return con;
    }
    else
    {
        if (m_curSize < m_maxSize)
        { 
            //还可以创建新的连接
            con = this->CreateConnection();
            if (con)
            {
                ++m_curSize;
                LeaveCriticalSection(&lock);
                return con;
            }
            else
            {
                LeaveCriticalSection(&lock);
                return NULL;
            }
        }
        else
        { //建立的连接数已经达到maxSize
            LeaveCriticalSection(&lock);
            return NULL;
        }
    }
}



void MysqlPool::ReleaseConnection(MYSQL * conn)
{
    if (conn)
    {
        EnterCriticalSection(&lock);
        connList.push_back(conn);
        LeaveCriticalSection(&lock);
    }
}


//销毁连接池,首先要先销毁连接池的中连接
void MysqlPool::DestoryDataBasePool()
{
    std::list<MYSQL*>::iterator icon;
    EnterCriticalSection(&lock);
    for (icon = connList.begin(); icon != connList.end(); ++icon)
    {
        //销毁连接池中的连接
        this->DestoryConnection(*icon); 
    }
    m_curSize = 0;
    //清空连接池中的连接
    connList.clear(); 
    LeaveCriticalSection(&lock);
}

//销毁一个连接
void MysqlPool::DestoryConnection(MYSQL* conn)
{
    if (conn) {
        mysql_close(conn);
        conn = NULL;
    }
}