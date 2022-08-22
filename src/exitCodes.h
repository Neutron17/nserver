#ifndef _NTR_EXCODE_H_
#define _NTR_EXCODE_H_ 1

enum ExitCode {
	E_SUCCESS = 0,
	E_ALLOC,
	E_IO,
	E_ARG,
	E_UNKNOWN,

	E_INIT,
	E_SOCK_BIND,
	E_SOCK_CONN,
	E_SOCK_ACPT,
	E_SOCK_CLOSE,
};

#endif /* ifndef _NTR_EXCODE_H_ */

