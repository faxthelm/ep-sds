#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>

main(void)
/*
**   Listing7.6.c - SLAVE: activated by a UDP datagram to port 3579
*/
{
struct sockaddr_in sock;
int len=sizeof(sock);
struct hostent *hp;
int port;
int fd;
char host[16];
char buf[80];

/*
**   Get host and port from MASTER
*/
read(0, buf, sizeof(buf));
sscanf(buf, "%s %d", host, &port);

/* initialize a UDP socket struct */
fd = socket(AF_INET, SOCK_DGRAM, 0);
memset((char *) &sock, 0, sizeof(sock));
hp = gethostbyname(host);/* MASTER */
memcpy(&sock.sin_addr, hp->h_addr, hp->h_length);
sock.sin_port = htons(port);
sock.sin_family = AF_INET;

/*
**   Register with MASTER
*/
gethostname(buf, sizeof(buf));
sendto(fd, buf, strlen(buf)+1, 0,
(struct sockaddr *) &sock, sizeof(sock));

/* . . . further processing */

close(fd);
return(0);
}

