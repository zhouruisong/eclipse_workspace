#include <stdio.h>
#include <sys/types.h>
#include "CMathndrControllor.h"

using namespace rm;

int32_t main(int32_t argc, char *argv[])
{
	CEventController *pController = CEventController::GetInstance();

	pController->Init();
	pController->Run();

	return 0;
}
