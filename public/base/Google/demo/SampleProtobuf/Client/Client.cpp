// Client.cpp : 定义控制台应用程序的入口点。
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

	//发送数据
	int count=0;
	while(count < 20)
	{
		// 创建套节字
		SOCKET s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(s == INVALID_SOCKET)
		{
			return 0;
		}
		// 也可以在这里调用bind函数绑定一个本地地址,否则系统将会自动安排
		// 填写远程地址信息
		sockaddr_in servAddr; 
		servAddr.sin_family = AF_INET;
		servAddr.sin_port = htons(4567);
		// 注意，这里要填写服务器程序所在机器的IP地址
		// 如果你的计算机没有联网，直接使用127.0.0.1即可
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
		//暂不考虑拆包封包问题
		::send(s,sDevInfo.c_str(),dev1.ByteSize(),0);
		std::cout << dev1.id() <<"  " << dev1.name() << std::endl;   
		Sleep(100);

		// 关闭套节字
		::closesocket(s);
		++count;
	}

	::WSACleanup();	

	system("Pause");

	return 0;
}

