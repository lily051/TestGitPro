#pragma once

class StringChange
{
public:
	StringChange(void);
	~StringChange(void);
	//½«TCHAR×ªÎªchar     
	static void TcharToChar(const TCHAR * tchar, char * _char)  
	{  
		int iLength ;  
		iLength = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);    
		WideCharToMultiByte(CP_ACP, 0, tchar, -1, _char, iLength, NULL, NULL);   
	} 
	static void CharToTchar (const char * _char, TCHAR * tchar)  
	{  
		int iLength ;  

		iLength = MultiByteToWideChar (CP_ACP, 0, _char, strlen (_char) + 1, NULL, 0) ;  
		MultiByteToWideChar (CP_ACP, 0, _char, strlen (_char) + 1, tchar, iLength) ;  
	} 
	static void CStringToChar(const CString &temp, char *result)
	{
		int len = WideCharToMultiByte(CP_ACP, 0, temp, -1, NULL, 0, NULL, NULL);
		WideCharToMultiByte(CP_ACP, 0, temp, -1, result, len + 1, NULL, NULL);
	}
	
};
