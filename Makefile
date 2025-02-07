TARGET ?= x86_64

SYS_ROOT = $(shell pwd)
BOOT_DIR = $(SYS_ROOT)/boot

BOOT_SOURCES = $(wildcard $(BOOT_DIR)/$(TARGET)/*.s)
BOOT_OBJECTS = $(patsubst %.s,%.o,$(BOOT_SOURCES))


KERNEL_DIR= $(SYS_ROOT)/kernel
KERNEL_ASM_SOURCES = $(shell find $(KERNEL_DIR) -name '*.s' | sort)
KERNEL_C_SOURCES = $(shell find $(KERNEL_DIR) -name '*.c' | sort)
KERNEL_RS_SOURCES = $(shell find $(KERNEL_DIR) -name '*.rs' | sort)

KERNEL_HEADERS = $(shell find $(KERNEL_DIR) -name '*.h' | sort)

KERNEL_OBJECTS = $(patsubst %.c,%.o,$(KERNEL_C_SOURCES)) $(patsubst %.rs,%.o,$(KERNEL_RS_SOURCES)) $(patsubst %.s,%.o,$(KERNEL_ASM_SOURCES))

KERNEL_DEPS = $(KERNEL_OBJECTS:.o=.d)



OS_ENTRY = $(BOOT_DIR)/kernel.elf

TRIPLE = $(TARGET)-elf
CC = $(TRIPLE)-gcc
CXX = $(TRIPLE)-g++
RUSTC = rustc --target x86_64-unknown-none 
AS = nasm
LD = $(TRIPLE)-ld
AR = $(TRIPLE)-ar


CFLAGS = -c -m64 -mcmodel=kernel -ffreestanding -nostdlib -mno-red-zone  -Wall -Werror -I. -I$(KERNEL_DIR)
LDFLAGS =  -nostdlib -z nodefaultlib -z max-page-size=0x1000
NASMFLAGS = -f elf64



.PHONY: all clean strip run iso clang-format


all: $(OS_ENTRY)

$(OS_ENTRY): $(BOOT_OBJECTS) $(KERNEL_OBJECTS)
	$(LD)  $(LDFLAGS) -T $(BOOT_DIR)/link.ld -o $@ $(BOOT_OBJECTS) $(KERNEL_OBJECTS)


%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -MF $(@:.o=.d) $< -o $@


%.o: %.rs
	$(RUSTC) --crate-type staticlib $< -o $@

%.o: %.s
	$(AS) $(NASMFLAGS) $< -o $@


clean:
	rm -f $(BOOT_OBJECTS) $(KERNEL_OBJECTS) $(KERNEL_BIN) $(OS_ENTRY) $(KERNEL_DEPS)
	rm -rf isodir revision.iso


strip: $(OS_ENTRY)
	strip $(OS_ENTRY)


iso: revision.iso

revision.iso: $(OS_ENTRY)
	mkdir -p isodir/boot/grub
	cp -r iso/* isodir
	cp $(OS_ENTRY) isodir/boot/kernel.elf

	mkdir -p $(SYS_ROOT)/isodir/usr/include
	mkdir -p $(SYS_ROOT)/isodir/usr/lib

	grub-mkrescue -o revision.iso isodir


run: revision.iso
	qemu-system-x86_64 -cdrom revision.iso -m 64M -smp 2 -serial stdio -d int -D qemu.log


clang-format:
	clang-format -i $(KERNEL_SOURCES)  $(KERNEL_HEADERS)


-include $(KERNEL_DEPS)