/*
 * CTcpNetProvider.cpp
 *
 *  Created on: 2015-4-21
 *      Author: root
 */

#include <sys/types.h>
#include <sys/socket.h>

#include "CTcpNetProvider.h"
#include "Osi.h"

namespace rm
{

#define MAX_QUEUE_LEN     1024

CTcpNetProvider::CTcpNetProvider()
{
	m_pProtocol = NULL;
	m_socket = -1;
	memset(&m_addr, 0x00, sizeof(m_addr));
}

CTcpNetProvider::~CTcpNetProvider()
{
}

int8_t CTcpNetProvider::Create(const char *addr, const int32_t  port)
{
	m_fd = OsiCreateSoket(AF_INET, SOCKET_STREAM, 0);
	if (m_fd < 0)
	{
		return -1;
	}

	SetBlock(false);
	OsiSetBuffSize(m_fd);

	struct sockaddr_in address;
	memset(&address, 0x00, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port=htons(port);
	inet_aton(addr, &address.sin_addr);

	socklen_t addrlen = sizeof(struct sockaddr_in);
	int32_t ret = OsiBind(m_fd, (struct sockaddr *) &address, addrlen);
	if (-1 == ret)
	{
		return -1;
	}

	if (-1 == OsiListen(m_fd, MAX_QUEUE_LEN))
	{
		return -1;
	}

	return 0;
}

int32_t CTcpNetProvider::Send(const int8_t *buff, const int32_t len, const char *addr = NULL,
		const int32_t port = 0)
{
	return 0;
}

int32_t CTcpNetProvider::Recive(void *buff, const size_t len, struct sockaddr *addr,
		socklen_t *addlen)
{
	int32_t ret = 0;
	while(true)
	{
		ret = OsiReceive(m_socket, buf, len);
		if (-1 == ret)
		{
			if (EINTR == errno)
			{
				continue;
			}

			if (errno)
			{
			}
		}
		else
		{
			socklen_t len = sizeof(struct sockaddr);
			if (-1 == OsiGePeerName(m_socket, m_addr, &len))
			{
				return -1;
			}
		}

		break;
	}

	return ret;
}

int32_t CTcpNetProvider::Accept(const size_t len, struct sockaddr *addr, socklen_t *addlen)
{
	m_socket = OsiAccept(m_fd, addr, addlen);
	if (m_socket < 0)
	{
		return -1;
	}

	return m_socket;
}


void CTcpNetProvider::SetBlock(bool block)
{
	OsiSetSocketBlock(m_fd, block ? 1 : 0);
}

void CTcpNetProvider::Flush()
{

}

IProtocol *CTcpNetProvider::GetProtocol()
{
	return 0;
}


}
