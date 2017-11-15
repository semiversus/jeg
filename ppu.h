#ifndef PPU_H
#define PPU_H

#include <stdint.h>

typedef int (*ppu_read_func_t) (void *reference, int address); // read data [8bit] from address [16bit]
typedef void (*ppu_write_func_t) (void *reference, int address, int value); // write data [8bit] to address [16bit]
typedef void (*ppu_update_frame_func_t) (void *reference, uint8_t* frame_data, int width, int height);

typedef struct nes_t nes_t;

typedef struct ppu_t {
  nes_t *nes; // reference to nes console

  // ppu state
  uint64_t last_cycle_number;
  uint32_t cycles_to_next_frame;
  int cycle;
  int scanline;

  int palette[32];
  int name_table[2048];
  int oam_data[256];

  // ppu registers
  int v; // current vram address (15bit)
  int t; // temporary vram address (15bit)
  int x; // fine x scoll (3bit)
  int w; // toggle bit (1bit)
  int f; // even/odd frame flag (1bit)

  int register_data;

  // background temporary variables
  int name_table_byte;
  int attribute_table_byte;
  int low_tile_byte;
  int high_tile_byte;
  uint64_t tile_data;

  // sprite temporary variables
  int sprite_count;
  uint32_t sprite_patterns[8];
  int sprite_positions[8];
  int sprite_priorities[8];
  int sprite_indicies[8];

  // memory accessable registers
  int ppuctrl;
  int ppumask;
  int ppustatus;
  int oam_address;
  int buffered_data;
   
  // memory interface to vram and vrom
  ppu_read_func_t read;
  ppu_write_func_t write;

  // frame data interface
  uint8_t frame_data[256*240];
  ppu_update_frame_func_t update_frame;
} ppu_t;

void ppu_init(ppu_t *ppu, nes_t *nes, ppu_read_func_t read, ppu_write_func_t write, ppu_update_frame_func_t update_frame);

void ppu_reset(ppu_t *ppu);

int ppu_read(ppu_t *ppu, int adr, uint64_t cycle_number); // read data [8bit] from address [16bit]
void ppu_write(ppu_t *ppu, int adr, int value, uint64_t cycle_number); // write data [8bit] to address [16bit]

int ppu_update(ppu_t *ppu); // update ppu to current cpu cycle, return number of cpu cycles to next frame

#endif
