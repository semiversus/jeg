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
    uint_fast8_t (*read) (struct nes_t *nes, uint_fast16_t address);
    void (*write) (struct nes_t *nes, uint_fast16_t address, uint_fast8_t data);
    void (*write_dma) (struct nes_t *nes, uint8_t *data);    
    uint_fast32_t (*update) (struct nes_t *nes);
    void (*reset) (struct nes_t *nes);
    void *internal;
  } ppu;
  
  cartridge_t cartridge;
  
  struct {
    uint_fast8_t  (*read) (struct nes_t *, uint_fast8_t controller);
    void (*write) (struct nes_t *nes, uint8_t data);
    void *internal;
  } controller;
  
  uint8_t ram_data[0x800];
} nes_t;

extern void nes_init(nes_t *ptNES);
extern void nes_reset(nes_t *);

extern nes_err_t nes_setup_rom(nes_t *, uint8_t *, uint_fast32_t );
extern void nes_iterate_frame(nes_t *); // run cpu until next complete frame

// interface used from cpu
extern void nes_cpu_bus_read(nes_t *, uint_fast16_t address);
extern void nes_cpu_bus_write(nes_t *, uint_fast16_t address, uint_fast8_t data);

#endif
