#ifndef _TAIRWRAPPER_H_
#define _TAIRWRAPPER_H_

extern "C" {
    #include "event2/http.h"
    #include "event2/http_struct.h"
    #include "event2/util.h"
    #include "event2/event.h"
	#include "event2/http_compat.h"
	#include "event2/keyvalq_struct.h"
	#include "event2/buffer.h"
    #include "event2/thread.h"
}
#include "tairthread.h"

#include <pthread.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string>
#include <map>
#include <vector>
using namespace std;
class tair_thread;

class tair_wrapper {
public:
	tair_wrapper();
	~tair_wrapper();

	void init(int thread_count);
	void dispatch(struct evhttp_request *req);

	bool do_cmd_put(const char *serveraddr, const char *group, const char *key, const char *value, int expire);
	bool do_cmd_pput(const char *serveraddr, const char *group, const char *prefix, const char *key, const char *value, int expire);
	char* do_cmd_get(const char *serveraddr, const char *group, const char *key);
	char* do_cmd_pget(const char *serveraddr, const char *group, const char *prefix, const char *key);
	bool do_cmd_remove(const char *serveraddr, const char *group, const char *key);
	bool do_cmd_premove(const char *serveraddr, const char *group, const char *prefix, const char *key);

private:
	tair_thread* pTair_thread;
};
#endif
