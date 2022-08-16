#ifndef _NTR_ASSERT_H_
#define _NTR_ASSERT_H_ 1
#include <stdbool.h>

void __assert_m(bool expr, const char *restrict msg, const char *restrict file,
#ifndef NO_FUNC
		const char *restrict func,
#endif
		int line, bool log);

#ifndef NO_FUNC
	#define ASSERT_M(EXPR, MSG) __assert_m(EXPR, MSG, __FILE__, __func__, __LINE__, true)
#else
	#define ASSERT_M(EXPR, MSG) __assert_m(EXPR, MSG, __FILE__, __LINE__, true)
#endif

#define ASSERT(EXPR) ASSERT_M(EXPR, NULL)

#endif /* ifndef _NTR_ASSERT_H_ */

