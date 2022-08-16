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

#include "log.h"
#include "assrt.h"
#include "msg.h"
#include "support.h"
#include "common.h"

#define THREAD_NUM 10

void handler(int sig, siginfo_t *info, void *context);

bool running = true;

int main(int argc, char *argv[]) {
	printf("pid: %d\n", getpid());
	struct sigaction sa = { 0 };
	sa.sa_sigaction = handler;
	sa.sa_flags = SA_SIGINFO
#ifdef SA_RESTART
		| SA_RESTART
#endif
		;
	memset(&sa.sa_mask, 0, sizeof(sa.sa_mask));
	//sigemptyset(&sa.sa_mask);
	if(sigaction(SIGINT, &sa, NULL) == -1) {
		perror("sigaction");
		return -1;
	}
	if(sigaction(SIGUSR1, &sa, NULL) == -1) {
		perror("sigaction");
		return -1;
	}
	while(running);
	memdeb_print();
	return 0;
}

void handler(int sig, siginfo_t *info, void *context) {
	printf("\n%d signal received\n", sig);
	if(sig == SIGINT) {
		running = false;
		return;
	}
	ASSERT(sig == SIGUSR1 || sig == SIGUSR2);
	ASSERT(context != NULL);

	//struct flock fl;
	//fl.l_type   = F_WRLCK;  /* read/write lock */
	//fl.l_whence = SEEK_SET; /* beginning of file */
	//fl.l_start  = 0;        /* offset from l_whence */
	//fl.l_len    = 0;        /* length, 0 = to EOF */
	//fl.l_pid    = getpid(); /* PID */

	const char *fname = "file";
	int file = open("file", O_CREAT);
	if(file == -1) {
		LOGF(L_ERR, "Couldn't open file '%s'\n", fname);
		abort();
	}
	close(file);

	//kill(info->si_pid, SIGUSR1);
	sigqueue(info->si_pid, SIGUSR1, (union sigval) { .sival_int = 1 });

	//fcntl(fileno(file), F_SETLKW, &fl); /* set lock */
	//sleep(2);

	//fl.l_type   = F_UNLCK;
	//fcntl(fileno(file), F_SETLK, &fl); /* unset lock */
}

