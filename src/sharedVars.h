#ifndef _NTR_SHARD_VS_H_
#define _NTR_SHARD_VS_H_ 1

// pid_t
#include <sys/types.h>
// at least C99
#if __STDC_VERSION__ >= 199901L
# include <stdbool.h>
#else
# error "stdbool not supported"
#endif

extern pid_t spid;
extern bool running;

#endif /* ifndef _NTR_SHARD_VS_H_ */
