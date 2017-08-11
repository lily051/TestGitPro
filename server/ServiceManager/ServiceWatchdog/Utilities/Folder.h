#pragma once
//
// Copyright (c) 2009 Hikvision Digital Technology Co., Ltd.
//
// Summary:
//     Folder 检测。
//
// Modification History:
//   2009-08-14:
//     V1.0 / 胡通海 / 创建
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
    //     检查 folder 是否存在，若不存在则创建。
    //
    static bool CheckFolder(const char* path);
    //
    // Summary:
    //     格式化文件夹路径。
    //
    static CStringA FormatFolderPath(const char* folderPath);
    //
    // Summary:
    //     格式化文件路径。
    //
    static CStringA FormatFilePath(const char* filePath);
    //
    // Summary:
    //     检查文件是否存在。
    //
    static bool FileExists(const char* filePath);
    //
    // Summary:
    //     删除指定文件。
    //
    static bool DeleteFile(const char* filePath);
    //
    // Summary:
    //     更改指定文件的文件名。
    //
    static bool RenameFile(const char* filePath, const char* renameFilePath);
    //
    // Summary:
    //     获取文件路径中的文件名。
    //
    static CStringA GetFileName(const char* filePath);
    //
    // Summary:
    //     将 ansi 字符串存储成 utf8 格式文件。
    //
    static bool SaveToUTF8File(const char* content, const char* filePath);
    //
    // Summary:
    //     读取 utf8 格式文件，并转换成 ansi 字符串。
    //
    static CStringA ReadFromUTF8File(const char* filePath);
    //
    // Summary:
    //     获取文件夹下的文件列表。
    //
    static bool GetFileList(const char* folderPath, list<FileInfo>& fileList);
    //
    // Summary:
    //     获取执行文件目录。
    //
    static CStringA GetExecutionRoot(void);
    //
    // Summary:
    //     获取执行文件路径。
    //
    static CStringA GetExecutionFileName(void);
    //
    // Summary:
    //     获取文件信息。
    //
    static FileInfo GetFileInfo(const char* filePath);
    //
    // Summary:
    //     删除文件夹。
    //
    static bool DeleteFolder(const char* folder);
    //
    // Summary:
    //     验证文件路径是否有效。
    //
    static bool IsFilePath(const char* filePath);
    //
    // Summary:
    //     移动文件，并覆盖目标文件
    //
    static bool MoveFile(const char* filePath, const char* targetPath);
};
