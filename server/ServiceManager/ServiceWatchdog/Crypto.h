/**************************************************************************
*   Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
*   FileName:       Crypto.h
*   Description:    helper for string encryption and decryption
*   Author:         JiangTianjing (jiangtj@hikvision.com)
*   Date:           2008-11-19
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         2008-11-19      JiangTianjing   created
**************************************************************************/

#pragma once

#include <Wincrypt.h>



class CCrypto
{
public:
	CCrypto(void);
	~CCrypto(void);

BOOL Encrypt(BYTE * pbBuff, DWORD dwBuffLen, DWORD *pDwDataLen);
BOOL Decrypt(BYTE * pbBuff, DWORD *pDwDataLen);

protected:
	HCRYPTKEY m_hKey;  // handle to key
	
	HCRYPTPROV m_hProv; // handle to a key container
	HCRYPTHASH m_hHash;	// the hash data handle

	// the secret password
	char *m_pszKey;
};
