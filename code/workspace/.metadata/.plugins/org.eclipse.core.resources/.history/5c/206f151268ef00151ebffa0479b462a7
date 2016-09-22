#ifndef LOG_CLOG_H_
#define LOG_CLOG_H_

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <stdarg.h>

#define __FILE_INFO__     __FILE__, __LINE__, __FUNCTION__
#define INFO(f, arg...)    event_info("%s:%d***%s"f"\n", __FILE_INFO__, ##arg)

void static event_info(const char *fmt, ...)
{
	char buff[1024];
	memset(&buff, 0x00, sizeof(buff));
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buff, sizeof(buff) - 1, fmt, ap);

	struct tm tt;
	struct timeval tv;
	memset(&tt, 0, sizeof(tt));
	memset(&tv, 0, sizeof(tv));
	gettimeofday(&tv, NULL);
	localtime_r(&tv.tv_sec, &tt);
	fprintf(stderr, "[%s]%04d-%02d-%02d %02d:%02d:%02d.%04ld %s", "DEBUG",
			tt.tm_year + 1900, tt.tm_mon + 1, tt.tm_mday, tt.tm_hour, tt.tm_min,
			tt.tm_sec, tv.tv_usec/1000, buff);

	va_end(ap);
}

#endif /* LOG_CLOG_H_ */
