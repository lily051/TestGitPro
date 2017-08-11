#include "DemoSockUtilsClass.h"
#include "HPR_Sock_Utils.h"
#include "HPR_Socket.h"
#include "HPR_Thread.h"
#include "HPR_Utils.h"
#include "DemoApp.h"

#include <iostream>
using namespace std;


void i_test::i_tcp_client::Test()
{
	//connect_timeout_0();
	//connect_timeout_3s();
	//connect_timeout_infinte();


	//send_timeout_infinite();
	//send_timeout_0();

	//sendn_timeout_infinite();
	//sendn_timeout_3s();

	//recv_timeout_0();
	//recv_timeout_3s();
	//recv_timeout_infinite();

	//recvn_timeout_0();
	//recvn_timeout_3s();
	//recvn_timeout_infinite();

}

void i_test::i_tcp_client::connect_timeout_0()
{
	hpr_int32 ret;
	hpr::hpr_net_addr remote;
	hpr_sock_t sock;

	remote.set_addr("172.7.10.200:10000");

	sock = hpr::hpr_sock_utils::socket(AF_INET, SOCK_STREAM, 0);
	if (sock == HPR_INVALID_SOCKET)
	{
		cout << ITEST_FAIL << "connect_timeout_0 hpr::hpr_sock_utils::socket" << endl;
		return;
	}

	hpr::hpr_sock_utils::set_nonblock(sock, 1);
	ret = hpr::hpr_sock_utils::connect(sock, remote, 0);
	if (ret == -1)
	{
		cout << ITEST_FAIL << "connect_timeout_0 hpr::hpr_sock_utils::connect" << endl;
	}
	else 
	{
		cout << ITEST_PASS << "connect_timeout_0" << endl;
	}
	hpr::hpr_sock_utils::set_nonblock(sock, 0);

	hpr::hpr_sock_utils::shutdown(sock, SHUT_RDWR);
	hpr::hpr_sock_utils::closesocket(sock);
	sock = HPR_INVALID_SOCKET;

	return;
}

void i_test::i_tcp_client::connect_timeout_infinte()
{
	hpr_int32 ret;
	hpr::hpr_net_addr remote;
	hpr_sock_t sock;

	remote.set_addr("172.7.10.200:10000");

	sock = hpr::hpr_sock_utils::socket(AF_INET, SOCK_STREAM, 0);
	if (sock == HPR_INVALID_SOCKET)
	{
		cout << ITEST_FAIL << "connect_timeout_infinte hpr::hpr_sock_utils::socket" << endl;
		return;
	}

	ret = hpr::hpr_sock_utils::set_nonblock(sock, 1);
	ret = hpr::hpr_sock_utils::connect(sock, remote, -1);
	if (ret == -1)
	{
		cout << ITEST_FAIL << "connect_timeout_infinte hpr::hpr_sock_utils::connect" << endl;
	}
	else 
	{
		cout << ITEST_PASS << "connect_timeout_infinte" << endl;
	}
 
	hpr::hpr_sock_utils::set_nonblock(sock, 0);
	hpr::hpr_sock_utils::shutdown(sock, SHUT_RDWR);
	hpr::hpr_sock_utils::closesocket(sock);
	sock = HPR_INVALID_SOCKET;

	return;
}

void i_test::i_tcp_client::connect_timeout_3s()
{
	hpr_int32 ret;
	hpr::hpr_net_addr remote;
	hpr_sock_t sock;

	remote.set_addr("172.7.10.200:10000");

	sock = hpr::hpr_sock_utils::socket(AF_INET, SOCK_STREAM, 0);
	if (sock == HPR_INVALID_SOCKET)
	{
		cout << ITEST_FAIL << "connect_timeout_3s hpr::hpr_sock_utils::socket" << endl;
		return;
	}

	hpr::hpr_sock_utils::set_nonblock(sock, 1);
	ret = hpr::hpr_sock_utils::connect(sock, remote, 3000);
	if (ret == -1)
	{
		cout << ITEST_FAIL << "connect_timeout_3s hpr::hpr_sock_utils::connect" << endl;
	}
	else 
	{
		cout << ITEST_PASS << "connect_timeout_3s" << endl;
	}
	hpr::hpr_sock_utils::set_nonblock(sock, 0);

	hpr::hpr_sock_utils::shutdown(sock, SHUT_RDWR);
	hpr::hpr_sock_utils::closesocket(sock);
	sock = HPR_INVALID_SOCKET;

	return;
}

