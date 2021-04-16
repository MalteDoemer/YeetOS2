#/bin/bash

IMAGE=disk.img
HDD=/dev/loop0
PART=/dev/loop1
MNT=/mnt/vdisk
KERNEL=YeetOS

qemu-system-i386 -accel kvm -drive format=raw,file=$IMAGE,if=ide -m 512 -name "YeetOS" -serial stdio -k ch
