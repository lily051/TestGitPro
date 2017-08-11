#include "stdafx.h"
#include "stdlib.h"
#include "stdio.h"
#include "direct.h"
#include "io.h"
#include "assert.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;
//������Ŀ¼���ࣺ���¼���ļ�����¼�������ļ�����
typedef struct _pathname
{
    std::string pathname;
    std::string name;
}pathname_name_t;
class CBrowseDir
{
public:
    //ȱʡ������
    CBrowseDir();

    char* GetInitDir(){return &m_szInitDir[0];};
    //���ó�ʼĿ¼Ϊdir���������false����ʾĿ¼������
    bool SetInitDir(const char *dir);

    /**  @fn    GetDirFilenames
    *    @brief �����ɼ�¼���ļ��б�������¼���ļ���
    *    @param Input:dir-�ɼ�¼���ļ�·��,filespec����ʹ��ͨ��� * ?�����ܰ���·��(��ʼ������ʼĿ¼������Ŀ¼����filespecָ�����͵��ļ�)
    *....@param Output:filename_vector-�ļ���������
    *    @return 
    */
    void GetDirFilenames(const char* dir,const char *filespec,vector<pathname_name_t>& filename_vector);
private:
    pathname_name_t pathname_name;
    char m_szInitDir[_MAX_PATH];          //��ų�ʼĿ¼�ľ���·������'\'��β
};