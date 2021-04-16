#/bin/bash

IMAGE=$(realpath disk.img)
HDD=/dev/loop1
PART=/dev/loop7
MNT=/mnt/vdisk
KERNEL=YeetOS

sudo losetup $HDD $IMAGE
sudo losetup -o 1048576 $PART $IMAGE
sudo mount $PART $MNT

sudo mkdir -p $MNT/boot
sudo mkdir -p $MNT/boot/grub
sudo cp grub.cfg $MNT/boot/grub/grub.cfg
sudo cp $KERNEL $MNT/boot/YeetOS

sudo umount $PART
sudo losetup -d $HDD
sudo losetup -d $PART