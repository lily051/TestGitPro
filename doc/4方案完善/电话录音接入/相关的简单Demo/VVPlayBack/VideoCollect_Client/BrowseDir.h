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
//遍历根目录的类：完成录像文件明记录和索引文件产生
typedef struct _pathname
{
    std::string pathname;
    std::string name;
}pathname_name_t;
class CBrowseDir
{
public:
    //缺省构造器
    CBrowseDir();

    char* GetInitDir(){return &m_szInitDir[0];};
    //设置初始目录为dir，如果返回false，表示目录不可用
    bool SetInitDir(const char *dir);

    /**  @fn    GetDirFilenames
    *    @brief 遍历采集录像文件夹保存所有录像文件名
    *    @param Input:dir-采集录像文件路径,filespec可以使用通配符 * ?，不能包含路径(开始遍历初始目录及其子目录下由filespec指定类型的文件)
    *....@param Output:filename_vector-文件保存容器
    *    @return 
    */
    void GetDirFilenames(const char* dir,const char *filespec,vector<pathname_name_t>& filename_vector);
private:
    pathname_name_t pathname_name;
    char m_szInitDir[_MAX_PATH];          //存放初始目录的绝对路径，以'\'结尾
};