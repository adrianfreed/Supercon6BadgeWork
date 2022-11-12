#include "badgeAssembler.h"
#include "badgeAssemblerOutput.h" 

int main()
{

	zeroPC("Counter to output port:");
	{			
		Addr back = inc(R0);
					mov(ROut, R0);
					beginrepeat(R8, 16);	
						beginrepeat(R1, 16);
							nop();
						endrepeat();
					endrepeat();
					jumpto( back);
	}		
	output();
		
}
