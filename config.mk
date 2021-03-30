
HOSTCC=gcc
HOSTCXX=g++
HOSTLD=g++

HOSTCFLAGS=-Wall -Wextra -O3
HOSTCXXFLAGS=$(HOSTCFLAGS) -std=c++17
HOSTLDFLAGS=-O1 -lSDL2 -lX11 -lpthread


AS=riscv32-elf-as
AR=riscv32-elf-ar rcs
CC=riscv32-elf-gcc
LD=riscv32-elf-ld

ASFLAGS=-march=rv32i -mabi=ilp32
ARFLAGS=
CFLAGS=-O2 -c -Iinclude -Wall -Wextra -ffreestanding -march=rv32i
LDFLAGS=-O1 -L/usr/local/lib/gcc/riscv32-elf/10.2.0/ -lgcc

