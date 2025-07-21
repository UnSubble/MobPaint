# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -Iinclude
LDFLAGS = -lSDL2 -lpthread -lcjson -lSDL2_image

# Source and output
SRC = src/*.c src/tools/*.c src/context/*.c
OUTDIR = out
OUT = $(OUTDIR)/mobpaint

.PHONY: all clean

all: $(OUT)

$(OUT): $(SRC) | $(OUTDIR)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

$(OUTDIR):
	mkdir -p $(OUTDIR)

clean:
	rm -rf $(OUTDIR)