//////////////////////////////////////////////////////////////////////////
void i_test::i_tcp_client::send_timeout_infinite()
{
	int sendlen = 0;
	int recvlen = 0;
	hpr::hpr_net_addr remote;
	char sndbuf[5];
	char rcvbuf[5];
	
	remote.set_addr("172.7.10.200:10000");

	hpr_sock_t sock = hpr::hpr_sock_utils::socket(AF_INET, SOCK_STREAM, 0);

	hpr::hpr_sock_utils::set_nonblock(sock, 1);
	if ( -1 == hpr::hpr_sock_utils::connect(sock, remote, -1))
	{
		cout << ITEST_FAIL << "hpr::hpr_sock_utils::connect" << endl;
	}
	//cout << ITEST_PASS << "hpr::hpr_sock_utils::connect timeout infinite" << endl;

	sndbuf[0] = '0';
	sndbuf[1] = '1';
	sndbuf[2] = '2';
	sndbuf[3] = '3';
	sndbuf[4] = '\0';

	sendlen = hpr::hpr_sock_utils::send(sock, sndbuf, 4);
	if (sendlen == 4)
	{
		recvlen = hpr::hpr_sock_utils::recvn(sock, rcvbuf, 4);
		if (recvlen > 0)
		{
			rcvbuf[recvlen] = 0;
			if (recvlen == sendlen && strcmp(sndbuf, rcvbuf) == 0)
			{
				cout << ITEST_PASS << "hpr::hpr_sock_utils::send send_timeout_infinite" << endl;
			}
			else
			{
				cout << ITEST_FAIL << "hpr::hpr_sock_utils::send send_timeout_infinite" << endl;
			}
		}

	}

	hpr::hpr_sock_utils::shutdown(sock, SHUT_RDWR);
	hpr::hpr_sock_utils::closesocket(sock);

}

void i_test::i_tcp_client::send_timeout_0()
{
	int sendlen = 0;
	int recvlen = 0;
	hpr::hpr_net_addr remote;
	char sndbuf[5];
	char rcvbuf[5];

	remote.set_addr("172.7.10.200:10000");

	hpr_sock_t sock = hpr::hpr_sock_utils::socket(AF_INET, SOCK_STREAM, 0);

	hpr::hpr_sock_utils::set_nonblock(sock, 1);
	if ( -1 == hpr::hpr_sock_utils::connect(sock, remote, -1))
	{
		cout << ITEST_FAIL << "send_timeout_0s hpr::hpr_sock_utils::connect" << endl;
	}

	sndbuf[0] = '0';
	sndbuf[1] = '1';
	sndbuf[2] = '2';
	sndbuf[3] = '3';

	sendlen = hpr::hpr_sock_utils::send(sock, sndbuf, 4, 0);
	if (sendlen > 0)
	{
		recvlen = hpr::hpr_sock_utils::recvn(sock, rcvbuf, sendlen);
		if (recvlen > 0)
		{
			cout << ITEST_PASS << "hpr::hpr_sock_utils::send send_timeout_0s" << endl;
		}
		else
		{
			cout << ITEST_FAIL << "hpr::hpr_sock_utils::send send_timeout_0s" << endl;
		}
	}
	else
	{
		cout << ITEST_PASS << "hpr::hpr_sock_utils::send send_timeout_0s" << endl;
	}

	hpr::hpr_sock_utils::shutdown(sock, SHUT_RDWR);
	hpr::hpr_sock_utils::closesocket(sock);

}


