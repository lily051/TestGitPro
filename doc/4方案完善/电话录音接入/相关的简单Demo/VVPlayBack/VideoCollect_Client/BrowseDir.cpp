#include "stdafx.h"
#include "BrowseDir.h"
//
CBrowseDir::CBrowseDir()
{
    //用当前目录初始化m_szInitDir
    getcwd(m_szInitDir,_MAX_PATH);

    //如果目录的最后一个字母不是'\',则在最后加上一个'\'
    int len=strlen(m_szInitDir);
    if (m_szInitDir[len-1] != '\\')
        strcat(m_szInitDir,"\\");
}
//
bool CBrowseDir::SetInitDir(const char *dir)
{
    //先把dir转换为绝对路径
    if (_fullpath(m_szInitDir,dir,_MAX_PATH) == NULL)
        return false;

    //判断目录是否存在
    if (_chdir(m_szInitDir) != 0)
        return false;

    //如果目录的最后一个字母不是'\',则在最后加上一个'\'
    int len=strlen(m_szInitDir);
    if (m_szInitDir[len-1] != '\\')
        strcat(m_szInitDir,"\\");

    return true;
}
//
void CBrowseDir::GetDirFilenames(const char* dir,const char *filespec,vector<pathname_name_t>& filename_vector)
{
    _chdir(dir);
    //首先查找dir中符合要求的文件
    long hFile;
    _finddata_t fileinfo;
    if ((hFile=_findfirst(filespec,&fileinfo)) != -1)
    {
        do
        {
            //检查是不是目录
            //如果不是,则进行处理
            if (!(fileinfo.attrib & _A_SUBDIR))
            {
                char filename[_MAX_PATH];
                strcpy(filename,dir);
                strcat(filename,fileinfo.name);
                //提取实际文件名（不含路径）
                std::string strName(fileinfo.name);
                int point = -1;
                point = strName.rfind(".");
                if (-1 == point)
                {
                    //TPLOG_ERROR("此文件名中没有查找'.'...");
                    continue;
                }
                std::string strmp4 = strName.substr(point);
                if (0 == strcmp(strmp4.c_str(),".mp4"))
                {
                    std::string name = strName.substr(0,point);
                    pathname_name.pathname = filename;
                    pathname_name.name = name;
                    filename_vector.push_back(pathname_name);
                    strName = "";
                }
            }
        } while (_findnext(hFile,&fileinfo) == 0);
        _findclose(hFile);
    }
    else
    {
        //TPLOG_ERROR("没有查找dir中符合要求的文件了...");
    }
    //查找dir中的子目录
    //因为在处理dir中的文件时，派生类的ProcessFile有可能改变了
    //当前目录，因此还要重新设置当前目录为dir。
    //执行过_findfirst后，可能系统记录下了相关信息，因此改变目录
    //对_findnext没有影响。
    _chdir(dir);
    if ((hFile=_findfirst("*.*",&fileinfo)) != -1)
    {
        do
        {
            //检查是不是目录 如果是,再检查是不是'.' 或 '..' 如果不是,进行迭代
            if ((fileinfo.attrib & _A_SUBDIR))
            {
                if (strcmp(fileinfo.name,".") != 0 && strcmp(fileinfo.name,"..") != 0)
                {
                    vector<pathname_name_t> tmp;
                    char subdir[_MAX_PATH];
                    strcpy(subdir,dir);
                    strcat(subdir,fileinfo.name);
                    strcat(subdir,"\\");
                    GetDirFilenames(subdir,filespec,tmp);
                    for (vector<pathname_name_t>::iterator it=tmp.begin();it<tmp.end();it++)
                    {
                        filename_vector.push_back(*it);
                    }
                }
            }
        } while (_findnext(hFile,&fileinfo) == 0);
        _findclose(hFile);
    }
    else
    {
        //TPLOG_ERROR("没有查找dir子目录中符合要求的文件了...");
    }
}