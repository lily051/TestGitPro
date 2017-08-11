#pragma once
#include <mysql.h> 
#include <string>
#include <list>
#include "GeneralDef.h"
class MysqlPool
{
public:
    ~MysqlPool(void);    
     MysqlPool(int maxSize,
        const std::string &name,
        const std::string &ip,
        const std::string &port,
        const std::string &user,
        const std::string &password); //构造方法
private:
    int m_curSize; //当前已建立的数据库连接数量
    int m_maxSize; //连接池中定义的最大数据库连接数
   

    std::string m_Ip;
    std::string m_Port;
    std::string m_Dbname;
    std::string m_Username;
    std::string m_Password;
    std::list<MYSQL *> connList; //连接池的容器队列
    CRITICAL_SECTION lock; //线程锁
    //	static DataBasePool *DataBasePool;
    MYSQL *driver;
    MYSQL *CreateConnection(); //创建一个连接
    void InitConnection(int iInitialSize); //初始化数据库连接池
    void DestoryConnection(MYSQL *conn); //销毁数据库连接对象
    void DestoryDataBasePool(); //销毁数据库连接池

public:

    bool isEmpty;
    std::wstring str_error;
    MYSQL *GetConnection(); //获得数据库连接
    void ReleaseConnection(MYSQL *conn); //将数据库连接放回到连接池的容器中
  //  MysqlPool * GetInstance(const std::string &name,const std::string &ip,const std::string &port,const std::string &user,const std::string &password);//获得数据库连接
};
