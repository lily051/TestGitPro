//
// Copyright (c) 2009 Hikvision Digital Technology Co., Ltd.
//
// FileName:
//     Folder.cpp
//
// Summary:
//     Folder 检测。
//
// Modification History:
//   2009-08-14:
//     V1.0 / 胡通海 / 创建
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
*   Description:    检查 folder 是否存在，若不存在则创建。
*   Input:          char* path : 文件夹路径。
*   Output:         (null)
*   Return:			true - 操作成功
*					false - 操作失败
**************************************************************************/
bool CFolder::CheckFolder(const char* path)
{
    return (TRUE == MakeSureDirectoryPathExists(FormatFolderPath(path)));
}

/**************************************************************************
*   Function:		ValidateLogin
*   Description:    格式化文件夹路径。
*   Input:          char* folderPath : 文件夹路径。
*   Output:         (null)
*   Return:			true - 操作成功
*					false - 操作失败
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
*   Description:    格式化文件路径。
*   Input:          char* folderPath : 文件夹路径。
*   Output:         (null)
*   Return:			true - 操作成功
*					false - 操作失败
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
*   Description:    检查文件是否存在。
*   Input:          char* filePath : 文件路径。
*   Output:         (null)
*   Return:			true - 文件存在
*					false - 文件不存在
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
*   Description:     删除指定文件。
*   Input:          char* filePath : 文件路径。
*   Output:         (null)
*   Return:			true - 操作成功
*					false - 操作失败
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
*   Description:    更改指定文件的文件名。
*   Input:          char* filePath : 文件路径。
                    char* renameFilePath : 重命名后的文件路径
*   Output:         (null)
*   Return:			true - 操作成功
*					false - 操作失败
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
*   Description:    获取文件路径中的文件名。
*   Input:          char* filePath : 文件路径。
*   Output:         (null)
*   Return:			文件路径中的文件名
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
*   Description:    将 ansi 字符串存储成 utf8 格式文件。
*   Input:          char* content : 文件内容。
                    char* filePath : 文件路径。
*   Output:         (null)
*   Return:			true - 操作成功
*					false - 操作失败
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
*   Description:     读取 utf8 格式文件，并转换成 ansi 字符串。
*   Input:          char* filePath : 文件路径。
*   Output:         (null)
*   Return:			 utf8 格式文件转换成 ansi 字符串
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
*   Description:    获取文件夹下的文件列表
*   Input:          char* folderPath : 文件夹路径。
*   Output:         list<FileInfo>& fileList : 文件夹下的文件列表
*   Return:			true - 操作成功
*					false - 操作失败
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
*   Description:    获取执行文件目录。
*   Input:          (null)
*   Output:         (null)
*   Return:			执行文件目录
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
*   Description:    获取执行文件路径。
*   Input:          (null)
*   Output:         (null)
*   Return:			执行文件路径
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
*   Description:    获取文件信息。
*   Input:          char* folderPath : 文件路径。
*   Output:         (null)
*   Return:			CFolder::FileInfo 文件信息
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
*   Description:    删除文件夹。
*   Input:          char* folder : 文件夹路径。
*   Output:         (null)
*   Return:			true - 操作成功
*					false - 操作失败
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
*   Description:    验证文件路径是否有效。
*   Input:          char* folderPath : 文件夹路径。
*   Output:         (null)
*   Return:			true - 操作成功
*					false - 操作失败
**************************************************************************/
bool CFolder::IsFilePath(const char* filePath)
{
    return (!GetFileName(filePath).IsEmpty());
}

/**************************************************************************
*   Function:		MoveFile
*   Description:    移动文件，并覆盖目标文件。
*   Input:          char* filePath : 待移动文件的路径。
                    char* targetPath : 移动后的路径。
*   Output:         (null)
*   Return:			true - 操作成功
*					false - 操作失败
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
