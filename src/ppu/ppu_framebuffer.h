#ifndef PPU_FRAMEBUFFER_H
#define PPU_FRAMEBUFFER_H

#include <stdint.h>
#include <stdbool.h>

#include "nes.h"

typedef union {
    uint8_t chValue;
    struct {
        uint8_t Low     : 4;
        uint8_t High    : 4;
    };
} compact_dual_pixels_t;

typedef uint8_t nes_screen_buffer_t[240][256];


typedef struct {
    uint_fast16_t   XScroll     : 5;
    uint_fast16_t   YScroll     : 5;
    uint_fast16_t   XToggleBit  : 1;
    uint_fast16_t   YToggleBit  : 1;
    uint_fast16_t   TileYOffsite: 3;
} vram_addr_t;

typedef struct {
    union {
        struct {
            uint8_t chNameTable[30][32];
            union {
                struct {
                    uint8_t     Square0 : 2;
                    uint8_t     Square1 : 2;
                    uint8_t     Square2 : 2;
                    uint8_t     Square3 : 2;
                }Group;
                uint8_t chValue;
            }AttributeTable[8][8];
        };
        uint8_t chBuffer[1024];
    };
} name_attribute_table_t;

typedef union {
    struct {
        uint8_t chY;
        uint8_t chIndex;
        union {
            struct {
                uint8_t         ColorH              : 2;
                uint8_t                             : 3;
                uint8_t         Priority            : 1;
                uint8_t         IsFlipHorizontally  : 1;
                uint8_t         IsFlipVertically    : 1;
            };
            uint8_t chValue;
        }Attributes;
        uint8_t chPosition;
    };
    uint32_t wValue;
}sprite_t;

typedef union {
    uint8_t     chBuffer[256];
    sprite_t    SpriteInfo[64];
} sprite_table_t;

typedef struct ppu_t {
    // ppu state
    uint_fast64_t last_cycle_number; // measured in cpu cycles
    int_fast32_t cycle;
    uint_fast16_t scanline;

    uint_fast8_t palette[32];
    
    name_attribute_table_t tNameAttributeTable[2];

    sprite_table_t tSpriteTable;

    struct {
        struct {
            uint_fast8_t chIndex;
            uint_fast8_t chY;
        } List[64] ;

        uint_fast8_t chVisibleCount;
        uint_fast8_t chCurrent;
    } SpriteYOrderList;

    // ppu registers
    union {
        vram_addr_t tVAddress;
        uint_fast16_t v; // current vram address (15bit)
    };
    union {
        vram_addr_t tTempVAddress;
        uint_fast16_t t; // temporary vram address (15bit)
    };

    uint_fast8_t x; // fine x scoll (3bit)
    uint_fast8_t w; // toggle bit (1bit)
    uint_fast8_t f; // even/odd frame flag (1bit)

    uint_fast8_t register_data;

    // background temporary variables
    uint_fast8_t name_table_byte;
    uint_fast8_t attribute_table_byte;

    uint_fast8_t low_tile_byte;
    uint_fast8_t high_tile_byte;
    uint_fast64_t tile_data;

    // sprite temporary variables
    uint_fast8_t sprite_count;
    uint32_t sprite_patterns      [JEG_MAX_ALLOWED_SPRITES_ON_SINGLE_SCANLINE];
    uint_fast8_t sprite_positions [JEG_MAX_ALLOWED_SPRITES_ON_SINGLE_SCANLINE];
    uint_fast8_t sprite_priorities[JEG_MAX_ALLOWED_SPRITES_ON_SINGLE_SCANLINE];
    uint_fast8_t sprite_indicies  [JEG_MAX_ALLOWED_SPRITES_ON_SINGLE_SCANLINE];

    // memory accessable registers
    int ppuctrl;
    int ppumask;
    int ppustatus;
    int oam_address;
    int buffered_data;

    // frame data interface
    uint8_t *video_frame_data;
} ppu_t;

extern void ppu_init(nes_t *nes, ppu_t *ppu, uint8_t *video_data_frame);

#endif
