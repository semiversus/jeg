.PHONY: all clean

SRCS_CPU6502=cpu6502.c cpu6502_opcodes.c cpu6502_debug.c
SRCS_NES=$(SRCS_CPU6502) ppu.c nes.c cartridge.c
SRCS_UI=$(SRCS_NES) ui.c

all: jeg test_klaus2m5

jeg: $(SRCS_UI)
	$(CC) $(SRCS_UI) -I. -O3 -o $@ `sdl-config --cflags --libs` -Wall -pedantic

test_klaus2m5: $(SRCS_CPU6502)
	cat test/klaus2m5/6502_functional_test.bin|xxd -i >test/klaus2m5/rom.inc
	$(CC) $(SRCS_CPU6502) -I. -Itest/klaus2m5 test/klaus2m5/system.c -o test_klaus2m5 -O3

test: test_klaus2m5
	./test_klaus2m5 test/klaus2m5/6502_functional_test.bin

clean:
	rm jeg test_klaus2m5 test/klaus2m5/rom.inc -rf
