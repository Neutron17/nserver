#ifndef _NTR_ARGS_H_
#define _NTR_ARGS_H_ 1

#if __STDC_VERSION__ >= 199901L
# include <stdbool.h>
#else
# error "stdbool is not supported"
#endif

struct ArgFields;

enum ArgAction {
	AA_LIST,
	AA_SEND
};

typedef union Args {
	struct ArgFields {
		bool isDebug		:1;
		bool startDeamon	:1;
		enum ArgAction act	:2;
		unsigned char actarg	:8;
	} fields;
	unsigned short bits;
} Args;

union Args parseArgs(int argc, char *argv[]);
void handleArgs(union Args args);

#endif /* ifndef _NTR_ARGS_H_ */
