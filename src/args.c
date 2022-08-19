#include "args.h"
#include <asm-generic/errno-base.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "assrt.h"
#include "exitCodes.h"
#include "log.h"
#include "common.h"

union Args parseArgs(int argc, char *argv[]) {
	Args ret = {
		.fields = {
			false, true, AA_LIST, 0
		}
	};

	int choice;
	while (1) {
		static struct option long_options[] = {
			/* Use flags like so:
			{"verbose",	no_argument,	&verbose_flag, 'V'}*/
			/* Argument styles: no_argument, required_argument, optional_argument */
			{"version",	no_argument,		0,	'v'},
			{"help",	no_argument,		0,	'h'},
			{"debug",	no_argument,		0,	'd'},
			{"verbose",	no_argument,		0,	'V'},
			{"deamon",	no_argument,		0,	'b'},
			{"list",	no_argument,		0,	'l'},
			{"send",	required_argument,	0,	's'},

			{0,0,0,0}
		};

		int option_index = 0;

		/* Argument parameters:
			no_argument: " "
			required_argument: ":"
			optional_argument: "::" */

		choice = getopt_long(argc, argv,
					"vhdVbls:",
					long_options, &option_index);

		if (choice == -1)
			break;

		switch(choice) {
			case 'v':
				printf("Verion: %d.%d\n", VERSION_MAJOR, VERSION_MINOR);
				break;
			case 'h':
				printf("Neutron Server\n"
					"\t-v --version:\tversion\n"
					"\t-h --help:\tprint this message\n"
					"\t-b --deamon:\tstart deamon\n"
					"\t-d -V --debug --verbose\n"
					"\t\n"
				);
				break;
			case 'V':
			case 'd':
				ret.fields.isDebug = true;
				break;
			case 'b':
				ret.fields.startDeamon = true;
				break;
			case 'l':
				ASSERT_M(optarg != NULL, "Argument is required");
				ret.fields.act = AA_LIST;
				break;
			case 's':
				ASSERT_M(optarg != NULL, "Argument is required");
				ret.fields.act = AA_SEND;
				ret.fields.actarg = strtoul(optarg, NULL, 10);
				/* EINVAL is not in C99
				 * and is impossible because its base 10 */
				if(errno == ERANGE) {
					LOGF(L_ERR, "Argument too big/small");
					exit(E_ARG);
				}
				break;
			case '?':
				LOGF(L_ERR, "Unknown argument '%c'", optopt);
				break;
			default:
				/* Not sure how to get here... */
				exit(E_ARG);
		}
	}

	/* Deal with non-option arguments here */
	if (optind < argc) {
		while (optind < argc){
		}
	}
	return ret;
}

void handleArgs(union Args args) {
}

