#!/bin/bash

BINUTILS=2.36.1
GCC=10.2.0

BIN_DIR=binutils-${BINUTILS}
GCC_DIR=gcc-${GCC}

mkdir -p tools
pushd tools

[ -f "binutils-${BINUTILS}.tar.xz" ] || wget "https://ftp.gnu.org/gnu/binutils/binutils-${BINUTILS}.tar.xz" 2>&1
[ $? -ne 0 ] && echo "Failed to download binutils" >&2 && exit 1
[ -f "gcc-${GCC}.tar.xz" ] || wget "https://ftp.gnu.org/gnu/gcc/gcc-${GCC}/gcc-${GCC}.tar.xz" 2>&1
[ $? -ne 0 ] && echo "Failed to download gcc" >&2 && exit 1

rm -rf "${BIN_DIR}" "${GCC_DIR}"
[ $? -ne 0 ] && echo "Failed to delete ${BIN_DIR} and/or ${GCC_DIR}" >&2 && exit 1

echo "Extracting binutils..." >&2
tar -xf "binutils-${BINUTILS}.tar.xz"
echo "Extracting gcc..." >&2
tar -xf "gcc-${GCC}.tar.xz"

mkdir -p "${BIN_DIR}/build" "${GCC_DIR}/build"
[ $? -ne 0 ] && exit 1

# Build & Install binutils
echo "Configuring binutils..." >&2
pushd ${BIN_DIR}/build
../configure --disable-multilib --enable-silent-rules --prefix=/usr/local --target=riscv32-elf --disable-nls --disable-werror --enable-lto --with-system-zlib 2>&1
[ $? -ne 0 ] && echo "Failed to configure binutils" >&2 && exit 1
echo "Building binutils..." >&2
nice -n1 make -j$(nproc) 2>&1
[ $? -ne 0 ] && echo "Failed to build binutils" >&2 && exit 1
echo "Installing binutils..." >&2
sudo make install 2>&1
[ $? -ne 0 ] && echo "Failed to install binutils" >&2 && exit 1
popd

# Download dependencies
pushd ${GCC_DIR}
echo "Downloading dependencies for GCC..." >&2
./contrib/download_prerequisites 2>&1
[ $? -ne 0 ] && echo "Failed to download dependencies for gcc" >&2 && exit 1
popd

# Build & Install gcc
pushd ${GCC_DIR}/build
echo Configuring GCC... >&2
../configure --disable-multilib --enable-silent-rules --target=riscv32-elf --prefix=/usr/local --with-abi=ilp32 --with-arch=rv32i --without-headers --enable-lto --enable-languages=c --disable-nls --with-system-zlib 2>&1
[ $? -ne 0 ] && echo "Failed to configure gcc" >&2 && exit 1
echo "Building GCC..." >&2
nice -n1 make -j$(nproc) all-gcc 2>&1
[ $? -ne 0 ] && echo "Failed to build gcc" >&2 && exit 1
echo "Building libgcc..." >&2
nice -n1 make -j$(nproc) all-target-libgcc 2>&1
[ $? -ne 0 ] && echo "Failed to build libgcc" >&2 && exit 1

echo "Installing GCC & libgcc..." >&2
sudo make install install-gcc install-target-libgcc
[ $? -ne 0 ] && echo "Failed to install gcc" >&2 && exit 1
popd

popd

