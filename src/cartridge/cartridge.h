#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <stdint.h>
#include "jeg_cfg.h"

struct nes_t; 

typedef enum {
    err_imcomplete_rom      = -7,
    err_unsupported_mapper  = -6,
    err_invalid_rom         = -5,
    err_illegal_size        = -4,
    err_illegal_parameter   = -3,
    err_illegal_pointer     = -2,
    err_unknown             = -1,
    ok                      = 0,
} cartridge_err_t;

//! \name iNES header 
//! @{
typedef struct {
    uint8_t chSign[4];                                                          //!< should be "NES\x1A"
    
    uint8_t chPRGROMBankCount;                                                  //!< number of 16KB PRG-ROM banks
    uint8_t chCHRROMBankCount;                                                  //!< number of 8KB CHR-ROM / VROM banks
    
    union {
        struct {
            uint8_t    VerticalMirroring           : 1;                         //!< 0 : Horizontal mirroring 1: Vertical mirroring
            uint8_t    BatteryBackedRAM            : 1;                         //!< presence of battery-backed RAM @ 0x6000-0x7FFF
            uint8_t    Trainer                     : 1;                         //!< presence of a 512-byte trainer @ 0x7000-0x71FF
            uint8_t    FourScreenMirroring         : 1;                         //!< 1 : overrides bit 0 to indicate four-screen mirroring should be used
            uint8_t    MapperLow                   : 4;                         //!< four lower bits of the mapper number
        };
        uint8_t chROMCTRL1;                                                     //!< ROM control 1
    };
    
    union {
        struct {
            union {
                struct {
                    uint8_t                         : 4;
                    uint8_t    MapperHigh           : 4;                        //!< four upper bits of the mapper number
                };
                uint8_t chROMCTRL2;                                             //!< ROM control 2
            };
            
            uint8_t ch8KRAMBankCount;                                           //!< number of 8 KB RAM Banks, assume 1 page of RAM when this is zero.
            uint8_t                                 : 8;
            uint8_t                                 : 8;
            uint8_t                                 : 8;
            uint8_t                                 : 8;
            uint8_t                                 : 8;
            uint8_t                                 : 8;
            uint8_t                                 : 8;
        };
        
        uint8_t chDiskDudeString[9];                                            //!< "DiskDude!"                                               
    };
}iNES_t;
//! @}

typedef struct {
  uint_fast32_t     wPRGAddressMask;
  uint8_t          *pchPRGMemory;
  uint_fast32_t     wCHRAddressMask;
  uint8_t          *pchCHRMemory;
  uint8_t           chIOData  [0x2000];
  uint8_t           chCHRData [0x3000];
  uint_fast8_t      chMapper;
  uint_fast8_t      chMirror;                                                   //!< 0-horizontal, 1-vertical, 2-none
} cartridge_t;

extern cartridge_err_t cartridge_init(struct nes_t *nes, cartridge_t *cartridge, uint8_t *rom_image, uint_fast32_t size);

#endif
