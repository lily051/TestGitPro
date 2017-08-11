#ifdef _UPLOADLOG_FTPHTTPUPLOAD_H
#define _UPLOADLOG_FTPHTTPUPLOAD_H
#endif //_UPLOADLOG_FTPHTTPUPLOAD_H

#pragma once
#include "../curl/curl.h"

typedef struct DataStruct
{
	curl_off_t totalSize;      //�ļ��ܴ�С
	curl_off_t nowSize;        //���ϴ���С
	DOUBLE process;            //����
	HWND m_hwnd;               //���  
	int point;                 //�ļ����±�
}DataStruct;

class FtpHttpUpload
{
public:
	CURL *curlhandle;
	HWND m_hwnd;
	int  m_Speed;
	char m_url[MAX_PATH];
	char m_localpath[MAX_PATH];
	char userpwd[MAX_PATH];
	int  m_point;
	DataStruct m_dataStruct;
public:
	FtpHttpUpload(void);
    int FtpUpload(const char * url, const char * localpath,long timeout, long tries,const char *userpwd);
	int HttpUpload(const char * url, const char * localpath, long timeout, long tries,const char *filename);
	~FtpHttpUpload(void);
};
