#include "cartridge.h"

void cartridge_load(cartridge_t *cartridge, char *filename);

// access cpu memory bus
int cartridge_read_prg(cartridge_t *cartridge, int adr) {
}

void cartridge_write_prg(cartridge_t *cartridge, int adr, int value);

// access ppu memory bus
int cartridge_read_chr(cartridge_t *cartridge, int adr);
void cartridge_write_chr(cartridge_t *cartridge, int adr, int value);

void cartridge_free(cartridge_t *cartridge);
