#include "tairclient.h"
#include "tairthread.h"
#include "cJSON.h"

tair_client::tair_client(tair_thread *parent)
{
	this->parent = parent;
	server_addr = NULL;
	group = NULL;
	pLog = NULL;

	cmd_map["get"] = &tair_client::do_cmd_get;
	cmd_map["mget"] = &tair_client::do_cmd_mget;
	cmd_map["put"] = &tair_client::do_cmd_put;
	cmd_map["remove"] = &tair_client::do_cmd_remove;
	cmd_map["mremove"] = &tair_client::do_cmd_mremove;
	cmd_map["pput"] = &tair_client::do_cmd_pput;
	cmd_map["pputs"] = &tair_client::do_cmd_pputs;
	cmd_map["pget"] = &tair_client::do_cmd_pget;
	cmd_map["pgets"] = &tair_client::do_cmd_pgets;
	cmd_map["premove"] = &tair_client::do_cmd_premove;
	cmd_map["premoves"] = &tair_client::do_cmd_premoves;
	cmd_map["get_range"] = &tair_client::do_cmd_get_range;

	key_format = 0;
}

tair_client::~tair_client()
{
	if (server_addr && group)
	{
		fprintf(stderr,
				"tairclient: tair client exit for server '%s' '%s'\n",
				server_addr, group);
	}
	if (server_addr)
	{
		delete[] server_addr;
	}
	if (group)
	{
		delete[] group;
	}
}

void tair_client::setLog(TairLog *pTairLog)
{
	pLog = pTairLog;
}

char *tair_client::canonical_key(char *key, char **akey, int *size)
{
	char *pdata = key;
	if (key_format == 1)
	{ // java
		*size = strlen(key) + 2;
		pdata = (char*) malloc(*size);
		pdata[0] = '\0';
		pdata[1] = '\4';
		memcpy(pdata + 2, key, strlen(key));
		*akey = pdata;
	}
	else if (key_format == 2)
	{ // raw
		pdata = (char*) malloc(strlen(key) + 1);
		util::string_util::conv_raw_string(key, pdata, size);
		*akey = pdata;
	}
	else
	{
		*size = strlen(key) + 1;
	}

	return pdata;
}

bool tair_client::connect(const char * serveraddr, const char * group)
{
	if (!serveraddr || !group)
	{
		return false;
	}
	this->server_addr = new char[strlen(serveraddr) + 1];
	memcpy(this->server_addr, serveraddr, strlen(serveraddr));
	this->server_addr[strlen(serveraddr)] = 0;
	this->group = new char[strlen(group) + 1];
	memcpy(this->group, group, strlen(group));
	this->group[strlen(group)];
	this->group[strlen(group)] = 0;

	client_helper.set_timeout(50000);
	client_helper.set_force_service(true);

	char tmp[2048];
	memset(tmp, 0, 2048);
	sprintf(tmp, "%s:%d connect server addr[%s]", __FILE__, __LINE__, server_addr);
	pLog->write_log(tmp, 0, 1);

	//fprintf(stderr, "server addr: %s\n", server_addr);
	return client_helper.startup(server_addr, NULL, group);
}

bool tair_client::do_cmd(const char *cmd, vector<char*> &param, char **data,
		int &len)
{
	cmd_call this_cmd_call = cmd_map[cmd];
	if (this_cmd_call == NULL)
	{
		return false;
	}
	return (this->*this_cmd_call)(param, data, len);
}

bool tair_client::do_cmd_get(vector<char*> &param, char **data, int &len)
{
	if (param.size() < 1)
	{
		return false;
	}
	data_entry *entry = NULL;
	char *akey = NULL;
	int pkeysize = 0;
	char *pkey = canonical_key(param[0], &akey, &pkeysize);
	data_entry key(pkey, pkeysize, false);
	int ret = client_helper.get(0, key, entry);

	if (ret == TAIR_RETURN_SUCCESS)
	{
		cJSON *pRoot = cJSON_CreateObject();
		cJSON* pArray = cJSON_CreateArray();
		cJSON_AddItemToObject(pRoot, "keys", pArray);
		cJSON* pItem = cJSON_CreateObject();
		cJSON_AddStringToObject(pItem, "key", param[0]);

		char *value_array = entry->get_data();
		char *pos = strstr(value_array, "|");
		if (pos == NULL)
		{
			cJSON_AddStringToObject(pItem, "value", value_array);
		}
		else
		{
			char value[2048] = { 0 };
			char createtime[50] = { 0 };
			memcpy(value, value_array, pos - value_array);
			cJSON_AddStringToObject(pItem, "value", value);

			value_array = pos + 1;
			pos = strstr(value_array, "|");
			if (pos != NULL)
			{
				memcpy(createtime, value_array, pos - value_array);
				cJSON_AddStringToObject(pItem, "createtime", createtime);
				value_array = pos + 1;
				cJSON_AddStringToObject(pItem, "expiretime", value_array);
			}
		}
		cJSON_AddItemToArray(pArray, pItem);
		char *out = cJSON_Print(pRoot);
		*data = out;
		len = strlen(out);
	}

	if (akey)
		free(akey);

	return ret == TAIR_RETURN_SUCCESS || ret == TAIR_RETURN_DATA_NOT_EXIST;
}

