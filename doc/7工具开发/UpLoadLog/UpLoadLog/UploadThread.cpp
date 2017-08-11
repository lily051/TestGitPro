#include "StdAfx.h"
#include "UploadThread.h"
IMPLEMENT_DYNCREATE(UploadThread, CWinThread) 
BEGIN_MESSAGE_MAP(UploadThread, CWinThread)  
	ON_THREAD_MESSAGE(WM_FTPUPLOAD, FtpUpload)  
	ON_THREAD_MESSAGE(WM_HTTPUPLOAD, HttpUpload)  
END_MESSAGE_MAP()

UploadThread::UploadThread(void)
{
	m_hwnd = NULL;
	m_url[MAX_PATH] = NULL;
	m_localpath[MAX_PATH] = NULL;
	userpwd[MAX_PATH] = NULL;
	m_filename[MAX_PATH] = NULL;
	timeout = 1;
	tries = 3;
}

UploadThread::~UploadThread(void)
{
}
BOOL UploadThread::InitInstance()  
{  
	// TODO:  perform and per-thread initialization here  
	return TRUE;  
}  

int UploadThread::ExitInstance()  
{  
	// TODO:  perform any per-thread cleanup here  
	return CWinThread::ExitInstance();  
}

void UploadThread::HttpUpload(WPARAM wParam, LPARAM lParam)  
{  
	FtpHttpUpload *ftp = new FtpHttpUpload;
	ftp->m_hwnd = m_hwnd;
	ftp->m_point = number;
	ftp->HttpUpload(m_url, m_localpath,1,3,m_filename);
	//ftp->HttpUpload(m_url, m_localpath,1,3,"Log.zip");
	//ftp->HttpUpload("http://localhost:9001/HTTPÉÏ´«/","D:\\DB_Download.exe",1,3,"DB_Download.exe");
}

void UploadThread::FtpUpload(WPARAM wParam, LPARAM lParam)
{
	FtpHttpUpload *ftp = new FtpHttpUpload;
	ftp->m_hwnd = m_hwnd;
	ftp->m_point = number;
	ftp->FtpUpload(m_url, m_localpath,1,4,"");
	/*ftp->FtpUpload("ftp://10.16.36.110:14147/LocalUser/Public/Log20161020093839.zip",
		"D:\\VSTEST\\UpLoadLog\\Release\\Log20161020093839.zip",1,4,"FTP:1111");*/
}