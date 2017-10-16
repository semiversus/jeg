#include "cpu6502.h"
#include "cpu6502_opcodes.h"

void cpu6502_reset(cpu6502_t *cpu) {
  // load program counter with address stored at 0xFFFC (low byte) and 0xFFFD (high byte)
  cpu->reg_PC=cpu->read(0xFFFC)+(cpu->read(0xFFFD)<<8);
  cpu->reg_SP=0xFD; // reset stack pointer
  SET_FLAGS(0x24); // set following status flags: UNUSED, INTERRUPT
}
#define SET_FLAGS(v) do{ \
    cpu->status_C=(v)&0x01; \
    cpu->status_Z=((v)>>1)&0x01; \
    cpu->status_I=((v)>>2)&0x01; \
    cpu->status_D=((v)>>3)&0x01; \
    cpu->status_B=((v)>>4)&0x01; \
    cpu->status_U=((v)>>5)&0x01; \
    cpu->status_V=((v)>>6)&0x01; \
    cpu->status_N=((v)>>7)&0x01; \
  } while(0);

#define GET_FLAGS() ((cpu->status_N<<7)|(cpu->status_V<<6)|(cpu->status_U<<5)| \
  (cpu->status_B<<4)|(cpu->status_D<<3)|(cpu->status_I<<2)|(cpu->status_Z<<1)| \
  cpu->status_C)

#define PUSH(v) do { \
  cpu->write(0x100|cpu->reg_SP, (v)&0xFF); \
  cpu->reg_SP--; \
  if (cpu->reg_SP<0) { \
    cpu->reg_SP=0xFF; \
  } \
} while(0);

#define READ16(adr) (cpu->read(adr)|(cpu->read((adr)+1)<<8))

#define READ16BUG(adr) (cpu->read(adr)|(cpu->read(()(adr)&0xFF00)+()((adr)+1)&0xFF))<<8))

#define PAGE_DIFFERS(a,b) ((a)&0xFF00!=(b)&0xFF00)

