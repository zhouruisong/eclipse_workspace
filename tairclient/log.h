#ifndef __TAITCLIENT_LOG__
#define __TAIRCLIENT_LOG__

#include <stdlib.h>
#include <sys/time.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<errno.h>
#include <stddef.h>
#include <time.h>

using namespace std;

class TairLog
{
public:
	int _level;
	int fd;	
private:
	char *log_data;
	static const char *const _errstr[];
public:
	TairLog();
	virtual ~TairLog();
public:
	virtual int init();
	int getLogLevel();
	void setLogLevel(const char *plevel);
	virtual void write_log(char *string_data, int size, int level);
	
};

extern TairLog *tair_log;
#endif 
