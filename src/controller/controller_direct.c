#include "nes.h"
#include "controller_direct.h"

static uint_fast8_t register_read (struct nes_t *nes, uint_fast8_t controller) {
    controller_direct_t *d = (controller_direct_t *)nes->controller.internal;
    uint8_t *shift_reg = &d->shift_reg[controller];
    uint8_t value=(*shift_reg)&1;
    
    *shift_reg =((*shift_reg)>>1)|0x80;
    
    return value;
}

static void register_write (struct nes_t *nes, uint_fast8_t data) {
    controller_direct_t *d = (controller_direct_t *)nes->controller.internal;
    
    if (data&0x01) {
        d->shift_reg[0]=d->data[0];
        d->shift_reg[1]=d->data[1];
    }
}

void controller_direct_init(nes_t *nes, controller_direct_t *controller_data) {
    nes->controller.internal = controller_data;
    nes->controller.read = register_read;
    nes->controller.write = register_write;
}
   
void controller_direct_set(nes_t *nes, uint8_t controller1, uint8_t controller2) {
    // [7:Right, 6:Left, 5:Down, 4:Up, 3:Start, 2:Select, 1:B, 0:A]
    controller_direct_t *d = (controller_direct_t *)nes->controller.internal;
    
    d->data[0] = controller1;
    d->data[1] = controller2;
}
