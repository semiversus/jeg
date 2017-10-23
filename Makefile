.PHONY: all clean

SRCS_CPU6502=cpu6502.c cpu6502_opcodes.c cpu6502_debug.c

SRC_NES=ppu.c

all: jeg_nes test_klaus2m5

test_klaus2m5:
	cat test/klaus2m5/6502_functional_test.bin|xxd -i >test/klaus2m5/rom.inc
	$(CC) $(SRCS_CPU6502) -I. -Itest/klaus2m5 test/klaus2m5/system.c -o test_klaus2m5 -O3

test: test_klaus2m5
	./test_klaus2m5 test/klaus2m5/6502_functional_test.bin

clean:
	rm test_klaus2m5 test/klaus2m5/rom.inc -rf
