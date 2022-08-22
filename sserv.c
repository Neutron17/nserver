#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>

#define S_PATH "server.socket"

int main(int argc, char *argv[]) {
	struct sockaddr_un addr;
	int sock = socket(AF_UNIX, SOCK_STREAM, 0);
	printf("socket: %d\n", sock);
	if(sock == -1) {
		perror("Couldn't open socket");
		return -1;
	}
	if(remove(S_PATH) == -1 && errno != ENOENT) {
		perror("remove");
		return -2;
	}
	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, S_PATH);
	if(bind(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1) {
		perror("bind");
		return -3;
	}
	if(listen(sock, 5) == -1) {
		perror("listen");
		return -4;
	}
	puts("Waiting");
	int client = accept(sock, NULL, NULL);
	struct sockaddr_un client_addr;
	printf("Accepted %d\n", client);
	socklen_t len;
	if(getpeername(client, (struct sockaddr *) &client_addr, &len) == -1) {
		perror("getpeername");
	} else {
		printf("Client socket filepath: %s\n", client_addr.sun_path);
	}

	char buf[16];
	printf("waiting to read...\n");
	int bytes_rec = recv(client, buf, sizeof(buf), 0);
	if (bytes_rec == -1){
	    perror("recv");
	    close(sock);
	    close(client);
	    exit(1);
	} else {
		printf("DATA RECEIVED = %s\n", buf);
	}

	if(close(sock) == -1)
		perror("close");
	return 0;
}

