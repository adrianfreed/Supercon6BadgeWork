#include <stdio.h>

// Simple Assembler for the Supercon.6 badge
// Adrian Freed 2022

typedef unsigned char Nibble;
typedef unsigned char Byte;
typedef int Addr;

typedef Nibble var;

// Names
extern const Nibble Carry;
extern const Nibble Zed;
#define notflag(f) (f+1)
extern const Nibble R0;
extern const Nibble R1;
extern const Nibble R2;
extern const Nibble R3;
extern const Nibble R4;
extern const Nibble R5;
extern const Nibble R6;
extern const Nibble R7;
extern const Nibble R8;
extern const Nibble R9;
extern const Nibble ROut;
extern const Nibble RIn ;
extern const Nibble RJsr;
extern const Nibble RPcl;
extern const Nibble RPcm;
extern const Nibble Rpch;

// each functions emits the correct opcode 
// and returns the position of the PC
Addr cp(Nibble reg);
Addr addR0(Nibble n);
Addr inc(Nibble reg);
Addr dec(Nibble reg);
Addr dsz( Nibble ry);
Addr orR0(Nibble n);
Addr andR0(Nibble n);
Addr xorR0(Nibble n);
Addr exr(Nibble n);
Addr bit(Nibble reg, Nibble n);
Addr bset(Nibble reg, Nibble bit);
Addr bclr(Nibble reg, Nibble bit);
Addr btgl(Nibble reg, Nibble bit);

Addr rrc(Byte nn);
Addr ret(Byte nn);

Addr skip1(Nibble flag);
Addr skip2(Nibble flag);
Addr skip3(Nibble flag);
Addr skip4(Nibble flag);


Addr add(Nibble rx, Nibble ry);
Addr adc(Nibble rx, Nibble ry);
Addr sub(Nibble rx, Nibble ry);
Addr sbb(Nibble rx, Nibble ry);
Addr or(Nibble rx, Nibble ry);
Addr and(Nibble rx, Nibble ry);
Addr xor(Nibble rx, Nibble ry);
Addr mov(Nibble rx, Nibble ry);
Addr movn(Nibble rx, Nibble constant);


Addr movtoi(Nibble x, Nibble y);   //MOV [XY], R0

Addr movfromi(Nibble x, Nibble y);  //MOV R0,[XY]

Addr movto(Byte location);   //MOV [NN], R0

Addr movfrom(Byte location);  //MOV R0,[NN]

Addr movpc(Byte nn);
Addr jr(Byte nn);

Addr jsr(Addr dest);

Addr outn(Nibble n);
Addr out( Nibble reg);
// pseudo codes
Addr jabs(Addr dest);
Addr nop();
Addr lsr(Nibble reg);
Addr cplR0();
Addr jumpto(unsigned dest);

int beginrepeat(Nibble reg, Nibble times);

Addr endrepeat();