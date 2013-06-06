#include "socket_wrapper.h"
void read_enough(int socket,char *dest,int len)
{
	int size=0;
	int offset=0;
	do
	{
		char buffer[len-offset];
		memset(buffer,'\0',len-offset);
		size=recv(socket,buffer,len-offset,0);//read the data
		memcpy(dest+offset,buffer,size);
		if(size==-1)
		{
			perror("socket error");
			break;
		}
		offset+=size;
	}while(offset<len);//guaranty the len
}
int session(int client)
{
	int length=0;
	int flag=client;//-1 the socket has closed;
	FILE *fp=NULL;
	while(1)
	{
		
		struct mesg message;
		memset(&message,'\0',sizeof(message));
		length=recv(client,&(message.len),sizeof(message.len),0);//read the length,one recv maybe enough,because it's just 4 bytes,otherwise you must read sevral times
		if(length<0)
		{
			perror("recv over,socket has closed");
			break;
		}
		if(length==0)
		{
			printf("no data read");
			continue;
		}	
		recv(client,&(message.type),sizeof(message.type),0);//read the type
		if(message.type==0x00)//this stand for start,no data
		{
			printf("somebody call me,i have to recv its datas\n");
			continue;
		}
		if(message.type==0x01)
		{
			printf("waiting next communication\n");
			break;//current session is over,don't close the socket
		}
		if(message.type==0xff)//if the type flag is 0xff ,then we know it is a file
		{
			read_enough(client,message.data,message.len);
			fp=fopen(message.data,"w");
		}
		if(message.type==0xfe)//if the type flag is 0xfe,it is for file content
		{

			read_enough(client,message.data,message.len);
			fwrite(message.data,1,message.len,fp);
		}
		if(message.type==0xfd)//current file is over
		{
			fclose(fp);
		}
		if(message.type==0xfc)
		{
			read_enough(client,message.data,message.len);
			printf("%s\n",message.data);
		}
		if(message.type==0xee)
		{
			flag=-1;
			printf("recieve over,closing the client socket\n");
			close(client);
		}
	}
	return flag;
}
void begin_session(int socket)
{
	struct mesg head;
	head.len=1;	
	head.type=0x00;
	send(socket,&head.len,4,0);
	send(socket,&head.type,1,0);
}
void close_session(int socket)
{
	struct mesg flag;
	flag.len=1;
	flag.type=0x01;
	send(socket,&flag.len,4,0);
	send(socket,&flag.type,1,0);
}
void close_conn(int socket)
{
	struct mesg over;
	over.len=1;	
	over.type=0xee;
	send(socket,&over.len,4,0);
	send(socket,&over.type,1,0);
	close(socket);

}
void send_msg(int socket,char * msg)
{
	struct mesg mes;
	mes.type=0xfc;
	strcpy(mes.data,msg);
	mes.len=strlen(msg);
	send(socket,&mes.len,4,0);
	send(socket,&mes.type,1,0);
	send(socket,mes.data,mes.len,0);
}
void send_file(int socket,char *filepath,char *filename)
{
	struct mesg header;
	header.type=0xff;
	strcpy(header.data,filename);
	int filenamelength=strlen(filename);
	header.len=filenamelength;
	send(socket,&header.len,4,0);
	send(socket,&header.type,1,0);
	send(socket,header.data,header.len,0);
	int length=0;
	FILE *fp=NULL;
	char *fileName=filepath;
	fp=fopen(fileName,"r");
	do
	{
		char buffer[1024];
		memset(buffer,'\0',1024);
		struct mesg body;
		body.type=0xfe;
		length=fread(buffer,1,1024,fp);
		body.len=length;
		memcpy(body.data,buffer,length);
		send(socket,&body.len,4,0);
		send(socket,&body.type,1,0);
		send(socket,body.data,body.len,0);
	}while(!feof(fp));
	fclose(fp);
	struct mesg tail;
	tail.type=0xfd;
	tail.len=1;
	send(socket,&tail.len,4,0);
	send(socket,&tail.type,1,0);
}
