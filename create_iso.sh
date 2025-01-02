apt-get update && apt-get install -y grub2-common xorriso grub-pc-bin
mkdir -p isodir/boot/grub
cp kernel-100 isodir/boot/
cp grub.cfg isodir/boot/grub/
grub-mkrescue -o kfs1.iso isodir
