#include "tairthread.h"
#include "tairclient.h"
#include "CStreamPipe.h"
#include "CThread.h"
#include "cJSON.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

tair_thread::tair_thread()
{
	thread_num = 0;
	active = false;
	m_pipe = new CStreamPipe();
	tair_client_map.clear();
}

tair_thread::~tair_thread()
{
	active = false;
	map<string, tair_client*>::iterator it = tair_client_map.begin();

	while (it != tair_client_map.end())
	{
		tair_client *client = it->second;
		delete client;
		++it;
	}

	if (m_pipe)
	{
		delete m_pipe;
		m_pipe = NULL;
	}
}

void tair_thread::init(int thread_num)
{
	this->thread_num = thread_num;
	active = true;
}

bool tair_thread::connect_tair(const char * serveraddr, const char * group)
{
	string key(serveraddr);
	key.append("_");

	key.append(group);
	map<string, tair_client*>::iterator it = tair_client_map.find(key);

	if (it != tair_client_map.end())
	{
		fprintf(stderr,
				"tairclient: tair connection for server '%s' '%s'\n",
				serveraddr, group);
		return true;
	}

	tair_client *client = new tair_client(this);
	client->setLog(tair_log);

	if (client)
	{
		if (client->connect(serveraddr, group))
		{
			tair_client_map.insert(
					map<string, tair_client*>::value_type(key, client));
		}
		else
		{
			delete client;
			return false;
		}

		return true;
	}
	else
	{
		delete client;
		return false;
	}
}

bool tair_thread::disconnect_tair(const char * serveraddr, const char * group)
{
	string key(serveraddr);
	key.append("_");
	key.append(group);

	map<string, tair_client*>::iterator it = tair_client_map.find(key);

	if (it != tair_client_map.end())
	{
		tair_client *client = (tair_client*) it->second;
		delete client;
		tair_client_map.erase(it);

		fprintf(stderr, "tairclient: tair disconnect for server '%s' '%s'\n",
				serveraddr, group);
		return true;
	}

	return false;
}

void* tair_thread::Run(void *ptr)
{
	char tmp[2048] = {0};
	sprintf(tmp, "tair_thread::Run");
	tair_log->write_log(tmp, 0, 0);

	void *pTxt = NULL;
	while (this->active)
	{
		int readLen = read(m_pipe->GetReadSocket(), &pTxt, sizeof(pTxt));
		if (readLen < 0)
		{
			memset(tmp, 0, 2048);
			// signal 4 and 11 need continue read
			if (errno == EAGAIN || errno == EINTR)
			{
				sprintf(tmp, "%s:%d errno[%d] message[%s]",
						__FILE__, __LINE__, errno, strerror(errno));

				usleep(1000);
				continue;
			}
			else
			{
				sprintf(tmp, "%s:%d errno[%d] message[%s]",
						__FILE__, __LINE__, errno, strerror(errno));
			}
			tair_log->write_log(tmp, 0, 0);
		}

		if (readLen > 0)
		{
			Context *ctx = static_cast<Context *>(pTxt);
			// 处理业务
			if (ctx)
			{
				this->parse_cmd(ctx->req);

				// 释放内存
				delete ctx;
				ctx = NULL;
			}
		}

//		usleep(1000);
		pTxt = NULL;
	}

	return NULL;
}

int8_t tair_thread::Write(Context *ctx)
{
	char tmp[2048];
	memset(tmp, 0, 2048);
	int ret = write(m_pipe->GetWriteSocket(), &ctx, sizeof(ctx));
	if (ret != sizeof(ctx))
	{
		sprintf(tmp, "%s:%d Write failed", __FILE__, __LINE__);
		tair_log->write_log(tmp, 0, 1);
		return -1;
	}

	return 0;
}

int8_t tair_thread::OnCreate()
{
	char tmp[2048];
	memset(tmp, 0, 2048);
	if (m_pipe->OnCreate() < 0)
	{
		sprintf(tmp, "%s:%d m_pipe->OnCreate failed", __FILE__, __LINE__);
		tair_log->write_log(tmp, 0, 0);
		return -1;
	}

	// 创建线程
	if (CreateThreatPool() < 0)
	{
		sprintf(tmp, "%s:%d CreateThreatPool failed", __FILE__, __LINE__);
		tair_log->write_log(tmp, 0, 0);
		return -1;
	}

	return 0;
}

int8_t tair_thread::CreateThreatPool()
{
	int8_t ret = 0;
	for (int32_t count = 0; count < thread_num; count++)
	{
		if (!CThread::Create(count))
		{
			ret = -1;
			break;
		}
	}

	return ret;
}

