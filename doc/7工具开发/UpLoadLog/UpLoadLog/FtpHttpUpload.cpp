#include "StdAfx.h"
#include "FtpHttpUpload.h"
#include "../curl/curl.h"
#include <sys/stat.h>
#include <string>
#include <assert.h>
using namespace std;
//初始化Curl库
FtpHttpUpload::FtpHttpUpload(void)
{
	userpwd[MAX_PATH] = NULL;
	curlhandle = NULL;
	curl_global_init(CURL_GLOBAL_ALL);
	m_url[MAX_PATH] = NULL;
	m_localpath[MAX_PATH] = NULL;
	userpwd[MAX_PATH] = NULL;
	curlhandle = curl_easy_init();
	memset(&m_dataStruct,0,sizeof(m_dataStruct));
}
//反初始化
FtpHttpUpload::~FtpHttpUpload(void)
{
	 curl_easy_cleanup(curlhandle);
	 curl_global_cleanup();
}
size_t GetcontentLengthFunc(void *ptr, size_t size, size_t nmemb, void *stream) 
{
	int r;
	long len = 0;
	r = sscanf((const char*)ptr, "Content-Length: %ld\n", &len);
	if (r)
		*((long *) stream) = len;
	return size * nmemb;
}
size_t ReadFunc(void *ptr, size_t size, size_t nmemb, void *stream)
{
	FILE *f = (FILE*)stream;
	size_t n;
	if (ferror(f))
		return CURL_READFUNC_ABORT;
	n = fread(ptr, size, nmemb, f) * size;
	return n;
}
size_t DiscardFunc(void *ptr, size_t size, size_t nmemb, void *stream) 
{
	return size * nmemb;
}
size_t ProcessCallBack(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
	DataStruct *dataStruct = (DataStruct*)clientp;
	if(ulnow == ultotal)
		return 0;
	dataStruct->nowSize = ulnow;
	//dataStruct->totalSize = ultotal;
	dataStruct->process = (DOUBLE)dlnow / (DOUBLE)dataStruct->totalSize;
	SendMessage(dataStruct->m_hwnd,WM_USER,WM_UPLOAD_PROCESS,(LPARAM)dataStruct);

	return 0;
}

