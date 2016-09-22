/*
 * CTimeOut.cpp
 *
 *  Created on: 2016-3-21
 *      Author: root
 */

#include "CTimeOut.h"
#include <stdio.h>

namespace rm
{

CTimeOut::CTimeOut()
{
}

CTimeOut::~CTimeOut()
{
	list<IEvent*>::iterator iter = m_eventlist.begin();
	for (; iter != m_eventlist.end(); ++iter)
	{
		delete (*iter);
	}
}

int8_t CTimeOut::OnCreate()
{
	return 0;
}
int8_t CTimeOut::OnRead()
{
	int64_t diff = 0;
	uint64_t cutt = 0;
	list<IEvent*>::iterator iter = m_eventlist.begin();
	IEvent *mod = NULL;
	for (; iter != m_eventlist.end(); ++iter)
	{
		mod  = *iter;
		if (NULL == mod)
		{
			continue;
		}
		if (m_interval > 1 && m_lasttime > 1)
		{
			diff = cutt - m_lasttime;
			if (diff < 0)
			{
				diff *= -1;
			}
			if (diff >= m_interval)
			{
				m_lasttime = cutt;
				mod->OnTimeOut();
				diff = m_interval;
			}
		}
		else
		{
			diff = m_interval - diff;
		}
	}
	return 0;
}
int8_t CTimeOut::OnWrite()
{
	return 0;
}
int8_t CTimeOut::WaitEvents()
{
	return 0;
}

int8_t CTimeOut::OnTimeOut()
{
	return 0;
}
}
