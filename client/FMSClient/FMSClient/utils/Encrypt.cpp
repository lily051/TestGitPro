#include "stdafx.h"
#include "./md5.h"

/**************************************************************************
*   Copyright 2011-2012 Hikvision Digital Technology Co., Ltd.
*   FileName:       Encrypt.cpp
*   Description:    字符串加密解密
*   Author:         yudan(yudan@hikvision.com.cn)
*   Date:           2011-1-15 10:05
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         2011-1-15 10:08      yudan   created
**************************************************************************/



//字符串加密
CString EncryptForString(CString strS)
{
	strS.ReleaseBuffer();
	CString strD;
    BSTR bTemp = strS.AllocSysString();
    _bstr_t bstr = bTemp;
    char * pS = bstr;
	for (int i=0; i<strS.GetLength(); i++)
	{
		char singleChar = pS[i];  //获取字符串中第i个字符
		int singleInt = (int)singleChar;  //字符i转化为ASCII码
		int nHun = singleInt/100;  //取其百位数
		int nTen = (singleInt%100)/10;  //取其十位数
		int nBit = singleInt%10;  //取其个位数
		int nSumHT = nHun + nTen;  //分别求其2位数之和
		int nSumTB = nTen + nBit;
		int nSumHB = nHun + nBit;
		int nNewInt = nSumHT*10000 + nSumTB*100 + nSumHB;  //三个和从左到右组成一个新的数
		if (i%2 == 0)  //奇数为 异或改变值
		{
			nNewInt = nNewInt^0x0000ffff;
		}
		CString strSingle;
		strSingle.Format(L"%d", nNewInt);  //转化为字符串
		int nLengthToSix = 6-strSingle.GetLength();  //字符串位数与6差几个
		for (int j=0; j<nLengthToSix; j++)  //字符串不足6位， 前补0
		{
			strSingle = L"0" + strSingle;
		}

		strD += strSingle;
	}
	::SysFreeString(bTemp);

	return strD;
}

//字符串解密
CString DecryptForString(CString strS)
{
	strS.ReleaseBuffer();
	CStringW strD;
	int nStrNum = strS.GetLength()/6;  //6个加密字符标识1个解密字符
	for (int i=0; i<nStrNum; i++)
	{
		CStringW strTemp;
		strTemp = strS.Left(6);  //取6个字符，  这6个字符将被解密为一个字符
		strS = strS.Right(strS.GetLength()-6);
		int nSingle = _wtoi(strTemp.GetBuffer());
		if (i%2 == 0)
		{
			nSingle = nSingle^0x0000ffff;
		}
		int nSumHT = nSingle/10000;
		int nSumTB = (nSingle%10000)/100;
		int nSumHB = nSingle%100;
		int nHun = (nSumHT + nSumHB - nSumTB)/2;
		int nTen = (nSumHT + nSumTB - nSumHB)/2;
		int nBit = (nSumHB + nSumTB - nSumHT)/2;
		nSingle = nHun*100 + nTen*10 + nBit;
		char sSingleChar = (char)nSingle;
		strTemp.Format(L"%c", sSingleChar);

		strD += strTemp;
	}

	return strD;
}

string CalculateMD5(LPCWSTR wsMessage)
{
    wstring wsPsw = wsMessage;
    string sPws = CP936_W2A(wsPsw);
    char * pInput = new char[sPws.length() + 1];
    if (pInput)
    {
        strncpy(pInput, sPws.c_str(), sPws.length() + 1);    
        string sMd5 = MD5String(pInput);
        delete [] pInput;
        return sMd5;
    }else
    {
        //CLIENT_ERROR("MD5: out of memory");
        return "";
    }
}