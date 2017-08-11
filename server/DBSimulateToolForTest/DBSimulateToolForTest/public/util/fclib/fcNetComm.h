#ifndef GUARD_FCNETCOMM_H
#define GUARD_FCNETCOMM_H
/**	@file    fcNetComm.h
 *	@note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 *	@brief   提供操作系统函数的差异屏蔽
 *
 *	@author	 xuezhihu
 *	@date	 2012/12/29
 *
 *	@note    下面的note和warning为可选项目
 *	@note    这里填写本文件的详细功能描述和注解
 *	@note    历史记录：
 *	@note    V0.0.1  创建
 *
 *	@warning 这里填写本文件相关的警告信息
 */

// os layer for socket operation
#ifdef WIN32
    #include <WinSock2.h>
    #pragma comment(lib, "Ws2_32.lib")
#else
    #error OS not support
    #include<netinet/in.h>
#endif

inline int fcNetInit(void)
{
#ifdef WIN32
    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData;
    int err = ::WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) 
    {
        return -1;
    }

    if (LOBYTE(wsaData.wVersion) != 2
        ||	HIBYTE(wsaData.wVersion) != 2) 
    {
        ::WSACleanup( );
        return -1; 
    }
#endif // end for WIN32

    return 0;
}
inline int fcNetFini(void)
{
#ifdef WIN32
    return ::WSACleanup();
#else
    return 0;
#endif
}

inline int fcSelect(
    int nfds,
    fd_set* readfds,
    fd_set* writefds,
    fd_set* exceptfds,
    const struct timeval* timeout
    )
{
#ifdef WIN32
    (nfds);
    return ::select(0, readfds, writefds, exceptfds, timeout);
#else
    return ::select(nfds + 1, readfds, writefds, exceptfds, timeout);
#endif
}

inline int fcCloseSocket(SOCKET s)
{
#ifdef WIN32
    return ::closesocket(s);
#else
    return ::close(s);
#endif
}

inline int fcSetReuseAddr(SOCKET s)
{
    BOOL optval = TRUE;
#ifndef WIN32
    return ::setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char*)&optval, sizeof(optval));
#else
    return ::setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char*)&optval, sizeof(optval));
#endif
}

inline int fcCloseSocketWithoutLinger(SOCKET s)
{
    struct linger lgr;
    lgr.l_onoff = 1;
    lgr.l_linger = 0;
    setsockopt(s, SOL_SOCKET, SO_LINGER, (const char *)&lgr, sizeof(lgr));
    return closesocket(s);
}

inline int fcSend(SOCKET s, const char* buf, int len)
{
#ifdef WIN32
    return ::send(s, buf, len, 0);
#else
    return ::send(s, buf, len, MSG_NOSIGNAL);
#endif
}

#endif // end of GUARD_FCNETCOMM_H