bool tair_client::do_cmd_mget(vector<char*> &param, char **data, int &len)
{
	if (param.size() < 1)
	{
		return false;
	}

	vector<data_entry*> keys;
	for (int i = 0; i < static_cast<int>(param.size() - 1); ++i)
	{
		char *akey = NULL;
		int pkeysize = 0;
		char *pkey = canonical_key(param[i], &akey, &pkeysize);
		data_entry* key = new data_entry(pkey, pkeysize, false);
		keys.push_back(key);
		if (akey)
			free(akey);
	}
	tair_keyvalue_map datas;
	int ret = client_helper.mget(0, keys, datas);
	if (ret == TAIR_RETURN_SUCCESS || ret == TAIR_RETURN_PARTIAL_SUCCESS)
	{
		cJSON *pRoot = cJSON_CreateObject();
		cJSON* pArray = cJSON_CreateArray();
		cJSON_AddItemToObject(pRoot, "keys", pArray);

		tair_keyvalue_map::iterator mit = datas.begin();
		for (; mit != datas.end(); ++mit)
		{
			cJSON* pItem = cJSON_CreateObject();
			cJSON_AddStringToObject(pItem, "key", mit->first->get_data());

			char *value_array = mit->second->get_data();
			char *pos = strstr(value_array, "|");
			if (pos == NULL)
			{
				cJSON_AddStringToObject(pItem, "value", value_array);
			}
			else
			{
				char value[2048] = { 0 };
				char createtime[50] = { 0 };
				memcpy(value, value_array, pos - value_array);
				cJSON_AddStringToObject(pItem, "value", value);

				value_array = pos + 1;
				pos = strstr(value_array, "|");
				if (pos != NULL)
				{
					memcpy(createtime, value_array, pos - value_array);
					cJSON_AddStringToObject(pItem, "createtime", createtime);
					value_array = pos + 1;
					cJSON_AddStringToObject(pItem, "expiretime", value_array);
				}
			}

			cJSON_AddItemToArray(pArray, pItem);
		}

		char *out = cJSON_Print(pRoot);
		*data = out;
		len = strlen(out);
	}

	vector<data_entry*>::iterator vit = keys.begin();
	for (; vit != keys.end(); ++vit)
	{
		delete *vit;
		(*vit) = NULL;
	}
	tair_keyvalue_map::iterator kv_mit = datas.begin();
	for (; kv_mit != datas.end();)
	{
		data_entry* key = kv_mit->first;
		data_entry* value = kv_mit->second;
		datas.erase(kv_mit++);
		delete key;
		key = NULL;
		delete value;
		value = NULL;
	}

	return (ret == TAIR_RETURN_SUCCESS || ret == TAIR_RETURN_PARTIAL_SUCCESS
			|| ret == TAIR_RETURN_DATA_NOT_EXIST);
}

bool tair_client::do_cmd_put(vector<char*> &param, char **data, int &len)
{
	if (param.size() < 2U)
	{
		return false;
	}

	int expired = 0;
	if (param.size() > 2U)
		expired = atoi(param[2]);

	char *akey = NULL;
	int pkeysize = 0;
	char *pkey = canonical_key(param[0], &akey, &pkeysize);
	data_entry key(pkey, pkeysize, false);
	data_entry entry(param[1], false);

	int ret = client_helper.put(0, key, entry, expired, 0);

	if (akey)
		free(akey);

	return ret == TAIR_RETURN_SUCCESS;
}

