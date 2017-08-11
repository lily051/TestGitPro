#pragma once

class CPublicLibraryMgr
{
public:
    CPublicLibraryMgr(CString strPath);
    ~CPublicLibraryMgr( void);

private:
	typedef struct _library_info
	{
		CString strFile;		// ¬∑æ∂
		HINSTANCE hInstance;

		_library_info()
		{
			hInstance = NULL;
		}
	}LIBRARY_INFO;
	std::map<int, LIBRARY_INFO> m_mapLibrary;

    void LoadDll(CString& strPath);
    void FreeDll();

    // ∂¡»°DLL≈‰÷√
    void LoadConfig(CString& strPath);
};
