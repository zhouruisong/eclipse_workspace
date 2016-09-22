/*
 * CMathndrControllor.cpp
 *
 *  Created on: 2015-4-10
 *      Author: root
 */
#include <stdio.h>
#include "CMathndrControllor.h"
#include "CommonEvent.h"
#include "CEventController.h"

namespace rm {

CMathndrControllor *CMathndrControllor::s_Instance = NULL;

CMathndrControllor::CMathndrControllor()
{
}

CMathndrControllor::~CMathndrControllor()
{
}

CMathndrControllor *CMathndrControllor::GetInstance()
{
	if (NULL == s_Instance)
	{
		s_Instance = new CMathndrControllor();
	}
	return s_Instance;
}

bool CMathndrControllor::InjectEvents()
{
	CommonEvent[0].pProtocol = new CTcpProtocol();
	return true;

	printf("successful");
}

bool CMathndrControllor::Init()
{
	IController *pIController = new CEventController();
	if (!pIController->Init())
	{
		return false;
	}


	return true;
}

bool CMathndrControllor::Run()
{
	return true;
}

}
