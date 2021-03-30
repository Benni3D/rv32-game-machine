include $(TOP)/config.mk

objects=$(patsubst src/%.cpp,obj/%.o,$(wildcard src/*.cpp))

all: rvgm_vm

rvgm_vm: obj $(objects)
	$(HOSTLD) -o $@ $(objects) $(HOSTLDFLAGS)

obj:
	mkdir -p obj

obj/%.o: src/%.cpp include
	$(HOSTCXX) -c -o $@ $< -Iinclude $(HOSTCXXFLAGS)

clean:
	rm -rf rvgm_vm obj

.PHONY: all clean
