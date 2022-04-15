CWD=$(shell pwd)
PROJECT=$(shell dirname $(CWD))
CURRENT=$(shell basename $(CWD))
CPU=$(shell uname -m)
ifeq ($(CPU), arm64)
	PREFIX=docker run --rm -ti -v $(PROJECT):/usr/src/myapp -w /usr/src/myapp/$(CURRENT) i386gcc i386-elf-
else
	PREFIX=
endif

CC=$(PREFIX)gcc
CFLAGS= -m32 -gstabs -std=c99 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
         -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -I../include

LD=$(PREFIX)ld.bfd
LDFLAGS = -melf_i386

AS=$(PREFIX)gcc
ASFLAGS= -m32 -DASSEMBLER -g -gstabs -I../include

OBJCOPY=$(PREFIX)objcopy

all: crt0.o

%.o: %.c
	$(CC) $(CFLAGS)  -c $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) -c $< -o $@

clean:
	rm -f *.o *~ task_dump_screen.o
