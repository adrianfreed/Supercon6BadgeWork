#include "badgeassembler.h"
#include "badgeoutput.h" 

int main()
{

	zeroPC("counter to output port");
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
