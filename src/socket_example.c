#include <stdio.h>
#include <stdlib.h>

#include <openssl/bio.h> /* BasicInput/Output streams */
#include <openssl/err.h> /* errors */
#include <openssl/ssl.h> /* core library */

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define HOST "geminiprotocol.net"
#define PORT "1965"

int main(void)
{
    struct addrinfo hints = {0};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    struct addrinfo *addrs;
    if (getaddrinfo(HOST, PORT, &hints, &addrs) < 0) {
        fprintf(stderr, "Could not get address of `"HOST"`: %s\n",
                strerror(errno));
        exit(1);
    }

    int fd = 0;

    for (struct addrinfo *addr = addrs; addr != NULL; addr = addr->ai_next) {
        fd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
        if (fd == -1) {
            fprintf(stderr, "Could not connect to socket %s\n", strerror(errno));
            exit(1);
        }
        if (connect(fd, addr->ai_addr, addr->ai_addrlen) == 0)
        {
            printf("Create socket %d\n", fd);
            printf("address: %s\n", inet_ntoa(((struct sockaddr_in *)addr->ai_addr)->sin_addr));
            break;
        }
        close(fd);
        fd = -1;
    }
    freeaddrinfo(addrs);
    if (fd == -1) {
        fprintf(stderr, "Could not connect to "HOST":"PORT": %s\n",
        strerror(errno));
        exit(1);
    }
    return 0;
}
