#include <stdio.h>
#include "badgeAssemblerOutput.h"
int pc = 0; // program counter - to track code location for relative move calculations
void zeroPC(char *s)
{	
	pc = 0;
	printf("%s\n", s);
}
// output the binary instructions in a way that is helpful to toggle in
unsigned lastinstruction = 0;
int pb(unsigned n)
{
	if(pc >= (1<<12) )
	{
		printf("warning program exceeds 4096 instructions\n");		
	}
	lastinstruction = n;
	
	for(int i=0;i<8;++i)
	{
		printf("%s", (pc & (1<< (8-1-i)))?"1":"0");
		if(i==3 || i==7)
			printf(" ");
	}
	printf(":    ");

	for(int i=0;i<12;++i)
	{
		printf("%s", (n & (1<< (12-1-i)))?"1":"0");
		if(i==3 || i==7)
			printf(" ");
	}
	printf("\n");
	return pc++;
}
void comment(char *s)
{
	printf("%s\n", s);
}
void output()
{
	printf("Total machine code %d instructions\n", pc);
}