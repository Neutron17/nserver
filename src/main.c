#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/stat.h>

#include "args.h"
#include "log.h"
#include "assrt.h"
#include "msg.h"
#include "support.h"
#include "stdext.h"
#include "llist.h"
#include "signals.h"
#include "common.h"

#define THREAD_NUM 255

void *threadstart(void *arg);

Node *availableFiles;
Node *usedFiles;

int main(int argc, char *argv[]) {
	spid = getpid();
	printf("pid: %d\n", spid);

	union Args args = parseArgs(argc, argv);
	handleArgs(args);


	availableFiles = llist_create(0);
	usedFiles = llist_create(0);
	for(int i = 1; i < 255; i++)
		llist_add(&availableFiles, i);

	pthread_t threads[THREAD_NUM];
	for (int i = 0; i < THREAD_NUM; i++)
		pthread_create(&threads[i], NULL, threadstart, NULL);
	struct sigaction sa = { 0 };
	sa.sa_sigaction = sighandler;
	sa.sa_flags =
#ifdef SA_RESTART
		SA_RESTART |
#endif
		SA_SIGINFO;
	memset(&sa.sa_mask, 0, sizeof(sa.sa_mask));
	//sigemptyset(&sa.sa_mask);
	if(sigaction(SIGINT, &sa, NULL) == -1) {
		LOG(L_ERR, "sigaction(SIGINT) failed");
		return -1;
	}
	if(sigaction(SIGUSR1, &sa, NULL) == -1) {
		LOG(L_ERR, "sigaction(SIGUSR1) failed");
		return -1;
	}
	ClientMsg msg = (ClientMsg){ .fields={
		2,
		.vmaj=VERSION_MAJOR, VERSION_MINOR,
		M_OPEN, 0
	} };
	sigqueue(spid, SIGUSR1, (union sigval) { .sival_int = msg.bits });
	//while(running);
	for(int i = 0; i < 255; i++) {
		llist_pop(&availableFiles);
		llist_pop(&usedFiles);
	}
	for (int i = 0; i < THREAD_NUM; i++)
		pthread_join(threads[i], NULL);
	//free(availableFiles);
	//free(usedFiles);
#ifdef MEM_DEBUG
	memdeb_print();
#endif
	return 0;
}

void *threadstart(void *arg) {
	return NULL;
}

