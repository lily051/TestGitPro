/**************************************************************************
*   Copyright 2007-2008 Hikvision Digital Technology Co., Ltd.
*   FileName:       Crypto.cpp
*   Description:    helper for string encryption and decryption
*   Author:         JiangTianjing (jiangtj@hikvision.com)
*   Date:           2008-11-19
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         2008-11-19      JiangTianjing   created
**************************************************************************/

#include "StdAfx.h"
#include ".\crypto.h"

///////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////

CCrypto::CCrypto(void)
: m_hKey(NULL)
, m_hProv(NULL)
, m_hHash(NULL)
{
	// the password
    m_pszKey = "IVMS8100: Etms";

	// acquire a handle to a particular key container 
	// within a particular cryptographic service provider (CSP). 

	// The best way to acquire a context is to try to open the container. 
	// If this attempt fails with "NTE_BAD_KEYSET", 
	// then create the container by using the CRYPT_NEWKEYSET flag.

	if (!CryptAcquireContext(&m_hProv, "8100 Etms", NULL, PROV_RSA_FULL, 0))
	{
		if (GetLastError() == NTE_BAD_KEYSET)
		{
			CryptAcquireContext(&m_hProv, "8100 Etms", NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET);
		}
	}

	if (ERROR_SUCCESS == GetLastError())
	{
		// handle to a CSP hash object

		if (CryptCreateHash(m_hProv, CALG_MD5, 0, 0, &m_hHash))
		{
			//  adds data to a specified hash object

			if (CryptHashData(m_hHash, (BYTE*)m_pszKey, (DWORD)strlen(m_pszKey), 0))
			{
				// generates cryptographic session keys derived from a base data value

				CryptDeriveKey(m_hProv, CALG_RC4, m_hHash, CRYPT_EXPORTABLE, &m_hKey);
			}
		}
	}
}

CCrypto::~CCrypto(void)
{
	if (m_hKey)
	{
		CryptDestroyKey(m_hKey);	//  releases the handle
	}

	if (m_hHash)
	{
		CryptDestroyHash(m_hHash);	//  destroys the hash object
	}

	// releases the handle of a cryptographic service provider (CSP) and a key container

	if (m_hProv)
	{
		CryptReleaseContext(m_hProv, 0); 
	}
}

/**************************************************************************
*   Function:		Encrypt
*   Description:    encrypt the pbBuff and return the real data len after encrypted.
*   Input:          BYTE * pbBuff: the buffer wanted to be encrypted.
*					DWORD dwBuffLen: the buffer data len want
*   Output:         DWORD *pDwDataLen: the actual encrypted data len is returned.
*   Return:			If the function succeeds, the return value is TRUE. 
*					If the function fails, the return value is FALSE.
**************************************************************************/
BOOL CCrypto::Encrypt(BYTE * pbBuff, DWORD dwBuffLen, DWORD *pDwDataLen)
{
	// call the CryptoAPI
	return CryptEncrypt(m_hKey, 0, TRUE, 0, pbBuff, pDwDataLen, dwBuffLen);
}

/**************************************************************************
*   Function:		Decrypt
*   Description:    decrypts the data previously encrypted
*   Input:          BYTE * pbBuff: the encrypted data buffer		
*   Output:         DWORD *pDwDataLen: indicating the length of the pbData buffer
*					after decrypted, contains the number of bytes of the decrypted plaintext.
*   Return:			return void
**************************************************************************/
BOOL CCrypto::Decrypt(BYTE * pbBuff, DWORD *pDwDataLen)
{
	// call the CryptoAPI
	return CryptDecrypt(m_hKey, 0, TRUE, 0, pbBuff, pDwDataLen);
}
