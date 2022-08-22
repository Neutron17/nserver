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
#include "exitCodes.h"
#include "log.h"
#include "assrt.h"
#include "lt.h"
#include "msg.h"
#include "support.h"
#include "stdext.h"
#include "llist.h"
#include "signals.h"
#include "global.h"

void *threadstart(void *arg);

Node *availableFiles;
Node *usedFiles;

pthread_t threads[THREAD_NUM];

int main(int argc, char *argv[]) {
	init();
	printf("pid: %d\n", spid);

	union Args args = parseArgs(argc, argv);
	handleArgs(args);

	for(int i = 1; i < 255; i++)
		llist_add(&availableFiles, i);

	for (int i = 0; i < THREAD_NUM; i++)
		pthread_create(&threads[i], NULL, threadstart, NULL);
	ClientMsg msg = (ClientMsg){ .fields={
		2,
		.vmaj=VERSION_MAJOR, VERSION_MINOR,
		M_OPEN, 0
	} };
	while(running);

	cleanUp(E_SUCCESS);
	return 0;
}

void *threadstart(void *arg) {
	return NULL;
}

