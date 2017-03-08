#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include "Osi.h"

int32_t OsiCreatePipe(int32_t domain, int32_t type, int32_t protocol, int32_t fd[2])
{
	return socketpair(AF_UNIX, type, protocol, fd);
}

int32_t OsiCreateSoket(int32_t domain, int32_t type, int32_t protocol)
{
	return socket(domain, type, protocol);
}

int32_t OsiBind(int32_t sockfd, struct sockaddr *addr, socklen_t addrlen)
{
	return bind(sockfd, addr, addrlen);
}

int32_t OsiListen(int32_t sockfd, int32_t backlog)
{
	return listen(sockfd, backlog);
}

int32_t OsiGePeerName(int32_t sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
	return getpeername(sockfd, addr, addrlen);
}

int32_t OsiConnect(int32_t sockfd, struct sockaddr *addr, socklen_t  addrlen)
{
	return connect(sockfd, addr, addrlen);
}

int32_t OsiAccept(int32_t sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
	return accept(sockfd, addr, addrlen);
}

int32_t OsiSend(int32_t sockfd, const void *msg, size_t len)
{
	return send(sockfd, msg, len, 0);
}

int32_t OsiReceive(int32_t sockfd, void *buf, size_t len)
{
    return recv(sockfd, buf, len, 0);
}

int32_t OsiSendto(int32_t sockfd, const void *msg, size_t len, u_int32_t flags, const struct sockaddr *to, socklen_t tolen)
{
	return sendto(sockfd, msg, len, flags, to, tolen);
}

int32_t OsiRecvfrom(int32_t sockfd, void *buf, size_t len, u_int32_t flags, struct sockaddr *from, socklen_t *fromlen)
{
	return recvfrom(sockfd, buf, len, flags, from, fromlen);
}

int32_t OsiPthreadCreate(pthread_t *tidp, thread_Fun func, void *arg)
{
	return pthread_create(tidp, NULL, func, arg);
}

int32_t OsiCreateMutex(pthread_mutex_t *mutex)
{
	return pthread_mutex_init(mutex, NULL);
}

int32_t OsiLockMutex(pthread_mutex_t *mutex)
{
	return pthread_mutex_lock(mutex);
}

int32_t OsiUnLockMutex(pthread_mutex_t *mutex)
{
	return pthread_mutex_unlock(mutex);
}

int32_t OsiDestroyMutex(pthread_mutex_t *mutex)
{
	return pthread_mutex_destroy(mutex);
}

void OsiSetSocketBlock(int32_t sockfd, int8_t isblock)
{
	int32_t flags = fcntl(sockfd, F_GETFL, 0);
	// isblock为0表示设置fd为非阻塞，1为阻塞
	if (!isblock)
	{
		flags |= O_NONBLOCK;
	}
	else
	{
		flags &= ~O_NONBLOCK;
	}

	fcntl(sockfd, F_SETFL, flags);
	return;
}

void OsiSetBuffSize(int32_t socketfd)
{
	// 设置发送接收缓冲区大小
	//设置socket发送、接收缓冲区大小为64k，默认为8k，最大为64k
	int32_t value=65535;
	setsockopt(socketfd, SOL_SOCKET, SO_RCVBUF, (char*)&value, sizeof(value));
	setsockopt(socketfd, SOL_SOCKET, SO_SNDBUF, (char*)&value, sizeof(value));
}

#ifdef __cplusplus
}
#endif
