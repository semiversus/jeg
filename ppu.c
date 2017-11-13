#include "ppu.h"
#include "nes.h"
#include <stdio.h>

void ppu_init(ppu_t *ppu, nes_t *nes, ppu_read_func_t read, ppu_write_func_t write, ppu_update_frame_func_t update_frame) {
  ppu_reset(ppu);
  ppu->nes=nes;
  ppu->read=read;
  ppu->write=write;
  ppu->update_frame=update_frame;
}

void ppu_reset(ppu_t *ppu) {
  ppu->cycle=340;
  ppu->scanline=240;
  ppu->ppuctrl=0;
  ppu->nmi_output=0;
  ppu->nmi_previous=0;
  ppu->t=0;
  ppu->ppumask=0;
  ppu->oam_address=0;
}

#define NMI_CHANGE() do { \
    if (ppu->nmi_output && (ppu->ppustatus&0x80) && !ppu->nmi_previous) { \
      ppu->nmi_delay=15; \
    } \
    ppu->nmi_previous=(ppu->nmi_output && (ppu->ppustatus&0x80)); \
  } while(0)

int ppu_read(ppu_t *ppu, int adr, uint64_t cycle_number) {
  int value, buffered;

  switch (adr) {
    case 0x2002:
      value=(ppu->register_data&0x1F)|(ppu->ppustatus&0xE0);
      ppu->ppustatus&=0x7F; // disable NMI occurred
      NMI_CHANGE();
      ppu->w=0;
      return value;
    case 0x2004:
      return ppu->oam_data[ppu->oam_address];
    case 0x2007:
      value=ppu->read(ppu->nes->reference, ppu->v);
      if (ppu->v%0x4000<0x3F00) {
        buffered=ppu->buffered_data;
        ppu->buffered_data=value;
        value=buffered;
      }
      else {
        ppu->buffered_data=ppu->read(ppu->nes->reference, ppu->v - 0x1000);
      }
      ppu->v+=(ppu->ppuctrl&0x04==0)?1:32;
      return value;
    default:
      // TODO: should not happen
      return 0;
  }
}

void ppu_write(ppu_t *ppu, int adr, int value, uint64_t cycle_number) {
  int address_temp;
  printf("%04X %d (%ld)\n", adr, value, cycle_number);

  ppu->register_data=value;

  switch (adr) {
    case 0x2000:
      ppu->ppuctrl=value;
      ppu->nmi_output=(value&0x80)?1:0;
      NMI_CHANGE();
      ppu->t=(ppu->t&0xF3FF)|((value&0x03)<<10);
      break;
    case 0x2001:
      ppu->ppumask=value;
      break;
    case 0x2003:
      ppu->oam_address=value;
      break;
    case 0x2004:
      ppu->oam_data[ppu->oam_address]=value;
      ppu->oam_address++;
      break;
    case 0x2005:
      if (!ppu->w) {
        ppu->t=(ppu->t&0xFFE0)|(value>>3);
        ppu->x=value&0x07;
        ppu->w=1;
      }
      else {
        ppu->t=(ppu->t&0x8FFF)|((value&0x07)<<12);
        ppu->t=(ppu->t&0xFC1F)|((value&0xF8)<<2);
        ppu->w=0;
      }
      break;
    case 0x2006:
      if (!ppu->w) {
        ppu->t=(ppu->t&0x80FF)|((value&0x3F)<<8);
        ppu->w=1;
      }
      else {
        ppu->t=(ppu->t&0xFF00)|value;
        ppu->v=ppu->t;
        ppu->w=0;
      }
      break;
    case 0x2007:
      ppu->write(ppu->nes->reference, ppu->v, value);
      ppu->v+=(ppu->ppuctrl&0x04==0)?1:32;
      break;
    case 0x4014:
      address_temp=value<<8;
      for(int i=0; i<256; i++) {
        ppu->oam_data[ppu->oam_address]=ppu->nes->cpu.read(ppu->nes->reference, address_temp);
        ppu->oam_address++;
        address_temp++;
      }
      ppu->nes->cpu.stall_cycles+=513;
      if (ppu->nes->cpu.cycle_number%2) {
        ppu->nes->cpu.stall_cycles++;
      }
      break;
  }
       
}

int ppu_update(ppu_t *ppu) {
  return 0;
}
