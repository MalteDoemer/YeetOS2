

export ARCH=x86

export MAKE=make --no-print-directory

export CC=i686-elf-gcc
export LD=i686-elf-ld -m elf_i386
export AR=i686-elf-ar
export AS=nasm -f elf32

export CPP_FLAGS=-ggdb -ffreestanding -nostdlib -fno-leading-underscore -I $(abspath include)
export AS_FLAGS=
export LD_FLAGS=

IMAGE=$(abspath disk.img)
HDD=/dev/loop0
PART=/dev/loop1
MNT=/mnt/vdisk
KERNEL=YeetOS

OBJECTS=\
kernel/kernel.o \
arch/$(ARCH)/arch.o \

SUBDIRS=arch/$(ARCH) kernel


YeetOS: subdirs
	$(LD) -T arch/$(ARCH)/link.ld $(LD_FLAGS) $(OBJECTS) -o YeetOS

subdirs: 
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done

clean:
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i clean; done


format:
	dd if=/dev/zero of=$(IMAGE) bs=512 count=131072
	printf "o\nn\np\n1\n\n\na\np\nw\n" | fdisk $(IMAGE) 
	sudo losetup -o 1048576 /dev/loop3 $(IMAGE)
	sudo mkfs.fat -h 2048 -R 8 /dev/loop3
	sudo losetup -d /dev/loop3

	$(MAKE) attach
	sudo grub-install --root-directory=$(MNT) --no-floppy --modules="normal part_msdos fat multiboot" $(HDD)
	$(MAKE) detach

attach:
	sudo losetup $(HDD) $(IMAGE)
	sudo losetup -o 1048576 $(PART) $(IMAGE)
	sudo mount $(PART) $(MNT)

detach:
	sudo umount $(PART)
	sudo losetup -d $(HDD)
	sudo losetup -d $(PART)

install: subdirs
	$(MAKE) attach
	sudo mkdir -p $(MNT)/boot
	sudo mkdir -p $(MNT)/boot/grub
	sudo cp grub.cfg $(MNT)/boot/grub/grub.cfg
	sudo cp $(KERNEL) $(MNT)/boot/YeetOS
	$(MAKE) detach

run: install
	qemu-system-i386.exe \
	-drive format=raw,file='\\wsl$$\Ubuntu$(IMAGE)',if=ide \
	-m 512 \
	-name "YeetOS" \
	-monitor stdio

debug: install
	qemu-system-i386.exe \
	-drive format=raw,file='\\wsl$$\Ubuntu$(IMAGE)',if=ide \
	-m 512 \
	-name "YeetOS" \
	-S -gdb tcp::9000 \
	-monitor stdio