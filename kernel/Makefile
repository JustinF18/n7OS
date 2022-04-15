CWD=$(shell pwd)
PROJECT=$(shell dirname $(CWD))
CURRENT=$(shell basename $(CWD))
CPU=$(shell uname -m)
ifeq ($(CPU), arm64)
	PREFIX=docker exec -it  -w /n7OS/$(CURRENT) i386gcc i386-elf-
else
	PREFIX=i386-elf-
endif

CC=$(PREFIX)gcc
CFLAGS= -m32 -gstabs -std=c99 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
         -nostartfiles -nodefaultlibs -Wall -Werror -I../include

LD=$(PREFIX)ld.bfd
LDFLAGS = -melf_i386

AS=$(PREFIX)gcc
ASFLAGS= -m32 -DASSEMBLER -g -gstabs -I../include

OBJCOPY=$(PREFIX)objcopy

OBJECTS = $(addsuffix .o, $(basename  $(wildcard *.c *.S)))

task_dump_screen.o: empty.o task_dump_screen.txt
		$(OBJCOPY) empty.o --add-section=.task_dump_screen=task_dump_screen.txt --set-section-flags=.task_dump_screen=contents,alloc,load,data task_dump_screen.o

kernel.o: $(OBJECTS)
	$(LD) $(LDFLAGS) -r -o kernel.o $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(AS) $(ASFLAGS) -c $< -o $@

clean:
	rm -f *.o *~ task_dump_screen.o