int cpu6502_run(cpu6502_t *cpu, int cycles_to_run) {
  opcode_tbl_entry opcode;
  int cycles_passed; // cycles used in one iteration
  int address; // calculated address for memory interaction
  int page_crossed; // flag if page is crossed during memory access

  do {
    cycles_passed=0;

    // check if cpu is stalling
    if (cpu->stall_cycles) {
      cycles_passed=cpu->stall_cycles;
      cpu->stall_cycles=0;
    }

    // check for interrupts
    if (cpu->interrupt_pending) {
      PUSH(cpu->reg_PC>>8); // high byte of program counter to stack
      PUSH(cpu->reg_PC); // low byte
      PUSH(GET_FLAGS() | 0x10); // push status flags to stack
      if (cpu->interrupt_pending==INTERRUPT_NMI) {
        cpu->reg_PC=READ16(0xFFFA);
      }
      else {
        cpu->reg_PC=READ16(0xFFFE);
      }
      cpu->flags_I=1;
      cycles_passed+=7;
      cpu->interrupt_pending=INTERRUPT_NONE,
    }

    // read op code
    opcode=opcode_tbl[cpu->read(cpu->reg_PC++)];

    // handle address mode
    page_crossed=0;

    switch (opcode.address_mode) {
      case ADR_ABSOLUTE:
        address=READ16(cpu->reg_PC+1);
        break;
      case ADR_ABSOLUTE_X:
        address=READ16(cpu->reg_PC+1)+cpu->reg_X;
        page_crossed=PAGE_DIFFERS(address-cpu->reg_X, address);
        break;
      case ADR_ABSOLUTE_Y:
        address=READ16(cpu->reg_PC+1)+cpu->reg_Y;
        page_crossed=PAGE_DIFFERS(address-cpu->reg_Y, address);
        break;
      case ADR_ACCUMULATOR:
        address=0;
        break;
      case ADR_IMMEDIATE:
        address=cpu.PC+1;
        break;
      case ADR_IMPLIED:
        address=0;
        break;
      case ADR_INDEXED_INDIRECT:
        address=READ16BUG(cpu->read(cpu->PC+1)+cpu->X)
        break;
      case ADR_INDIRECT:
        address=READ16BUG(READ16(cpu->reg_PC+1))
        break;
      case ADR_INDIRECT_INDEXED:
        address=READ16BUG(cpu->read(cpu->reg_PC+1))+cpu->reg_Y;
        page_crossed=PAGE_DIFFERS(address-cpu->reg_Y, address);
        break;
      case ADR_RELATIVE:
        address=cpu->read(cpu->reg_PC+1)
        if (address<0x80) {
          address+=cpu->reg_PC+2;
        }
        else {
          address+=cpu->reg_PX+2-0x100;
        }
        break;
      case ADR_ZERO_PAGE:
        address=cpu->read(cpu->reg_PC+1)
        break;
      case ADR_ZERO_PAGE_X:
        address=cpu->read(cpu->reg_PC+1)+cpu->reg_X;
        break;
      case ADR_ZERO_PAGE_Y:
        address=cpu->read(cpu->reg_PC+1)+cpu->reg_Y;
        break;
    }

    address&=0xFFFF; // mask 16 bit

    cpu->reg_PC+=opcode.bytes; // update program counter
    cycles_passed+=opcode.cycles; // update cycles for this opcode

    if page_crossed { // if page was crossed see if instruction needs additional cycle
      cycles_passed+=opcode.page_cross_cycles;
    }

    switch(opcode.operation) {
      case OP_ADC:
        cpu->reg_A=cpu->reg_A+cpu->read(address)+cpu->flag_C;

        break;
      case OP_AHX:
        break;
      case OP_ALR:
        break;
      case OP_ANC:
        break;
      case OP_AND:
        break;
      case OP_ARR:
        break;
      case OP_ASL:
        break;
      case OP_AXS:
        break;
      case OP_B:
        break;
      case OP_BEQ:
        break;
      case OP_BIT:
        break;
      case OP_BMI:
        break;
      case OP_BNE:
        break;
      case OP_BPL:
        break;
      case OP_BRK:
        break;
      case OP_BVC:
        break;
      case OP_BVS:
        break;
      case OP_C:
        break;
      case OP_D:
        break;
      case OP_DEC:
        break;
      case OP_DEX:
        break;
      case OP_DEY:
        break;
      case OP_EOR:
        break;
      case OP_I:
        break;
      case OP_INC:
        break;
      case OP_INX:
        break;
      case OP_INY:
        break;
      case OP_ISC:
        break;
      case OP_JMP:
        break;
      case OP_JSR:
        break;
      case OP_KIL:
        break;
      case OP_LAS:
        break;
      case OP_LAX:
        break;
      case OP_LDA:
        break;
      case OP_LDX:
        break;
      case OP_LDY:
        break;
      case OP_LSR:
        break;
      case OP_NOP:
        break;
      case OP_ORA:
        break;
      case OP_P:
        break;
      case OP_PHA:
        break;
      case OP_PHP:
        break;
      case OP_PLA:
        break;
      case OP_PLP:
        break;
      case OP_RLA:
        break;
      case OP_ROL:
        break;
      case OP_ROR:
        break;
      case OP_RRA:
        break;
      case OP_RTI:
        break;
      case OP_RTS:
        break;
      case OP_SAX:
        break;
      case OP_SBC:
        break;
      case OP_SEC:
        break;
      case OP_SED:
        break;
      case OP_SEI:
        break;
      case OP_SHX:
        break;
      case OP_SHY:
        break;
      case OP_SLO:
        break;
      case OP_SRE:
        break;
      case OP_STA:
        break;
      case OP_STX:
        break;
      case OP_STY:
        break;
      case OP_TAS:
        break;
      case OP_TAX:
        break;
      case OP_TAY:
        break;
      case OP_TSX:
        break;
      case OP_TXA:
        break;
      case OP_TXS:
        break;
      case OP_TYA:
        break;
      case OP_V:
        break;
      case OP_X:
        break;
      case OP_XAA:
        break;
      case OP_Y:
        break;
    }

    cycles_to_run-=cycles_passed;
  } while (cycle_to_run>0);

  return cycle_index;
}

void cpu6502_trigger_interrupt(cpu6502_t *cpu, cpu6502_interrupt_t interrupt) {
  // TODO: could it happen that NMI and IRQ occuring the same time?
  switch (interrupt) {
    case INTERRUPT_NMI:
      cpu->interrupt_pending=INTERRUPT_NMI;
      break;
    case INTERRUPT_IRQ:
      if (cpu->status_I) {
        cpu->interrupt_pending=INTERRUPT_NMI;
      }
      break;
  }
}
