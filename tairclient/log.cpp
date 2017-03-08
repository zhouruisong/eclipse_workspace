#include "log.h"

const char * const TairLog::_errstr[] = {"INFO","TRACE"};

TairLog::TairLog()
{
	_level = 0;
}

TairLog::~TairLog()
{
	if (fd) {
		close(fd);
	}
	if(log_data) {
		free(log_data);
		log_data = NULL;
	}
}

int TairLog::init()
{
	log_data = (char *)malloc(124);
	if (log_data == NULL)
	{
		perror("malloc");
		exit(0);
	}

	fd = open((const char *)("/usr/local/sandai/streamengine/logs/tair.log"), O_RDWR|O_CREAT|O_APPEND);
	if (fd < 0)
	{
		perror("open");
		exit(0);
	}
	printf("init log file successful \n");	
	return 0;
}

int TairLog::getLogLevel()
{
	return _level;
}

void TairLog::setLogLevel(const char *plevel)
{
	if (plevel == NULL)
	{
		return;
	}
	int l = sizeof(_errstr) / sizeof(char*);
	for (int i = 0; i < l; i++)
	{
		if (strcasecmp(plevel, _errstr[i]) == 0)
		{
			_level = i;
			break;
		}
	}
}

void TairLog::write_log(char *string_data, int size, int level)
{
	if (level > _level)
	{
		return;
	}

	timeval tv;
    	if (gettimeofday(&tv, NULL) == -1) {
        	exit(0);
    	}

	struct tm* tm;
	if ((tm = localtime(&tv.tv_sec)) == NULL) {
            return;
        }

	int log_time = snprintf(log_data, 2048, 
                "[%d-%02d-%02d %02d:%02d:%02d.%03d] ", 
                1900 + tm->tm_year, 1 + tm->tm_mon, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, (int)(tv.tv_usec / 1000));

	
	if (fd < 0) { //reopen file
		fd = open((const char *)("/usr/local/sandai/streamengine/logs/tair.log"), O_RDWR|O_CREAT);
	}
	char temp_buf[1024];
	memset(temp_buf, 0, 1024);

	strcat(temp_buf, log_data);
	strcat(temp_buf, string_data);
    strcat(temp_buf, "\n");
	//printf("time:%s data:%s\n", log_data, temp_buf);
	write(fd, temp_buf, strlen(temp_buf));
}
