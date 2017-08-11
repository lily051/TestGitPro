#include "StdAfx.h"
#include "MysqlPool.h"

//���ӳصĹ��캯��
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

//���ӳص���������
MysqlPool::~MysqlPool() {
    this->DestoryDataBasePool();
}

/*
//��ȡ���ӳض���
MysqlPool*MysqlPool::GetInstance()
{
    static MysqlPool DataBasePool(10);
    return &DataBasePool;
}*/

//��ʼ�����ӳأ����������������һ����������
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


//��������,����һ��Connection
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


//�����ӳ��л��һ������
MYSQL*MysqlPool::GetConnection()
{
    MYSQL*con;
    EnterCriticalSection(&lock);

    if (connList.size() > 0)
    {	//���ӳ������л�������
        con = connList.front(); //�õ���һ������
        connList.pop_front(); 	//�Ƴ���һ������
        if (mysql_ping(con))
        {	//��������Ѿ����رգ�ɾ�������½���һ��
            con = this->CreateConnection();
        }
        //�������Ϊ�գ��򴴽����ӳ���
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
            //�����Դ����µ�����
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
        { //�������������Ѿ��ﵽmaxSize
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


//�������ӳ�,����Ҫ���������ӳص�������
void MysqlPool::DestoryDataBasePool()
{
    std::list<MYSQL*>::iterator icon;
    EnterCriticalSection(&lock);
    for (icon = connList.begin(); icon != connList.end(); ++icon)
    {
        //�������ӳ��е�����
        this->DestoryConnection(*icon); 
    }
    m_curSize = 0;
    //������ӳ��е�����
    connList.clear(); 
    LeaveCriticalSection(&lock);
}

//����һ������
void MysqlPool::DestoryConnection(MYSQL* conn)
{
    if (conn) {
        mysql_close(conn);
        conn = NULL;
    }
}