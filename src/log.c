#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>

#include "log.h"
#include "assrt.h"
#include "common.h"
#include "stdext.h"

const char *logprefix[3] = {
	"INFO: ", "WARINING: ", "ERROR: "
};

void logtofile(const char *msg);
// appends runtime info to dest
void appendRTimeInfo(char *dest, const char *file,
#ifndef NO_FUNC
		const char *func,
#endif
		int line);

void __logf(enum LogLevel level, const char *file,
#ifndef NO_FUNC
		const char *func,
#endif
		int line, const char *format, ...) {
	va_list ap;
	va_start(ap, format);

	char message[512];
	strncpy(message, logprefix[level], 9);
	{
		char tmp[256];
		vsnprintf(tmp, 256, format, ap);
		strcat(message, tmp);
	}
	va_end(ap);
	appendRTimeInfo(message, file,
#ifndef NO_FUNC
			func,
#endif
			line);

	printf("%s\n", message);
	logtofile(message);
}

void __log(enum LogLevel level, const char *file,
#ifndef NO_FUNC
		const char *func,
#endif
		int line, const char *msg) {
	char message[512];
	strncpy(message, logprefix[level], 9);
	strncat(message, msg, 256);
	printf("%s\n", message);

	appendRTimeInfo(message, file,
#ifndef NO_FUNC
			func,
#endif
			line);

	// outputing
	printf("%s\n", message);
	logtofile(message);

}

void appendRTimeInfo(char *dest, const char *file,
#ifndef NO_FUNC
		const char *func,
#endif
		int line) {
	// not ASSERT because it would call log
	__assert_m(dest != NULL, "Dest was NULL, while appending runtime info", __FILE__, __func__, __LINE__, false);

	// ignore warning because, it thinks that message can be 512 long, but it at most can be 265
	snprintf(dest+strnlen(dest, 265), 512, "\n\t in file: '%s', "
#ifndef NO_FUNC
			"in function: %s, "
#endif

			"at line: %d, errno: %s",
			file,
#ifndef NO_FUNC
			func,
#endif
			line, strerror(errno));
}

void logtofile(const char *msg) {
	FILE *lfile = fopen(LOGFILE_NAME, "a");
	if(!lfile) {
		LOG(L_ERR, "Could not open logfile");
		return;
	}
	fprintf(lfile, "%s\n", msg);
}

