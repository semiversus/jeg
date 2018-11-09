#include "nes.h"
#include <string.h>
#include <stdbool.h>
#include "jeg_cfg.h"


static uint_fast16_t cpu6502_bus_read (void *ref, uint_fast16_t address) 
{
    nes_t* nes=(nes_t *)ref;

    if (address<0x2000) {
        return *(uint16_t*)&nes->ram_data[address & 0x7FF];
        
    } else if (address>=0x6000) {
        return nes->cartridge.read_prg(nes->cartridge.internal, address);
        
    } else if (address<0x4000) {
        return nes->ppu.read(nes, address);
        
    } else if (address==0x4016 || address==0x4017) {
        return nes->controller.read(nes, address-0x4016);
    }
    return 0;
}

static void cpu6502_bus_write (void *ref, uint_fast16_t address, uint_fast8_t value) 
{
    nes_t* nes=(nes_t *)ref;

    if (address<0x2000) {
        nes->ram_data[address & 0x7FF] = value;
        
    } else if (address<0x4000) {
        nes->ppu.write(nes, address, value);
        
    } else if (address==0x4014) {
        nes->ppu.write_dma(nes, &(nes->ram_data[value<<8]) );
        nes->cpu.stall_cycles += 513;
        if (nes->cpu.cycle_number & 0x01) {
            nes->cpu.stall_cycles++;
        }
        
    } else if (address==0x4016) {
        nes->controller.write(nes, value);
        
    } else if (address>=0x6000) {
        nes->cartridge.write_prg(nes->cartridge.internal, address, value);
    } 
}

#if JEG_USE_EXTERNAL_DRAW_PIXEL_INTERFACE == ENABLED
bool nes_init(nes_t *ptNES, nes_cfg_t *ptCFG) 
#else
void nes_init(nes_t *ptNES)
#endif
{ 
 #if JEG_USE_EXTERNAL_DRAW_PIXEL_INTERFACE == ENABLED
 bool bResult = false;
 #endif
    do {
    #if JEG_USE_EXTERNAL_DRAW_PIXEL_INTERFACE == ENABLED
        
        if (    NULL == ptNES 
            ||  NULL == ptCFG) {
            break;
        } else if (NULL == ptCFG->fnDrawPixel) {
            break;
        }
    #else
        if ( NULL == ptNES ) {
            break;
        }
    #endif

    cpu6502_init(&ptNES->cpu, ptNES, &cpu6502_bus_read, &cpu6502_bus_write);
    
    #if JEG_USE_EXTERNAL_DRAW_PIXEL_INTERFACE == ENABLED
        {
            ppu_cfg_t tCFG = {
                ptNES,
                ppu_bus_read,
                ppu_bus_write,
                ptCFG->fnDrawPixel,
                ptCFG->ptTag,
            };
            if (ppu_init(&ptNES->ppu, &tCFG)) {
                break;
            }
        }
        bResult = true;
    #endif
        
    } while(false);
    nes_reset(ptNES);
#if JEG_USE_EXTERNAL_DRAW_PIXEL_INTERFACE == ENABLED
    return bResult;
#endif
}

void nes_reset(nes_t *nes)
{
    cpu6502_reset(&nes->cpu);
    nes->ppu.reset(nes);
    memset(&nes->ram_data, 0, 0x800);
}

void nes_iterate_frame(nes_t *nes) 
{
    cpu6502_run(&nes->cpu, nes->ppu.update(nes));
}
