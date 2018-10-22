NES_SRC_PATH=../../src/

INCLUDE_PATHS_NES=cartridge cpu ppu .
SRCS_NES=cartridge/cartridge.c cpu/cpu6502.c ppu/ppu.c nes.c

# target specific
SRCS=$(addprefix $(NES_SRC_PATH), $(SRCS_NES)) benchmark.c
INCLUDE_PATHS=$(addprefix $(NES_SRC_PATH), $(INCLUDE_PATHS_NES))

benchmark: $(SRCS)
	$(CC) $(SRCS) $(addprefix -I,$(INCLUDE_PATHS)) -O3 -o $@ -Wall -pedantic -DWITHOUT_DECIMAL_MODE $(CFLAGS)

clean:
	rm benchmark -rf