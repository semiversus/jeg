CPU_SRC_PATH=../../src/cpu/

INCLUDE_PATHS_CPU=. ..
SRCS_CPU=cpu6502_opcodes.c cpu6502.c

# target specific
SRCS=$(addprefix $(CPU_SRC_PATH), $(SRCS_CPU)) system.c
INCLUDE_PATHS=$(addprefix $(CPU_SRC_PATH), $(INCLUDE_PATHS_CPU))

run: klaus2m5_bin
	./klaus2m5_bin

klaus2m5_bin: $(SRCS)
	cat 6502_functional_test.bin|xxd -i >rom.inc
	$(CC) $(SRCS) $(addprefix -I,$(INCLUDE_PATHS)) -o $@ -O3 -DJEG_USE_6502_DECIMAL_MODE=1

clean:
	rm klaus2m5_bin rom.inc -rf

.PHONY: run clean
