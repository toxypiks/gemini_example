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

int main(int argc, char** argv)
{
    char* param = "";
    char* request_end = "\r\n";

    if(argc > 1) {
        param = argv[1];
        request_end = "/\r\n";
    }

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

    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    SSL_CTX *ctx = SSL_CTX_new(TLS_client_method());

    if (ctx == NULL) {
        fprintf(stderr, "ERROR: could not initialize the SSL context: %s\n",
                strerror(errno));
        exit(1);
    }

    SSL *ssl = SSL_new(ctx);
    SSL_set_fd(ssl, fd);

    if (SSL_connect(ssl) < 0) {
        fprintf(stderr, "ERROR: could not connect via SSL: %s\n",
                strerror(errno));
        exit(1);
    }

    const char* request = "gemini://geminiprotocol.net/";

    char request_buf[1024] = {0};
    sprintf(request_buf,"%s%s%s", request, param, request_end);
    printf("%s\n", request_buf);

    SSL_write(ssl, request_buf, strlen(request));

    char buffer[1024];
    ssize_t n = SSL_read(ssl, buffer, sizeof(buffer));
    while (n > 0) {
        fwrite(buffer, 1, n, stdout);
        n = SSL_read(ssl, buffer, sizeof(buffer));
    }

    SSL_set_shutdown(ssl, SSL_RECEIVED_SHUTDOWN | SSL_SENT_SHUTDOWN);
    SSL_shutdown(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ctx);

    close(fd);

    return 0;
}
