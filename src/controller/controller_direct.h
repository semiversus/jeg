#ifndef CONTROLLER_DIRECT_H
#define CONTROLLER_DIRECT_H

#include "nes.h"

typedef struct controller_direct_t {
    uint8_t data[2];
    uint8_t shift_reg[2];
} controller_direct_t;

void controller_direct_init(nes_t *nes, controller_direct_t *controller);
void controller_direct_set(nes_t *nes, uint8_t controller1, uint8_t controller2);

#endif