#include "nes.h"

void nes_init(nes_t *nes, ppu_update_frame_func_t update_frame, controller_read_func_t controller_read) {
  nes_reset(nes);
  nes->ppu.update_frame=update_frame;
  nes->controller_read=controller_read;
}

void nes_reset(nes_t *nes) {
  ppu_reset(&nes->ppu);
}

void nes_load_rom(nes_t *nes, char *filename);

void nes_iterate_frame(nes_t *nes); // run cpu until next complete frame

void nes_store_state(nes_t *nes, uint8_t *data); // stores state to given array
void nes_load_state(nes_t *nes, uint8_t *data); // load state to given array

