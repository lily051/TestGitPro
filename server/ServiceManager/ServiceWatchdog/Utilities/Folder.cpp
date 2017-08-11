//
// Copyright (c) 2009 Hikvision Digital Technology Co., Ltd.
//
// FileName:
//     Folder.cpp
//
// Summary:
//     Folder ��⡣
//
// Modification History:
//   2009-08-14:
//     V1.0 / ��ͨ�� / ����
//
#include "StdAfx.h"
#include "Folder.h"
#include <afx.h>
#include <imagehlp.h>
#pragma comment(lib, "imagehlp.lib")
#include <stack>
using namespace std;

CFolder::CFolder(void)
{
}

CFolder::~CFolder(void)
{
}

/**************************************************************************
*   Function:		CheckFolder
*   Description:    ��� folder �Ƿ���ڣ����������򴴽���
*   Input:          char* path : �ļ���·����
*   Output:         (null)
*   Return:			true - �����ɹ�
*					false - ����ʧ��
**************************************************************************/
bool CFolder::CheckFolder(const char* path)
{
    return (TRUE == MakeSureDirectoryPathExists(FormatFolderPath(path)));
}

/**************************************************************************
*   Function:		ValidateLogin
*   Description:    ��ʽ���ļ���·����
*   Input:          char* folderPath : �ļ���·����
*   Output:         (null)
*   Return:			true - �����ɹ�
*					false - ����ʧ��
**************************************************************************/
CStringA CFolder::FormatFolderPath(const char* folderPath)
{
    if (NULL != folderPath)
    {
        CStringA strPath2(folderPath);
        strPath2.Trim();
        strPath2.Replace('/', '\\');
        CStringA strPath;
        strPath.Preallocate(strPath2.GetLength() + 1);

        int len = strPath2.GetLength();
        char preChar = ' ';
        for (int i = 0; i < len; i++)
        {
            if ('\\' == strPath2[i] && '\\' == preChar)
            {
                continue;
            }

            preChar = strPath2[i];
            strPath.AppendChar(preChar);
        }

        int idx = strPath.ReverseFind('.');
        if (idx >= 0)
        {
            int idx2 = strPath.ReverseFind('\\');
            idx2 = idx2 < 0 ? 0 : idx2;
            if (idx > idx2)
            {
                strPath.ReleaseBufferSetLength(idx2);
            }
        }

        strPath.TrimRight('\\');
        strPath.AppendChar('\\');

        return strPath;
    }

    return "\\";
}

/**************************************************************************
*   Function:		FormatFilePath
*   Description:    ��ʽ���ļ�·����
*   Input:          char* folderPath : �ļ���·����
*   Output:         (null)
*   Return:			true - �����ɹ�
*					false - ����ʧ��
**************************************************************************/
CStringA CFolder::FormatFilePath(const char* filePath)
{
    if (NULL != filePath)
    {
        CStringA strPath2(filePath);
        strPath2.Trim();
        strPath2.Replace('/', '\\');
        CStringA strPath;
        strPath.Preallocate(strPath2.GetLength() + 1);

        int len = strPath2.GetLength();
        char preChar = ' ';
        for (int i = 0; i < len; i++)
        {
            if ('\\' == strPath2[i] && '\\' == preChar)
            {
                continue;
            }

            preChar = strPath2[i];
            strPath.AppendChar(preChar);
        }

        return strPath;
    }

    return "";
}

/**************************************************************************
*   Function:		FileExists
*   Description:    ����ļ��Ƿ���ڡ�
*   Input:          char* filePath : �ļ�·����
*   Output:         (null)
*   Return:			true - �ļ�����
*					false - �ļ�������
**************************************************************************/
bool CFolder::FileExists(const char* filePath)
{
    if (NULL != filePath)
    {
        CFileFind find;
        BOOL rtn = find.FindFile(FormatFilePath(filePath));
        find.Close();

        return (TRUE == rtn);
    }

    return false;
}

