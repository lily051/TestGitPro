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
        const std::string &password); //���췽��
private:
    int m_curSize; //��ǰ�ѽ��������ݿ���������
    int m_maxSize; //���ӳ��ж����������ݿ�������
   

    std::string m_Ip;
    std::string m_Port;
    std::string m_Dbname;
    std::string m_Username;
    std::string m_Password;
    std::list<MYSQL *> connList; //���ӳص���������
    CRITICAL_SECTION lock; //�߳���
    //	static DataBasePool *DataBasePool;
    MYSQL *driver;
    MYSQL *CreateConnection(); //����һ������
    void InitConnection(int iInitialSize); //��ʼ�����ݿ����ӳ�
    void DestoryConnection(MYSQL *conn); //�������ݿ����Ӷ���
    void DestoryDataBasePool(); //�������ݿ����ӳ�

public:

    bool isEmpty;
    std::wstring str_error;
    MYSQL *GetConnection(); //������ݿ�����
    void ReleaseConnection(MYSQL *conn); //�����ݿ����ӷŻص����ӳص�������
  //  MysqlPool * GetInstance(const std::string &name,const std::string &ip,const std::string &port,const std::string &user,const std::string &password);//������ݿ�����
};
