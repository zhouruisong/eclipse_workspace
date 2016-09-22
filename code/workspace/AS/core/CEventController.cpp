/*
 * CEventController.cpp
 *
 *  Created on: 2015-4-17
 *      Author: root
 */

#include "CEventController.h"

namespace rm
{

CEventController *CEventController::s_Instance = NULL;

CEventController::CEventController()
{
	mgr = NULL;
}

CEventController::~CEventController()
{
}

bool CEventController::Init()
{
	if (NULL == mgr)
	{
		mgr = new CEventListener();
		if (-1 == mgr->OnCreate())
		{
			return true;
		}
	}
	return false;
}

void CEventController::Run()
{
	mgr->WaitEvents();
}

bool CEventController::InjectEvents(IEvent *pIEvent)
{
	if (-1 == pIEvent->OnCreate())
	{
		return false;
	}
	mgr->AddEpollEvent(pIEvent);
	return true;
}

CEventController *CEventController::GetInstance()
{
	if (NULL == s_Instance)
	{
		s_Instance = new CEventController();
		s_Instance->Init();
	}

	return s_Instance;
}


}