void i_test::i_tcp_client::send_timeout_3s()
{
	int sendlen = 0;
	int recvlen = 0;
	hpr::hpr_net_addr remote;
	char sndbuf[5];
	char rcvbuf[5];

	remote.set_addr("172.7.10.200:10000");

	hpr_sock_t sock = hpr::hpr_sock_utils::socket(AF_INET, SOCK_STREAM, 0);

	hpr::hpr_sock_utils::set_nonblock(sock, 1);

	if ( -1 == hpr::hpr_sock_utils::connect(sock, remote, -1))
	{
		cout << ITEST_FAIL << "send_timeout_3s hpr::hpr_sock_utils::connect" << endl;
	}

	sndbuf[0] = '0';
	sndbuf[1] = '1';
	sndbuf[2] = '2';
	sndbuf[3] = '3';

	sendlen = hpr::hpr_sock_utils::send(sock, sndbuf, 4, 0);
	if (sendlen > 0)
	{
		recvlen = hpr::hpr_sock_utils::recv(sock, rcvbuf, sizeof(rcvbuf));
		if (recvlen > 0)
		{
			cout << ITEST_PASS << "hpr::hpr_sock_utils::send send_timeout_3s" << endl;
		}
		else
		{
			cout << ITEST_FAIL << "hpr::hpr_sock_utils::send send_timeout_3s" << endl;
		}
	}
	else
	{
		cout << ITEST_PASS << "hpr::hpr_sock_utils::send send_timeout_3s" << endl;
	}

	hpr::hpr_sock_utils::shutdown(sock, SHUT_RDWR);
	hpr::hpr_sock_utils::closesocket(sock);

}

//////////////////////////////////////////////////////////////////////////
void i_test::i_tcp_client::sendn_timeout_infinite()
{
	int sendlen = 0;
	//int recvlen = 0;
	hpr::hpr_net_addr remote;
	const int LEN = 1024*1024*200;
	char *sndbuf = new char[LEN];

	remote.set_addr("172.7.10.200:10000");

	hpr_sock_t sock = hpr::hpr_sock_utils::socket(AF_INET, SOCK_STREAM, 0);

	hpr::hpr_sock_utils::set_nonblock(sock, 1);
	if ( -1 == hpr::hpr_sock_utils::connect(sock, remote, -1))
	{
		cout << ITEST_FAIL << "sendn_timeout_3s hpr::hpr_sock_utils::connect" << endl;
		hpr::hpr_sock_utils::closesocket(sock);
		delete []sndbuf;
		return;
	}

	sendlen = hpr::hpr_sock_utils::sendn(sock, sndbuf, LEN, -1);
	if (sendlen > 0)
	{
		cout << ITEST_PASS << "hpr::hpr_sock_utils::send sendn_timeout_infinite" << endl;
		cout << "sendlen is " << sendlen << endl;
		cout << "need send len = " << LEN << endl;
	}
	else
	{
		cout << ITEST_PASS << "hpr::hpr_sock_utils::send sendn_timeout_infinite" << endl;
	}

	hpr::hpr_sock_utils::shutdown(sock, SHUT_RDWR);
	hpr::hpr_sock_utils::closesocket(sock);
	delete []sndbuf;
}

