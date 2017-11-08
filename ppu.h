#ifndef PPU_H
#define PPU_H

#include <stdint.h>

typedef int (*ppu_read_func_t) (void *reference, int address); // read data [8bit] from address [16bit]
typedef void (*ppu_write_func_t) (void *reference, int address, int value); // write data [8bit] to address [16bit]
typedef void (*ppu_update_frame_func_t) (void *reference, uint8_t* frame_data, int width, int height);

typedef struct ppu_t {
  uint64_t last_cycle_number;

  // memory interface to vram and vrom
  void *reference;
  ppu_read_func_t read;
  ppu_write_func_t write;

  // frame data interface
  ppu_update_frame_func_t update_frame;
} ppu_t;

void ppu_init(ppu_t *ppu, void *reference, ppu_read_func_t read, ppu_write_func_t write, ppu_update_frame_func_t update_frame);

void ppu_reset(ppu_t *ppu);

int ppu_read(ppu_t *ppu, int adr, uint64_t cycle_number); // read data [8bit] from address [16bit]
void ppu_write(ppu_t *ppu, int adr, int value, uint64_t cycle_number); // write data [8bit] to address [16bit]

int ppu_run(ppu_t *ppu, int n_cycles); // update given cycles, returns cycles until next complete frame

#endif
