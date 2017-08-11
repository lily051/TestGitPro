#include "stdafx.h"
#include "BrowseDir.h"
//
CBrowseDir::CBrowseDir()
{
    //�õ�ǰĿ¼��ʼ��m_szInitDir
    getcwd(m_szInitDir,_MAX_PATH);

    //���Ŀ¼�����һ����ĸ����'\',����������һ��'\'
    int len=strlen(m_szInitDir);
    if (m_szInitDir[len-1] != '\\')
        strcat(m_szInitDir,"\\");
}
//
bool CBrowseDir::SetInitDir(const char *dir)
{
    //�Ȱ�dirת��Ϊ����·��
    if (_fullpath(m_szInitDir,dir,_MAX_PATH) == NULL)
        return false;

    //�ж�Ŀ¼�Ƿ����
    if (_chdir(m_szInitDir) != 0)
        return false;

    //���Ŀ¼�����һ����ĸ����'\',����������һ��'\'
    int len=strlen(m_szInitDir);
    if (m_szInitDir[len-1] != '\\')
        strcat(m_szInitDir,"\\");

    return true;
}
//
void CBrowseDir::GetDirFilenames(const char* dir,const char *filespec,vector<pathname_name_t>& filename_vector)
{
    _chdir(dir);
    //���Ȳ���dir�з���Ҫ����ļ�
    long hFile;
    _finddata_t fileinfo;
    if ((hFile=_findfirst(filespec,&fileinfo)) != -1)
    {
        do
        {
            //����ǲ���Ŀ¼
            //�������,����д���
            if (!(fileinfo.attrib & _A_SUBDIR))
            {
                char filename[_MAX_PATH];
                strcpy(filename,dir);
                strcat(filename,fileinfo.name);
                //��ȡʵ���ļ���������·����
                std::string strName(fileinfo.name);
                int point = -1;
                point = strName.rfind(".");
                if (-1 == point)
                {
                    //TPLOG_ERROR("���ļ�����û�в���'.'...");
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
        //TPLOG_ERROR("û�в���dir�з���Ҫ����ļ���...");
    }
    //����dir�е���Ŀ¼
    //��Ϊ�ڴ���dir�е��ļ�ʱ���������ProcessFile�п��ܸı���
    //��ǰĿ¼����˻�Ҫ�������õ�ǰĿ¼Ϊdir��
    //ִ�й�_findfirst�󣬿���ϵͳ��¼���������Ϣ����˸ı�Ŀ¼
    //��_findnextû��Ӱ�졣
    _chdir(dir);
    if ((hFile=_findfirst("*.*",&fileinfo)) != -1)
    {
        do
        {
            //����ǲ���Ŀ¼ �����,�ټ���ǲ���'.' �� '..' �������,���е���
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
        //TPLOG_ERROR("û�в���dir��Ŀ¼�з���Ҫ����ļ���...");
    }
}