void i_test::i_tcp_client::sendn_timeout_3s()
{
	int sendlen = 0;
	//int recvlen = 0;
	hpr::hpr_net_addr remote;
	const int LEN = 1024*1024*200;
	char *sndbuf = new char[LEN];

	remote.set_addr("172.7.10.200:10000");

	hpr_sock_t sock = hpr::hpr_sock_utils::socket(AF_INET, SOCK_STREAM, 0);

	hpr::hpr_sock_utils::set_nonblock(sock, 1);
	if ( -1 == hpr::hpr_sock_utils::connect(sock, remote, -1))
	{
		cout << ITEST_FAIL << "sendn_timeout_3s hpr::hpr_sock_utils::connect" << endl;
		hpr::hpr_sock_utils::closesocket(sock);
		delete []sndbuf;
		return;
	}

	sendlen = hpr::hpr_sock_utils::sendn(sock, sndbuf, LEN, 30000);
	if (sendlen > 0)
	{
		cout << ITEST_PASS << "hpr::hpr_sock_utils::send send_timeout_3s" << endl;
		cout << "in 300ms sendlen is " << sendlen << endl;
		cout << "need send len = " << LEN << endl;
	}
	else
	{
		cout << ITEST_PASS << "hpr::hpr_sock_utils::send sendn_timeout_3s" << endl;
	}

	hpr::hpr_sock_utils::shutdown(sock, SHUT_RDWR);
	hpr::hpr_sock_utils::closesocket(sock);
	delete []sndbuf;
	
}

//////////////////////////////////////////////////////////////////////////
void i_test::i_tcp_client::recv_timeout_0()
{
	int sendlen = 0;
	int recvlen = 0;
	hpr::hpr_net_addr remote;
	char sndbuf[5];
	char rcvbuf[5];

	remote.set_addr("172.7.10.200:10000");

	hpr_sock_t sock = hpr::hpr_sock_utils::socket(AF_INET, SOCK_STREAM, 0);

	hpr::hpr_sock_utils::set_nonblock(sock, 1);
	if ( -1 == hpr::hpr_sock_utils::connect(sock, remote, -1))
	{
		cout << ITEST_FAIL << "recv_timeout_0 hpr::hpr_sock_utils::connect" << endl;
	}

	sndbuf[0] = '0';
	sndbuf[1] = '1';
	sndbuf[2] = '2';
	sndbuf[3] = '3';

	sendlen = hpr::hpr_sock_utils::send(sock, sndbuf, 4, -1);
	if (sendlen > 0)
	{
		HPR_Sleep(1000);
		recvlen = hpr::hpr_sock_utils::recv(sock, rcvbuf, sendlen, 0);
		if (recvlen >= 0)
		{
			cout << ITEST_PASS << "hpr::hpr_sock_utils::send recv_timeout_0" << endl;
		}
		else
		{	
			cout << ITEST_FAIL << "hpr::hpr_sock_utils::send recv_timeout_0" << endl;
		}
	}
	else
	{
		cout << ITEST_PASS << "hpr::hpr_sock_utils::send recv_timeout_0" << endl;
	}

	hpr::hpr_sock_utils::shutdown(sock, SHUT_RDWR);
	hpr::hpr_sock_utils::closesocket(sock);	
}

void i_test::i_tcp_client::recv_timeout_3s()
{
	int sendlen = 0;
	int recvlen = 0;
	hpr::hpr_net_addr remote;
	char sndbuf[5];
	char rcvbuf[5];

	remote.set_addr("172.7.10.200:10000");

	hpr_sock_t sock = hpr::hpr_sock_utils::socket(AF_INET, SOCK_STREAM, 0);

	hpr::hpr_sock_utils::set_nonblock(sock, 1);
	if ( -1 == hpr::hpr_sock_utils::connect(sock, remote, -1))
	{
		cout << ITEST_FAIL << "recv_timeout_3s hpr::hpr_sock_utils::connect" << endl;
	}

	sndbuf[0] = '0';
	sndbuf[1] = '1';
	sndbuf[2] = '2';
	sndbuf[3] = '3';

	sendlen = hpr::hpr_sock_utils::send(sock, sndbuf, 4, -1);
	if (sendlen > 0)
	{
		recvlen = hpr::hpr_sock_utils::recv(sock, rcvbuf, sendlen, 3000);
		if (recvlen >= 0)
		{
			cout << ITEST_PASS << "hpr::hpr_sock_utils::send recv_timeout_3s" << endl;
		}
		else
		{	
			cout << ITEST_FAIL << "hpr::hpr_sock_utils::send recv_timeout_3s" << endl;
		}
	}
	else
	{
		cout << ITEST_PASS << "hpr::hpr_sock_utils::send recv_timeout_3s" << endl;
	}

	hpr::hpr_sock_utils::shutdown(sock, SHUT_RDWR);
	hpr::hpr_sock_utils::closesocket(sock);	
}

