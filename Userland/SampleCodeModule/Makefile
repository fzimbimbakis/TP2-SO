include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard [^_]*.c)
SOURCES_ASM=$(wildcard *.asm)
LOADERSRC=_loader.c
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
ALL_OBJECTS=$(LOADEROBJECT) $(OBJECTS_ASM) $(OBJECTS)
LOADEROBJECT=$(LOADERSRC:.c=.o)


all: $(MODULE)

$(MODULE): $(STATICLIBS) $(ALL_OBJECTS)
	$(LD) $(LDFLAGS) -T sampleCodeModule.ld $(ALL_OBJECTS) -o ../$(MODULE)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -rf *.o

.PHONY: all clean print
