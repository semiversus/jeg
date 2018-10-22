DIRS=platform/windows platform/linux test/benchmark test/klaus2m5 test/nes_roms

MAKEFLAGS=-s

all: $(DIRS)

$(DIRS):
	$(MAKE) -C $@

clean:
	for path in $(DIRS); do make -C $$path clean; done

.PHONY: $(DIRS)
