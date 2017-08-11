#pragma once

/**************************************************************************
*   Copyright 2011-2012 Hikvision Digital Technology Co., Ltd.
*   FileName:       Encrypt.h
*   Description:    字符串加密解密
*   Author:         yudan(yudan@hikvision.com.cn)
*   Date:           2011-1-15 10:05
*   Modification History:
*    <version>      <time>          <author>        <desc>
*       1.0         2011-1-15 10:05      yudan   created
**************************************************************************/

/************************************************************************/
/* 加密算法EncryptForString
	1.对于字符串string加密，取string的每个字符，对每个字符p分别加密
	2.对于字符p的加密，首先获取p的ASCII码，获取后获取ASCII码的百位数H十位数T和个位数B 3个数
	3.获取的3个数两两相加成3个新的数 s=H+T, m=T+B, n=H+B
	4.三个新的数以如下方式组成一个新的至少6位数(允许首位为0）：smn
	5.如果字符在字符串中位置为奇位数位置 则将smn与0x0000ffff做异或运算，获取新的值
	6.将数值转化为6位的字符串
	7.将所有单个字符加密后的字符串拼接，获得的字符串即为加密字符。
*/
/************************************************************************/

//字符串加密
CString EncryptForString(CString strS);

//字符串解密
CString DecryptForString(CString strS);

string CalculateMD5(LPCWSTR wsMessage);