void i_test::i_tcp_client::recv_timeout_infinite()
{
	int sendlen = 0;
	int recvlen = 0;
	hpr::hpr_net_addr remote;
	char sndbuf[5];
	char rcvbuf[5];

	remote.set_addr("172.7.10.200:10000");

	hpr_sock_t sock = hpr::hpr_sock_utils::socket(AF_INET, SOCK_STREAM, 0);

	hpr::hpr_sock_utils::set_nonblock(sock, 1);
	if ( -1 == hpr::hpr_sock_utils::connect(sock, remote, -1))
	{
		cout << ITEST_FAIL << "recv_timeout_infinite hpr::hpr_sock_utils::connect" << endl;
	}

	sndbuf[0] = '0';
	sndbuf[1] = '1';
	sndbuf[2] = '2';
	sndbuf[3] = '3';

	sendlen = hpr::hpr_sock_utils::send(sock, sndbuf, 4, -1);
	if (sendlen > 0)
	{
		recvlen = hpr::hpr_sock_utils::recv(sock, rcvbuf, sendlen, -1);
		if (recvlen >= 0)
		{
			cout << ITEST_PASS << "hpr::hpr_sock_utils::send recv_timeout_infinite" << endl;
		}
		else
		{	
			cout << ITEST_FAIL << "hpr::hpr_sock_utils::send recv_timeout_infinite" << endl;
			cout << "errno = " << WSAGetLastError() << endl;
		}
	}
	else
	{
		cout << ITEST_PASS << "hpr::hpr_sock_utils::send recv_timeout_infinite" << endl;
	}

	hpr::hpr_sock_utils::shutdown(sock, SHUT_RDWR);
	hpr::hpr_sock_utils::closesocket(sock);	
}

void i_test::i_tcp_client::recvn_timeout_0()
{
	int sendlen = 0;
	int recvlen = 0;
	hpr::hpr_net_addr remote;
	char sndbuf[5];
	char rcvbuf[5];

	remote.set_addr("172.7.10.200:10000");

	hpr_sock_t sock = hpr::hpr_sock_utils::socket(AF_INET, SOCK_STREAM, 0);

	hpr::hpr_sock_utils::set_nonblock(sock, 1);
	if ( -1 == hpr::hpr_sock_utils::connect(sock, remote, -1))
	{
		cout << ITEST_FAIL << "recvn_timeout_0 hpr::hpr_sock_utils::connect" << endl;
	}

	sndbuf[0] = '0';
	sndbuf[1] = '1';
	sndbuf[2] = '2';
	sndbuf[3] = '3';

	sendlen = hpr::hpr_sock_utils::send(sock, sndbuf, 4, -1);
	if (sendlen > 0)
	{
		recvlen = hpr::hpr_sock_utils::recvn(sock, rcvbuf, sendlen, 0);
		if (recvlen >= 0)
		{
			cout << ITEST_PASS << "hpr::hpr_sock_utils::send recvn_timeout_0" << endl;
			//cout << "recvlen = " << recvlen << endl;	
		}
		else
		{	
			cout << ITEST_FAIL << "hpr::hpr_sock_utils::send recvn_timeout_0" << endl;
		}
	}
	else
	{
		cout << ITEST_PASS << "hpr::hpr_sock_utils::send recvn_timeout_0" << endl;
	}

	hpr::hpr_sock_utils::shutdown(sock, SHUT_RDWR);
	hpr::hpr_sock_utils::closesocket(sock);	
}

