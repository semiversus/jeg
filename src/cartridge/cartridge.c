#include <string.h>
#include "cartridge.h"
#include "jeg_cfg.h"
#include "nes.h"

cartridge_err_t cartridge_load(cartridge_t *ptCartridge, uint8_t *pchData, uint_fast32_t wSize) {
    iNES_t *ptHeader = (iNES_t *)pchData;
    uint_fast32_t wPRGSize, wCHRSize;
    
    if (NULL == ptCartridge || NULL == pchData) {
        return err_illegal_pointer;
    }

    // check minimum size (header is 16 bytes)
    if (wSize < sizeof(iNES_t)) {
        return err_illegal_size;
    }

    if (strncmp("NES\x1A", (const char *)&ptHeader->chSign, 4)) {
        return err_invalid_rom;
    }

    wPRGSize = 0x4000 * ptHeader->chPRGROMBankCount;
    wCHRSize = 0x2000 * ptHeader->chCHRROMBankCount;

    ptCartridge->chMirror =     ptHeader->VerticalMirroring 
                            +  (ptHeader->FourScreenMirroring << 1);

    if (strncmp("DiskDude!", (const char *)ptHeader->chDiskDudeString, 9)) {
        //! if "DiskDude!" string is present, ignore upper 4 bits for mapper
        ptCartridge->chMapper = ptHeader->MapperLow;
    } else {
        ptCartridge->chMapper = ptHeader->MapperLow | (ptHeader->MapperHigh << 4);
    }


    if (ptCartridge->chMapper !=0 && ptCartridge->chMapper !=3 ) { // ines #3 is just a quick fix for a test rom
        return err_unsupported_mapper;
    }

    if (wSize < (    wPRGSize 
                +    wCHRSize 
                +    sizeof(iNES_t)                                             //!< should be 16 bytes
                +    (ptHeader->Trainer ? 512 : 0 ))) {
        return err_imcomplete_rom;
    }

    memset(ptCartridge->chIOData, 0, 0x2000);

    //! skip header and trainer data
    ptCartridge->pchPRGMemory =   pchData 
                            +   sizeof(iNES_t)                                  //!< should be 16 bytes
                            +   (ptHeader->Trainer ? 512 : 0 ); 
    
    if (wCHRSize) {
        ptCartridge->pchCHRMemory = ptCartridge->pchPRGMemory + wPRGSize;
        
    } else {
        ptCartridge->pchCHRMemory = ptCartridge->chCHRData;
        wCHRSize = 0x2000;

        memset(ptCartridge->pchCHRMemory, 0, 0x2000);
    }

    //! generate mask
    ptCartridge->wCHRAddressMask = wCHRSize - 1;
    ptCartridge->wPRGAddressMask = wPRGSize - 1;
    
    memset(ptCartridge->chCHRData, 0, 0x3000);
    
    return ok;
}

//! \brief access cpu memory bus
uint_fast16_t cartridge_read_prg(cartridge_t *cartridge, uint_fast16_t hwAddress) {

    if (hwAddress >= 0x8000) {
        return *(uint16_t*)&cartridge->pchPRGMemory[hwAddress & cartridge->wPRGAddressMask];
    }    
    
    return *(uint16_t*)&cartridge->chIOData[hwAddress & 0x1FFF];
}

void cartridge_write_prg(cartridge_t *cartridge, uint_fast16_t hwAddress, uint_fast8_t value) {

    if (hwAddress >= 0x8000) {
        cartridge->pchPRGMemory[ hwAddress & cartridge->wPRGAddressMask] = value;
    } else {
        cartridge->chIOData[hwAddress & 0x1fff] = value;
    }
  
}

//! \brief name table mirroring look up table
const static uint_fast8_t mirror_lookup[20] = {
    0,0,1,1,                //!< vertical mirroring
    0,1,0,1,                //!< horizontal mirroring
    0,0,0,0,                //!< single screen mirroring 0
    1,1,1,1,                //!< single screen mirroring 1
    0,1,2,3                 //!< Full/No mirroring
};


int mirror_address (int mode, int address) {
  address=address%0x1000;
  return mirror_lookup[mode*4+(address/0x400)]*0x400+(address%0x400);
}

//! \brief access ppu memory bus
uint_fast8_t cartridge_read_chr(cartridge_t *cartridge, uint_fast16_t hwAddress) {
    if (hwAddress <0x2000) {
        return cartridge->pchCHRMemory[hwAddress & cartridge->wCHRAddressMask];
    } else if (hwAddress<0x3F00) {
        return cartridge->chCHRData[0x2000+mirror_address(cartridge->chMirror, hwAddress)%2048];
    }
    // TODO: assert
    return 0;
}

void cartridge_write_chr(cartridge_t *cartridge, uint_fast16_t hwAddress, uint_fast8_t value) {
    if (hwAddress < 0x2000) {
        cartridge->pchCHRMemory[hwAddress & cartridge->wCHRAddressMask] = value;
    } else if (hwAddress<0x3F00) {
        cartridge->chCHRData[0x2000+mirror_address(cartridge->chMirror, hwAddress)%2048] = value;
    } 
}

cartridge_err_t cartridge_init(nes_t *nes, cartridge_t *cartridge, uint8_t *pchData, uint_fast32_t wSize) {
    nes->cartridge.internal = cartridge;
    nes->cartridge.read_prg = cartridge_read_prg;
    nes->cartridge.write_prg = cartridge_write_prg;
    nes->cartridge.read_chr = cartridge_read_chr;
    nes->cartridge.write_chr = cartridge_write_chr;
    return cartridge_load(cartridge, pchData, wSize);
}