#pragma once
//
// Copyright (c) 2009 Hikvision Digital Technology Co., Ltd.
//
// Summary:
//     Folder ��⡣
//
// Modification History:
//   2009-08-14:
//     V1.0 / ��ͨ�� / ����
//
#include <list>
#include <string>
using namespace std;

class CFolder
{
public:
    CFolder(void);
    ~CFolder(void);

public:
    typedef struct _FileInfo_
    {
        CStringA RelativeFilePath;
        DWORD FileSize;
    }FileInfo;

public:
    //
    // Summary:
    //     ��� folder �Ƿ���ڣ����������򴴽���
    //
    static bool CheckFolder(const char* path);
    //
    // Summary:
    //     ��ʽ���ļ���·����
    //
    static CStringA FormatFolderPath(const char* folderPath);
    //
    // Summary:
    //     ��ʽ���ļ�·����
    //
    static CStringA FormatFilePath(const char* filePath);
    //
    // Summary:
    //     ����ļ��Ƿ���ڡ�
    //
    static bool FileExists(const char* filePath);
    //
    // Summary:
    //     ɾ��ָ���ļ���
    //
    static bool DeleteFile(const char* filePath);
    //
    // Summary:
    //     ����ָ���ļ����ļ�����
    //
    static bool RenameFile(const char* filePath, const char* renameFilePath);
    //
    // Summary:
    //     ��ȡ�ļ�·���е��ļ�����
    //
    static CStringA GetFileName(const char* filePath);
    //
    // Summary:
    //     �� ansi �ַ����洢�� utf8 ��ʽ�ļ���
    //
    static bool SaveToUTF8File(const char* content, const char* filePath);
    //
    // Summary:
    //     ��ȡ utf8 ��ʽ�ļ�����ת���� ansi �ַ�����
    //
    static CStringA ReadFromUTF8File(const char* filePath);
    //
    // Summary:
    //     ��ȡ�ļ����µ��ļ��б�
    //
    static bool GetFileList(const char* folderPath, list<FileInfo>& fileList);
    //
    // Summary:
    //     ��ȡִ���ļ�Ŀ¼��
    //
    static CStringA GetExecutionRoot(void);
    //
    // Summary:
    //     ��ȡִ���ļ�·����
    //
    static CStringA GetExecutionFileName(void);
    //
    // Summary:
    //     ��ȡ�ļ���Ϣ��
    //
    static FileInfo GetFileInfo(const char* filePath);
    //
    // Summary:
    //     ɾ���ļ��С�
    //
    static bool DeleteFolder(const char* folder);
    //
    // Summary:
    //     ��֤�ļ�·���Ƿ���Ч��
    //
    static bool IsFilePath(const char* filePath);
    //
    // Summary:
    //     �ƶ��ļ���������Ŀ���ļ�
    //
    static bool MoveFile(const char* filePath, const char* targetPath);
};
