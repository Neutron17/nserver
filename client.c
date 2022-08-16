#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

#include "msg.h"

bool islocked(const char *fname);
void sigHandler(int sig, siginfo_t *info, void *context);

bool running = true;

int main(int argc, char *argv[]) {
	if(argc < 2) {
		fprintf(stderr, "ERROR: Not enough arguments");
		return -1;
	}
	struct sigaction sa = {0};
	sa.sa_sigaction = sigHandler;
	sa.sa_flags = SA_RESTART | SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	if(sigaction(SIGINT, &sa, NULL) == -1) {
		perror("sigaction");
		return -1;
	}
	if(sigaction(SIGUSR1, &sa, NULL) == -1) {
		perror("sigaction");
		return -1;
	}


	struct Msg msg = { 1, 0, M_ERR, 0, 1, 0 };
	sigqueue(atoi(argv[1]), SIGUSR1,
			(union sigval) {
				.sival_int = ((int)msg)
			}
		);
	//kill(atoi(argv[1]), SIGUSR1);
	while(running);

	return 0;
}

void sigHandler(int sig, siginfo_t *info, void *context) {
	if(sig == SIGINT)
		running  = false;
	if(sig != SIGUSR1)
		return;
	printf("pid:%d data:%d\n", info->si_pid, info->si_value.sival_int);
}

bool islocked(const char *fname) {
	int f = open(fname, O_RDONLY);
	if(f == -1) {
		 fprintf(stderr, "ERROR: Couldn't open file: '%s'\n", strerror(errno));
	}
	struct flock fl;
	memset(&fl, 0, sizeof(struct flock));
	if(fcntl(f, F_GETLK, &fl) == -1) {
		if(errno == EACCES || errno == EAGAIN) {
			close(f);
			return true;
		} else {
			fprintf(stderr, "ERROR: %s\n", strerror(errno));
			return false;
		}
	} else {
		close(f);
		return false;
	}

}

