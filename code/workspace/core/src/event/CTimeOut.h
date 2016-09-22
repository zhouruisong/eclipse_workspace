/*
 * CTimeOut.h
 *
 *  Created on: 2016-3-21
 *      Author: root
 */

#ifndef CTIMEOUT_H_
#define CTIMEOUT_H_

#include "IEvent.h"
#include <list>

namespace rm
{

using namespace std;
class IEvent;

class CTimeOut: public IEvent
{
public:
	CTimeOut();
	virtual ~CTimeOut();

	virtual int8_t OnCreate();
	virtual int8_t OnRead();
	virtual int8_t OnWrite();
	virtual int8_t WaitEvents();
	virtual int8_t OnTimeOut();

private:
	list<IEvent*> m_eventlist;
};

}

#endif /* CTIMEOUT_H_ */