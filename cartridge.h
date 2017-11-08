#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <stdint.h>

typedef struct cartridge_t {
  int prg_size;
  uint8_t *prg_memory;
  int chr_size;
  uint8_t *chr_memory;
  int mapper;
  int mirror;
  int battery;
} cartridge_t;

void cartridge_load(cartridge_t *cartridge, char *filename);

// access cpu memory bus
int cartridge_read_prg(cartridge_t *cartridge, int adr);
void cartridge_write_prg(cartridge_t *cartridge, int adr, int value);

// access ppu memory bus
int cartridge_read_chr(cartridge_t *cartridge, int adr);
void cartridge_write_chr(cartridge_t *cartridge, int adr, int value);

void cartridge_free(cartridge_t *cartridge);

#endif
