include Makefile.inc

MEM_MANAGER=buddy # buddy o mm_manager
KERNEL=kernel.bin
SOURCES=$(wildcard *.c)
SOURCES_INTERRUPTIONS=$(wildcard interruptions/*.c)
SOURCES_INTERRUPTIONS_ASM=$(wildcard interruptions/*.asm)
SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_INTERRUPTIONS=$(SOURCES_INTERRUPTIONS:.c=.o)
OBJECTS_INTERRUPTIONS_ASM=$(SOURCES_INTERRUPTIONS_ASM:.asm=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
LOADERSRC=loader.asm

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

ALL_OBJECTS= $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS_INTERRUPTIONS) $(OBJECTS_INTERRUPTIONS_ASM)

all: $(MEM_MANAGER) $(KERNEL)

$(KERNEL): $(STATICLIBS) $(ALL_OBJECTS)
	$(LD) $(LDFLAGS) -T kernel.ld -o $@ $^ memoryManager.o

buddy:
	$(GCC) $(GCCFLAGS) -I./include -c "memoryManager/buddyManager.c" -o memoryManager.o

mm_manager:
	$(GCC) $(GCCFLAGS) -I./include -c "memoryManager/memoryManager.c" -o memoryManager.o

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -I./interruptions -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf $(ALL_OBJECTS) *.bin memoryManager.o

.PHONY: all clean
