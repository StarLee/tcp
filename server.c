#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "mesg.h"
#include "socket_wrapper.h"
int make_socket(uint16_t port)
{
	int sock;
	struct sockaddr_in name;
	sock=socket(PF_INET,SOCK_STREAM,0);
	if(sock<0)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}
	name.sin_family=AF_INET;
	name.sin_port=htons(port);
	name.sin_addr.s_addr=htonl(INADDR_ANY);	
	if(bind(sock,(struct sockaddr *)&name,sizeof(name))<0)
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}
	return sock;	
}
int fsave(const char *fileName)
{
	int socket=make_socket(6000);
	if(listen(socket,5)<-1)//the quene length
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	int client=accept(socket,NULL,NULL);
	if(client<0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}
	
	int flag=session(client);
	printf("***send msg to client\n");
	begin_session(client);
	if(flag!=-1)
	{
		send_msg(client,"hello client");
		send_file(client,"/home/starlee/uwsgi.ini","/home/starlee/uwsgin.ini_bak");
	}		
	close_session(client);
	printf("---------------------------------------------\n");
	printf("%d\n",flag);
	printf("---------------------------------------------\n");
	return 0;
}
int main(int argc,char **argv)
{
	fsave("/home/starlee/out.data");
}
