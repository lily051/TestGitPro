#include "DemoApp.h"
#include "DemoClass.h"
#include <iostream>

int test_tcp_server();
int test_tcp_client();
int test_udp_normal();
int test_udp_select_ctime();
int test_udp_select_vtime();
int test_multicast();

void Test_SockUtils()
{
	//test_multicast();
	test_udp_select_ctime();
	test_udp_select_vtime();
	//test_udp_normal();
	test_tcp_client();
	test_tcp_server();
}

int test_tcp_server()
{
	return 0;
}

int test_tcp_client()
{
	return 0;
}

int test_udp_normal()
{
	hpr_int32 ret = -1;
	hpr_sock_t sockfd = HPR_INVALID_SOCKET;
	hpr::hpr_net_addr addr_local(8000);
	hpr::hpr_net_addr addr_remote;
	addr_remote.set_host("172.7.10.111");
	addr_remote.set_port(10000);
	char data[16] = {'1', '2', '3', '4', '\0'};

	sockfd = hpr::hpr_sock_utils::socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd == HPR_INVALID_SOCKET)
	{
		std::cout << "!!! socket error" << endl;
		cout << "errno = " << WSAGetLastError()  << endl;
		return -1;
	}
	
	ret = hpr::hpr_sock_utils::bind(sockfd, addr_local);
	if (ret == -1)
	{
		cout << "!!! bind error" << endl;
		return -1;
	}

	hpr::hpr_time_tick t1;
	if (5 != hpr::hpr_sock_utils::sendto(sockfd, data, 5, addr_remote))
	{
		cout << "!!! sendto error" << endl;
		cout << "errno = " << WSAGetLastError()  << endl;
	}
	hpr::hpr_time_tick t2;
	hpr_int64 tt = t2.get_time() - t1.get_time();
	cout << "sendto elapse time = ";
	//printf("%ld\n", tt);
	cout << (hpr_int32)tt << endl;

	t1.update_time();
	ret = hpr::hpr_sock_utils::recvfrom(sockfd, data, sizeof(data), addr_remote);
	if (ret > 0)
	{
		data[ret] = '\0';
		cout << "recvfrom data is " << data << endl;
	}
	else
	{
		cout << "!!! recvfrom error" << endl;
	}
	t2.update_time();
	cout << "recvfrom elapse time = ";
	//printf("%ld\n", (t2.get_time() - t1.get_time()));
	cout << (hpr_int32)(t2.get_time() - t1.get_time()) << endl;

	return 0;
}

int test_udp_select_ctime()
{
	hpr_int32 ret = -1;
	hpr_sock_t sockfd = HPR_INVALID_SOCKET;
	hpr::hpr_net_addr addr_local(8000);
	hpr::hpr_net_addr addr_remote;
	addr_remote.set_host("172.7.10.111");
	addr_remote.set_port(10000);
	char data[16] = {'1', '2', '3', '4', '\0'};
	timeval tv = {5, 0};

	sockfd = hpr::hpr_sock_utils::socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd == HPR_INVALID_SOCKET)
	{
		cout << "!!! socket error" << endl;
		cout << "errno = " << WSAGetLastError()  << endl;
		return -1;
	}

	ret = hpr::hpr_sock_utils::bind(sockfd, addr_local);
	if (ret == -1)
	{
		cout << "!!! bind error" << endl;
		return -1;
	}

	hpr::hpr_time_tick t1;
	fd_set fd_w;
	fd_set fd_r;
	FD_ZERO(&fd_w);
	FD_ZERO(&fd_r);
	FD_SET(sockfd, &fd_w);
	ret = hpr::hpr_sock_utils::select_ctime(sockfd+1, NULL, &fd_w, NULL, &tv);
	if (ret > 0 && FD_ISSET(sockfd, &fd_w))
	{
		if (5 != hpr::hpr_sock_utils::sendto(sockfd, data, 5, addr_remote))
		{
			cout << "!!! sendto error" << endl;
			cout << "errno = " << WSAGetLastError()  << endl;
		}
	}
	hpr::hpr_time_tick t2;
	//cout << "sendto elapse time = ";
	//printf("%ld\n", (t2.get_time() - t1.get_time()));
	//cout << (hpr_int32)(t2.get_time() - t1.get_time()) << endl;

	//cout << "after sendto tv is " << tv.tv_sec << "+" << tv.tv_usec << endl;

	t1.update_time();
	FD_SET(sockfd, &fd_r);
	ret = hpr::hpr_sock_utils::select_ctime(sockfd+1, &fd_r, NULL, NULL, &tv);
	if (ret > 0 && FD_ISSET(sockfd, &fd_r))
	{
		ret = hpr::hpr_sock_utils::recvfrom(sockfd, data, sizeof(data), addr_remote);
		if (ret > 0)
		{
			data[ret] = '\0';
			//cout << "recvfrom data is " << data << endl;
		}
		else
		{
			cout << "!!! recvfrom error" << endl;
		}
	}
	t2.update_time();
	//cout << "after recvfrom tv is " << tv.tv_sec << "+" << tv.tv_usec << endl;
	//cout << "recvfrom elapse time = ";
	//printf("%ld\n",  (t2.get_time() - t1.get_time()));
	//cout << (hpr_int32)(t2.get_time() - t1.get_time()) << endl;

	cout << ITEST_PASS << "select_ctime" << endl;
	
	hpr::hpr_sock_utils::shutdown(sockfd, SHUT_RDWR);
	hpr::hpr_sock_utils::closesocket(sockfd);

	return 0;
}

