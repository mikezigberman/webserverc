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