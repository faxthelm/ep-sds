#include <pthread.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/signal.h>
#include <stdlib.h>
#include <string.h>


#define PORT 9876
#define MAX 50

typedef struct {
pthread_t tid;
int emUso;
FILE *fp;
int line;
char file[32];
char buf[80];
int cli;
int id;
} thread;

void threadResponse(thread *th);

int main() {

thread listaThreads[MAX];

for(int i=0; i<MAX; i++){
listaThreads[i].emUso = 0;
listaThreads[i].id = i;
}

struct sockaddr_in sock;
int socklen=sizeof(sock);
fd_set fds;
int acc;
int opt=1;
int cliServer;
char bufServer[80];

acc = socket(AF_INET, SOCK_STREAM, 0);

sock.sin_family = AF_INET;
sock.sin_port = htons(PORT);
sock.sin_addr.s_addr = htonl(INADDR_ANY);
bind(acc, (struct sockaddr *) &sock, socklen);
setsockopt(acc, SOL_SOCKET, SO_REUSEADDR,
(char *) &opt, sizeof(opt));

while(1){
listen(acc,5);
cliServer = accept(acc, (struct sockaddr *) &sock, &socklen);
recv(cliServer, bufServer, sizeof(bufServer), 0);
for(int i=0; i<MAX; i++){
if(listaThreads[i].emUso == 0){
listaThreads[i].emUso = 1;
listaThreads[i].cli = cliServer;
strcpy(listaThreads[i].buf, bufServer);
pthread_create(&listaThreads[i].tid, NULL, threadResponse, &listaThreads[i]);
break;
}
}
}
return 0;
}


void threadResponse(thread *th){
sscanf(th->buf, "%s %d", &(th->file), &(th->line));
th->fp = fopen(th->file, "r");
char* resp = malloc(strlen(th->buf) + 10);
while(th->line--){
fgets(th->buf, sizeof(th->buf), th->fp);
}
sprintf(resp, " %s %d ", th->buf, th->id);
fclose(th->fp);
th->buf[strlen(th->buf)-1] = '\0';
send(th->cli, resp , strlen(resp)+1, 0);
close(th->cli);
sleep(1);
th->emUso = 0;
}

