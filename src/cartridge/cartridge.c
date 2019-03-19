#include <string.h>
#include "cartridge.h"
#include "jeg_cfg.h"
#include "nes.h"
#include "mapper0.h"

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

cartridge_err_t cartridge_init(nes_t *nes, cartridge_t *cartridge, uint8_t *pchData, uint_fast32_t wSize) {
    nes->cartridge.internal = cartridge;

    iNES_t *ptHeader = (iNES_t *)pchData;
    uint_fast32_t wPRGSize, wCHRSize;

    if (NULL == cartridge || NULL == pchData) {
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

    cartridge->chMirror =     ptHeader->VerticalMirroring
                            +  (ptHeader->FourScreenMirroring << 1);

    if (strncmp("DiskDude!", (const char *)ptHeader->chDiskDudeString, 9)) {
        //! if "DiskDude!" string is present, ignore upper 4 bits for mapper
        cartridge->chMapper = ptHeader->MapperLow;
    } else {
        cartridge->chMapper = ptHeader->MapperLow | (ptHeader->MapperHigh << 4);
    }

    switch (cartridge->chMapper) {
        case 0:
        case 3:
            mapper0_init(nes);
            break;
        case 1:
            //mapper1_init(cartridge);
            break;
        default:
            return err_unsupported_mapper;
    }

    if (wSize < (    wPRGSize
                +    wCHRSize
                +    sizeof(iNES_t)                                             //!< should be 16 bytes
                +    (ptHeader->Trainer ? 512 : 0 ))) {
        return err_imcomplete_rom;
    }

    memset(cartridge->chIOData, 0, 0x2000);

    //! skip header and trainer data
    cartridge->pchPRGMemory =   pchData
                            +   sizeof(iNES_t)                                  //!< should be 16 bytes
                            +   (ptHeader->Trainer ? 512 : 0 );

    if (wCHRSize) {
        cartridge->pchCHRMemory = cartridge->pchPRGMemory + wPRGSize;

    } else {
        cartridge->pchCHRMemory = cartridge->chCHRData;
        wCHRSize = 0x2000;

        memset(cartridge->pchCHRMemory, 0, 0x2000);
    }

    //! generate mask
    cartridge->wCHRAddressMask = wCHRSize - 1;
    cartridge->wPRGAddressMask = wPRGSize - 1;

    memset(cartridge->chCHRData, 0, 0x3000);

    return ok;
}