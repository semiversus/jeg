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

int ppu_read(ppu_t *ppu, int adr, uint64_t cycle_number) {
  return 0;
}

#define NMI_CHANGE() do { \
    if (ppu->nmi_output && ppu->nmi_occurred && !ppu->nmi_previous) { \
      ppu->nmi_delay=15; \
    } \
    ppu->nmi_previous=(ppu->nmi_output && ppu->nmi_occurred); \
  } while(0)

void ppu_write(ppu_t *ppu, int adr, int value, uint64_t cycle_number) {
  int address_temp;
  printf("%04X %d (%ld)\n", adr, value, cycle_number);
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
      ppu->write(ppu->reference, ppu->v, value);
      ppu->v+=(ppu->ppuctrl&0x04==0)?1:32;
      break;
    case 0x2014:
      address_temp=value<<8;
      for(int i=0; i<256; i++) {
        ppu->oam_data[ppu->oam_address]=ppu->nes->cpu.read(ppu->nes->reference, address_temp);
        ppu->oam_address++;
        address_temp++;
      }
      break;
  }
       
}

int ppu_run(ppu_t *ppu, int n_cycles) {
  return 0;
}
