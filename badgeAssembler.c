#include <stdio.h>
#include "badgeAssembler.h"
#include "badgeAssemblerOutput.h"
//
// Simple Assembler/Code Synthesizer for the Supercon.6 badge
// This first version uses a single pass 
// Adrian Freed 2022

// This is dedicated to the memory of Kathleen Booth
// who was part of a small team that built some of the earliest digital computers,
// programmed them and designed one of the first assembler languages.
// She was a pioneering researcher on neural networks and wrote important influential
// books on programming in the 1950s.
//
extern int pc;
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
Addr cp(Nibble reg)
{
	return pb(0x000 | (reg % 16));
}
Addr addR0(Nibble n)
{
	return pb(0x010 | (n % 16));
}
Addr inc(Nibble reg)
{
	return pb(0x020 | (reg % 16));
}
Addr dec(Nibble reg)
{
	return pb(0x030 | (reg % 16));
}
Addr dsz( Nibble ry)
{	
	return 	pb(0x040 | ry);
}
Addr orR0(Nibble n)
{
	return pb(0x050 | (n % 16));
}

Addr andR0(Nibble n)
{
	return pb(0x060 | (n % 16));
}
Addr xorR0(Nibble n)
{
	return pb(0x070 | (n % 16));
}
Addr exr(Nibble n)
{
	return pb(0x080 | (n % 16));
}
Addr bit(Nibble reg, Nibble n)
{
	reg %= 4;
	n %=4;
	return 	pb(0x090| (reg<<2) | n);
}

Addr bset(Nibble reg, Nibble bit)
{	
	if(reg>3 || bit > 3)
	{
		printf("warning: bset register and bit have to be less than 4\n");
	}
	return pb(0x0a0 | (reg<<2) || bit);
}
Addr bclr(Nibble reg, Nibble bit)
{	
	if(reg>3 || bit > 3)
	{
		printf("warning: bset register and bit have to be less than 4\n");
	}
	return pb(0x0b0 | (reg<<2) || bit);
}
Addr btgl(Nibble reg, Nibble bit)
{	
	if(reg>3 || bit > 3)
	{
		printf("warning: bset register and bit have to be less than 4\n");
	}
	return pb(0x0c0 | (reg<<2) || bit);
}


Addr rrc(Byte nn)
{
	return 	pb(0x0d0| nn);
}

Addr ret(Byte nn)
{
	return 	pb(0x0e0| nn);
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
Addr xor(Nibble rx, Nibble ry)
{	
	return 	pb(0x700| (rx<<4) | ry);
}
Addr mov(Nibble rx, Nibble ry)
{	
	return 	pb(0x800| (rx<<4) | ry);
}
Addr movn(Nibble rx, Nibble constant)
{	
	return 	pb(0x900| (rx<<4) | constant);
}

Addr movtoi(Nibble x, Nibble y)   //MOV [XY], R0
{	
	return 	pb(0xa00| (x<<4) | y);
}
Addr movfromi(Nibble x, Nibble y)  //MOV R0,[XY]
{	
	return 	pb(0xb00| (x<<4) | y);
}
Addr movto(Byte location)   //MOV [NN], R0
{	
	return 	pb(0xc00| location);
}
Addr movfrom(Byte location)  //MOV R0,[NN]
{	
	return 	pb(0xd00| location);
}
Addr movpc(Byte nn)
{
	return 	pb(0xe00| nn);
} 
Addr jr(Byte nn)
{
	return 	pb(0xf00| nn);
}

Addr jsr(Addr dest)
{
	pb(0xe00| dest>>4);
	return movn(0xc,(dest&0xf) );
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
Addr jabs(Addr dest)
{
	pb(0xe00| dest>>4);
	return movn(0xd,(dest&0xf) );
}
Addr nop()
{
	return mov(R0, R0);
}
Addr lsr(Nibble reg)
{
	andR0(0);
	return rrc(reg);
}
Addr cplR0()
{
	return xorR0(0xf);
}
Addr jumpto(unsigned dest)
{
	return jr(0xff & (dest- pc-1));
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
