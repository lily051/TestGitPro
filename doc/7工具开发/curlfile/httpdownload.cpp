#include "stdafx.h"
#include "httpdownload.h"
#include <iostream>

httpdownload::httpdownload(bool* cancel)
{
	cancel_ = cancel;
	nPos = 0;
}

httpdownload::~httpdownload()
{
}

size_t httpdownload::DownloadCallback(void* pBuffer, size_t nSize, size_t nMemByte, void* pParam)  
{  
	FILE* fp = (FILE*)pParam;  
	size_t nWrite = fwrite(pBuffer, nSize, nMemByte, fp);  

	return nWrite;  
}  

int httpdownload::ProgressCallback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)  
{  
	httpdownload* dd = (httpdownload*)clientp;

	if ( dltotal > -0.1 && dltotal < 0.1 )  
	{
		return 0;
	}
	dd->nPos = (int) ( (dlnow/dltotal)*100 );  
	//֪ͨ�������������ؽ���
	//std::cout << "dltotal: " << (long)dltotal << " ---- dlnow:" << (long)dlnow << std::endl;
	std::cout << "npos: " << dd->nPos << std::endl;
	if(*dd->cancel_)
	{
		//1. ���ط�0ֵ�ͻ���ֹ curl_easy_perform ִ��
		return -2;
	}
	return 0;  
}

bool httpdownload::DownloadFile(std::string URLADDR, std::wstring path, int speed/* = 1024*/)
{
	//��ʼ��curl  
	CURL *curl = curl_easy_init();  
	curl_easy_setopt(curl, CURLOPT_URL, URLADDR.c_str());  

	//���ý������ݵĻص� 
	FILE* file = _wfopen(path.c_str(), L"wb");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, DownloadCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA,file);
	curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 5);  
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);  
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);  
	curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, ProgressCallback);
	curl_easy_setopt(curl, CURLOPT_PROGRESSDATA,this);
	curl_easy_setopt(curl, CURLOPT_MAX_RECV_SPEED_LARGE, (curl_off_t)1024*speed);	// bytes/s
	CURLcode retcCode = curl_easy_perform(curl);

	const char* pError = curl_easy_strerror(retcCode);
	std::cout << "pError: " << pError << std::endl;
	fclose(file);
	//����curl����ǰ��ĳ�ʼ��ƥ��  
	curl_easy_cleanup(curl);

	return !retcCode;
}