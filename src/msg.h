#ifndef _NTR_S_MSG_H_
#define _NTR_S_MSG_H_ 1
#include <fcntl.h>

enum MsgType {
	M_INFO = 0,
	M_WARN,
	M_ERR,
	M_NONE
};

/*
 * Instructions that the server can send a process
 * they all have an nibble for more information storage
 * WARNING: there can at most be 8 of these(3 bits)
*/
enum ServerInstruction {
	/* order a self destruct
	 * additional arguments:
	 * 	1	: immediately (next options are ignored)(0 - false: clean up, 1 - true)
	 * 	10	: dump logs (0 - next options are ignored)
	 * 	100	: log to file
	 * 	1000	: send log to server
	*/
	S_SELFD = 0,
	/* server will kill the process
	 * no additional arguments
	*/
	S_KILL = 1,
	/* print debug info
	 * additional arguments:
	 * 	1	: stdout,
	 * 	10	: file,
	 * 	100	: send server
	 * 	1000	: undefined
	 * multiple options can be applied
	*/
	S_DEBUG = 2,

};

typedef union {
	struct {
		// Verion
		unsigned char vmaj	:4;
		unsigned char vmin	:4;
		// Message type
		enum MsgType type	:2;
		// unused padding 1
		unsigned char padd1	:6;
		// Filename 0-255
		unsigned char fname	:8;
		// unused padding 2
		unsigned char padd2	:8;
	} _fields;
	unsigned int bits;
} ClientMsg;

typedef union {
	struct {
		// Verion
		unsigned char vmaj		:4 ;
		unsigned char vmin		:4 ;
		// 1: succes, 0: fail
		unsigned char status		:1 ;
		// see: enum ServerInstruction
		enum ServerInstruction instruct	:3 ;
		// see: enum ServerInstruction
		unsigned char instructOpts	:4 ;
		// unused padding
		short padd			:16;
	} _fields;
	unsigned int bits;
} ServerMsg;

#endif//_NTR_S_MSG_H_

