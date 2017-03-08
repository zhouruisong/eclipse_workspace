#include <string.h>
#include "CStreamPipe.h"
#include "Osi.h"

CStreamPipe::CStreamPipe()
{
	m_Write = -1;
	m_Read = -1;
}

CStreamPipe::~CStreamPipe()
{
}

int8_t CStreamPipe::OnCreate()
{
	int fd[2];
	memset(&fd, 0x00, sizeof(fd));
	if (-1 == OsiCreatePipe(AF_UNIX, SOCK_STREAM, 0, fd))
	{
		printf("OsiCreatePipe failed\n");
		return -1;
	}

	m_Write = fd[0];
	m_Read = fd[1];

	// set fd[0] non block
	OsiSetSocketBlock(m_Write, 0);
	// set fd[1] block
	OsiSetSocketBlock(m_Read, 1);

	//设置发送接收缓冲区大小
	//设置socket发送、接收缓冲区大小为64k，默认为8k，最大为64k
	int32_t value=65535;
	setsockopt(m_Read, SOL_SOCKET, SO_RCVBUF, (char*)&value, sizeof(value));
	setsockopt(m_Write, SOL_SOCKET, SO_SNDBUF, (char*)&value, sizeof(value));

	return 0;
}

void CStreamPipe::SetReadSocket(int32_t fd)
{
	m_Read = fd;
}

int32_t CStreamPipe::GetWriteSocket()
{
	return m_Write;
}

int32_t CStreamPipe::GetReadSocket()
{
	return m_Read;
}
