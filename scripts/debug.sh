#/bin/bash

IMAGE=disk.img
HDD=/dev/loop0
PART=/dev/loop1
MNT=/mnt/vdisk
KERNEL=YeetOS

qemu-system-i386 -drive format=raw,file=$IMAGE,if=ide -S -gdb tcp::9000 -m 512 -name "YeetOS" -serial stdio -k ch