void i_test::i_tcp_client::recvn_timeout_3s()
{
	int sendlen = 0;
	int recvlen = 0;
	hpr::hpr_net_addr remote;
	char sndbuf[5];
	char rcvbuf[5];

	remote.set_addr("172.7.10.200:10000");

	hpr_sock_t sock = hpr::hpr_sock_utils::socket(AF_INET, SOCK_STREAM, 0);

	hpr::hpr_sock_utils::set_nonblock(sock, 1);
	if ( -1 == hpr::hpr_sock_utils::connect(sock, remote, -1))
	{
		cout << ITEST_FAIL << "recvn_timeout_3s hpr::hpr_sock_utils::connect" << endl;
	}

	sndbuf[0] = '0';
	sndbuf[1] = '1';
	sndbuf[2] = '2';
	sndbuf[3] = '3';

	sendlen = hpr::hpr_sock_utils::send(sock, sndbuf, 4, -1);
	if (sendlen > 0)
	{
		recvlen = hpr::hpr_sock_utils::recvn(sock, rcvbuf, sendlen, 3000);
		if (recvlen >= 0)
		{
			cout << ITEST_PASS << "hpr::hpr_sock_utils::send recvn_timeout_3s" << endl;
			//cout << "recvlen = " << recvlen << endl;	
		}
		else
		{	
			cout << ITEST_FAIL << "hpr::hpr_sock_utils::send recvn_timeout_3s" << endl;
		}
	}
	else
	{
		cout << ITEST_PASS << "hpr::hpr_sock_utils::send recvn_timeout_3s" << endl;
	}

	hpr::hpr_sock_utils::shutdown(sock, SHUT_RDWR);
	hpr::hpr_sock_utils::closesocket(sock);	
}

