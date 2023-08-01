# UEFI bootloader makefile.

VERBOSE := 1

ifeq ($(VERBOSE), 1)
	Q :=
else
	Q := @
endif

CROSS_COMPILE := x86_64-w64-mingw32

CC := $(CROSS_COMPILE)-gcc
LD := $(CROSS_COMPILE)-ld
NM := $(CROSS_COMPILE)-nm
STRIP := $(CROSS_COMPILE)-strip
OBJCOPY := $(CROSS_COMPILE)-objcopy
OBJDUMP := $(CROSS_COMPILE)-objdump

BUILD_DIR := build
TARGET := $(BUILD_DIR)/BOOTX64.EFI

SRCS := $(shell find src -type f -name '*.c')
OBJS := $(SRCS:%.c=$(BUILD_DIR)/%.o)

CFLAGS := -std=c17 -Wall -Wextra -Wpedantic -mno-red-zone -ffreestanding -nostdlib
CFLAGS += -Iinclude/gnu-efi
LDFLASGS := --subsystem 10 -e efi_main

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "Linking ..."
	$(Q)$(LD) $(OBJS) -o $@ $(LDFLASGS)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "Compiling $^"
	$(Q)$(CC) $(CFLAGS) -c $< -o $@

.PHONY: run
run: all
	@tools/build_gpt.sh
	qemu-system-x86_64 -bios bin/bios64.bin -net none -drive format=raw,unit=0,file=build/disk.img

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

