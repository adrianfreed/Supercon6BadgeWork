#include <stdio.h>

// Pr

typedef unsigned char Nibble;
typedef unsigned char Byte;
typedef int Addr;

typedef Nibble var;


int pc = 0; // program counter - to track code location for relative move calculations

void zeroPC(char *s)
{	
	pc = 0;
	printf("%s\n", s);
}
int pb(unsigned n)
{
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
// Names
const Nibble Carry = 0;
const Nibble Zed = 2;
#define notflag(f) (f+1)
const Nibble R0 = 0;
const Nibble R1 = 1;
const Nibble R2 = 2;
const Nibble R3 = 3;
const Nibble R4 = 4;
const Nibble R5 = 5;
const Nibble R6 = 6;
const Nibble R7 = 7;
const Nibble R8 = 8;
const Nibble R9 = 9;
const Nibble ROut = 10;
const Nibble RIn = 11;
const Nibble RJsr = 12;
const Nibble RPcl = 13;
const Nibble RPcm = 14;
const Nibble Rpch = 15;

// each functions emits the correct opcode 
// and returns the position of the PC
Addr inc(Nibble reg)
{
	return pb(0x20 | (reg % 16));
}
Addr skip1(Nibble flag) {
	return pb(0x0f0 | (flag<<2) |1);
}
Addr skip2(Nibble flag) {
	return 	pb(0x0f0 | (flag<<2) |2);
}
Addr skip3(Nibble flag) {
	return 	pb(0x0f0 | (flag<<2) |3);
}
Addr skip4(Nibble flag) {
	return 	pb(0x0f0 | (flag<<2) |0);
}
Addr mov(Nibble rx, Nibble ry)
{	
	return 	pb(0x800| (rx<<4) | ry);
}
Addr xor(Nibble rx, Nibble ry)
{	
	return 	pb(0x700| (rx<<4) | ry);
}
Addr xorR0n( Nibble nn)
{	
	return 	pb(0x70|  nn);
}
Addr add(Nibble rx, Nibble ry)
{	
	return 	pb(0x100| (rx<<4) | ry);
}
Addr adc(Nibble rx, Nibble ry)
{	
	return 	pb(0x200| (rx<<4) | ry);
}
Addr sub(Nibble rx, Nibble ry)
{	
	return 	pb(0x300| (rx<<4) | ry);
}
Addr sbb(Nibble rx, Nibble ry)
{	
	return 	pb(0x400| (rx<<4) | ry);
}
Addr or(Nibble rx, Nibble ry)
{	
	return 	pb(0x500| (rx<<4) | ry);
}
Addr and(Nibble rx, Nibble ry)
{	
	return 	pb(0x600| (rx<<4) | ry);
}
Addr dsz( Nibble ry)
{	
	return 	pb(0x040 | ry);
}
Addr movn(Nibble rx, Nibble constant)
{	
	return 	pb(0x900| (rx<<4) | constant);
}
Addr movto(Byte location)   //MOV [NN], R0
{	
	return 	pb(0xc00| location);
}
Addr movfrom(Byte location)  //MOV R0,[NN]
{	
	return 	pb(0xd00| location);
}
Addr movtoi(Nibble x, Nibble y)   //MOV [XY], R0
{	
	return 	pb(0xa00| (x<<4) | y);
}
Addr movfromi(Nibble x, Nibble y)  //MOV R0,[XY]
{	
	return 	pb(0xb00| (x<<4) | y);
}
Addr movpc(Byte nn)
{
	return 	pb(0xe00| nn);
} 
Addr jr(Byte nn)
{
	return 	pb(0xf00| nn);
}
Addr jabs(Addr dest)
{
	pb(0xd00| dest>>4);
	return moven(0xd,(dest&0xf) );
}
Addr rrc(Byte nn)
{
	return 	pb(0xd0| nn);
}
Addr bit(Nibble reg, Nibble n)
{
	reg %= 4;
	n %=4;
	return 	pb(0x90| (reg<<2) | n);
}

Addr outn(Nibble n)
{
	return movn(ROut, n);
}
Addr out( Nibble reg)
{
	return mov(ROut, reg);
}
// pseudo codes
Addr nop()
{
	return mov(R0, R0);
}
Addr jumpto(unsigned dest)
{
	jr(0xff & (dest- pc-1));
}

// utilities for looping
Addr astack[1000];
Nibble regstack[1000];
Addr astackp;
int beginrepeat(Nibble reg, Nibble times)
{
	movn(reg%16,times%16 );
	regstack[astackp] = reg%16;
	astack[astackp++] = pc+1;
	return pc;
}

Addr endrepeat()
{
	dsz(regstack[--astackp]);
	return jumpto(astack[astackp]);
}

SimpleExamples()
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

CustomDacExamples()
{

zeroPC("Square tone generator");
{			movn(R0, dBank);
	Addr back = xor(ROut, R0);
				jumpto( back);
}				
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
zeroPC("DAC Sawtooth");
{
				movn(ROut,dReset ); // reset the Dac Counter
				movn(ROut, 0 );
				movn(R0,dClock );
	Addr back =	xor(ROut, R0);      // clock the Dac Counter
				xor(ROut, R0);
				jumpto(back);
}

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
}


main()
{
	SimpleExamples();
	CustomDacExamples();
}