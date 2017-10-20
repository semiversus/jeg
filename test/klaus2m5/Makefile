SRCS=system.c ../../src/cpu/cpu6502.c ../../src/cpu/cpu6502_opcodes.c
OBJS=$(SRCS:.c=.o)

CFLAGS=-Wall -pedantic -O3 -I../../src/cpu

test: test.bin
	@./test.bin

test.bin: $(OBJS)
	$(CC) $(OBJS) -o $@

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm test.bin $(OBJS)
