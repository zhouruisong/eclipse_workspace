/*
 * CUdpEvent.h
 *
 *  Created on: 2015-6-4
 *      Author: root
 */

#ifndef CUDPEVENT_H_
#define CUDPEVENT_H_

#include "IEvent.h"

namespace rm
{

class CUdpEvent :public IEvent
{
public:
	CUdpEvent();
	virtual ~CUdpEvent();
};

}

#endif /* CUDPEVENT_H_ */
