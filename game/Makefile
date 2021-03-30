include $(TOP)/config.mk

objects=$(patsubst src/%.c,obj/%.o,$(wildcard src/*.c))

all: game.bin

game.bin: game.elf
	riscv32-elf-objcopy -O binary $< $@

game.elf: obj $(objects)
	$(LD) -T linker.ld -o $@ $(TOP)/lib/crt0.o $(objects) $(TOP)/libc.a $(LDFLAGS)

obj:
	mkdir -p obj

obj/%.o: src/%.c
	$(CC) -o $@ $< $(CFLAGS) -I$(TOP)/lib/include

clean:
	rm -rf obj game.elf game.bin

.PHONY: all clean
