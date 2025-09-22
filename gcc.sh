# sudo apt install -y build-essential bison flex libgmp3-dev libmpc-dev libmfr-dev texinfo libisl-dev

export PREFIX="$PWD/opt/cross"
export TARGET=i386-elf
export PATH="$PREFIX/bin:$PATH"

git clone git://sourceware.org/git/binutils-gdb.git
git clone https://gcc.gnu.org/git/gcc.git
cd binutils-gdb
git switch binutils-2_41-branch
cd ../gcc
git switch --detach releases/gcc-13.2.0

mkdir -p build-binutils
cd build-binutils
../binutils-gdb/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
make install
cd ..

which -- $TARGET-as || echo $TARGET-as is not in the PATH

mkdir -p build-gcc
cd build-gcc
../gcc/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc
