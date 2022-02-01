#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <time.h>
#include <sys/socket.h>

// http://localhost:8000

#define SERVER_PORT    "8000"
#define MAX_CONNECTION 1000

/*
Client
    socket()
  ? bind()    ?
    connect()
    ----------------
    write()   | read()
    send()    | recv()
    sendto()  | recvfrom()
    writev()  | readv()
    sendmsg() | recvmsg()
    ----------------
    close()
Server
    socket()
    bind()
    listen()
    accept()
    ----------------
    write()   | read()
    send()    | recv()
    sendto()  | recvfrom()
    writev()  | readv()
    sendmsg() | recvmsg()
    ----------------
    close()
*/

typedef enum
{
    eHTTP_UNKNOWN = 0
    ,eHTTP_CONNECT
    ,eHTTP_DELETE
    ,eHTTP_GET
    ,eHTTP_HEAD
    ,eHTTP_OPTIONS
    ,eHTTP_PATCH
    ,eHTTP_POST
    ,eHTTP_PUT
    ,eHTTP_TRACE
}eHTTPMethod;

typedef struct
{
    eHTTPMethod type;
    char        path[255];
}sHTTPHeader;

void *get_client_addr(struct sockaddr *);
int create_socket(const char *);

void http_request(int);
void parse_http_request(const char*, sHTTPHeader *);
void send_message(int, const char*);
void send_404(int);

int main()
{
    int sock;

    sock = create_socket(SERVER_PORT);
    if(sock < 0)
    {
        fprintf(stderr, "error create socket\n");
        return -1;
    }

    printf("server created!\n");

    struct sockaddr_storage client_addr;
    int client_d;
    //char client_ip
    while(1)
    {
        socklen_t s_size = sizeof(client_addr);
        client_d = accept(sock, (struct sockaddr*)&client_addr, &s_size);

        if(client_d == -1)
        {
            fprintf(stderr, "error accept\n");
            return -1;
        }

        char ip[INET6_ADDRSTRLEN];
        inet_ntop(client_addr.ss_family, get_client_addr((struct sockaddr *)&client_addr), ip, sizeof ip);
        printf("server: got connection from %s\n", ip);

        // read
        http_request(client_d);

        close(client_d);
    }

    return 0;
}