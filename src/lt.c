#include "lt.h"
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#include "log.h"
#include "exitCodes.h"
#include "signals.h"
#include "llist.h"
#include "global.h"

extern pthread_mutex_t logmutex, sigthmut;
extern pid_t spid;
extern Node *availableFiles;
extern Node *usedFiles;
extern pthread_t threads[THREAD_NUM];

void init() {
	pthread_mutex_init(&logmutex, 0);
	pthread_mutex_init(&sigthmut, 0);
	loginit(L_ALL, L_ALL);
	spid = getpid();

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
		cleanUp(E_INIT);
	}
	if(sigaction(SIGUSR1, &sa, NULL) == -1) {
		LOG(L_ERR, "sigaction(SIGUSR1) failed");
		cleanUp(E_INIT);
	}
	availableFiles = llist_create(0);
	usedFiles = llist_create(0);
}

__attribute__((noreturn))
void cleanUp(int ret) {
	LOGF(L_INFO, "Cleaning up with return code: %d", ret);
	pthread_mutex_destroy(&logmutex);
	pthread_mutex_destroy(&sigthmut);
	for(int i = 0; i < 255; i++) {
		llist_pop(&availableFiles);
		llist_pop(&usedFiles);
	}
	for (int i = 0; i < THREAD_NUM; i++)
		pthread_join(threads[i], NULL);

	// will wait for loging to finish
	logdestroy();
	exit(ret);
}

