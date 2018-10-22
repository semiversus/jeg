NES_SRC_PATH=../../src/

INCLUDE_PATHS_NES=cartridge cpu ppu .
SRCS_NES=cartridge/cartridge.c cpu/cpu6502_opcodes.c cpu/cpu6502.c ppu/ppu.c nes.c

# target specific
SRCS=$(addprefix $(NES_SRC_PATH), $(SRCS_NES)) ../linux/ui.c
INCLUDE_PATHS=$(addprefix $(NES_SRC_PATH), $(INCLUDE_PATHS_NES)) include/SDL

jeg.exe: $(SRCS)
	i686-w64-mingw32-gcc $(SRCS) $(addprefix -I,$(INCLUDE_PATHS)) -mwindows -O3 -o $@ -D_GNU_SOURCE=1 -D_REENTRANT -Llibs -lmingw32 -lSDL2main -lSDL2 -Wall -pedantic -DWITHOUT_DECIMAL_MODE $(CFLAGS)

clean:
	rm jeg.exe -rf