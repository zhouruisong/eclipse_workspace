#ifndef _TAIRCLIENT_H_
#define _TAIRCLIENT_H_
#include <string.h>
#include <map>
#include <vector>
#include "tair_client_api_impl.hpp"
#include "log.h"

using namespace tair;
using namespace std;

class tair_client;
class tair_thread;
typedef bool (tair_client::*cmd_call)(vector<char*> &param, char **data,
		int &len);
class tair_client
{
public:
	tair_client(tair_thread *parent);
	~tair_client();

	void setLog(TairLog *pTairLog);
	char *canonical_key(char *key, char **akey, int *size);
	bool do_cmd(const char *cmd, vector<char*> &param, char **data, int &len);
	bool connect(const char *serveraddr, const char *group);
	bool do_cmd_get(vector<char*> &param, char **data, int &len);
	bool do_cmd_mget(vector<char*> &param, char **data, int &len);
	bool do_cmd_put(vector<char*> &param, char **data, int &len);
	bool do_cmd_remove(vector<char*> &param, char **data, int &len);
	bool do_cmd_mremove(vector<char*> &param, char **data, int &len);
	bool do_cmd_pput(vector<char*> &param, char **data, int &len);
	bool do_cmd_pputs(vector<char*> &param, char **data, int &len);
	bool do_cmd_pget(vector<char*> &param, char **data, int &len);
	bool do_cmd_pgets(vector<char*> &param, char **data, int &len);
	bool do_cmd_premove(vector<char*> &param, char **data, int &len);
	bool do_cmd_premoves(vector<char*> &param, char **data, int &len);
	bool do_cmd_get_range(vector<char*> &param, char **data, int &len);

private:
	tair_thread *parent;
	tair_client_impl client_helper;
	map<string, cmd_call> cmd_map;

	int key_format;
	char *server_addr;
	char *group;
	TairLog *pLog;
};

#endif
