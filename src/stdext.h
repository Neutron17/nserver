#ifndef _NTR_STDEXT_H_
#define _NTR_STDEXT_H_ 1

/* Standard extension
 * 	- strnlen
*/

#ifndef HAVE_STRNLEN
#include <stddef.h>
#include <string.h>
inline size_t strnlen(const char *str, size_t n) {
    const char * stop = (char *)memchr(str, '\0', n);
    return stop ? stop - str : n;
}
#endif

#endif /* ifndef _NTR_STDEXT_H_ */
