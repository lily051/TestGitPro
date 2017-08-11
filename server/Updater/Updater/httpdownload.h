#ifndef __HTTP_DOWNLOAD_DOMAIN
#define __HTTP_DOWNLOAD_DOMAIN

#include <string>
#include "curl/curl.h"

class httpdownload
{
public:
	httpdownload();
	~httpdownload();
	bool DownloadFile(std::string url,std::string path, int speed = 1024, HWND hWnd = NULL);	//kbyte/s
	HWND m_hWnd;

private:
	static size_t DownloadCallback(void* pBuffer, size_t nSize, size_t nMemByte, void* pParam);
	static int ProgressCallback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);
};

#endif