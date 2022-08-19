#ifndef _NTR_S_MSG_H_
#define _NTR_S_MSG_H_ 1
#include <fcntl.h>

enum MsgType {
	M_INFO = 0,
	M_WARN,
	M_ERR,
	M_NONE
};

struct ClientField;
typedef union {
	struct ClientField {
		// Verion
		unsigned char vmaj	:4;
		unsigned char vmin	:4;
		// Filename 0-255
		unsigned char fname	:8;
		// Message type
		enum MsgType type	:2;
		// unused padding 2
		unsigned short padd2	:10;
	} fields;
	unsigned int bits;
} ClientMsg;

/*
 * Instructions that the server can send a process
 * they all have an nibble for more information storage
 * WARNING: there can at most be 8 of these(3 bits)
*/
enum ServerInstruction {
#define SELFD_IMID	1
#define SELFD_DUMP	2
#define SELFD_LOGF	4
#define SELFD_LOGS	8
	/* order a self destruct
	 * additional arguments:
	 * 	1	: immediately (next options are ignored)(0 - false: clean up, 1 - true)
	 * 	10	: dump logs (0 - next options are ignored)
	 * 	100	: log to file
	 * 	1000	: send log to server
	*/
	SI_SELFD = 0,

	/* server will kill the process
	 * no additional arguments
	*/
	SI_KILL = 1,

#define DEBUG_STDOUT	1
#define DEBUG_LOGF	2
#define DEBUG_LOGS	4
#define DEBUG_UNDEF	8
	/* print debug info
	 * additional arguments:
	 * 	1	: stdout,
	 * 	10	: file,
	 * 	100	: send server
	 * 	1000	: undefined
	 * multiple options can be applied
	*/
	SI_DEBUG = 2,
};



#define STATUS_SUCCESS 1
#define STATUS_FAILURE 0
struct ServerField;
typedef union {
	struct ServerField {
		// Verion
		unsigned char vmaj		:4;
		unsigned char vmin		:4;
		// 1: succes, 0: fail
		unsigned char status		:1;
		// see: enum ServerInstruction
		enum ServerInstruction instruct	:3;
		// see: enum ServerInstruction
		unsigned char instructOpts	:4;
		// file name
		unsigned char fname		:8;
		// unused padding
		unsigned char padd		:8;
	} fields;
	unsigned int bits;
} ServerMsg;

#endif//_NTR_S_MSG_H_