void tair_thread::parse_cmd(struct evhttp_request *req)
{
	char tmp[2048];
	vector<char*> param;

	const char *default_value = "0";
	char value_array[2048] = { 0 };
	int npos = 0;
	bool error = false;

	switch (evhttp_request_get_command(req))
	{
		case EVHTTP_REQ_GET:
		{
			const char *uri = evhttp_request_uri(req);
			if (uri != NULL)
			{
				char *decoded_uri = evhttp_decode_uri(uri);

				struct evkeyvalq params;

				evhttp_parse_query(decoded_uri, &params);

				const char *command = evhttp_find_header(&params, "command");
				const char *server_addr = evhttp_find_header(&params,
						"server_addr");
				const char *group_name = evhttp_find_header(&params, "group_name");

				if (!command || !server_addr || !group_name)
				{
					goto GET_END;
				}

				if (strcasecmp(command, "put") == 0)
				{
					const char *key = evhttp_find_header(&params, "key");

					const char *value = evhttp_find_header(&params, "value");
					const char *createtime = evhttp_find_header(&params,
							"createtime");

					const char *expiretime = evhttp_find_header(&params,
							"expiretime");
					if (!key || !value)
					{
						error = true;
						goto GET_END;
					}
					param.push_back((char*) key);
					if (createtime && expiretime)
					{
						memcpy(value_array, value, strlen(value));
						npos = strlen(value);
						value_array[npos] = '|';
						npos++;
						memcpy(value_array + npos, createtime, strlen(createtime));
						npos += strlen(createtime);
						value_array[npos] = '|';
						npos++;
						memcpy(value_array + npos, expiretime, strlen(expiretime));
						param.push_back((char*) value_array);
					}
					else
					{
						param.push_back((char*) value);
					}
				}
				else if (strcasecmp(command, "get") == 0
						|| strcasecmp(command, "remove") == 0)
				{
					const char *key = evhttp_find_header(&params, "key");
					if (!key)
					{
						error = true;
						goto GET_END;
					}
					param.push_back((char*) key);

				}
				else if (strcasecmp(command, "pget") == 0
						|| strcasecmp(command, "premove") == 0)
				{
					const char *prefix = evhttp_find_header(&params, "prefix");
					const char *key = evhttp_find_header(&params, "key");
					if (!prefix || !key)
					{
						error = true;
						goto GET_END;
					}
					param.push_back((char*) prefix);
					param.push_back((char*) key);
				}

				GET_END:
				if (error)
				{
					evhttp_send_reply(req, 403, "Forbidden", NULL);
				}
				else
				{
					memset(tmp, 0, 2048);
					sprintf(tmp, "%s:%d do_cmd[%s],[%s],[%s]", __FILE__, __LINE__,
							server_addr, group_name, command);
					tair_log->write_log(tmp, 0, 1);
					do_cmd(req, server_addr, group_name, command, param);
				}
				free(decoded_uri);
			}
		}
		break;

		case EVHTTP_REQ_POST:
		{
			struct evbuffer *buf = evhttp_request_get_input_buffer(req);
			size_t length = evbuffer_get_length(buf);
			if (length <= 0)
			{
				evhttp_send_reply(req, 403, "Forbidden", NULL);
				return;
			}
			char *data = (char *) malloc(length);
			evbuffer_copyout(buf, data, length);
			cJSON *pRoot = cJSON_Parse(data);
			cJSON *command = NULL;
			cJSON *server_addr = NULL;
			cJSON *group_name = NULL;

			if (!pRoot)
			{
				error = true;
				goto POST_END;
			}
			command = cJSON_GetObjectItem(pRoot, "command");
			server_addr = cJSON_GetObjectItem(pRoot, "server_addr");
			group_name = cJSON_GetObjectItem(pRoot, "group_name");

			if (!command || !server_addr || !group_name)
			{
				error = true;
				goto POST_END;
			}

			if (strcasecmp(command->valuestring, "get") == 0
					|| strcasecmp(command->valuestring, "remove") == 0
					|| strcasecmp(command->valuestring, "mget") == 0
					|| strcasecmp(command->valuestring, "mremove") == 0)
			{
				cJSON *keys = cJSON_GetObjectItem(pRoot, "keys");
				if (keys && cJSON_GetArraySize(keys) > 0)
				{
					for (int i = 0; i < cJSON_GetArraySize(keys); i++)
					{
						cJSON *item = cJSON_GetArrayItem(keys, i);
						if (item)
						{
							cJSON *key = cJSON_GetObjectItem(item, "key");
							if (key->valuestring)
							{
								param.push_back(key->valuestring);
							}
						}
					}
				}
				else
				{
					error = true;
					goto POST_END;
				}
			}
			else if (strcasecmp(command->valuestring, "put") == 0
					|| strcasecmp(command->valuestring, "puts") == 0)
			{
				cJSON *keys = cJSON_GetObjectItem(pRoot, "keys");
				if (keys && cJSON_GetArraySize(keys) > 0)
				{
					for (int i = 0; i < cJSON_GetArraySize(keys); i++)
					{
						cJSON *item = cJSON_GetArrayItem(keys, i);
						cJSON *key = cJSON_GetObjectItem(item, "key");
						cJSON *value = cJSON_GetObjectItem(item, "value");
						cJSON *createtime = cJSON_GetObjectItem(item, "createtime");
						cJSON *expiretime = cJSON_GetObjectItem(item, "expiretime");
						if (!key || !value)
						{
							error = true;
							goto POST_END;
						}
						param.push_back(key->valuestring);

						if (createtime && expiretime)
						{
							memcpy(value_array, value->valuestring, strlen(value->valuestring));
							npos = strlen(value->valuestring);
							value_array[npos] = '|';
							npos++;

							if (createtime->valuestring)
							{
								memcpy(value_array + npos, createtime->valuestring,
										strlen(createtime->valuestring));
								npos += strlen(createtime->valuestring);
							}
							else if (createtime->valueint)
							{
								char valueint[50] = { 0 };
								sprintf(valueint, "%d", createtime->valueint);
								memcpy(value_array + npos, valueint,
										strlen(valueint));
								npos += strlen(valueint);
							}
							else if (createtime->valuedouble)
							{
								char valuedouble[50] = { 0 };
								sprintf(valuedouble, "%d",
										(int) (createtime->valuedouble));
								memcpy(value_array + npos, valuedouble,
										strlen(valuedouble));
								npos += strlen(valuedouble);
							}

							value_array[npos] = '|';
							npos++;

							if (expiretime->valuestring)
							{
								memcpy(value_array + npos, expiretime->valuestring,
										strlen(expiretime->valuestring));
							}
							else if (expiretime->valueint)
							{
								char valueint[50] = { 0 };
								sprintf(valueint, "%d", expiretime->valueint);
								memcpy(value_array + npos, valueint,
										strlen(valueint));
							}
							else if (expiretime->valuedouble)
							{
								char valuedouble[50] = { 0 };
								sprintf(valuedouble, "%d",
										(int) (expiretime->valuedouble));
								memcpy(value_array + npos, valuedouble,
										strlen(valuedouble));
							}

							param.push_back((char*) value_array);
						}
						else
						{
							param.push_back(value->valuestring);
						}
					}
				}
				else
				{
					error = true;
					goto POST_END;
				}
			}
			else if (strcasecmp(command->valuestring, "pget") == 0
					|| strcasecmp(command->valuestring, "premove") == 0
					|| strcasecmp(command->valuestring, "pgets") == 0
					|| strcasecmp(command->valuestring, "premoves") == 0)
			{
				cJSON *keys = cJSON_GetObjectItem(pRoot, "keys");
				if (keys && cJSON_GetArraySize(keys) > 0)
				{
					for (int i = 0; i < cJSON_GetArraySize(keys); i++)
					{
						cJSON *item = cJSON_GetArrayItem(keys, i);
						cJSON *prefix = cJSON_GetObjectItem(item, "prefix");
						cJSON *key = cJSON_GetObjectItem(item, "key");
						if (prefix && key)
						{
							param.push_back(prefix->valuestring);
							param.push_back(key->valuestring);
						}
						else
						{
							error = true;
							goto POST_END;
						}
					}
				}
				else
				{
					error = true;
					goto POST_END;
				}
			}
			else if (strcasecmp(command->valuestring, "pput") == 0
					|| strcasecmp(command->valuestring, "pputs") == 0)
			{
				cJSON *keys = cJSON_GetObjectItem(pRoot, "keys");
				if (keys && cJSON_GetArraySize(keys) > 0)
				{
					for (int i = 0; i < cJSON_GetArraySize(keys); i++)
					{
						cJSON *item = cJSON_GetArrayItem(keys, i);
						cJSON *prefix = cJSON_GetObjectItem(item, "prefix");
						cJSON *key = cJSON_GetObjectItem(item, "key");
						cJSON *value = cJSON_GetObjectItem(item, "value");
						cJSON *createtime = cJSON_GetObjectItem(item, "createtime");
						cJSON *expiretime = cJSON_GetObjectItem(item, "expiretime");
						if (prefix && key && value)
						{
							param.push_back(prefix->valuestring);
							param.push_back(key->valuestring);
							if (createtime && expiretime)
							{
								memcpy(value_array, value->valuestring,
										strlen(value->valuestring));
								npos = strlen(value->valuestring);
								value_array[npos] = '|';
								npos++;
								if (createtime->valuestring)
								{
									memcpy(value_array + npos,
											createtime->valuestring,
											strlen(createtime->valuestring));
									npos += strlen(createtime->valuestring);
								}
								else if (createtime->valueint)
								{
									char valueint[50] = { 0 };
									sprintf(valueint, "%d", createtime->valueint);
									memcpy(value_array + npos, valueint,
											strlen(valueint));
									npos += strlen(valueint);
								}
								else if (createtime->valuedouble)
								{
									char valuedouble[50] = { 0 };
									sprintf(valuedouble, "%d",
											(int) (createtime->valuedouble));
									memcpy(value_array + npos, valuedouble,
											strlen(valuedouble));
									npos += strlen(valuedouble);
								}
								value_array[npos] = '|';
								npos++;

								if (expiretime->valuestring)
								{
									memcpy(value_array + npos,
											expiretime->valuestring,
											strlen(expiretime->valuestring));
								}
								else if (expiretime->valueint)
								{
									char valueint[50] = { 0 };
									sprintf(valueint, "%d", expiretime->valueint);
									memcpy(value_array + npos, valueint,
											strlen(valueint));
								}
								else if (expiretime->valuedouble)
								{
									char valuedouble[50] = { 0 };
									sprintf(valuedouble, "%d",
											(int) (expiretime->valuedouble));
									memcpy(value_array + npos, valuedouble,
											strlen(valuedouble));
								}

								param.push_back((char*) value_array);
							}
							else
							{
								param.push_back(value->valuestring);
							}
						}
						else
						{
							error = true;
							goto POST_END;
						}
					}
				}
				else
				{
					error = true;
					goto POST_END;
				}
			}
			else if (strcasecmp(command->valuestring, "get_range") == 0)
			{
				cJSON *keys = cJSON_GetObjectItem(pRoot, "keys");
				if (keys)
				{
					cJSON *prefix = cJSON_GetObjectItem(keys, "prefix");
					cJSON *offset = cJSON_GetObjectItem(keys, "offset");
					cJSON *limit = cJSON_GetObjectItem(keys, "limit");
					cJSON *skey = cJSON_GetObjectItem(keys, "skey");
					cJSON *ekey = cJSON_GetObjectItem(keys, "ekey");
					if (prefix)
					{
						param.push_back(prefix->valuestring);
						if (offset)
						{
							param.push_back(offset->valuestring);
						}
						else
						{
							param.push_back((char*) default_value);
						}

						if (limit)
						{
							param.push_back(limit->valuestring);
						}
						else
						{
							param.push_back((char*) default_value);
						}

						if (skey)
						{
							param.push_back(skey->valuestring);
						}
						if (ekey)
						{
							param.push_back(ekey->valuestring);
						}
					}
					else
					{
						error = true;
						goto POST_END;
					}
				}
			}
			else
			{
				error = true;
			}

			POST_END:
			if (error)
			{
				evhttp_send_reply(req, 403, "Forbidden", NULL);
			}
			else
			{
				memset(tmp, 0, 2048);
				sprintf(tmp, "%s:%d do_cmd[%s],[%s],[%s]", __FILE__, __LINE__,
						server_addr->valuestring, group_name->valuestring,
						command->valuestring);
				tair_log->write_log(tmp, 0, 1);

				do_cmd(req, server_addr->valuestring, group_name->valuestring,
						command->valuestring, param);
			}

			free(data);
			cJSON_Delete(pRoot);
		}
		break;

		default:
		{
			memset(tmp, 0, 2048);
			sprintf(tmp, "%s:%d parse_cmd req[%d]", __FILE__, __LINE__,
					evhttp_request_get_command(req));
			tair_log->write_log(tmp, 0, 0);
		}
	}

	return;
}

