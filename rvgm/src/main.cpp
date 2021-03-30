#include <cstdio>
#include "video.hpp"
#include "cpu.hpp"

int main(int argc, char* argv[]) {
   using namespace rvgm;
   auto bus = std::make_shared<Bus>();
   bus->add_device(0x0000'0000, std::make_shared<RAM>(65536));
   bus->add_device(0x0001'0000, std::make_shared<RAM>(65536));
   bus->add_device(0x1000'0000, std::make_shared<RAM>(1 << 20));
   bus->add_device(0x8000'0000, std::make_shared<SerialDevice>(stdin, stdout));
   bus->add_device(0x8000'0004, std::make_shared<TimePort>());
   bus->add_device(0x8000'0008, std::make_shared<RandomPort>());
   bus->add_device(0x2000'0000, std::make_shared<VideoAdapter>(320, 200));

   bus->load_file("game.bin");

   Processor cpu{bus};
   while (cpu.clk());
   //printf("%08x\n", cpu.regs[1].read());
   return cpu.regs[1].read();
}
