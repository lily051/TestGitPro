#include "stdafx.h"
#include "./md5.h"

/**************************************************************************
*   Copyright 2011-2012 Hikvision Digital Technology Co., Ltd.
*   FileName:       Encrypt.cpp
*   Description:    �ַ������ܽ���
*   Author:         yudan(yudan@hikvision.com.cn)
*   Date:           2011-1-15 10:05
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         2011-1-15 10:08      yudan   created
**************************************************************************/



//�ַ�������
CString EncryptForString(CString strS)
{
	strS.ReleaseBuffer();
	CString strD;
    BSTR bTemp = strS.AllocSysString();
    _bstr_t bstr = bTemp;
    char * pS = bstr;
	for (int i=0; i<strS.GetLength(); i++)
	{
		char singleChar = pS[i];  //��ȡ�ַ����е�i���ַ�
		int singleInt = (int)singleChar;  //�ַ�iת��ΪASCII��
		int nHun = singleInt/100;  //ȡ���λ��
		int nTen = (singleInt%100)/10;  //ȡ��ʮλ��
		int nBit = singleInt%10;  //ȡ���λ��
		int nSumHT = nHun + nTen;  //�ֱ�����2λ��֮��
		int nSumTB = nTen + nBit;
		int nSumHB = nHun + nBit;
		int nNewInt = nSumHT*10000 + nSumTB*100 + nSumHB;  //�����ʹ��������һ���µ���
		if (i%2 == 0)  //����Ϊ ���ı�ֵ
		{
			nNewInt = nNewInt^0x0000ffff;
		}
		CString strSingle;
		strSingle.Format(L"%d", nNewInt);  //ת��Ϊ�ַ���
		int nLengthToSix = 6-strSingle.GetLength();  //�ַ���λ����6���
		for (int j=0; j<nLengthToSix; j++)  //�ַ�������6λ�� ǰ��0
		{
			strSingle = L"0" + strSingle;
		}

		strD += strSingle;
	}
	::SysFreeString(bTemp);

	return strD;
}

//�ַ�������
CString DecryptForString(CString strS)
{
	strS.ReleaseBuffer();
	CStringW strD;
	int nStrNum = strS.GetLength()/6;  //6�������ַ���ʶ1�������ַ�
	for (int i=0; i<nStrNum; i++)
	{
		CStringW strTemp;
		strTemp = strS.Left(6);  //ȡ6���ַ���  ��6���ַ���������Ϊһ���ַ�
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