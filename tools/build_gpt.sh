#!/bin/bash

BUILD_DIR=$(dirname $(realpath $0))/../build
echo $BUILD_DIR

mkdir -p $BUILD_DIR

DISK=$BUILD_DIR/disk.img
PARTITION=$BUILD_DIR/part.img

# Create GTP disk image.
dd if=/dev/zero of=$DISK bs=512 count=93750
parted $DISK -s -a minimal mklabel gpt
parted $DISK -s -a minimal mkpart EFI FAT16 2048s 93716s
parted $DISK -s -a minimal toggle 1 boot

# Create FAT16 partition.
dd if=/dev/zero of=$PARTITION bs=512 count=91669
mformat -i $PARTITION -h 32 -t 32 -n 64 -c 1
mmd -i $PARTITION ::/EFI
mmd -i $PARTITION ::/EFI/BOOT
mcopy -i $PARTITION $BUILD_DIR/BOOTX64.EFI ::/EFI/BOOT

# Write partition into the disk.
dd if=$PARTITION of=$DISK bs=512 count=91669 seek=2048 conv=notrunc

