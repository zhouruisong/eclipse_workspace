#ifndef _TAIR_THREAD_H_
#define _TAIR_THREAD_H_

extern "C" {
    #include "event2/http.h"
    #include "event2/http_struct.h"
    #include "event2/util.h"
    #include "event2/event.h"
    #include "event2/http_compat.h"
    #include "event2/keyvalq_struct.h"
    #include "event2/buffer.h"
}

#include "CThread.h"
#include "CStreamPipe.h"
#include <pthread.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string>
#include <string.h>
#include <map>
#include <vector>
using namespace std;

class tair_client;

struct Context {
	Context()
	{
		req = NULL;
	}

	~Context()
	{
		req = NULL;
	}
	struct evhttp_request *req;
};

class tair_thread: public CThread
{
public:
    tair_thread();
    ~tair_thread();
    void init(int thread_num);
    int8_t OnCreate();
    int8_t Write(Context *ctx);
    bool connect_tair(const char *serveraddr, const char *group);
    bool disconnect_tair(const char *serveraddr, const char *group);
    void parse_cmd(struct evhttp_request *req);
    void do_cmd(struct evhttp_request * req, const char *serveraddr,
    		const char *groupname, const char *cmd, vector<char*> &param);

    virtual void *Run(void *ptr);

private:
    int8_t CreateThreatPool();

    bool active;
    int32_t thread_num;
    CStreamPipe* m_pipe;
    map<string, tair_client*> tair_client_map;
};

#endif


