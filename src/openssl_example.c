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

#define HOST "toxypiks.github.io"
#define PORT "443"

int main(void)
{
    int fd = 0;
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0) {
        fprintf(stderr, "Could not connect to socket %s\n", strerror(errno));
        exit(1);
    }
    printf("Creat socket %d\n", fd);
    return 0;
}
