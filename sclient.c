#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <signal.h>

#include "msg.h"

#define C_PATH "client.socket"
//#define S_PATH "server.socket"

void handler(int sig, siginfo_t *info, void *c);

int main(int argc, char *argv[]) {
	if(argc < 2) {
		fprintf(stderr, "ERROR: Not enough arguments");
		return -1;
	}
	struct sigaction sa = {0};
	sa.sa_sigaction = handler;
	sa.sa_flags =
#ifdef SA_RESTART
		SA_RESTART |
#endif
		SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	if(sigaction(SIGINT, &sa, NULL) == -1) {
		perror("sigaction");
		return -1;
	}
	if(sigaction(SIGUSR1, &sa, NULL) == -1) {
		perror("sigaction");
		return -1;
	}
	sigqueue(atoi(argv[1]), SIGUSR1, (union sigval) {
		.sival_int =
			((ClientMsg) {
			 .fields = {
			 	.aId=0,
			 	1,0, M_OPEN, 0
			}}).bits
	});
	while(1) {}
	return 0;
}

void handler(int sig, siginfo_t *info, void *c) {
	if(sig == SIGINT)
		exit(0);
	printf("signald, %d %d\n", info->si_signo, info->si_value.sival_int);

	char S_PATH[11];
	ServerMsg srv;
	srv.bits = info->si_value.sival_int;
	sprintf(S_PATH, "files/%d", srv.fields.fname);
	int len;
	struct sockaddr_un client_addr;
	struct sockaddr_un server_addr;
	int sock = socket(AF_UNIX, SOCK_STREAM, 0);
	printf("socket: %d\n", sock);
	if(sock == -1) {
		perror("Couldn't open socket");
//		return -1;
	}
	/*if(remove(S_PATH) == -1 && errno != ENOENT) {
		perror("remove");
		return -2;
	}*/
	memset(&client_addr, 0, sizeof(struct sockaddr_un));
	client_addr.sun_family = AF_UNIX;
	strcpy(client_addr.sun_path, C_PATH);
	len = sizeof(client_addr);

	if(bind(sock, (struct sockaddr *)&client_addr, len) == -1) {
		perror("bind");
		goto cl;
	}

	server_addr.sun_family = AF_UNIX;
	strcpy(server_addr.sun_path, S_PATH);

	if(connect(sock, (struct sockaddr *)&server_addr, len) == -1) {
		perror("connect");
		goto cl;
	}
	char buff[16] = "hi";
	int numRead;
	//while((numRead = read(STDIN_FILENO, buff, 16)) > 0) {
		if(write(sock, buff, 16) != 16) {
			perror("write");
			goto cl;
		}

	//}
	if(numRead == -1)
		perror("numRead");
cl:
	close(sock);
	exit(0);
}

