// Server.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <WinSock2.h>
#pragma comment(lib,"WS2_32")
#include "../message/tutorial.Device.pb.h"
#include <string>
#include <iostream>

#if _DEBUG
#pragma comment(lib,"../protobuf-2.4.1/lib_d/libprotobuf.lib")
#else
#pragma comment(lib,"../protobuf-2.4.1/lib_r/libprotobuf.lib")
#endif

int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2,2);
	if(::WSAStartup(sockVersion, &wsaData) != 0)
	{
		exit(0);
	}

	// 创建套节字
	SOCKET sListen = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sListen == INVALID_SOCKET)
	{
		return 0;
	}

	// 填充sockaddr_in结构
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(4567);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;

	// 绑定这个套节字到一个本地地址
	if(::bind(sListen, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		return 0;
	}

	// 进入监听模式
	if(::listen(sListen, SOMAXCONN) == SOCKET_ERROR)
	{
		return 0;
	}

	// 循环接受客户的连接请求
	sockaddr_in remoteAddr; 
	int nAddrLen = sizeof(remoteAddr);

	while(TRUE)
	{
		SOCKET sClient;
		// 接受一个新连接
		sClient = ::accept(sListen, (SOCKADDR*)&remoteAddr, &nAddrLen);
		if(sClient == INVALID_SOCKET)
		{
			continue;
		}
		char buff[1024];
		int nRecv = ::recv(sClient, buff, 1024, 0);
		//暂不考虑拆包封包问题
		if(nRecv > 0)
		{
			tutorial::Device dev1; 
			buff[nRecv]='\0';
			std::string sDevInfo = buff;
			if (dev1.ParseFromString(sDevInfo)) 
			{ 
				std::cout << dev1.id() <<"  " << dev1.name() << std::endl;    
			}  
		}
		// 关闭同客户端的连接
		::closesocket(sClient);
	}

	// 关闭监听套节字
	::closesocket(sListen);


	::WSACleanup();	

	return 0;
}