bool tair_client::do_cmd_remove(vector<char*> &param, char **data, int &len)
{
	if (param.size() < 1)
	{
		return false;
	}
	char *akey = NULL;
	int pkeysize = 0;
	char *pkey = canonical_key(param[0], &akey, &pkeysize);
	data_entry key(pkey, pkeysize, false);
	int ret = client_helper.remove(0, key);

	if (akey)
		free(akey);

	return ret == TAIR_RETURN_SUCCESS;
}

bool tair_client::do_cmd_mremove(vector<char*> &param, char **data, int &len)
{
	if (param.size() < 1)
	{
		return false;
	}
	vector<data_entry*> keys;
	for (int i = 0; i < static_cast<int>(param.size() - 1); ++i)
	{
		char *akey = NULL;
		int pkeysize = 0;
		char *pkey = canonical_key(param[i], &akey, &pkeysize);
		data_entry* key = new data_entry(pkey, pkeysize, false);
		keys.push_back(key);
		if (akey)
			free(akey);
		//todo delete key
	}

	int ret = client_helper.mdelete(0, keys);
	fprintf(stderr, "mremove: %s, ret: %d\n", client_helper.get_error_msg(ret),
			ret);
	vector<data_entry*>::iterator vit = keys.begin();
	for (; vit != keys.end(); ++vit)
	{
		delete *vit;
		(*vit) = NULL;
	}

	return ret == TAIR_RETURN_SUCCESS;
}

bool tair_client::do_cmd_pput(vector<char*> &param, char **data, int &len)
{
	if (param.size() < 3)
	{
		return false;
	}
	data_entry pkey, skey, value;
	pkey.set_data(param[0], strlen(param[0]) + 1);
	skey.set_data(param[1], strlen(param[1]) + 1);
	value.set_data(param[2], strlen(param[2]) + 1);
	int expired = 0;
	if (param.size() > 3U)
		expired = atoi(param[3]);

	int ret = client_helper.prefix_put(0, pkey, skey, value, expired, 0);

	char tmp[2048];
	memset(tmp, 0, 2048);
	sprintf(tmp, "%s:%d do_cmd_pput ret[%d]", __FILE__, __LINE__, ret);
	pLog->write_log(tmp, 0, 1);

	return ret == TAIR_RETURN_SUCCESS;
}

bool tair_client::do_cmd_pputs(vector<char*> &param, char **data, int &len)
{
	return false;
}

bool tair_client::do_cmd_pget(vector<char*> &param, char **data, int &len)
{
	if (param.size() < 2)
	{
		return false;
	}
	data_entry pkey, skey;
	pkey.set_data(param[0], strlen(param[0]) + 1);
	skey.set_data(param[1], strlen(param[1]) + 1);
	data_entry *value = NULL;

	int ret = client_helper.prefix_get(0, pkey, skey, value);
	if (ret == TAIR_RETURN_SUCCESS && value != NULL)
	{
		cJSON *pRoot = cJSON_CreateObject();
		cJSON* pArray = cJSON_CreateArray();
		cJSON_AddItemToObject(pRoot, "keys", pArray);
		cJSON* pItem = cJSON_CreateObject();
		cJSON_AddStringToObject(pItem, "prefix", pkey.get_data());
		cJSON_AddStringToObject(pItem, "key", skey.get_data());

		char *value_array = value->get_data();
		char *pos = strstr(value_array, "|");
		if (pos == NULL)
		{
			cJSON_AddStringToObject(pItem, "value", value_array);
		}
		else
		{
			char value[2048] = { 0 };
			char createtime[50] = { 0 };
			memcpy(value, value_array, pos - value_array);
			cJSON_AddStringToObject(pItem, "value", value);

			value_array = pos + 1;
			pos = strstr(value_array, "|");
			if (pos != NULL)
			{
				memcpy(createtime, value_array, pos - value_array);
				cJSON_AddStringToObject(pItem, "createtime", createtime);
				value_array = pos + 1;
				cJSON_AddStringToObject(pItem, "expiretime", value_array);
			}
		}

		cJSON_AddItemToArray(pArray, pItem);

		char *out = cJSON_Print(pRoot);
		*data = out;
		len = strlen(out);

		char buff[4096];
		memset(buff, 0, 4096);

		char tmp[2048];
		memset(tmp, 0, 2048);

		sprintf(tmp, "%s:%d pkey[%s], skey[%s], ret[%d]",
		__FILE__, __LINE__, param[0], param[1], ret);
		pLog->write_log(tmp, 0, 1);

		snprintf(buff, len + 2, "%s", out);
		pLog->write_log(buff, 0, 1);

		return true;
	}
	else if (ret == TAIR_RETURN_DATA_NOT_EXIST || value == NULL)
	{
		char tmp[2048];
		memset(tmp, 0, 2048);
		sprintf(tmp, "%s:%d pkey[%s], skey[%s], ret[%d] tair return data not exist",
				__FILE__, __LINE__, param[0], param[1], ret);
		pLog->write_log(tmp, 0, 1);

		return true;
	}
	else
	{
		char tmp[2048];
		memset(tmp, 0, 2048);
		sprintf(tmp, "%s:%d pkey[%s], skey[%s], ret[%d]",
		__FILE__, __LINE__, param[0], param[1], ret);
		pLog->write_log(tmp, 0, 1);

		return false;
	}
}

