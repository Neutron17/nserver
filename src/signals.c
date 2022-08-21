#include "signals.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "log.h"
#include "stdext.h"
#include "assrt.h"
#include "common.h"

void sighandler(int sig, siginfo_t *info, void *context) {
	const int cpid = info->si_pid;
	char *cname = (char *) pidname(cpid);
	LOGF(L_INFO, "Signal '%d', received from %d(%s)", sig, cpid, cname);
	if(sig == SIGINT) {
		running = false;
		free(cname);
		return;
	}
	ASSERT(sig == SIGUSR1 || sig == SIGUSR2);
	ASSERT(context != NULL);

	ClientMsg clientM;
	// because "ISO C forbids casts to union type"
	clientM.bits = (unsigned)info->si_value.sival_int;
	ServerMsg intr = {
		.fields = {
			VERSION_MAJOR, VERSION_MINOR,
			STATUS_SUCCESS,
			SI_DEBUG, DEBUG_LOGS
		}
	};

	FILE *file = NULL;
	const size_t fname_len = 6 + 3;
	char *fname = malloc(fname_len);
	strcpy(fname, "files/1");
	// TODO
	//itoa(clientM.fields.fname, fname + 6, 10);
	file = fopen(fname, "w");
	if(!file) {
		LOGF(L_ERR, "Couldn't open file '%s'\n", fname);
		exit(E_IO);
	}
	fprintf(file, "hi");
	LOGF(L_INFO, "Opened file: '%s', requested by '%d'(%s)", fname, cpid, cname);
	free(fname);
	fclose(file);

	if(spid != cpid)
		sigqueue(cpid, SIGUSR1, (union sigval) { .sival_int = intr.bits });
	free(cname);
}

