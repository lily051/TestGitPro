#ifndef __DEMO_SOCKUTILS_CLASS_H__
#define __DEMO_SOCKUTILS_CLASS_H__

#include "DemoClass.h"
#include "HPR_Sock_Utils.h"

namespace i_test
{
	class i_tcp_client : public test
	{
	public:
		void Test();
			
	public:
		void connect_timeout_0();
		void connect_timeout_infinte();
		void connect_timeout_3s();


		void send_timeout_0();
		void send_timeout_infinite();
		void send_timeout_3s();

		//////////////////////////////////////////////////////////////////////////
		void sendn_timeout_0();
		void sendn_timeout_infinite();
		void sendn_timeout_3s();
		
		//////////////////////////////////////////////////////////////////////////
		void recv_timeout_0();
		void recv_timeout_3s();
		void recv_timeout_infinite();

		//////////////////////////////////////////////////////////////////////////
		void recvn_timeout_0();
		void recvn_timeout_3s();
		void recvn_timeout_infinite();

		void reuseaddr();
	};

	class i_tcp_server : public test
	{
	public:
		static void* CALLBACK accept_svc(void* param);
		static void* CALLBACK echo_svc(void* param);
		static void echo(hpr_sock_t sock);

	public:
		void Test();

	public:
		void start();
		void stop();

		void accept();
		

	private:
		hpr_sock_t m_sockfd;
		hpr_handle m_hThreadId;
	};
}

#endif

