/*
 * CLog.h
 *
 *  Created on: 2016-3-30
 *      Author: root
 */

#ifndef CLOG_H_
#define CLOG_H_

#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdarg.h>

#define STR(x) _STR(x)
#define _STR(x) #x
#define    LOG_INFO    __FILE__":"STR(__LINE__)
#define    LOG_INFO1    LOG_INFO":"__FUNCTION__
#define __func__    LOG_INFO1
//#define __func__  (__FILE__":"__FUNCTION__ ":"STR(__LINE__))
#define __func1__ STR(__FUNCTION__<<":"<< __LINE__<<":")

//#define    debug(f, arg...)    printDebug(" %s:***"f"", __func__, ##arg)
#define    debug(f, arg...)    printDebug("%s***"f"", LOG_INFO, ##arg)


void printDebug(const char *fmt, ...)
{
	char buff[1024];
	memset(&buff, 0x00, sizeof(buff));
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buff, sizeof(buff), fmt, ap);

	struct tm tt;
	struct timeval tv;
	memset(&tt, 0x00, sizeof(tt));
	memset(&tv, 0x00, sizeof(tv));
	gettimeofday(&tv, NULL);
	localtime_r(&tv.tv_sec, &tt);
	fprintf(stderr, "[%s] %4d-%02d-%02d %02d:%02d:%02d,%ld %s\n", "DEBUG",
			tt.tm_year + 1900, tt.tm_mon + 1, tt.tm_mday, tt.tm_hour,
			tt.tm_min, tt.tm_sec, tv.tv_usec, buff);
	va_end(ap);
}

#endif /* CLOG_H_ */
