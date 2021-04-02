#/bin/bash

IMAGE=$(realpath disk.img)
HDD=/dev/loop0
PART=/dev/loop1
MNT=/mnt/vdisk
KERNEL=YeetOS

dd if=/dev/zero of=$IMAGE bs=512 count=131072
printf "o\nn\np\n1\n\n\na\np\nw\n" | fdisk $IMAGE
sudo losetup -o 1048576 /dev/loop3 $IMAGE
sudo mkfs.fat -h 2048 -R 8 /dev/loop3
sudo losetup -d /dev/loop3

sudo losetup $HDD $IMAGE
sudo losetup -o 1048576 $PART $IMAGE
sudo mount $PART $MNT

sudo grub-install --root-directory=$MNT --no-floppy --modules="normal part_msdos fat multiboot" $HDD

sudo umount $PART
sudo losetup -d $HDD
sudo losetup -d $PART