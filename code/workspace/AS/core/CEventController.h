/*
 * CEventController.h
 *
 *  Created on: 2015-4-17
 *      Author: root
 */

#ifndef CEVENTCONTROLLER_H_
#define CEVENTCONTROLLER_H_

#include<list>
#include "IController.h"
#include "CEventListener.h"

namespace rm
{

class CEventController: public IController
{
public:
	CEventController();
	virtual ~CEventController();

	virtual void Run();
	virtual bool InjectEvents(IEvent *pIEvent);
	virtual bool Init();
	static CEventController *GetInstance();
private:
	static CEventController *s_Instance;
	list<IEvent *> m_eventList;
	CEventListener *mgr;
};

}

#endif /* CEVENTCONTROLLER_H_ */
