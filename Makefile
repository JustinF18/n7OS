KERNEL=kernel.bin
PXE=boot.PXE

CC= gcc
CFLAGS= -m32 -g -gstabs -std=c99 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
         -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -Iinclude

LD= ld
LDFLAGS = -melf_i386

AS= gcc
ASFLAGS= -m32 -DASSEMBLER -g -gstabs -Iinclude

OBJCOPY= objcopy

LIBS= lib/lib.o

QEMU = /usr/bin/qemu-system-i386
QEMUOPTS = -cpu pentium -rtc base=localtime -k fr -m 256M -D /tmp/qemu_log.txt  -kernel kernel.bin
QEMUGDB= -s -S -gdb tcp::1234

DEBUG=gdb
DIRS=--directory=kernel --directory=boot --directory=bin --directory=lib

EMACS=emacs

# cible principale, on nettoie tout avant
.PHONY: all
all: clean kernel.bin

$(LIBS):
	(cd lib ; make)

kernel/kernel.o:
	(cd kernel ; make kernel.o)

kernel/task_dump_screen.o:
	(cd kernel ; make task_dump_screen.o)

boot/crt0.o:
	(cd boot ; make)

bin/app.o:
	(cd bin ; make)

# generation du noyau
kernel.bin: boot/crt0.o $(LIBS) kernel/task_dump_screen.o bin/app.o kernel/kernel.o
	$(LD) $(LDFLAGS) -e entry -Tboot/kernel.lds $^ -o $@

clean:
	(cd bin ; make clean)
	(cd lib ; make clean)
	(cd kernel ; make clean)
	(cd boot ; make clean)
	rm -f $(KERNEL) core *~

dbg-qemu: kernel.bin
	$(QEMU) $(QEMUOPTS) $(QEMUGDB) &
	$(DEBUG) $(DIRS) $^
	pkill qemu

dbg-vscode: kernel.bin
	$(QEMU) $(QEMUOPTS) $(QEMUGDB) &

dbg: all
	$(EMACS) --eval '(progn (make-term "QEMU" "qemu-system-i386" nil "-s" "-S" "-m" "256M" "-kernel" "kernel.bin" "-display" "curses") (split-window-horizontally) (split-window-vertically) (balance-windows) (gdb "$(DEBUG) $(DIRS) -i=mi kernel.bin") (insert "target remote :1234") (other-window 2) (toggle-frame-fullscreen) (switch-to-buffer "*QEMU*") (other-window -2))'


run: kernel.bin
	$(QEMU) $(QEMUOPTS)

archive: clean
	(cd .. ; tar cvf - n7OS | gzip > n7OS_`date +"%F"`.tgz)
