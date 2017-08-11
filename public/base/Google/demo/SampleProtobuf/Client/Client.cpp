// Client.cpp : �������̨Ӧ�ó������ڵ㡣
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

	//��������
	int count=0;
	while(count < 20)
	{
		// �����׽���
		SOCKET s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(s == INVALID_SOCKET)
		{
			return 0;
		}
		// Ҳ�������������bind������һ�����ص�ַ,����ϵͳ�����Զ�����
		// ��дԶ�̵�ַ��Ϣ
		sockaddr_in servAddr; 
		servAddr.sin_family = AF_INET;
		servAddr.sin_port = htons(4567);
		// ע�⣬����Ҫ��д�������������ڻ�����IP��ַ
		// �����ļ����û��������ֱ��ʹ��127.0.0.1����
		servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
		if(::connect(s, (sockaddr*)&servAddr, sizeof(servAddr)) == -1)
		{
			return 0;
		}

		std::string sDevInfo = "";
		tutorial::Device dev1; 
		dev1.set_id(count+8100);
		dev1.set_name("HIK-Dev1");
		if (!dev1.SerializeToString(&sDevInfo)) 
		{ 
			std::cerr << "Failed to SerializeToString." << std::endl;
		}
		//�ݲ����ǲ���������
		::send(s,sDevInfo.c_str(),dev1.ByteSize(),0);
		std::cout << dev1.id() <<"  " << dev1.name() << std::endl;   
		Sleep(100);

		// �ر��׽���
		::closesocket(s);
		++count;
	}

	::WSACleanup();	

	system("Pause");

	return 0;
}

