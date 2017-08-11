#pragma once

class CPublicLibraryMgr
{
public:
    CPublicLibraryMgr(CString strPath);
    ~CPublicLibraryMgr( void);

private:
	typedef struct _library_info
	{
		CString strFile;		// ·��
		HINSTANCE hInstance;

		_library_info()
		{
			hInstance = NULL;
		}
	}LIBRARY_INFO;
	std::map<int, LIBRARY_INFO> m_mapLibrary;

    void LoadDll(CString& strPath);
    void FreeDll();

    // ��ȡDLL����
    void LoadConfig(CString& strPath);
};
