#include "badgeassembler.h"
#include "badgeoutput.h" 

void SimpleExamples()
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


// Examples for  DAC/Comparator board with custom chip for the badge
const Nibble dClock = 1; // clock the 8-bit down counter feeding the DAC
const Nibble dReset = 2; // reset the counter
const Nibble dPulldown = 4; // this turns off an 20mA open drain transistor
							// use this to drive LEDs or discharge caps for measurement
const Nibble dBank = 8; // select which of the 2 banks
						//  of 3 comparators to output to the In port of the badge
Addr countSetBits(Nibble reg)
{
	skip1(notflag(Zed));
	return inc(reg);
}

void CustomDacExamples()
{

zeroPC("Square tone generator");
{			movn(R0, dBank);
	Addr back = xor(ROut, R0);
				jumpto( back);
}
output();
				
zeroPC("Pulse generator");
{
				movn(R0, dBank);
	Addr back = xor(ROut, R0);
				xor(ROut, R0);
				nop();
				nop();
				nop();
				nop();
				nop();
				nop();
				jumpto( back);
}	
output();

zeroPC("DAC Sawtooth");
{
				movn(ROut,dReset ); // reset the Dac Counter
				movn(ROut, 0 );
				movn(R0,dClock );
	Addr back =	xor(ROut, R0);      // clock the Dac Counter
				xor(ROut, R0);
				jumpto(back);
}
output();

zeroPC("Comparators as PWM");
{
				movn(ROut,dReset ); // reset the Dac Counter
				movn(ROut, 0 );
				movn(R1, dClock);
				

Addr toggleclock = movn(ROut,0);
				xor(ROut, R1);
				xor(ROut, R1);
				mov(R2, RIn);
				movn(ROut,dBank);
				mov(R3, RIn);				
				jumpto(toggleclock);
}	
output();
			
zeroPC("Single 4-bit ADC Value");
{
	Addr back= 	movn(ROut,dReset ); // reset the Dac Counter
				movn(ROut, 0 );
				var cmp0 = R2;
				movn(cmp0, 0);
				beginrepeat(R8, 16);	
					movn(R0, dClock);
					movn(ROut,0);

					beginrepeat(R1, 16);
						xor(ROut, R0);
					endrepeat();
					beginrepeat(R1, 16);
						xor(ROut, R0);
					endrepeat();
					bit(3, 0);
					countSetBits(cmp0);		
				endrepeat();
				
				mov(R0, cmp0);
				mov(R3, cmp0);
				movto(0);
				jumpto(back);
}
output();

zeroPC("Single 8-bit ADC Value");
{
	Addr back= 	movn(ROut,dReset ); // reset the Dac Counter
				movn(ROut, 0 );
				var cmp0lsb = R2;
				var cmp0msb = R3;
				movn(cmp0lsb, 0);
				movn(cmp0msb, 0);
				movn(R0, dClock);

				beginrepeat(R8, 16);	
					beginrepeat(R1, 16);
						xor(ROut, R0);
						xor(ROut, R0);
						
						bit(3, 0);
						skip1(notflag(Zed));
							inc(cmp0lsb);
						skip1(notflag(Carry));
							inc(cmp0msb);	
	 				endrepeat();						
				endrepeat();
				
				mov(R0, cmp0lsb);
				mov(R4, cmp0lsb);
				movto(16);

				mov(R0, cmp0msb);
				mov(R5, cmp0msb);
				movto(17);
			jumpto(back);
}
output();

zeroPC("ADC 4-bit values to the store");
{
Addr back= movn(ROut,dReset ); // reset the Dac Counter
				movn(ROut, 0 );
				var cmp0 = R2;
				var cmp1 = R3;
				var cmp2 = R4;
				var cmp3 = R5;
				var cmp4 = R6;
				var cmp5 = R7;
				movn(cmp0, 0);
				movn(cmp1, 0);
				movn(cmp2, 0);
				movn(cmp3, 0);
				movn(cmp4, 0);				
				movn(cmp5, 0);
				beginrepeat(R8, 16);	
					movn(R0, dClock);
					movn(ROut,0);

					beginrepeat(R1, 16);
						xor(ROut, R0);
					endrepeat();
					beginrepeat(R1, 16);
						xor(ROut, R0);
					endrepeat();

					bit(3, 0);
					countSetBits(cmp0);
					bit(3, 1);
					countSetBits(cmp1);
					bit(3, 2);
					countSetBits(cmp2);
									
					movn(ROut,dBank);
					bit(3, 0);
					countSetBits(cmp3);
					bit(3, 1);
					countSetBits(cmp4);
					bit(3, 2);
					countSetBits(cmp5);

				endrepeat();
				
				mov(R0, cmp0);
				movto(0);
				mov(R0, cmp1);
				movto(1);	
				mov(R0, cmp2);
				movto(2);
				mov(R0, cmp3);
				movto(3);
				mov(R0, cmp4);
				movto(4);
				mov(R0, cmp5);
				movto(5);			
				jumpto(back);
}
output();

}


int main()
{
	SimpleExamples();
	CustomDacExamples();
}