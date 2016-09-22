/*
 * CEventListener.h
 *
 *  Created on: 2015-4-16
 *      Author: root
 */

#ifndef CEVENTLISTENER_H_
#define CEVENTLISTENER_H_

#include "IEvent.h"

namespace rm
{

class CEventListener: public IEvent
{
public:
	CEventListener();
	virtual ~CEventListener();

	virtual int8_t AddEpollEvent(IEvent *pEvent);
	virtual int8_t RemoveEvent(IEvent *pEvent);

	virtual int8_t OnCreate();
	virtual int8_t OnWrite();
	virtual int8_t OnRead();
	virtual int8_t WaitEvents();

private:
	bool m_isRunning;
	bool m_isShutdown;
};

}

#endif /* CEVENTLISTENER_H_ */
