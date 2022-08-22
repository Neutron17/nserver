#include "signals.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "log.h"
#include "stdext.h"
#include "assrt.h"
#include "global.h"

#define LISTEN_BACKLOG 50

pthread_mutex_t sigmutex;

static int thcount = 0;
pthread_mutex_t sigthmut;

struct SigArg {
	int sig;
	siginfo_t info;
	int th;
};

struct Socket {
	int fd;
	struct sockaddr_un addr;
	char fname[12];
};

pthread_t pthreads[255] = { 0 };

static void *createServer(void *arg);
static struct Socket socketCreate(const char *name);
static void socketDestroy();
static inline void sendPID(pid_t pid, int bits);

void sighandler(int sig, siginfo_t *info, void *context) {
	//pthread_mutex_lock(&sigmutex);
	if(thcount > 255)
		return;
	struct SigArg *arg = malloc(sizeof(struct SigArg));
	arg->sig = sig;
	arg->th = thcount;
	memcpy(&arg->info, info, sizeof(siginfo_t));
	pthread_mutex_lock(&sigthmut);
	thcount++;
	pthread_mutex_unlock(&sigthmut);
	pthread_create(&pthreads[thcount], NULL, createServer, (void *)arg);
	//pthread_mutex_unlock(&sigmutex);
}

static void *createServer(void *arg) {
	int sig = ((struct SigArg*)arg)->sig;
	siginfo_t info = ((struct SigArg*)arg)->info;

	const int cpid = info.si_pid;
	const char *cname = pidname(cpid);
	LOGF(L_INFO, "Signal '%d', received from %d(%s)", sig, cpid, cname);
	if(sig == SIGINT) {
		running = false;
		if(cname && strcmp(cname, "unknown") != 0)
			free((char *)cname);
		return NULL;
	}
	ASSERT(sig == SIGUSR1 || sig == SIGUSR2);

	ClientMsg clientM;
	// because "ISO C forbids casts to union type"
	clientM.bits = (unsigned)info.si_value.sival_int;
	LOGF(L_INFO, "%d", clientM.fields.type);
	ServerMsg intr = {
		.fields = {
			VERSION_MAJOR, VERSION_MINOR,
			STATUS_SUCCESS,
			SI_DEBUG, DEBUG_LOGS, ((struct SigArg *)arg)->th
		}
	};

	const size_t fname_len = 6 + 3;
	char fname[fname_len];
	sprintf(fname, "files/%d", ((struct SigArg *)arg)->th);

	struct Socket sock = socketCreate(fname);
	LOGF(L_INFO, "Opened file: '%s', requested by '%d'(%s)", fname, cpid, cname);
	sendPID(cpid, intr.bits);

	puts("Waiting...");
	struct sockaddr_un client_addr;
	socklen_t len;
	int client = accept(sock.fd, (struct sockaddr *) &client_addr, &len);
	printf("Accepted %d\n", client);
	if(getpeername(client, (struct sockaddr *) &client_addr, &len) == -1) {
		LOG(L_ERR, "getpeername");
	} else {
		printf("Client socket filepath: %s\n", client_addr.sun_path);
	}

	char buf[16];
	printf("waiting to read...\n");
	int bytes_rec = recv(client, buf, sizeof(buf), 0);
	if (bytes_rec == -1){
		LOG(L_ERR, "recv");
		close(sock.fd);
		close(client);
		exit(1);
	} else {
		printf("DATA RECEIVED = %s\n", buf);
	}

	if(close(sock.fd) == -1)
		LOG(L_ERR, "close");

	//pthread_join(pthreads[((struct SigArg *)arg)->th], NULL);

	pthread_mutex_lock(&sigthmut);
	thcount--;
	pthread_mutex_unlock(&sigthmut);
	free((struct SigArg *)arg);
	if(cname && strcmp(cname, "unknown") != 0)
		free((char *)cname);
	return NULL;
}

static struct Socket socketCreate(const char *fname) {
	struct Socket sock;
	sock.fd = socket(AF_UNIX, SOCK_STREAM, 0);
	printf("socket: %d\n", sock.fd);
	if(sock.fd == -1) {
		LOG(L_ERR, "Couldn't open socket");
		exit(-1);
	}
	if(unlink(fname) == -1 && errno != ENOENT) {
		LOG(L_ERR, "remove");
		exit(-2);
	}
	strcpy(sock.fname, fname);
	memset(&sock.addr, 0, sizeof(struct sockaddr_un));
	sock.addr.sun_family = AF_UNIX;
	strcpy(sock.addr.sun_path, fname);
	if(bind(sock.fd, (struct sockaddr *)&sock.addr, sizeof(struct sockaddr_un)) == -1) {
		LOG(L_ERR, "bind");
		exit(-3);
	}
	// mark passive
	if(listen(sock.fd, LISTEN_BACKLOG) == -1) {
		LOG(L_ERR, "listen");
		exit(-4);
	}
	return sock;
}

static void socketDestroy(struct Socket sock) {
	close(sock.fd);
	unlink(sock.fname);
}

static inline void sendPID(pid_t pid, int bits) {
	sigqueue(pid, SIGUSR1, (union sigval) { .sival_int = bits });
}