//check ip is ok
int is_valid_ip(const char *ip)
{
	int section = 0; //每一节的十进制值
	int dot = 0; //几个点分隔符
	int last = -1; //每一节中上一个字符
	while (*ip)
	{
		if (*ip == '.')
		{
			dot++;
			if (dot > 3)
			{
				return 0;
			}
			if (section >= 0 && section <= 255)
			{
				section = 0;
			}
			else
			{
				return 0;
			}
		}
		else if (*ip >= '0' && *ip <= '9')
		{
			section = section * 10 + *ip - '0';
			if (last == '0')
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
		last = *ip;
		ip++;
	}

	if (section >= 0 && section <= 255)
	{
		if (3 == dot)
		{
			section = 0;
			return 0;
		}
	}
	return 1;
}

void tair_thread::do_cmd(struct evhttp_request * req, const char *serveraddr,
		const char *groupname, const char *cmd, vector<char*> &param)
{
	char tmp[2048];
	tair_client *client, *client_slave, *client_master = NULL;

	char *data = NULL;
	int len = 0, master = 0, slave = 0;
	char log_tempbuf[1024];

	bool ret = true;

	char tmp_serveraddr[256];
	memset(tmp_serveraddr, 0, 256);
	memcpy(tmp_serveraddr, serveraddr, strlen(serveraddr));

	char *serveraddr_master = strtok(tmp_serveraddr, ",");

	vector<char *> tmpserveraddr;
	tmpserveraddr.clear();

	if (serveraddr_master)
	{
		tmpserveraddr.push_back(serveraddr_master);
	}

	while ((serveraddr_master = strtok(NULL, ",")) != NULL)
	{
		if (is_valid_ip(serveraddr_master))
		{
			//evhttp_send_reply(req, 503, "Service Unavailable", NULL);
			//return;
			break;
		}
		tmpserveraddr.push_back(serveraddr_master);
	}

	if (tmpserveraddr.size() == 1)
	{
		map<string, tair_client*>::iterator it;
		string key_master(tmpserveraddr[0]);
		key_master.append("_");
		key_master.append(groupname);
		it = tair_client_map.find(key_master);
		if (it == tair_client_map.end())
		{
			client_master = new tair_client(this);
			client_master->setLog(tair_log);
			ret = client_master->connect(tmpserveraddr[0], groupname);

			if (ret)
			{
				//fprintf(stderr, "insert client master");
				tair_client_map.insert(
						map<string, tair_client*>::value_type(key_master,
								client_master));
				client = (tair_client*) client_master;
				ret = client->do_cmd(cmd, param, &data, len);
			}
			else
			{
				delete client_master;
			}
		}
		else
		{
			client = (tair_client*) it->second;
//			client->setLog(tair_log);
			ret = client->do_cmd(cmd, param, &data, len);
		}
	}
	else if (tmpserveraddr.size() == 0)
	{
		evhttp_send_reply(req, 503, "Service Unavailable", NULL);
		return;
	}
	else
	{
		int addresscount = tmpserveraddr.size();
		srand((unsigned) time( NULL));
		int static_count = rand() % addresscount;

		map<string, tair_client*>::iterator it;
		string key_master(tmpserveraddr[static_count]);
		key_master.append("_");
		key_master.append(groupname);

		it = tair_client_map.find(key_master);

		if (it != tair_client_map.end())
		{
			client = (tair_client*)it->second;
//			client->setLog(tair_log);
			ret = client->do_cmd(cmd, param, &data, len);
		}
		else
		{
			client_master = new tair_client(this);
			client_master->setLog(tair_log);
			ret = client_master->connect(tmpserveraddr[static_count],
					groupname);

			if (ret)
			{
				tair_client_map.insert(map<string, tair_client*>::value_type(key_master,
						client_master));
				client = (tair_client*)client_master;
				ret = client->do_cmd(cmd, param, &data, len);
			}
			else
			{
				delete client_master;

			}
		}
	}

	memset(log_tempbuf, 0, 1024);
	strcat(log_tempbuf, "Request Method:");
	strcat(log_tempbuf, cmd);

	if (ret)
	{
		if (len > 0)
		{
			struct evbuffer * buf = evbuffer_new();
			evbuffer_prepend(buf, data, (size_t) len);
			evhttp_send_reply(req, 200, "OK", buf);
			evbuffer_free(buf);

			strcat(log_tempbuf, " Status:");
			strcat(log_tempbuf, "200 OK");

			tair_log->write_log(log_tempbuf, 0, 0);
		}
		else
		{
			strcat(log_tempbuf, " Status:");
			strcat(log_tempbuf, "200 OK");

			tair_log->write_log(log_tempbuf, 0, 0);
			evhttp_send_reply(req, 200, "OK", NULL);
		}
	}
	else
	{
		evhttp_send_reply(req, 503, "Service Unavailable", NULL);
		tair_log->write_log("Service Unavailable", 0, 0);
		if (client)
		{
			//to do
		}
	}

	if (data)
	{
		fprintf(stderr, data);
		fprintf(stderr, "\n");
		free(data);
	}
}