bool tair_client::do_cmd_pgets(vector<char*> &param, char **data, int &len)
{
	return false;
}

bool tair_client::do_cmd_premove(vector<char*> &param, char **data, int &len)
{
	if (param.size() < 2)
	{
		return false;
	}
	data_entry pkey, skey;
	pkey.set_data(param[0], strlen(param[0]) + 1);
	skey.set_data(param[1], strlen(param[1]) + 1);
	int ret = client_helper.prefix_remove(0, pkey, skey);
	return ret == TAIR_RETURN_SUCCESS;
}

bool tair_client::do_cmd_premoves(vector<char*> &param, char **data, int &len)
{
	return false;
}

bool tair_client::do_cmd_get_range(vector<char*> &param, char **data, int &len)
{
	if (param.size() < 1)
	{
		return false;
	}
	vector<data_entry *> values;
	data_entry pkey, skey, ekey;
	int offset = 0, limit = 0;
	pkey.set_data(param[0], strlen(param[0]) + 1);
	if (param.size() > 1)
	{
		offset = atoi(param[1]);
	}
	if (param.size() > 2)
	{
		limit = atoi(param[2]);
	}
	if (param.size() > 3)
	{
		skey.set_data(param[3], strlen(param[3]) + 1);
	}
	if (param.size() > 4)
	{
		ekey.set_data(param[4], strlen(param[4]) + 1);
	}

	int ret = client_helper.get_range(0, pkey, skey, ekey, offset, limit,
			values);

	if (ret == TAIR_RETURN_SUCCESS)
	{
		cJSON *pRoot = cJSON_CreateObject();
		cJSON *prefix = cJSON_CreateObject();
		cJSON_AddStringToObject(prefix, "prefix", pkey.get_data());
		cJSON_AddItemToObject(pRoot, "prefix", prefix);

		cJSON* pArray = cJSON_CreateArray();
		cJSON_AddItemToObject(pRoot, "keys", pArray);

		for (vector<data_entry *>::iterator it = values.begin();
				it != values.end(); ++it)
		{
			data_entry *entry = *it;
			cJSON* pItem = cJSON_CreateObject();
			cJSON_AddStringToObject(pItem, "key", entry->get_data());
			delete entry;

			++it;
			entry = *it;

			char *value_array = entry->get_data();
			char *pos = strstr(value_array, "|");
			if (pos == NULL)
			{
				cJSON_AddStringToObject(pItem, "value", value_array);
			}
			else
			{
				char value[2048] = { 0 };
				char createtime[50] = { 0 };
				memcpy(value, value_array, pos - value_array);
				cJSON_AddStringToObject(pItem, "value", value);

				value_array = pos + 1;
				pos = strstr(value_array, "|");
				if (pos != NULL)
				{
					memcpy(createtime, value_array, pos - value_array);
					cJSON_AddStringToObject(pItem, "createtime", createtime);
					value_array = pos + 1;
					cJSON_AddStringToObject(pItem, "expiretime", value_array);
				}
			}

			delete entry;

			cJSON_AddItemToArray(pArray, pItem);
		}
		char *out = cJSON_Print(pRoot);
		*data = out;
		len = strlen(out);
		return true;
	}
	else if (ret == TAIR_RETURN_DATA_NOT_EXIST)
	{
		return true;
	}
	else
	{
		return false;
	}
}
