#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "mesg.h"
#include <string.h>
#include "socket_wrapper.h"
int make_socket()
{
	int sock;
	sock=socket(PF_INET,SOCK_STREAM,0);
	if(sock<0)
	{
		perror("socket error");
		exit(EXIT_FAILURE);
	}
	return sock;	
}
int fsend(const char **flist,int amount)
{
	int socket=make_socket();
	struct sockaddr_in server;
	server.sin_family=AF_INET;
	server.sin_port=htons(6000);
	server.sin_addr.s_addr=inet_addr("127.0.0.1");
	int flag=connect(socket,(struct sockaddr *)&server,sizeof(server));
	if(flag==0)
	{
		/*int i=0;
		for(;i<amount;i++)
		{
		}*/
		begin_session(socket);
		send_msg(socket,"hello");
		send_msg(socket,"world");
		send_msg(socket,"my name is lixing");
		send_file(socket,"/home/starlee/test.data","/home/starlee/test.data2");
		send_msg(socket,"1");
		send_file(socket,"/home/starlee/test.data","/home/starlee/test.data3");
		send_msg(socket,"2");
		send_file(socket,"/home/starlee/test.data","/home/starlee/test.data4");
		send_msg(socket,"3");
		close_session(socket);
		printf("***waiting server return\n");
		session(socket);
		//close_conn(socket);
	}
	else
	{
		perror("connect error");
	}
}
int main(int argc,char **argv)
{
	fsend(NULL,0);
}
