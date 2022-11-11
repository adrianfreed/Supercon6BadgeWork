#include <stdio.h>
#include "badgeoutput.h"

unsigned code[4096];
int pc  = 0;
void zeroPC(char *s)
{	
	pc = 0;
}
// store the output so we can wrap it in hexcode header to send to board
unsigned lastinstruction = 0;
int pb(unsigned n)
{
	if(pc >= (1<<12) )
	{
		fprintf(stderr, "warning program exceeds 4096 instructions\n");		
	}
	lastinstruction = n;
	code[pc] = n;
	
	return pc++;
}

void comment(char *s)
{
}
void output()
{
	// header
	putchar(0x00);
	putchar(0xFF);
	putchar(0x00);
	putchar(0xFF);
	putchar(0xA5);
	putchar(0xC3);
	//length
	putchar(pc&0xff);
	putchar(pc>>8);
	for(int i = 0; i<pc; ++i)
	{
		putchar(code[i]&0xff);
		putchar(code[i]>>8);
	}
	// checksum
	unsigned checksum = pc;
	for(int i = 0; i<pc; ++i)
	{
		checksum += code[i];
	}
	putchar(checksum&0xff);
	putchar(checksum>>8);
}