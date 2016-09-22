/*
 * CStreamPipe.cpp
 *
 *  Created on: 2015-4-12
 *      Author: root
 */

#include <string.h>
#include "CStreamPipe.h"
#include "Osi.h"

namespace rm {

CStreamPipe::CStreamPipe()
{
	m_peer = NULL;
}

CStreamPipe::~CStreamPipe()
{
	if (m_peer)
	{
		delete m_peer;
		m_peer = NULL;
	}
}

int8_t CStreamPipe::OnCreate()
{
	if (!m_peer)
	{
		m_peer = new CStreamPipe();
		m_peer->m_peer = this;
	}

	int fd[2];
	memset(&fd, 0x00, sizeof(fd));
	if (-1 == OsiCreatePipe(AF_UNIX, 0, SOCK_STREAM, fd[2]))
	{
		return -1;
	}

	m_fd = fd[0];
	m_peer->SetPeerSocket(fd[1]);

	// 设置fd非阻塞
	OsiSetSocketBlock(m_fd, 0);
	OsiSetSocketBlock(m_peer->GetSocket(), 1);

	// 设置发送接收缓冲区大小
	//设置socket发送、接收缓冲区大小为64k，默认为8k，最大为64k
	int32_t value=65535;
	setsockopt(m_fd, SOL_SOCKET, SO_RCVBUF, (char*)&value, sizeof(value));
	setsockopt(m_peer->GetSocket(), SOL_SOCKET, SO_SNDBUF, (char*)&value, sizeof(value));

	return 0;
}

void CStreamPipe::SetPeerSocket(int32_t fd)
{
	m_fd = fd;
}

int32_t CStreamPipe::GetSocket()
{
	return m_fd;
}

int32_t CStreamPipe::GetPeerSocket()
{
	return m_peer->GetSocket();
}

}
