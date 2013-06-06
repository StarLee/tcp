#ifndef _SOCKET_WRAPPER_H_ 
#define _SOCKET_WRAPPER_H_
#include "mesg.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
int session(int client);//socket communication for the recv endpoint,for get data
void begin_session(int socket);//for send endpoint
void close_conn(int socket);//for send endpoint
void send_msg(int socket,char *msg);//send a message,less than 1024
void send_file(int socket,char *filepath,char *filename);//send a file
void close_session(int client);//
#endif
