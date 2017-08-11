#pragma once

class coding
{
private:
	char* pResult;
    wchar_t *pwText;  
public:
	coding(void);
	~coding(void);
	inline char* UnicodeToAnsi( const wchar_t* szStr )
	{	
		int nLen = WideCharToMultiByte( CP_ACP, 0, szStr, -1, NULL, 0, NULL, NULL );
		if (nLen == 0)
		{
			return NULL;
		}
		if( NULL == pResult)
		{
			pResult = new char[nLen];
		}
		WideCharToMultiByte( CP_ACP, 0, szStr, -1, pResult, nLen, NULL, NULL );
		return pResult;
	}


    //ANSIתunicode  
    inline wchar_t* AnsiToUnicode(const char *str)  
    {  
        DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, str, -1, NULL, 0);  
        if(NULL == pwText)
        {
            pwText = new wchar_t[dwNum]; 
        }
        
        if(!pwText)  
        {  
            delete []pwText;  
        }  
        MultiByteToWideChar (CP_ACP, 0, str, -1, pwText, dwNum);  
        return pwText;  
    }
};
