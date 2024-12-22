# KFS-1

KFS stands for Kernel From Scratch. It's a list of projects that teaches you how
to create a kernel by yourself.

## Dependencies

In order for this project to work, you have to have the following dependencies:
 - build-essential
 - grub-pc
 - xorriso
 - mtools
 - nasm
 - qemu-system-i386

## Building

 - `make`: Builds a kfs1.iso image that is bootable
 - `make clean`: Removes object files
 - `make fclean`: Removes object and .iso files
 - `make re`: Executes fclean and make rules
 - `make run`: Launches .iso file in a VM with qemu
 - `make run_kernel`: Launches .bin file in a VM qemu
