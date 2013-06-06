#include <stdio.h>
#include "mesg.h"
#include <string.h>
int main(int argc,char **args)
{
	struct mesg mes;
	mes.len=1;
	mes.type=0xff;
	char * data="hello";
	strcpy(mes.data,data);
	printf("%d\n",sizeof(mes));
	printf("len:%d\n",mes.len);
	printf("%x\n",mes.type);
	printf("%s\n",mes.data);
	return 0;
}