void i_test::i_tcp_client::recvn_timeout_infinite()
{
	int sendlen = 0;
	int recvlen = 0;
	hpr::hpr_net_addr remote;
	char sndbuf[5];
	char rcvbuf[5];

	remote.set_addr("172.7.10.200:10000");

	hpr_sock_t sock = hpr::hpr_sock_utils::socket(AF_INET, SOCK_STREAM, 0);

	hpr::hpr_sock_utils::set_nonblock(sock, 1);
	if ( -1 == hpr::hpr_sock_utils::connect(sock, remote, -1))
	{
		cout << ITEST_FAIL << "recvn_timeout_infinite hpr::hpr_sock_utils::connect" << endl;
	}

	sndbuf[0] = '0';
	sndbuf[1] = '1';
	sndbuf[2] = '2';
	sndbuf[3] = '3';

	sendlen = hpr::hpr_sock_utils::send(sock, sndbuf, 4, -1);
	if (sendlen > 0)
	{
		recvlen = hpr::hpr_sock_utils::recvn(sock, rcvbuf, sendlen, -1);
		if (recvlen >= 0)
		{
			cout << ITEST_PASS << "hpr::hpr_sock_utils::send recvn_timeout_infinite" << endl;
			//cout << "recvlen = " << recvlen << endl;	
		}
		else
		{	
			cout << ITEST_FAIL << "hpr::hpr_sock_utils::send recvn_timeout_infinite" << endl;
		}
	}
	else
	{
		cout << ITEST_PASS << "hpr::hpr_sock_utils::send recvn_timeout_infinite" << endl;
	}

	hpr::hpr_sock_utils::shutdown(sock, SHUT_RDWR);
	hpr::hpr_sock_utils::closesocket(sock);	
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


void i_test::i_tcp_server::Test()
{
	cout << "+++Test hpr::hpr_sock_utils" << endl;

	m_sockfd = HPR_INVALID_SOCKET;

	start();
	stop();
}

void* CALLBACK i_test::i_tcp_server::accept_svc(void* param)
{
	if (param == NULL)
	{
		return 0;
	}

	i_test::i_tcp_server* server = (i_test::i_tcp_server*)(param);

	server->accept();

	return 0;
}

void* CALLBACK i_test::i_tcp_server::echo_svc(void* param)
{
	if (param == NULL)
	{
		return 0;
	}

	hpr_sock_t* sock = (hpr_sock_t*)(param);

	echo(*sock);

	delete sock;
	sock = NULL;

	return 0;
}

void i_test::i_tcp_server::start()
{
	m_sockfd = hpr::hpr_sock_utils::socket(AF_INET, SOCK_STREAM, 0);
	if (m_sockfd == HPR_INVALID_SOCKET)
	{
		cout << ITEST_FAIL << "hpr::hpr_sock_utils::socket" << endl;
		return;
	}
	cout << ITEST_PASS << "hpr::hpr_sock_utils::socket" << endl;

	hpr::hpr_net_addr local;
	local.set_port(10000);

	if (-1 == hpr::hpr_sock_utils::bind(m_sockfd, local))
	{
		cout << ITEST_FAIL << "hpr::hpr_sock_utils::bind" << endl;
		hpr::hpr_sock_utils::closesocket(m_sockfd);
		m_sockfd = HPR_INVALID_SOCKET;
		return;
	}
	cout << ITEST_PASS << "hpr::hpr_sock_utils::bind" << endl;

	if (-1 == hpr::hpr_sock_utils::listen(m_sockfd, 5))
	{
		cout << ITEST_FAIL << "hpr::hpr_sock_utils::listen" << endl;
		hpr::hpr_sock_utils::closesocket(m_sockfd);
		m_sockfd = HPR_INVALID_SOCKET;
		return;
	}
	cout << ITEST_PASS << "hpr::hpr_sock_utils::listen" << endl;

	m_hThreadId = HPR_Thread_Create(accept_svc, this, 0);

	return;
}

void i_test::i_tcp_server::stop()
{
	for (;;)
	{
		cout << "press any key to finish test tcp server case" << endl;
		cin.get();
		break;
	}

	hpr::hpr_sock_utils::shutdown(m_sockfd, SHUT_RD);
	hpr::hpr_sock_utils::closesocket(m_sockfd);
	m_sockfd = HPR_INVALID_SOCKET;

	HPR_Thread_Wait(m_hThreadId);

	return;
}

void i_test::i_tcp_server::accept()
{
	timeval tv = {5, 0};
	hpr_sock_t sock;
	hpr_int32 ret = 0;
	hpr::hpr_net_addr client;

	for (;;)
	{
		tv.tv_sec = 5;
		tv.tv_usec = 0;
		ret = hpr::hpr_sock_utils::is_rd(m_sockfd, &tv);
		if (ret > 0)
		{
			sock = hpr::hpr_sock_utils::accept(m_sockfd, client, -1);
			if (sock != HPR_INVALID_SOCKET)
			{
				cout << ITEST_PASS << "hpr::hpr_sock_utils::accept" << endl;
				hpr_sock_t* s = new hpr_sock_t;
				*s = sock;
				HPR_ThreadDetached_Create(echo_svc, (void*)(s), 0);
			}
			else
			{
				//cout << "accept failed ,quit" <<endl;
				break;
			}
		}
		else if (ret < 0)
		{
			break;
		}
		else
		{
			cout << "accept timeout , wait continue" <<endl;
		}

	}
}

void i_test::i_tcp_server::echo(hpr_sock_t sock)
{
	timeval tv = {5, 0};
	int recvlen = 0;
	char buf[1024];

	for (;;)
	{
		if (hpr::hpr_sock_utils::is_rd(sock, &tv) > 0)
		{
			recvlen = hpr::hpr_sock_utils::recv(sock, buf, sizeof(buf));
			buf[recvlen] = 0;
			hpr::hpr_sock_utils::sendn(sock, buf, recvlen);
		}
		else 
		{
			break;
		}
	}


	return;
}
