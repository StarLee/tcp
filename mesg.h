#ifndef _MES_H_
#define _MES_H_
struct mesg
{
	int len;
	unsigned char type;
	unsigned char data[1024];
};
#endif