//HTTP上传函数
int FtpHttpUpload::HttpUpload(const char * url, const char * localpath, long timeout, long tries,const char *filename)
{
	CURL *curl;  
	CURLcode res;  
	struct curl_httppost *formpost=NULL;  
	struct curl_httppost *lastptr=NULL;  
	struct curl_slist *headerlist=NULL;  
	static const char buf[] = "Expect:";  
	curl_global_init(CURL_GLOBAL_ALL);  
	curl_formadd(&formpost,  
		&lastptr,  
		CURLFORM_COPYNAME, "sendfile",  
		CURLFORM_FILE, localpath,  
		CURLFORM_END);  
	curl_formadd(&formpost,  
		&lastptr,  
		CURLFORM_COPYNAME, "filename",  
		CURLFORM_COPYCONTENTS, filename,  
		CURLFORM_END);  
	curl_formadd(&formpost,  
		&lastptr,  
		CURLFORM_COPYNAME, "submit",  
		CURLFORM_COPYCONTENTS, "Submit",  
		CURLFORM_END);  

	curl = curl_easy_init(); 
	headerlist = curl_slist_append(headerlist, buf);  
	if(curl)
	{ 
		curl_easy_setopt(curl, CURLOPT_URL, url);  
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);  
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);  
		res = curl_easy_perform(curl);  
		if(res != CURLE_OK && res != CURLE_WRITE_ERROR)
		{
			curl_easy_strerror(res);
			UPLOAD_LOG_ERROR("HTTP Upload failed,Error:%d",res);
			SendMessage(m_hwnd,WM_USER,WM_HTTPUOLOAD_FAILD,NULL);
		}
		else
		{
			UPLOAD_LOG_INFO("HTTP Upload Successfully!");
			SendMessage(m_hwnd,WM_USER,WM_HTTPLOAD_OK,NULL);
		}
		curl_easy_cleanup(curl);  
		curl_formfree(formpost);  
		curl_slist_free_all (headerlist);  
	}  
	return 0;  


}
//FTP上传函数
int FtpHttpUpload::FtpUpload(const char * url, const char * localpath, long timeout, long tries,const char *userpwd)
{
	FILE *f;
    long uploaded_len = -1;
	int use_resume = 0;
	struct stat file_info;
	//获取本地文件大小信息
	if(stat(localpath, &file_info) == 0)
	{
		uploaded_len = file_info.st_size; 
		use_resume = 1;
	}
    CURLcode r = CURLE_GOT_NOTHING;
    f = fopen(localpath, "rb");
    if (f == NULL) {
        perror(NULL);
        return 0;
    }
    curl_easy_setopt(curlhandle, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(curlhandle, CURLOPT_URL, url);
    curl_easy_setopt(curlhandle, CURLOPT_USERPWD, userpwd);   
    if (timeout)
        curl_easy_setopt(curlhandle, CURLOPT_FTP_RESPONSE_TIMEOUT, timeout);
    curl_easy_setopt(curlhandle, CURLOPT_HEADERFUNCTION, GetcontentLengthFunc);
    curl_easy_setopt(curlhandle, CURLOPT_HEADERDATA, &uploaded_len);
    curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, DiscardFunc);
    curl_easy_setopt(curlhandle, CURLOPT_READFUNCTION, ReadFunc);
    curl_easy_setopt(curlhandle, CURLOPT_READDATA, f);
    curl_easy_setopt(curlhandle, CURLOPT_FTPPORT, "-");
    curl_easy_setopt(curlhandle, CURLOPT_FTP_CREATE_MISSING_DIRS, 1L);
    curl_easy_setopt(curlhandle, CURLOPT_VERBOSE, 1L);
    //上传进度
	curl_easy_setopt(curlhandle, CURLOPT_NOPROGRESS, 0L);
	curl_easy_setopt(curlhandle, CURLOPT_XFERINFOFUNCTION, ProcessCallBack); 
	
	m_dataStruct.m_hwnd = m_hwnd;
	m_dataStruct.point = m_point;
	m_dataStruct.totalSize = uploaded_len;
	curl_easy_setopt(curlhandle, CURLOPT_XFERINFODATA, &m_dataStruct);
	//上传速度限制speed*1024KB
	//curl_easy_setopt(curlhandle, CURLOPT_MAX_SEND_SPEED_LARGE, 1024*speed); 
    for (int c = 0; (r != CURLE_OK) && (c < tries); c++) {
        if (c) {
            curl_easy_setopt(curlhandle, CURLOPT_NOBODY, 1L);
            curl_easy_setopt(curlhandle, CURLOPT_HEADER, 1L);
            r = curl_easy_perform(curlhandle);
            if (r != CURLE_OK)
                continue;
            curl_easy_setopt(curlhandle, CURLOPT_NOBODY, 0L);
            curl_easy_setopt(curlhandle, CURLOPT_HEADER, 0L);
            fseek(f, uploaded_len, SEEK_SET);
            curl_easy_setopt(curlhandle, CURLOPT_APPEND, 1L);
        }
        else {
            curl_easy_setopt(curlhandle, CURLOPT_APPEND, 0L);
        }
        r = curl_easy_perform(curlhandle);
    }
    fclose(f);
	if (r == CURLE_OK)
	{
		UPLOAD_LOG_INFO("FTP Upload Successfully!");
		SendMessage(m_hwnd,WM_USER,WM_FTPUPLOAD_OK,NULL);
        return 1;
	}
    else 
	{
		if (r == CURLE_UPLOAD_FAILED)
		{
			SendMessage(m_hwnd,WM_USER,WM_FILE_EXIST,NULL);
		}
		else
		{
			UPLOAD_LOG_ERROR("FTP Upload failed,Error: %d",r);
			SendMessage(m_hwnd,WM_USER,WM_FTPUOLOAD_FAILD,NULL);
		}
        return 0;
    }
}