int test_udp_select_vtime()
{
	hpr_int32 ret = -1;
	hpr_sock_t sockfd = HPR_INVALID_SOCKET;
	hpr::hpr_net_addr addr_local(8000);
	hpr::hpr_net_addr addr_remote;
	addr_remote.set_host("172.7.10.111");
	addr_remote.set_port(10000);
	char data[16] = {'1', '2', '3', '4', '\0'};
	timeval tv = {5, 0};

	sockfd = hpr::hpr_sock_utils::socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd == HPR_INVALID_SOCKET)
	{
		cout << "!!! socket error" << endl;
		cout << "errno = " << WSAGetLastError()  << endl;
		return -1;
	}
	cout << ITEST_PASS << "socket" << endl;

	ret = hpr::hpr_sock_utils::bind(sockfd, addr_local);
	if (ret == -1)
	{
		cout << "!!! bind error" << endl;
		return -1;
	}
	cout << ITEST_PASS << "bind" << endl;

	hpr::hpr_time_tick t1;
	fd_set fd_w;
	fd_set fd_r;
	FD_ZERO(&fd_w);
	FD_ZERO(&fd_r);
	FD_SET(sockfd, &fd_w);
	ret = hpr::hpr_sock_utils::select_vtime(sockfd+1, NULL, &fd_w, NULL, &tv);
	if (ret > 0 && FD_ISSET(sockfd, &fd_w))
	{
		if (5 != hpr::hpr_sock_utils::sendto(sockfd, data, 5, addr_remote))
		{
			cout << "!!! sendto error" << endl;
			cout << "errno = " << WSAGetLastError()  << endl;
		}
		cout << ITEST_PASS << "sendto" << endl;
	}
	hpr::hpr_time_tick t2;
	hpr_int64 tt = (t2.get_time() - t1.get_time());
	//cout << "sendto elapse time = ";
	//printf("%ld\n", tt);
	//cout << (hpr_int32)(tt) << endl;
	//cout <<  "errno = " << WSAGetLastError() << endl;

	//cout << "after sendto tv is " << tv.tv_sec << "+" << tv.tv_usec << endl;

	t1.update_time();
	FD_SET(sockfd, &fd_r);
	ret = hpr::hpr_sock_utils::select_vtime(sockfd+1, &fd_r, NULL, NULL, &tv);
	if (ret > 0 && FD_ISSET(sockfd, &fd_r))
	{
		ret = hpr::hpr_sock_utils::recvfrom(sockfd, data, sizeof(data), addr_remote);
		if (ret > 0)
		{
			data[ret] = '\0';
			cout << ITEST_PASS << "recvfrom" << endl;
			//cout << "recvfrom data is " << data << endl;
		}
		else
		{
			cout << "!!! recvfrom error" << endl;
		}
	}
	t2.update_time();
	tt = (t2.get_time() - t1.get_time());
	//cout << "after recvfrom tv is " << tv.tv_sec << "+" << tv.tv_usec << endl;
	//cout << "recvfrom elapse time = ";
	//printf("%ld\n", (t2.get_time() - t1.get_time()));
	//cout << (hpr_int32)(t2.get_time() - t1.get_time()) << endl;
	if (strcmp(data, "1234") == 0)
	{
		cout << ITEST_PASS << "select_vtime" << endl;
	}

	hpr::hpr_sock_utils::shutdown(sockfd, SHUT_RDWR);
	hpr::hpr_sock_utils::closesocket(sockfd);

	cout << ITEST_PASS << "shutdown" << endl;
	cout << ITEST_PASS << "closesocket" << endl;

	return 0;
}

int test_multicast()
{
	return 0;
}

int test_select()
{
#if defined (__linux__)
	hpr_int32 ret = 0;
	hpr::hpr_time_tick t1;
	timeval tv = {5, 500};

	ret = hpr::hpr_sock_utils::select_ctime(0, NULL, NULL, NULL, &tv);
	hpr::hpr_time_tick t2;
	if (ret != 0 )
	{
		std::cout << "elapse time = " << t2.get_time() - t1.get_time() << endl;
		if ((t1.get_time() + 5*HPR_MS_PER_SEC) > t2.get_time())
		{
			cout << "!!! select_ctime func error" << endl; 
		}
	}
#endif

	return 0;
}
