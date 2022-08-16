#include "assrt.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "log.h"
#include "common.h"

void __assert_m(bool expr, const char *restrict msg, const char *restrict file,
#ifndef NO_FUNC
		const char *restrict func,
#endif
		int line, bool log) {
	if(!expr) {
		char mesg[256];
		snprintf(mesg, 256, "ERROR: Assertion failed\n"
				"\tin file: '%s'\n"
#ifndef NO_FUNC
				"\tin function: '%s'\n"
#endif
				"\tat line: %d\n"
				"\terrno: %d\n"
				"\tstrerror: '%s'\n"

				, file,
#ifndef NO_FUNC
				func,
#endif
				line, errno, strerror(errno)
		);
		if(msg) {
			strncat(mesg, "\tMessage: '", 12);
			strncat(mesg, msg, 32);
			strncat(mesg, "'", 2);

			//snprintf(mesg, 256, "%s\tMessage: '%s'\n", mesg, msg);
		}
		fprintf(stderr, "%s", mesg);
		if(log)
			LOGF(L_ERR, "%s", msg);
		abort();
	}
}

