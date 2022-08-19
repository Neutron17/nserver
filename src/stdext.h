#ifndef _NTR_STDEXT_H_
#define _NTR_STDEXT_H_ 1

/* Standard extension
 * 	- strnlen:	safe strlen
 * 	- strrev:	reverse string
 * 	- itoa:		int to string
*/

#ifndef HAVE_STRNLEN
#include <stddef.h>
size_t strnlen(const char *str, size_t n);
#endif

void strrev(char *arr, int start, int end);
char *itoa(int number, char *arr, int base);
const char *pidname(int pid);

#endif /* ifndef _NTR_STDEXT_H_ */
