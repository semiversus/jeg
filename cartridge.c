#include "cartridge.h"
#include <stdio.h>

int cartridge_setup(cartridge_t *cartridge, uint8_t *data, uint32_t size) {
  // check minimum size (header is 16 bytes)
  if (size<16) {
    return 1;
  }

  // check header to be "NES\x1A"
  if (data[0]!='N' || data[1]!='E' || data[2]!='S' || data[3]!=0x1A) {
    return 2;
  }

  cartridge->prg_size=16384*data[4];
  cartridge->chr_size=8192*data[5];
  cartridge->mirror=(data[6]&0x01)+((data[6]>>2)&0x02);
  cartridge->mapper=(data[6]>>4)+(data[7]&0xF0);

  if (size<cartridge->prg_size+cartridge->chr_size+16+(data[6]&0x04?512:0)) {
    return 3;
  }

  cartridge->prg_memory=data+(data[6]&0x04?512:0)+16; // skip header and trainer data
  cartridge->chr_memory=cartridge->prg_memory+cartridge->prg_size;

  return 0;
}


// access cpu memory bus
int cartridge_read_prg(cartridge_t *cartridge, int adr) {
  return cartridge->prg_memory[(adr-0x8000)%cartridge->prg_size];
}

void cartridge_write_prg(cartridge_t *cartridge, int adr, int value) {
  cartridge->prg_memory[adr-0x8000]=value;
}

// access ppu memory bus
int cartridge_read_chr(cartridge_t *cartridge, int adr) {
  return cartridge->chr_memory[adr];
}

void cartridge_write_chr(cartridge_t *cartridge, int adr, int value) {
  cartridge->chr_memory[adr]=value;
}
