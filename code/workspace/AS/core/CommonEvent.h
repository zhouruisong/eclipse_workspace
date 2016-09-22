/*
 * CommonEvent.h
 *
 *  Created on: 2015-4-20
 *      Author: root
 */

#ifndef COMMONEVENT_H_
#define COMMONEVENT_H_

struct CommonEvent
{
	IEvent *pEvent;
	INetProvider *pNetProvider;
	ITxnDriver *pITxnDriver;
	IProtocol *pProtocol;
};

#endif /* COMMONEVENT_H_ */
