#ifndef NES_H
#define NES_H

#include <stdint.h>

#include "ppu.h"
#include "cpu6502.h"
#include "cartridge.h"

typedef uint16_t (*controller_read_func_t) (void *reference); // read controller data [controller1, controller2] with [Right, Left, Down, Up, Start, Select, B, A]

typedef struct nes_t {
  void *reference; // pointer to reference added as argument to all callbacks
  cpu6502_t cpu;
  ppu_t ppu;
  cartridge_t cartridge;
  controller_read_func_t controller_read;
  uint8_t controller_data[2];
  int ram_data[0x800];
} nes_t;

void nes_init(nes_t *nes, void *reference, ppu_update_frame_func_t update_frame, controller_read_func_t controller_read);
int nes_setup_cartridge(nes_t *nes, uint8_t *data, uint32_t size);
void nes_reset(nes_t *nes);

void nes_iterate_frame(nes_t *nes); // run cpu until next complete frame

void nes_store_state(nes_t *nes, uint8_t *data); // stores state to given array
void nes_load_state(nes_t *nes, uint8_t *data); // load state to given array

#endif