/**************************************************************************
*   Function:		DeleteFile
*   Description:     ɾ��ָ���ļ���
*   Input:          char* filePath : �ļ�·����
*   Output:         (null)
*   Return:			true - �����ɹ�
*					false - ����ʧ��
**************************************************************************/
bool CFolder::DeleteFile(const char* filePath)
{
    if (NULL != filePath)
    {
        return (TRUE == ::DeleteFileA(filePath));
    }

    return false;
}

/**************************************************************************
*   Function:		RenameFile
*   Description:    ����ָ���ļ����ļ�����
*   Input:          char* filePath : �ļ�·����
                    char* renameFilePath : ����������ļ�·��
*   Output:         (null)
*   Return:			true - �����ɹ�
*					false - ����ʧ��
**************************************************************************/
bool CFolder::RenameFile(const char* filePath, const char* renameFilePath)
{
    if (FileExists(filePath))
    {
        return (TRUE == ::MoveFileA(filePath, renameFilePath));
    }

    return false;
}

/**************************************************************************
*   Function:		GetFileName
*   Description:    ��ȡ�ļ�·���е��ļ�����
*   Input:          char* filePath : �ļ�·����
*   Output:         (null)
*   Return:			�ļ�·���е��ļ���
**************************************************************************/
CStringA CFolder::GetFileName(const char* filePath)
{
    if (NULL != filePath)
    {
        CStringA file(FormatFilePath(filePath));
        int idx = file.ReverseFind('\\');
        idx = idx < 0 ? 0 : idx;
        file = file.Right(file.GetLength() - 1 - idx);

        file.Trim();
        idx = file.ReverseFind('.');
        if (idx <= 0 || idx >= (file.GetLength() - 1))
        {
            file.Empty();
        }

        return file;
    }

    return "";
}
/**************************************************************************
*   Function:		SaveToUTF8File
*   Description:    �� ansi �ַ����洢�� utf8 ��ʽ�ļ���
*   Input:          char* content : �ļ����ݡ�
                    char* filePath : �ļ�·����
*   Output:         (null)
*   Return:			true - �����ɹ�
*					false - ����ʧ��
**************************************************************************/
bool CFolder::SaveToUTF8File(const char* content, const char* filePath)
{
    if (NULL != content && NULL != filePath && IsFilePath(filePath))
    {
        try
        {
            int num = MultiByteToWideChar(CP_ACP, NULL, content, -1, NULL, NULL);
            wchar_t* buffw = new wchar_t[num];
            MultiByteToWideChar(CP_ACP, NULL, content, -1, buffw, num);

            int len = WideCharToMultiByte(CP_UTF8, 0, buffw, num - 1, NULL, NULL, NULL, NULL); 
            char* lpsz = new char[len + 1]; 
            WideCharToMultiByte(CP_UTF8, 0, buffw, num - 1, lpsz, len, NULL, NULL);
            lpsz[len]='\0';
            delete[] buffw;

            try
            {
                CheckFolder(filePath);

                CStdioFile localFile(filePath, CFile::modeCreate | CFile::typeBinary | CFile::modeWrite | CFile::shareDenyWrite);

                localFile.Write(lpsz, len);

                localFile.Close();
                delete[] lpsz;
                lpsz = NULL;

                return true;
            }
            catch (CFileException/*& fe*/)
            {
                OutputDebugString("hth >> CFolder::SaveToUTF8File - CFileException\n");
                //fe;
                if (lpsz != NULL)
                {
                    delete[] lpsz;
                }

                return false;
            }
        }
        catch (...)
        {
            OutputDebugString("hth >> CFolder::SaveToUTF8File - Exception\n");
            return false;
        }
    }

    return false;
}

