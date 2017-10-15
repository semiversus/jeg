#include "cpu_6502.h"

#define _SET_FLAGS(v) do{ \
    cpu->status_C=(v)&0x01; \
    cpu->status_Z=((v)>>1)&0x01; \
    cpu->status_I=((v)>>2)&0x01; \
    cpu->status_D=((v)>>3)&0x01; \
    cpu->status_B=((v)>>4)&0x01; \
    cpu->status_U=((v)>>5)&0x01; \
    cpu->status_V=((v)>>6)&0x01; \
    cpu->status_N=((v)>>7)&0x01; \
  } while(0);

void cpu_6502_reset(cpu_6502_t *cpu) {
  // load program counter with address stored at 0xFFFC (low byte) and 0xFFFD (high byte)
  cpu->reg_PC=cpu->read(0xFFFC)+(cpu->read(0xFFFD)<<8);
  cpu->reg_SP=0xFD; // reset stack pointer
  _SET_FLAGS(0x24); // set following status flags: UNUSED, INTERRUPT
}

int cpu_6502_run(cpu_6502_t *cpu, int n_cycles) {
  int opcode, cycle_index=0;

  do {

    // check if cpu is stalling
    if (cpu->stall_cycles) {
      if (cpu->stall_cycles>=n_cycles) {
        cpu->stall_cycles-=n_cycles;
        n_cycles=0;
        break;
      }
      else {
        n_cycles-=cpu->stall_cycles;
        cpu->stall_cycles=0;
      }
    }

    // read op code
    opcode=cpu->read(cpu->reg_PC++);

    // TODO...


  } while (cycle_index<n_cycles);

  return cycle_index;
}

void cpu_6502_trigger_interrupt(cpu_6502_t *cpu, cpu_6502_interrupt_t interrupt) {
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
