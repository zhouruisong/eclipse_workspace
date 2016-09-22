/*
 * CEventListener.cpp
 *
 *  Created on: 2015-4-16
 *      Author: root
 */

#include "CEventListener.h"

namespace rm
{

CEventListener::CEventListener()
{
	m_isRunning = true;
	m_isShutdown = false;
}

CEventListener::~CEventListener()
{
}

int8_t CEventListener::OnCreate()
{
	if (m_fd < 0)
	{
		return -1;
	}
	if (-1 == CreateEpoll(MAX_EPOLL_SIZE))
	{
		return -1;
	}

	SetReadWriteFlag();
	OsiSetSocketBlock(m_fd, 0);
	OsiSetBuffSize(m_fd);

	return 0;
}

int8_t CEventListener::OnRead()
{
    return 0;
}

int8_t CEventListener::OnWrite()
{
	return 0;
}

int8_t CEventListener::WaitEvents()
{
	if (-1 == Create())
	{
		return -1;
	}

	struct epoll_event events[MAX_EPOLL_SIZE];
	memset($event, 0x00, sizeof(event));

	IEvent *pEvent = NULL;
	int ret = 0;
	int32_t i = 0;

	while(m_isRunning)
	{
		ret = epoll_wait(m_fd, &event, MAX_EPOLL_SIZE, 0);
		for (i = 0; i < ret; ++i)
		{
			pEvent = static_cast<IEvent *>(events[i].data.ptr);
			// 有数据到来
			if (events[i].events & EPOLLIN == EPOLLIN)
			{
				pEvent->OnRead();
			}
			// 有数据要写
			if (events[i].events & EPOLLOUT == EPOLLOUT)
			{
				pEvent->OnWrite();
			}
		}

		// 遍历eventlist 检查是否超时
		// 超时调用TimeOut()

		if (m_isShutdown)
		{
			break;
		}
	}

	return 0;
}

int8_t CEventListener::AddEpollEvent(IEvent *pEvent)
{
	if (-1 == pEvent->OnCreate())
	{
		return -1;
	}

	return AddEpoll(pEvent->GetFd(), pEvent);
}
int8_t CEventListener::RemoveEvent(IEvent *pEvent)
{
	if (-1 == pEvent->OnCreate())
	{
		return -1;
	}

	return RemoveEvent(pEvent->GetFd(), pEvent);
}

}
