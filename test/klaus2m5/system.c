#include <stdio.h>
#include "cpu6502.h"
#include "cpu6502_debug.h"

cpu6502_t cpu;
int data[65536];

int _read(int adr) {
  return data[adr];
}

void _write(int adr, int value) {
  data[adr]=value;
}

int main(void) {
  FILE *test_file;
  test_file=fopen("6502_functional_test.bin", "rb");
  for (int i=0; i<65536; i++) {
    data[i]=fgetc(test_file);
  }
  fclose(test_file);

  cpu.read=_read;
  cpu.write=_write;

  cpu6502_reset(&cpu);

  cpu.reg_PC=0x400;

  cpu6502_run(&cpu, 100000000); // run 100 Million Clock Cycles

  if (cpu.reg_PC==0x3399) {
    printf("=== All tests successfull! ===\n");
    return 0;
  }
  else {
    printf("=== Test failed: PC=$%04X ===\n", cpu.reg_PC);
    return 1;
  }

}
