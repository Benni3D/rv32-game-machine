include $(TOP)/config.mk

objects_c=$(patsubst src/%.c,obj/%.o,$(wildcard src/*.c))
objects_s=$(patsubst src/%.s,obj/%.o,$(wildcard src/*.s))
objects=$(objects_c) $(objects_s)

all: libc.a crt0.o

obj:
	mkdir -p obj

crt0.o: crt0.s
	$(AS) -o $@ $< $(ASFLAGS)

libc.a: obj $(objects)
	$(AR) $@ $(objects) $(ARFLAGS)


obj/%.o: src/%.c include
	$(CC) -o $@ $< $(CFLAGS)

obj/%.o: src/%.s
	$(AS) -o $@ $< $(ASFLAGS)

clean:
	rm -rf obj crt0.o libc.a

.PHONY: all clean