/**************************************************************************
*   Function:		ReadFromUTF8File
*   Description:     ��ȡ utf8 ��ʽ�ļ�����ת���� ansi �ַ�����
*   Input:          char* filePath : �ļ�·����
*   Output:         (null)
*   Return:			 utf8 ��ʽ�ļ�ת���� ansi �ַ���
**************************************************************************/
CStringA CFolder::ReadFromUTF8File(const char* filePath)
{
    try
    {
        if (FileExists(filePath))
        {
            CStringA utf8Tmp;
            try
            {
                CStdioFile localFile(filePath, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone);

                int nLen = (int)localFile.GetLength();
                utf8Tmp.Preallocate(nLen + 1);
                localFile.Read(utf8Tmp.GetBuffer(0), nLen);
                utf8Tmp.ReleaseBuffer(nLen);
            }
            catch (CFileException/*& fe*/)
            {
                OutputDebugString("hth >> CFolder::ReadFromUTF8File - CFileException\n");
                //fe;

                return "";
            }

            int utf8Offset = 0;
            if (0xef == (unsigned char)utf8Tmp[0] && 0xbb == (unsigned char)utf8Tmp[1] && 0xbf == (unsigned char)utf8Tmp[2]) utf8Offset = 3;

            int num = MultiByteToWideChar(CP_UTF8, NULL, utf8Tmp.GetBuffer(0) + utf8Offset, -1, NULL, NULL);
            wchar_t* buffw = new wchar_t[num];
            MultiByteToWideChar(CP_UTF8, NULL, utf8Tmp.GetBuffer(0) + utf8Offset, -1, buffw, num);

            CStringA ansiTmp;
            int len = WideCharToMultiByte(CP_ACP, 0, buffw, num - 1, NULL, NULL, NULL, NULL); 
            ansiTmp.Preallocate(len + 1);
            WideCharToMultiByte(CP_ACP, 0, buffw, num - 1, ansiTmp.GetBuffer(0), len, NULL, NULL);
            ansiTmp.ReleaseBuffer(len);
            delete[] buffw;

            return ansiTmp;
        }
    }
    catch (...)
    {
        OutputDebugString("hth >> CFolder::ReadFromUTF8File - Exception\n");
    }

    return "";
}

