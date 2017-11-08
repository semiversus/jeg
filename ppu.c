#include "ppu.h"

void ppu_init(ppu_t *ppu, void *reference, ppu_read_func_t read, ppu_write_func_t write, ppu_update_frame_func_t update_frame) {
  ppu_reset(ppu);
  ppu->reference=reference;
  ppu->read=read;
  ppu->write=write;
  ppu->update_frame=update_frame;
}

void ppu_reset(ppu_t *ppu) {
}

int ppu_read(ppu_t *ppu, int adr, uint64_t cycle_number) {
  return 0;
}

void ppu_write(ppu_t *ppu, int adr, int value, uint64_t cycle_number) {
}

int ppu_run(ppu_t *ppu, int n_cycles) {
  return 0;
}
