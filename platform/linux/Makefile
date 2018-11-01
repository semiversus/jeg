NES_SRC_PATH=../../src/

INCLUDE_PATHS_NES=cartridge cpu ppu controller .
SRCS_NES=cartridge/cartridge.c cpu/cpu6502.c ppu/ppu_framebuffer.c nes.c controller/controller_direct.c

# target specific
SRCS=$(addprefix $(NES_SRC_PATH), $(SRCS_NES)) ui.c
INCLUDE_PATHS=$(addprefix $(NES_SRC_PATH), $(INCLUDE_PATHS_NES))

jeg: $(SRCS)
	$(CC) $(SRCS) $(addprefix -I,$(INCLUDE_PATHS)) -O3 -o $@ `sdl2-config --cflags --libs` -Wall -pedantic $(CFLAGS)

clean:
	rm jeg -rf
