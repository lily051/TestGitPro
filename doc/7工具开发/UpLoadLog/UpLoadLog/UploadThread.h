#pragma once

class UploadThread :public CWinThread
{
	DECLARE_DYNCREATE(UploadThread)
public:
	HWND m_hwnd;
	char m_url[MAX_PATH];
	char m_localpath[MAX_PATH];
	char userpwd[MAX_PATH];
	char m_filename[MAX_PATH];
	int  number;
	LONG timeout;
	LONG tries;
public:
	UploadThread(void);
	~UploadThread(void);
public:  
	virtual BOOL InitInstance();  
	virtual int ExitInstance();  
public:
	void HttpUpload(WPARAM wParam, LPARAM lParam);
	void FtpUpload(WPARAM wParam, LPARAM lParam);
protected:  
	DECLARE_MESSAGE_MAP()
};
