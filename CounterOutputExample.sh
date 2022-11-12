cc -o CounterOutputExample CounterOutputExample.c badgeAssembler.c badgeAssemblerTextOut.c
cc -o CounterOutputExampleBinaryOutput CounterOutputExample.c badgeAssembler.c badgeAssemblerBinaryOut.c
./CounterOutputExample
./CounterOutputExampleBinaryOutput > CounterOutputExample.bin
