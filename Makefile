# Makefile for standalone building of Daniels example
# Double-check RETRO68 variable below and run 'make'.

# path to RETRO68
RETRO68=../../../Retro68-build/toolchain

PREFIX=$(RETRO68)/m68k-unknown-elf
CC=$(RETRO68)/bin/m68k-unknown-elf-gcc
CXX=$(RETRO68)/bin/m68k-unknown-elf-g++
REZ=$(RETRO68)/bin/Rez
MAKEAPPL=$(RETRO68)/bin/MakeAPPL

CXXFLAGS=-std=c++11
LDFLAGS=-Wl,-gc-sections -lm
RINCLUDES=$(PREFIX)/RIncludes
REZFLAGS=-I$(RINCLUDES)

all: Daniels.bin
.PHONY: all clean

clean:
	rm -f Daniels.bin Daniels.APPL Daniels.dsk Daniels.flt Daniels.flt.gdb
	rm -f FixedBenchmark.bin FixedBenchmark.APPL FixedBenchmark.dsk FixedBenchmark.flt FixedBenchmark.flt.gdb
	rm -f fixed.o main.o main2.o fixedbenchmark.o

Daniels.bin Daniels.APPL Daniels.dsk: Daniels.flt
	$(REZ) $(REZFLAGS) \
		-DFLT_FILE_NAME="\"Daniels.flt\"" "$(RINCLUDES)/Retro68APPL.r" \
		-t "APPL" -c "????" \
		-o Daniels.bin --cc Daniels.APPL --cc Daniels.dsk

Daniels.flt: main.o
	$(CC) $^ -o $@ $(LDFLAGS)
