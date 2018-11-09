#include <stdio.h>
#include <stdlib.h>
#include "ppu_framebuffer.h"
#include "cartridge.h"
#include "nes.h"

int main(int argc, char* argv[]) {
  int i, result;
  ppu_t ppu;
  cartridge_t cartridge;
  nes_t nes_console;
  FILE *rom_file;
  uint8_t *rom_data;
  uint32_t rom_size;
  uint8_t video_frame_data[256*240];

  // load rom file
  if (argc<2) {
    printf("need rom file as argument\n");
    return 1;
  }

  rom_file=fopen(argv[1], "rb");

  if (rom_file==NULL) {
    printf("not able to open rom file %s\n", argv[1]);
    return 2;
  }

  fseek(rom_file, 0, SEEK_END);
  rom_size=ftell(rom_file);
  fseek(rom_file, 0, SEEK_SET);
  rom_data=malloc(rom_size);
  if (fread(rom_data, 1, rom_size, rom_file)!=rom_size) {
    printf("unable to read data from rom file %s\n", argv[1]);
    return 3;
  }
  fclose(rom_file);

  // init nes
  ppu_init(&nes_console, &ppu, video_frame_data);
  result = cartridge_init(&nes_console, &cartridge, rom_data, rom_size);  
  nes_init(&nes_console);
  if (result) {
    printf("unable to parse rom file (result:%d)\n", result);
    return 6;
  }

  for (i=0; i<1000; i++) {
    nes_iterate_frame(&nes_console);    
  }

  free(rom_data);
  
  return 0;
}

