#include "nes.h"
#include <string.h>

static int cpu6502_bus_read (void *nes, int address) {
  int value;

  if (address<0x2000) {
    return ((nes_t*)nes)->ram_data[address%0x800];
  }
  else if (address<0x4000) {
    return ppu_read(&((nes_t*)nes)->ppu, 0x2000+address%8, ((nes_t*)nes)->cpu.cycle_number);
  }
  else if (address==0x4014) {
    return ppu_read(&((nes_t*)nes)->ppu, address, ((nes_t*)nes)->cpu.cycle_number);
  }
  else if (address==0x4016) {
    value=((nes_t*)nes)->controller_data[0]&1;
    ((nes_t*)nes)->controller_data[0]=(((nes_t*)nes)->controller_data[0]>>1)|0x80;
    return value;
  }
  else if (address==0x4017) {
    value=((nes_t*)nes)->controller_data[1]&1;
    ((nes_t*)nes)->controller_data[1]=(((nes_t*)nes)->controller_data[1]>>1)|0x80;
    return value;
  }
  else if (address>=0x6000) {
    return cartridge_read_prg(&((nes_t*)nes)->cartridge, address);
  }
  else {
    // TODO: log this event
    return 0;
  }
}

static void cpu6502_bus_write (void *nes, int address, int value) {
  if (address<0x2000) {
    ((nes_t*)nes)->ram_data[address%0x800]=value;
  }
  else if (address<0x4000) {
    ppu_write(&((nes_t*)nes)->ppu, 0x2000+address%8, value, ((nes_t*)nes)->cpu.cycle_number);
  }
  else if (address==0x4014) {
    ppu_write(&((nes_t*)nes)->ppu, address, value, ((nes_t*)nes)->cpu.cycle_number);
  }
  else if (address==0x4016 && value&0x01) {
    value=((nes_t*)nes)->controller_read();
    ((nes_t*)nes)->controller_data[0]=value&0xFF;
    ((nes_t*)nes)->controller_data[1]=(value>>8)&0xFF;
  }
  else if (address>=0x6000) {
    cartridge_write_prg(&((nes_t*)nes)->cartridge, address, value);
  }
  else {
    // TODO: log this event
  }
}

static int ppu_bus_read (void *nes, int address) {
}

static void ppu_bus_write (void *nes, int address, int value) {
}

void nes_init(nes_t *nes, ppu_update_frame_func_t update_frame, controller_read_func_t controller_read) {
  nes_reset(nes);
  nes->controller_read=controller_read;
  cpu6502_init(&nes->cpu, nes, cpu6502_bus_read, cpu6502_bus_write);
  ppu_init(&nes->ppu, nes, ppu_bus_read, ppu_bus_write, update_frame);
}

void nes_reset(nes_t *nes) {
  cpu6502_reset(&nes->cpu);
  ppu_reset(&nes->ppu);
  memset(&nes->ram_data, 0, 0x800);
}

void nes_load_rom(nes_t *nes, char *filename);

void nes_iterate_frame(nes_t *nes); // run cpu until next complete frame

void nes_store_state(nes_t *nes, uint8_t *data); // stores state to given array
void nes_load_state(nes_t *nes, uint8_t *data); // load state to given array

