#include <stdio.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

#include "CLog.h"

//timer
void seconds_sleep(int32_t seconds)
{
	struct timeval tv;
	tv.tv_sec = seconds;
	tv.tv_usec = 0;
	int err;
	do
	{
		err = select(0, NULL, NULL, NULL, &tv);
		if (0 == err)
		{
			tv.tv_sec = seconds;
			tv.tv_usec = 0;
			debug("timer, err[%d]", err);
		}
		else
		{
			debug("err[%d]", err);
			break;
		}
	} while (true);
}
int main(int argc, char **argv)
{
	debug("%s", __func1__);
	seconds_sleep(2);
	return 0;
}
