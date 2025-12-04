ASM = nasm
CC = gcc
LD = ld

CFLAGS = -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib
LDFLAGS = -m elf_i386

all: kernel.elf iso/os.iso

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

asm.o: kernel.asm
	$(ASM) -f elf32 kernel.asm -o asm.o

kernel.elf: kernel.o asm.o link.ld
	$(LD) $(LDFLAGS) -T link.ld -o kernel.elf asm.o kernel.o

iso/os.iso: kernel.elf grub.cfg
	mkdir -p iso/boot/grub
	cp kernel.elf iso/boot/
	cp grub.cfg iso/boot/grub/
	grub-mkrescue -o iso/os.iso iso

clean:
	rm -f *.o *.elf
	rm -rf iso
