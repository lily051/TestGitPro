#pragma once

/**************************************************************************
*   Copyright 2011-2012 Hikvision Digital Technology Co., Ltd.
*   FileName:       Encrypt.h
*   Description:    �ַ������ܽ���
*   Author:         yudan(yudan@hikvision.com.cn)
*   Date:           2011-1-15 10:05
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         2011-1-15 10:05      yudan   created
**************************************************************************/

/************************************************************************/
/* �����㷨EncryptForString
	1.�����ַ���string���ܣ�ȡstring��ÿ���ַ�����ÿ���ַ�p�ֱ����
	2.�����ַ�p�ļ��ܣ����Ȼ�ȡp��ASCII�룬��ȡ���ȡASCII��İ�λ��Hʮλ��T�͸�λ��B 3����
	3.��ȡ��3����������ӳ�3���µ��� s=H+T, m=T+B, n=H+B
	4.�����µ��������·�ʽ���һ���µ�����6λ��(������λΪ0����smn
	5.����ַ����ַ�����λ��Ϊ��λ��λ�� ��smn��0x0000ffff��������㣬��ȡ�µ�ֵ
	6.����ֵת��Ϊ6λ���ַ���
	7.�����е����ַ����ܺ���ַ���ƴ�ӣ���õ��ַ�����Ϊ�����ַ���
*/
/************************************************************************/

//�ַ�������
CString EncryptForString(CString strS);

//�ַ�������
CString DecryptForString(CString strS);

string CalculateMD5(LPCWSTR wsMessage);