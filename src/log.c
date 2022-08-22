#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <pthread.h>
#include <fcntl.h>

#include "log.h"
#include "assrt.h"
#include "global.h"
#include "stdext.h"

static const char *logprefix[3] = {
	"INFO: ", "WARINING: ", "ERROR: "
};

pthread_mutex_t logmutex;

static void logtofile(const char *msg);
// appends runtime info to dest
static void appendRTimeInfo(char *dest, const char *file,
#ifndef NO_FUNC
		const char *func,
#endif
		int line);

static enum LogLevel stdoutMask, fileMask;
static int logfile;

void loginit(enum LogLevel _stdoutMask, enum LogLevel _fileMask) {
	stdoutMask = _stdoutMask;
	fileMask = _fileMask;
	logfile = open(LOGFILE_NAME, O_WRONLY|O_APPEND|O_CREAT);
	if(logfile == -1) {
		fprintf(stderr, "ERROR: Couldn't open logfile, %s, %s\n", LOGFILE_NAME, strerror(errno));
	}
}

void logdestroy() {
	if(logfile != -1) {
		pthread_mutex_lock(&logmutex);
		close(logfile);
		pthread_mutex_unlock(&logmutex);
	}
}

void __logf(enum LogLevel level, const char *file,
#ifndef NO_FUNC
		const char *func,
#endif
		int line, const char *format, ...) {
	va_list ap;
	va_start(ap, format);

	char message[512] = "";
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

	printf("level: %d, smask: %d, fmask: %d\n", level, stdoutMask, fileMask);
	if(stdoutMask != L_NONE && level >= stdoutMask)
		printf("%s\n", message);
	if(fileMask != L_NONE && level >= fileMask)
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
	printf("level: %d, smask: %d, fmask: %d\n", level, stdoutMask, fileMask);
	if(stdoutMask != L_NONE && level >= stdoutMask)
		printf("%s\n", message);
	if(fileMask != L_NONE && level >= fileMask)
		logtofile(message);
}

static void appendRTimeInfo(char *dest, const char *file,
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

			"at line: %d, errno: %s\n",
			file,
#ifndef NO_FUNC
			func,
#endif
			line, strerror(errno));
}

static void logtofile(const char *msg) {
	if(logfile == -1)
		return;
	pthread_mutex_lock(&logmutex);
	// Error already printed in loginit
	async_write_str(logfile, (char *)msg, 512);
	pthread_mutex_unlock(&logmutex);
}

