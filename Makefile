TOP=$(PWD)
include $(TOP)/config.mk

all: rvgm_vm libc.a game.bin

rvgm_vm: rvgm/src rvgm/include libc.a
	$(MAKE) TOP=$(TOP) -C rvgm all
	cp rvgm/$@ $@

libc.a: lib/include lib/src
	$(MAKE) TOP=$(TOP) -C lib all
	cp lib/$@ $@

game.bin: game/include game/src libc.a
	$(MAKE) TOP=$(TOP) -C game all
	cp game/$@ $@

dump:
	riscv32-elf-objdump -d game/game.elf

run: all
	./rvgm_vm game.bin

clean:
	$(MAKE) TOP=$(TOP) -C rvgm clean
	$(MAKE) TOP=$(TOP) -C lib clean
	$(MAKE) TOP=$(TOP) -C game clean
	rm -f rvgm_vm libc.a game.bin

.PHONY: all dump run clean
