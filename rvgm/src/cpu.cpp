#include <cstdio>
#include "cpu.hpp"
#include "debug.hpp"

namespace rvgm {
   void Processor::reset() noexcept {
      regs.reset();
      pc = 0;
   }
   bool Processor::clk() noexcept {
      const Instruction instr{ bus->read32(pc) };
      pc += 4;
      return exec(instr);
   }

   
	bool Processor::exec(const Instruction instr) {
	   std::uint32_t err_inf=0;
#if DEBUG
      std::fprintf(stderr, "Executing %08x at %08x\n", instr.raw, pc - 4);
#endif
      switch (instr.opcode) {
		case 0b011'0111: // LUI [U]
			regs[instr.upper_imm.rd] = instr.upper_imm.imm();
			break;
		case 0b001'0111: // AUIPC [U]
			regs[instr.upper_imm.rd] = pc + instr.upper_imm.imm();
			break;
			
		case 0b011'0011: { // ALU [R]
			const bool bit30 = (instr.reg.funct7 >> 5) & 1;
			auto& rd = regs[instr.reg.rd];
			const std::uint32_t rs1 = regs[instr.reg.rs1];
			const std::uint32_t rs2 = regs[instr.reg.rs2];
			switch (instr.reg.funct3) {
			case 0b000: // ADD/SUB
				if (bit30) rd = rs1 - rs2;
				else rd = rs1 + rs2;
				break;
			case 0b001: // SLL
				rd = rs1 << rs2;
				break;
			case 0b010: // SLT
				rd = (std::int32_t)rs1 < (std::int32_t)rs2;
				break;
			case 0b011: // SLTU
				rd = rs1 < rs2;
				break;
			case 0b100: // XOR
				rd = rs1 ^ rs2;
				break;
			case 0b101: // SRL/SRA
				if (bit30) rd = (std::int32_t)rs1 >> rs2;
				else rd = rs1 >> rs2;
				break;
			case 0b110: // OR
				rd = rs1 | rs2;
				break;
			case 0b111: // AND
				rd = rs1 & rs2;
				break;
			default: goto illegal_instruction;
			}
			break;
		}
		case 0b001'0011: { // ALUI [I]
			auto& rd = regs[instr.imm.rd];
			const std::uint32_t rs1 = regs[instr.imm.rs1];
			const std::uint32_t shamt = instr.imm_shift.shamt;
			const std::uint32_t imm = instr.imm.imm();
			const bool bit30 = (instr.imm_shift.funct7 >> 5) & 1;
			switch (instr.imm.funct3) {
			case 0b000: // ADDI
				rd = rs1 + imm;
				break;
			case 0b010: // SLTI
				rd = (std::int32_t)rs1 < (std::int32_t)imm;
				break;
			case 0b011: // SLTIU
				rd = rs1 < imm;
				break;
			case 0b100: // XORI
				rd = rs1 ^ imm;
				break;
			case 0b110: // ORI
				rd = rs1 | imm;
				break;
			case 0b111: // ANDI
				rd = rs1 & imm;
				break;
				
			case 0b001: // SLLI
				rd = rs1 << shamt;
				break;
			case 0b101: // SRL/SRA
				if (bit30) rd = (std::int32_t)rs1 >> shamt;
				else rd = rs1 >> shamt;
				break;
			default: goto illegal_instruction;
			}
			break;
		}
		case 0b000'0011: { // LB{,U}/LH{,U}/LW
			auto& rd = regs[instr.imm.rd];
			const std::uint32_t addr = regs[instr.imm.rs1] + instr.imm.imm();
			std::uint32_t tmp;
			switch (instr.imm.funct3) {
			case 0b000: // LB
				tmp = bus->read8(addr);
				if (tmp & 0x80) tmp |= 0xffff'ff00;
				rd = tmp;
				break;
			case 0b001: // LH
				tmp = bus->read16(addr);
				if (tmp & 0x8000) tmp |= 0xffff'0000;
				rd = tmp;
				break;
			case 0b010: // LW
				rd = bus->read32(addr);
				break;
			case 0b100: // LBU
				rd = bus->read8(addr);
				break;
			case 0b101: // LHU
				rd = bus->read16(addr);
				break;
			default: goto illegal_instruction;
			}
			break;
		}
		case 0b010'0011: { // SB/SH/SW
			const std::uint32_t addr = regs[instr.store.rs1] + instr.store.imm();
			const std::uint32_t rs2 = regs[instr.store.rs2];
			switch (instr.store.funct3) {
			case 0b000: // SB
            bus->write8(addr, rs2 & 0xff);
				break;
			case 0b001: // SH
				bus->write16(addr, rs2 & 0xffff);
				break;
			case 0b010: // SW
				bus->write32(addr, rs2);
				break;
			default: goto illegal_instruction;
			}
			break;
		}
		case 0b110'1111: { // JAL
			const std::uint32_t dest = instr.jump.imm() + pc - 4;
			if (dest & 3) { err_inf = dest; goto unaligned_jump; }
			regs[instr.jump.rd] = pc;
			pc = dest;
			break;
		}
		case 0b110'0111: { // JALR
			const std::uint32_t dest = regs[instr.imm.rs1] + instr.imm.imm();
			if (dest & 3) { err_inf = dest; goto unaligned_jump; }
			regs[instr.imm.rd] = pc;
			pc = dest;
			break;
		}
		case 0b110'0011: { // BRANCH
			const std::uint32_t addr = pc + instr.branch.imm() - 4;
			const std::uint32_t rs1 = regs[instr.branch.rs1];
			const std::uint32_t rs2 = regs[instr.branch.rs2];
			bool cond;
			if (addr & 3) { err_inf = addr; goto unaligned_jump; }
			switch (instr.branch.funct3) {
			case 0b000: // BEQ
				cond = (rs1 == rs2);
				break;
			case 0b001: // BNE
				cond = (rs1 != rs2);
				break;
			case 0b100: // BLT
				cond = ((std::int32_t)rs1 < (std::int32_t)rs2);
				break;
			case 0b101: // BGE
				cond = ((std::int32_t)rs1 >= (std::int32_t)rs2);
				break;
			case 0b110: // BLTU
				cond = (rs1 < rs2);
				break;
			case 0b111: // BGEU
				cond = (rs1 >= rs2);
				break;
			default: goto illegal_instruction;
			}
			if (cond) pc = addr;
			break;
		}
		case 0b000'1111: // FENCE (NOP)
			break;
		case 0b111'0011: // ECALL/EBREAK
			return !(instr.raw & 0x0010'0000);
		default: goto illegal_instruction;
		}
		return true;
	illegal_instruction:
      std::fprintf(stderr, "%08x: illegal instruction: %08x\n", pc - 4, instr.raw);
      return false;
	unaligned_jump:
      std::fprintf(stderr, "%08x: jumping to unaligned address %08x\n", pc - 4, err_inf);
		return false;
	}
}
