/*
 * CAsyncController.cpp
 *
 *  Created on: 2015-4-16
 *      Author: root
 */

#include <sys/types.h>
#include "CAsyncController.h"
#include "ICallBack.h"

using namespace rm;

CAsyncController::CAsyncController(ICallBack *pCallBack, u_int32_t count)
{
	m_pipe = NULL;
	m_pCallBack = pCallBack;
	m_Count = count;
}

CAsyncController::~CAsyncController()
{
	if (m_pipe)
	{
		delete m_pipe;
		m_pipe = NULL;
	}

	if (m_pCallBack)
	{
		delete m_pCallBack;
		m_pCallBack = NULL;
	}
}

int8_t CAsyncController::SendMessage(void *pMessage)
{
	return 0;
}

int8_t CAsyncController::Run(const int8_t thread_index)
{
	return 0;
}

int8_t CAsyncController::Write(u_int64_t des, u_int64_t len)
{
	return 0;
}

int8_t CAsyncController::Read(u_int64_t des, u_int64_t len)
{
	return 0;
}

int8_t CAsyncController::OnCreate()
{
	if (NULL == m_pipe)
	{
		m_pipe = new CStreamPipe();
	}

	if (-1 == m_pipe->OnCreate())
	{
		return -1;
	}

	// 加锁

	// 创建线程
	if (-1 == CreateThreatPool())
	{
		return -1;
	}

	return 0;
}


int8_t CAsyncController::CreateThreatPool()
{
	int8_t ret = 0;
	for (u_int32_t count = 0; count < m_Count; count++)
	{
		if (!CThread::Create(count))
		{
			ret = -1;
			break;
		}
	}

	return ret;
}
