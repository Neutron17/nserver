#ifndef _NTR_S_LOG
#define _NTR_S_LOG 1
#include <stdarg.h>

enum LogLevel {
	L_INFO = 0,
	L_WARN,
	L_ERR
};

#define LOGFILE_NAME "logs.txt"

#ifndef NO_FUNC
	#define LOG(LVL, MSG) __log(LVL, __FILE__, __func__, __LINE__, MSG)
	#define LOGF(LVL, ...) __logf(LVL, __FILE__, __func__, __LINE__, __VA_ARGS__)
#else
	#define LOG(LVL, MSG) __log(LVL, __FILE__, __LINE__, MSG)
	#define LOGF(LVL, ...) __logf(LVL, __FILE__, __LINE__, __VA_ARGS__)
#endif

void __logf(enum LogLevel level, const char *file,
#ifndef NO_FUNC
		const char *func,
#endif
		int line, const char *format, ...);
void __log(enum LogLevel level, const char *file,
#ifndef NO_FUNC
		const char *func,
#endif
		int line, const char *msg);

#endif //_NTR_S_LOG
