#include "tairwrapper.h"
#include "tairclient.h"
#include "tairthread.h"
#include "cJSON.h"

tair_wrapper::tair_wrapper()
{
	pTair_thread = new tair_thread();;
}

tair_wrapper::~tair_wrapper()
{
	if (pTair_thread)
	{
		delete pTair_thread;
		pTair_thread = NULL;
	}
}

void tair_wrapper::init(int thread_count)
{
	pTair_thread->init(thread_count);
	if (0 != pTair_thread->OnCreate())
	{
		printf("tair_wrapper::OnCreate failed\n");
		return;
	}
}

void tair_wrapper::dispatch(struct evhttp_request *req)
{
	char tmp[2048];
	memset(tmp, 0, 2048);

	if (NULL == req)
	{
		sprintf(tmp, "%s:%d req = NULL", __FILE__, __LINE__);
		tair_log->write_log(tmp, 0, 0);
		return;
	}

	Context *msg = new Context();
	memset(msg, 0x00, sizeof(Context));
	msg->req = req;

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
					memset(tmp, 0, 2048);
					sprintf(tmp, "%s:%d error", __FILE__, __LINE__);
					tair_log->write_log(tmp, 0, 0);

					evhttp_send_reply(req, 403, "Forbidden", NULL);
					free(decoded_uri);
					delete msg;
					return;
				}
				if (strcasecmp(command, "connect") == 0
						|| strcasecmp(command, "disconnect") == 0)
				{
					memset(tmp, 0, 2048);
					sprintf(tmp, "%s:%d error", __FILE__, __LINE__);
					tair_log->write_log(tmp, 0, 0);

					evhttp_send_reply(req, 200, "OK", NULL);
					delete msg;
				}
				else
				{
					int ret = pTair_thread->Write(msg);
					if (ret != 0)
					{
						memset(tmp, 0, 2048);
						sprintf(tmp, "%s:%d Write failed", __FILE__, __LINE__);
						tair_log->write_log(tmp, 0, 0);

						evhttp_send_reply(req, 503, "Service Unavailable", NULL);
						delete msg;
					}
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
				memset(tmp, 0, 2048);
				sprintf(tmp, "%s:%d length <= 0", __FILE__, __LINE__);
				tair_log->write_log(tmp, 0, 0);

				evhttp_send_reply(req, 403, "Forbidden", NULL);
				return;
			}
			char *data = (char*) malloc(length + 1);
			evbuffer_copyout(buf, data, length);
			data[length] = 0;

			cJSON *pRoot = cJSON_Parse(data);
			if (pRoot == NULL)
			{
				memset(tmp, 0, 2048);
				sprintf(tmp, "%s:%d pRoot == NULL", __FILE__, __LINE__);
				tair_log->write_log(tmp, 0, 0);

				evhttp_send_reply(req, 403, "Forbidden", NULL);
				free(data);
				delete msg;
				return;
			}

			cJSON *command = cJSON_GetObjectItem(pRoot, "command");
			cJSON *server_addr = cJSON_GetObjectItem(pRoot, "server_addr");
			cJSON *group_name = cJSON_GetObjectItem(pRoot, "group_name");
			if (!command || !server_addr || !group_name)
			{
				memset(tmp, 0, 2048);
				sprintf(tmp, "%s:%d null value", __FILE__, __LINE__);
				tair_log->write_log(tmp, 0, 0);

				evhttp_send_reply(req, 403, "Forbidden", NULL);
				free(data);
				delete msg;
				cJSON_Delete(pRoot);
				return;
			}
			if (strcasecmp(command->valuestring, "connect") == 0
					|| strcasecmp(command->valuestring, "disconnect") == 0)
			{
				memset(tmp, 0, 2048);
				sprintf(tmp, "%s:%d invalid value", __FILE__, __LINE__);
				tair_log->write_log(tmp, 0, 0);

				evhttp_send_reply(req, 200, "OK", NULL);
				delete msg;
			}
			else
			{
				int ret = pTair_thread->Write(msg);
				if (ret != 0)
				{
					memset(tmp, 0, 2048);
					sprintf(tmp, "%s:%d Write failed", __FILE__, __LINE__);
					tair_log->write_log(tmp, 0, 0);

					evhttp_send_reply(req, 503, "Service Unavailable", NULL);
					delete msg;
				}
			}
			free(data);
			cJSON_Delete(pRoot);
		}
		break;

		default:
		{
			memset(tmp, 0, 2048);
			sprintf(tmp, "%s:%d invalid req[%d]", __FILE__, __LINE__,
					evhttp_request_get_command(req));
			tair_log->write_log(tmp, 0, 0);
		}
	}
}

