NES_SRC_PATH=../../src/

INCLUDE_PATHS_NES=cartridge cpu ppu .
SRCS_NES=cartridge/cartridge.c cpu/cpu6502_opcodes.c cpu/cpu6502.c ppu/ppu.c nes.c

# target specific
SRCS=$(addprefix $(NES_SRC_PATH), $(SRCS_NES)) ui.c
INCLUDE_PATHS=$(addprefix $(NES_SRC_PATH), $(INCLUDE_PATHS_NES))

jeg:
	$(CC) $(SRCS) $(addprefix -I,$(INCLUDE_PATHS)) -O3 -o $@ `sdl2-config --cflags --libs` -Wall -pedantic $(CFLAGS)

clean:
	rm jeg -rf

.PHONY: clean