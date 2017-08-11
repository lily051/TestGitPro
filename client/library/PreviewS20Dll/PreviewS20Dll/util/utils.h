/**************************************************************************
*   Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
*   FileName:       utils.h
*   Description:    ʵ�ú���
*   Author:         JiangTianjing (jiangtj@hikvision.com)
*   Date:           2008-10-20 Monday
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         2008-10-20      JiangTianjing   created
**************************************************************************/
#pragma once
#include "TCharExpand.h"
#include <process.h>
#pragma comment(lib, "version.lib")


Image * LoadImageFromResource(UINT uID);
Bitmap * LoadBitmapFromResource(UINT uID);
HANDLE Create_File(TCHAR *csFileName);
void SetRegion(CWnd * pWnd, CBitmap & bitmap);


//ljg 2010/03/22  λͼ
HBITMAP ResourceLoad_Bmp(UINT nID);
int GetFontSize(int nPixelSize);
int GetFontSize0();
CString GetFontName();
int GetFontCharset();

//�����ļ��к��ļ�
HANDLE CreateMyFile(const CString &strFileName);
/** @fn         SaveBinaryFile
 *  @brief      �������ݵ�ָ���ļ�
 *  @param[IN]  pData: ��������
 *  @param[IN]  nDataSize: ���ݴ�С
 *  @param[IN]  strFilePath: �ļ���
 *  @return     TRUE: �ɹ���FALSE: ʧ��
 */
BOOL SaveBinaryFile(PBYTE pData, DWORD nDataSize, const CString &strFilePath);
/** @fn         SuperMakeDir
 *  @brief      ���ָ��Ŀ¼�Ƿ���ڣ�����������򴴽�����strFilePath����ʾ������Ŀ
 *  @param[IN]  strFilePath: �ļ��ľ���·��
 *  @return     TRUE: �ɹ���FALSE: ʧ��
 */
BOOL SuperMakeDir(PCSTR strFilePath);
BOOL CreateFileDir(TCHAR *csFilePath);
/** @fn         CheckDiskSpace
 *  @brief      ���ָ�������Ƿ��п��ÿռ�
 *  @param[IN]  strDisk: ָ���̷�
 *  @param[IN]  dwFreeSpace: ���ÿռ�����(MBΪ��λ)
 *  @return     TRUE: �У�FALSE: ��
 */
BOOL CheckDiskSpace(const CString &strDisk, DWORD dwFreeSpace);

// dsx unicode utf8 cp936 �ַ�����ͽ�����صĺ���---begin
int A2W_Internal(UINT CodePage,DWORD dwFlags,LPCSTR lpMultiByteStr,int cbMultiByte,LPWSTR& lpWideCharStr,DWORD* pdwError);
int W2A_Internal(UINT CodePage,DWORD dwFlags,LPCWSTR lpWideCharStr,int cchWideChar,LPSTR& lpMultiByteStr,LPCSTR lpDefaultChar,    				 LPBOOL lpUsedDefaultChar,DWORD* pdwError);
bool CP936_A2W(LPCSTR lpMultiByteStr, int cbMultiByte, LPWSTR& lpWideCharStr, int& cchWideChar, DWORD* pdwError = NULL);
bool CP936_W2A(LPCWSTR lpWideCharStr, int cchWideChar, LPSTR& lpMultiByteStr, int& cbMultiByte, DWORD* pdwError = NULL);
bool UTF8_A2W(LPCSTR lpMultiByteStr, int cbMultiByte, LPWSTR& lpWideCharStr, int& cchWideChar, DWORD* pdwError = NULL);
bool UTF8_W2A(LPCWSTR lpWideCharStr, int cchWideChar, LPSTR& lpMultiByteStr, int& cbMultiByte, DWORD* pdwError = NULL);
std::wstring CP936_A2W(const std::string& strA);
std::string CP936_W2A(const std::wstring& strW);
CStringA CStringW2A(CStringW strText);
std::string UTF8_MFCSTRW2A(const CStringW& strW);
CStringW UTF8_A2MFCSTRW(const std::string& strA);
void CodeSet_Free(void* p);
int My_atoi(CString strNum);

HWND MyCreateToolTip(int toolID, HWND hDlg,  TCHAR* pText);