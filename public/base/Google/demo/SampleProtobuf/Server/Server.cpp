// Server.cpp : �������̨Ӧ�ó������ڵ㡣
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

	// �����׽���
	SOCKET sListen = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sListen == INVALID_SOCKET)
	{
		return 0;
	}

	// ���sockaddr_in�ṹ
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(4567);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;

	// ������׽��ֵ�һ�����ص�ַ
	if(::bind(sListen, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		return 0;
	}

	// �������ģʽ
	if(::listen(sListen, SOMAXCONN) == SOCKET_ERROR)
	{
		return 0;
	}

	// ѭ�����ܿͻ�����������
	sockaddr_in remoteAddr; 
	int nAddrLen = sizeof(remoteAddr);

	while(TRUE)
	{
		SOCKET sClient;
		// ����һ��������
		sClient = ::accept(sListen, (SOCKADDR*)&remoteAddr, &nAddrLen);
		if(sClient == INVALID_SOCKET)
		{
			continue;
		}
		char buff[1024];
		int nRecv = ::recv(sClient, buff, 1024, 0);
		//�ݲ����ǲ���������
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
		// �ر�ͬ�ͻ��˵�����
		::closesocket(sClient);
	}

	// �رռ����׽���
	::closesocket(sListen);


	::WSACleanup();	

	return 0;
}

