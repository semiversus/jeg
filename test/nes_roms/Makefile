NES_SRC_PATH=../../src/

INCLUDE_PATHS_NES=cartridge cpu ppu .
SRCS_NES=cartridge/cartridge.c cpu/cpu6502.c ppu/ppu_framebuffer.c nes.c

# target specific
SRCS=$(addprefix $(NES_SRC_PATH), $(SRCS_NES)) test_roms.c
INCLUDE_PATHS=$(addprefix $(NES_SRC_PATH), $(INCLUDE_PATHS_NES))

run: test_roms_bin
	./test_roms_bin -p test.key

test_roms_bin: $(SRCS)
	$(CC) $(SRCS) $(addprefix -I,$(INCLUDE_PATHS)) `sdl-config --cflags --libs` -o $@ -O3 -DWITHOUT_DECIMAL_MODE

clean:
	rm test_roms_bin -rf