/**************************************************************************
*   Function:		GetFileList
*   Description:    ��ȡ�ļ����µ��ļ��б�
*   Input:          char* folderPath : �ļ���·����
*   Output:         list<FileInfo>& fileList : �ļ����µ��ļ��б�
*   Return:			true - �����ɹ�
*					false - ����ʧ��
**************************************************************************/
bool CFolder::GetFileList(const char* folderPath, list<FileInfo>& fileList)
{
    CStringA folderBase(FormatFolderPath(folderPath));

    stack<string> stackSubFolder;
    stackSubFolder.push("");
    
    CStringA tmpPath, relativePath, fullPath;
    tmpPath.Preallocate(128);
    relativePath.Preallocate(128);
    fullPath.Preallocate(128);

    CFileFind finder;
    FileInfo fi;
    fi.RelativeFilePath.Preallocate(128);
    while ((int)stackSubFolder.size() > 0)
    {
        relativePath = stackSubFolder.top().c_str();
        fullPath = folderBase + relativePath + "*.*";
        stackSubFolder.pop();
        
        BOOL bFind = finder.FindFile(fullPath);
        while (bFind)
        {
            bFind = finder.FindNextFile();

            if (finder.IsDots() || finder.IsSystem() || finder.IsHidden())
            {
                continue;
            }

            if (finder.IsDirectory())
            {
                tmpPath = finder.GetFilePath();
                tmpPath = tmpPath.Right(tmpPath.GetLength() - folderBase.GetLength());
                tmpPath.AppendChar('\\');

                stackSubFolder.push(tmpPath.GetBuffer(0));
            }
            else
            {
                fi.RelativeFilePath = relativePath + finder.GetFileName();
                fi.FileSize = (DWORD)finder.GetLength();
                fileList.push_back(fi);
            }
        }
    }
    finder.Close();

    return ((int)fileList.size() > 0);
}
/**************************************************************************
*   Function:		GetExecutionRoot
*   Description:    ��ȡִ���ļ�Ŀ¼��
*   Input:          (null)
*   Output:         (null)
*   Return:			ִ���ļ�Ŀ¼
**************************************************************************/
CStringA CFolder::GetExecutionRoot(void)
{
    CStringA rootPath;
    rootPath.Preallocate(128);
    DWORD nLength;
    if (0 != (nLength = GetModuleFileNameA(NULL, rootPath.GetBuffer(0), 128 - 1)))
    {
        rootPath.ReleaseBuffer(nLength);
        int idx = rootPath.ReverseFind('\\');
        rootPath = rootPath.Left(idx + 1);
    }

    return rootPath;
}
/**************************************************************************
*   Function:		GetExecutionFileName
*   Description:    ��ȡִ���ļ�·����
*   Input:          (null)
*   Output:         (null)
*   Return:			ִ���ļ�·��
**************************************************************************/
CStringA CFolder::GetExecutionFileName(void)
{
    CStringA filePath;
    filePath.Preallocate(128);
    DWORD nLength;
    if (0 != (nLength = GetModuleFileNameA(NULL, filePath.GetBuffer(0), 128 - 1)))
    {
        filePath.ReleaseBuffer(nLength);
    }

    return filePath;
}
/**************************************************************************
*   Function:		GetFileInfo
*   Description:    ��ȡ�ļ���Ϣ��
*   Input:          char* folderPath : �ļ�·����
*   Output:         (null)
*   Return:			CFolder::FileInfo �ļ���Ϣ
**************************************************************************/
CFolder::FileInfo CFolder::GetFileInfo(const char* filePath)
{
    FileInfo fi;

    if (NULL != filePath)
    {
        CFileFind finder;
        if (finder.FindFile(FormatFilePath(filePath)))
        {
            finder.FindNextFile();
            fi.FileSize = (DWORD)finder.GetLength();
            fi.RelativeFilePath = finder.GetFileName();
        }
    }

    return fi;
}
/**************************************************************************
*   Function:		DeleteFolder
*   Description:    ɾ���ļ��С�
*   Input:          char* folder : �ļ���·����
*   Output:         (null)
*   Return:			true - �����ɹ�
*					false - ����ʧ��
**************************************************************************/
bool CFolder::DeleteFolder(const char* folder)
{
    bool rtn = false;

    if (NULL != folder)
    {
        rtn = true;

        CStringA folderPath(FormatFolderPath(folder));

        CFileFind finder;
        BOOL bFind = finder.FindFile(folderPath + "*.*");
        while (bFind)
        {
            bFind = finder.FindNextFile();

            if (finder.IsDots())
            {
                continue;
            }

            if (finder.IsDirectory())
            {
                if (false == DeleteFolder(finder.GetFilePath()))
                {
                    rtn = false;
                    break;
                }
            }
            else
            {
                if (FALSE == DeleteFileA(finder.GetFilePath()))
                {
                    rtn = false;
                    break;
                }
            }
        }
        finder.Close();

        if (true == rtn)
        {
            rtn = (TRUE == RemoveDirectoryA(folderPath));
        }
    }

    return rtn;
}
/**************************************************************************
*   Function:		IsFilePath
*   Description:    ��֤�ļ�·���Ƿ���Ч��
*   Input:          char* folderPath : �ļ���·����
*   Output:         (null)
*   Return:			true - �����ɹ�
*					false - ����ʧ��
**************************************************************************/
bool CFolder::IsFilePath(const char* filePath)
{
    return (!GetFileName(filePath).IsEmpty());
}

/**************************************************************************
*   Function:		MoveFile
*   Description:    �ƶ��ļ���������Ŀ���ļ���
*   Input:          char* filePath : ���ƶ��ļ���·����
                    char* targetPath : �ƶ����·����
*   Output:         (null)
*   Return:			true - �����ɹ�
*					false - ����ʧ��
**************************************************************************/
bool CFolder::MoveFile(const char* filePath, const char* targetPath)
{
    if (NULL != filePath && NULL != targetPath)
    {
        if (FileExists(filePath) && (!FileExists(targetPath) || true == DeleteFile(targetPath)))
        {
            return (TRUE == ::MoveFileA(filePath, targetPath));
        }
    }

    return false;
}
