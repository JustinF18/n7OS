CC= gcc
CFLAGS= -m32 -gstabs -std=c99 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
         -nostartfiles -nodefaultlibs -Wall -Werror -I../include

LD= ld
LDFLAGS = -melf_i386

AS= gcc
ASFLAGS= -m32 -DASSEMBLER -g -gstabs -Iinclude

OBJCOPY= objcopy

AR= ar

OBJECTS = $(addsuffix .o, $(basename  $(wildcard *.c)))

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) -c $< -o $@

lib.o:  $(OBJECTS)
	$(LD) $(LDFLAGS) -r -o lib.o $(OBJECTS)

clean:
	rm -f *.o *~ *.a
