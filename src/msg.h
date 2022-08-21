/*************************
* Creator: Neutron17	 *
* Project: NServer	 *
* File:    msg.h    	 *
* License: GPL-3.0	 *
* Description: 		 *
*	definitions for  *
*	signal messaging *
*************************/
#ifndef _NTR_S_MSG_H_
#define _NTR_S_MSG_H_ 1

// at most 15 types (4 bits - M_MAX)
enum MsgType {
	/* sent when client application
	 * is opened/run
	 * no additional arguments:
	 * */
	M_OPEN = 0,
#define CLOSE_PEACEF 0
#define CLOSE_FORCEF 1
	/* sent when client application
	 * is closed/finished
	 * additional arguments:
	 *	1	: type(0-peaceful, 1-forceful)
	 *	10 	:
	 *	100 	:
	 *	1000	:
	 */
	M_CLOSE = 1,

#define LOG_INFO 0
#define LOG_WARN 1
#define LOG_ERR  2
#define LOG_NONE 3
	/* sent when the client
	 * requests to log to the server
	 * additional arguments:
	 *	1	: 0-info,1-warn
	 *	10 	: 0-err,1-none
	 *	100 	:
	 *	1000	:
	 */
	M_LOG = 2,

	// do NOT put anything after M_MAX
	M_MAX
};

#define BYTE(NAME) unsigned char NAME:8
#define BIT(NAME)  unsigned char NAME:1
// half byte (4 bits)
#define NIBBLE(NAME) unsigned char NAME:4

struct ClientField;
/* send with sigqueue,
 * pass the bits field as the sigval_int field
 * of union sigval
*/
typedef union {
	struct ClientField {
		// application id (0-255)
		BYTE(aId);
		// Verion
		NIBBLE(vmaj);
		NIBBLE(vmin);
		// Message type
		enum MsgType type	:4;
		NIBBLE(msgOpts);
		// unused padding
		unsigned short padd	:8;
	} fields;
	unsigned int bits;
} ClientMsg;

/*
 * Instructions that the server can send a process
 * they all have an nibble for more information storage
 * WARNING: there can at most be 8(with SI_MAX) of these(3 bits)
*/
enum ServerInstruction {
#define SELFD_IMID	1
#define SELFD_NOW 	1
#define SELFD_DUMP	2
#define SELFD_LOGF	4
#define SELFD_LOGS	8
	/* order a self destruct
	 * additional arguments:
	 * 	1	: immediately (1 - next options are ignored)(0 - false: clean up, 1 - true)
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

	SI_MAX
};

struct ServerField;
typedef union {
	struct ServerField {
		// Verion
		NIBBLE(vmaj);
		NIBBLE(vmin);

#define STATUS_SUCCESS 1
#define STATUS_FAILURE 0
		// 1: succes, 0: fail
		BIT(status);
		// see: enum ServerInstruction
		enum ServerInstruction instruct	:3;
		// see: enum ServerInstruction
		NIBBLE(instructOpts);
		// file name 0-255
		BYTE(fname);
		// unused padding
		BYTE(padd);
	} fields;
	unsigned int bits;
} ServerMsg;

#endif//_NTR_S_MSG_H_

