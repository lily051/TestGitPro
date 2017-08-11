#include "DemoApp.h"
#include "DemoClass.h"

static int judge_host_port(hpr::hpr_net_addr& addr, const char* host, hpr_int32 port, const char* desc)
{
	if (addr.get_port() != port)
	{
		cout << ITEST_FAIL << desc << " get_port()" << endl;
		return -1;
	}	

	if (strcmp(addr.get_host(), host) != 0)
	{
		cout << ITEST_FAIL << desc << " get_host() != host" << endl;
		cout << addr.get_host() << endl;
		return -1;
	}

	::IN_ADDR inaddr;
	if (-1 == addr.get_addr(inaddr))
	{
		cout << ITEST_FAIL << desc << " get_addr(inaddr) return -1" << endl;
		return -1;
	}
	if (strcmp(inet_ntoa(inaddr), host) != 0)
	{
		cout << ITEST_FAIL << desc << " get_addr(ip, iplen) inet_ntoa(inaddr) != host" << endl;
		return -1;
	}

	cout << ITEST_PASS << desc << endl;

	return 0;
}

void Test_NetAddr()
{
	//char host[32];

	::sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(8000);
	sockaddr.sin_addr.s_addr = inet_addr("172.7.28.205"); 

	hpr::hpr_net_addr addr_no_host;
	addr_no_host.set_addr(8000);
	judge_host_port(addr_no_host, "0.0.0.0", 8000, "set_addr(const hpr_int32 port, const char* host = NULL)");

	hpr::hpr_net_addr addr_host_port;
	addr_host_port.set_host("172.7.28.205");
	addr_host_port.set_port(8000);
	judge_host_port(addr_host_port, "172.7.28.205", 8000, "set_addr(const char* host) & set_port(const hpr_int32 port)");

	hpr::hpr_net_addr addr_url;
	addr_url.set_addr("172.7.28.205:8000");
	judge_host_port(addr_url, "172.7.28.205", 8000, "set_addr(const char* url)");

	hpr::hpr_net_addr addr_sockaddr;
	addr_sockaddr.set_addr(sockaddr);
	judge_host_port(addr_sockaddr, "172.7.28.205", 8000, "set_addr(const SOCKADDR_IN addr)");

	hpr::hpr_net_addr addr1(sockaddr);
	hpr::hpr_net_addr addr2("172.7.28.205:8000");
	hpr::hpr_net_addr addr3(8000, "172.7.28.205");
	hpr::hpr_net_addr addr4(addr_host_port);

	if ( (addr1 != addr2) || (addr2 != addr3) || (addr3 != addr4) || (addr4 != addr_url))
	{
		cout << ITEST_FAIL << "hpr::hpr_net_addr !=" << endl;
	}
	else
	{
		cout << ITEST_PASS << "hpr::hpr_net_addr !=" << endl;
		judge_host_port(addr1, "172.7.28.205", 8000, "all construction");
	}
}
