A Simple Assembler/Code Synthesizer for the Supercon.6 badge

This first version uses a single pass 
  Adrian Freed 2022

This is dedicated to the memory of Kathleen Booth
who was part of a small team that built some of the earliest digital computers,
programmed them and designed one of the first assembler languages.
She was a pioneering researcher on neural networks and wrote important influential
books on programming in the 1950s.

 
Rather than translate an assembler language, this code synthesizer provides
a library of constants and functions for each opcode.
These functions return the address of the next slot an opcode will be written to (a sort of Program Counter)
This means the result can be captured in a variable and used later to compute jumps back to that address
for loops etc.

It was developed on the first morning of Supercon 2022 and tweaked over subsequent days as required for
a hardware driver I was writing for the badge.
Not all the instructions have been tested but most of them have.
