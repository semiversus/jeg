#ifndef NES_H
#define NES_H

#include "cpu6502.h"
#include "cartridge.h"
#include <stdbool.h>
#include <stdint.h>
#include "jeg_cfg.h"

struct nes_t;

typedef struct nes_t {
  cpu6502_t cpu;
  struct {
    uint_fast8_t (*read) (struct nes_t *, uint_fast16_t);
    void (*write) (struct nes_t *, uint_fast16_t, uint_fast8_t);
    uint_fast32_t (*update) (struct nes_t *);
    void (*reset) (struct nes_t *);
    void *internal;
  } ppu;
  cartridge_t cartridge;
  uint8_t controller_data[2];
  uint8_t controller_shift_reg[2];
  uint8_t ram_data[0x800];
} nes_t;

extern void nes_init(nes_t *ptNES);

extern nes_err_t nes_setup_rom(nes_t *, uint8_t *, uint_fast32_t );

extern void nes_reset(nes_t *);

extern void nes_iterate_frame(nes_t *); // run cpu until next complete frame
extern void nes_set_controller(nes_t *, uint8_t , uint8_t ); // [7:Right, 6:Left, 5:Down, 4:Up, 3:Start, 2:Select, 1:B, 0:A]
extern bool nes_is_ready_to_refresh(nes_t *nes);

#endif
