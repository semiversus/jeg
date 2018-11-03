#include "nes.h"
#include "cpu6502_debug.h"
#include <string.h>
#include <stdbool.h>
#include "jeg_cfg.h"


static uint_fast8_t cpu6502_bus_read (void *ref, uint_fast16_t address) 
{
    nes_t* nes=(nes_t *)ref;

    if (address<0x2000) {
        return nes->ram_data[address & 0x7FF];
        
    } else if (address>=0x6000) {
        return cartridge_read_prg(&nes->cartridge, address);
        
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
        cartridge_write_prg(&nes->cartridge, address, value);
    } 
}

#if JEG_USE_EXTRA_16BIT_BUS_ACCESS == ENABLED
static uint_fast16_t cpu6502_bus_readw (void *ref, uint_fast16_t hwAddress) 
{
    nes_t* nes=(nes_t *)ref;

    if ( hwAddress<0x2000 ) {
        return *(uint16_t *)&(nes->ram_data[hwAddress & 0x7FF]);
    } else  if (hwAddress>=0x6000) {
        return cartridge_readw_prg(&nes->cartridge, hwAddress);
    } else {
        return cpu6502_bus_read(ref, hwAddress) | (cpu6502_bus_read(ref, hwAddress + 1) << 8);
    }
}

static void cpu6502_bus_writew (void *ref, uint_fast16_t hwAddress, uint_fast16_t hwValue) 
{
    // it is not used...
}
#endif

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
    #if JEG_USE_EXTRA_16BIT_BUS_ACCESS == ENABLED
        {
            cpu6502_cfg_t tCFG = {
                ptNES,
                &cpu6502_bus_read,
                &cpu6502_bus_write,
            #if JEG_USE_EXTRA_16BIT_BUS_ACCESS == ENABLED
                &cpu6502_bus_readw,
                &cpu6502_bus_writew,
            #endif
            };
            if (! cpu6502_init(&ptNES->cpu, &tCFG)) {
                break;
            }
        } 
    #else
        cpu6502_init(&ptNES->cpu, ptNES, &cpu6502_bus_read, &cpu6502_bus_write);
    #endif
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
    
#if JEG_USE_EXTERNAL_DRAW_PIXEL_INTERFACE == ENABLED
    return bResult;
#endif
}

nes_err_t nes_setup_rom(nes_t *ptNES, uint8_t *pchData, uint_fast32_t wSize) 
{
    nes_err_t tResult = nes_err_illegal_pointer;
    
    do {
        if (NULL == ptNES || NULL == pchData) {
            break;
        }

        tResult = cartridge_setup(&(ptNES->cartridge), pchData, wSize);
        if (nes_ok == tResult) {
            nes_reset(ptNES);
        }
    } while(0);
    
    return tResult;
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
