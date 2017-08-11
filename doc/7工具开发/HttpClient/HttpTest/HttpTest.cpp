// HttpTest.cpp : 定义控制台应用程序的入口点。
//
#include <stdio.h>
#include <iostream>
#include "../HttpClientAPI.h"
int main()
{
	std::string strUrl = "http://10.10.48.101/randomCode.jpg";
	std::string strResponse;
	//Http_Client_Get(strUrl, strResponse);
	std::string strFilepath("E:\\12345.jpg");
	Http_Client_Download(strUrl, strFilepath);
	Http_Client_Fini();
	std::cout<<strResponse<<std::endl;
	getchar();
	return 0;
}

