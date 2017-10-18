#include "cpu6502_debug.h"
#include "cpu6502_opcodes.h"
#include "cpu6502.h"
#include <stdio.h>

void cpu6502_dump(cpu6502_t *cpu) {
  struct opcode_tbl_entry opcode=opcode_tbl[cpu->read(cpu->reg_PC)];

  printf("A=$%02X X=$%02X Y=$%02X SP=$%02X PC=$%04X ", cpu->reg_A, cpu->reg_X, cpu->reg_Y, cpu->reg_SP, cpu->reg_PC);
  printf("%c%c%c%c", cpu->status_N?'N':'n', cpu->status_V?'V':'v', cpu->status_U?'U':'u', cpu->status_B?'B':'b');
  printf("%c%c%c%c ", cpu->status_D?'D':'d', cpu->status_I?'I':'i', cpu->status_Z?'Z':'z', cpu->status_C?'C':'c');
  printf("%s\n", opcode.mnemonic);
}
