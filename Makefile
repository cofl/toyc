COMPILE=meson compile -C .build
.PHONY: all toyc clean

all: toyc
.build:
	@meson setup .build
toyc: .build
	$(COMPILE)
	@cp .build/src/toyc toyc
clean:
	$(COMPILE) --clean
	rm -f toyc
