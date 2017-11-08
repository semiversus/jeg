#ifndef CATRIDGE_H
#define CATRIDGE_H

#include <stdint.h>

typedef struct catridge_t {
  int prg_size;
  uint8_t *prg_memory;
  int chr_size;
  uint8_t *chr_memory;
  int mapper;
  int mirror;
  int battery;
} catridge_t;

void catridge_load(catrdige_t *catridge, char *filename);

// access cpu memory bus
void catridge_read_prg(catridge_t *catridge, int adr);
void catridge_write_prg(catridge_t *catridge, int adr, int value);

// access ppu memory bus
void catridge_read_chr(catridge_t *catridge, int adr);
void catridge_write_chr(catridge_t *catridge, int adr, int value);

void catridge_free(catridge_t *catridge);

#endif
