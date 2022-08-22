#ifndef _NTR_SIGS_H_
#define _NTR_SIGS_H_ 1

#include <signal.h>
#include <sys/types.h>
#include <pthread.h>
#include "msg.h"

extern pthread_mutex_t sigmutex;
extern pthread_mutex_t sigthmut;

// main signal handler
void sighandler(int sig, siginfo_t *info, void *context);
// Send ServerMsg to process id
void sendSMtoPID(ServerMsg msg, pid_t pid);

#endif /* ifndef _NTR_SIGS_H_ */
