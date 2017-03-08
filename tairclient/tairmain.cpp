#include <sys/socket.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "tairwrapper.h"
#include "log.h"

TairLog *tair_log = new TairLog();

static void syntax(void)
{
	fprintf(stdout, "Syntax: tairclient server <docroot>\n");
}

tair_wrapper wrapper;

/* Callback used for the /tair URI */
static void tair_request_cb(struct evhttp_request *req, void *arg)
{
	wrapper.dispatch(req);
}

int main(int argc, char **argv)
{
	// ignore singal sigint
	struct sigaction action;
	action.sa_handler = SIG_IGN;
	sigemptyset(&action.sa_mask);
	sigaction(SIGALRM, &action, NULL);

	struct event_base *base;
	struct evhttp *http;
	struct evhttp_bound_socket *handle;

	char *addr = NULL;
	unsigned short port = 0;
	int threadnum = 4;
	int loglevel = 0;

	// ignore signal
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
	{
		return (1);
	}
	if (signal(SIGUSR1, SIG_IGN) == SIG_ERR)
	{
		return (1);
	}
	if (signal(SIGUSR2, SIG_IGN) == SIG_ERR)
	{
		return (1);
	}

	if (argc >= 2)
	{
		addr = argv[1];
	}
	if (argc >= 3)
	{
		port = atoi(argv[2]);
	}

	if (argc >= 4)
	{
		threadnum = atoi(argv[3]);
	}

	if (argc >= 5)
	{
		loglevel = atoi(argv[4]);
	}

	if (port == 0)
	{
		port = 80;
	}

	tair_log->init();
	evthread_use_pthreads();

	// set log level
	if (loglevel == 0)
	{
		tair_log->setLogLevel("INFO");
	}
	else
	{
		tair_log->setLogLevel("TRACE");
	}

	wrapper.init(threadnum);

	base = event_base_new();
	if (!base)
	{
		fprintf(stderr, "Couldn't create an event_base: exiting\n");
		return 1;
	}

	/* Create a new evhttp object to handle requests. */
	http = evhttp_new(base);
	if (!http)
	{
		fprintf(stderr, "couldn't create evhttp. Exiting.\n");
		return 1;
	}

	/* The /dump URI will dump all requests to stdout and say 200 ok. */
	evhttp_set_cb(http, "/tair", tair_request_cb, NULL);

	/* We want to accept arbitrary requests, so we need to set a "generic"
	 * cb.  We can also add callbacks for specific paths. */
	//evhttp_set_gencb(http, send_document_cb, argv[1]);
	/* Now we tell the evhttp what port to listen on */
	handle = evhttp_bind_socket_with_handle(http,
			addr == NULL ? "0.0.0.0" : addr, port);
	if (!handle)
	{
		fprintf(stderr, "tairclient: couldn't bind to port %d. Exiting.\n",
				(int) port);
		return 1;
	}

	{
		/* Extract and display the address we're listening on. */
		struct sockaddr_storage ss;
		evutil_socket_t fd;
		ev_socklen_t socklen = sizeof(ss);
		char addrbuf[128];
		void *inaddr;
		const char *addr;
		int got_port = -1;
		fd = evhttp_bound_socket_get_fd(handle);
		memset(&ss, 0, sizeof(ss));
		if (getsockname(fd, (struct sockaddr *) &ss, &socklen))
		{
			return 1;
		}
		if (ss.ss_family == AF_INET)
		{
			got_port = ntohs(((struct sockaddr_in*) &ss)->sin_port);
			inaddr = &((struct sockaddr_in*) &ss)->sin_addr;
		}
		else if (ss.ss_family == AF_INET6)
		{
			got_port = ntohs(((struct sockaddr_in6*) &ss)->sin6_port);
			inaddr = &((struct sockaddr_in6*) &ss)->sin6_addr;
		}
		else
		{
			fprintf(stderr, "tairclient: Weird address family %d\n",
					ss.ss_family);
			return 1;
		}
		addr = evutil_inet_ntop(ss.ss_family, inaddr, addrbuf, sizeof(addrbuf));
		if (!addr)
		{
			fprintf(stderr, "tairclient: evutil_inet_ntop failed\n");
			return 1;
		}
	}

	event_base_dispatch(base);

	evhttp_free(http);

	return 0;
}

