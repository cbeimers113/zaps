SRCS = main.c $(wildcard src/*/*.c src/*/*/*.c)
INCLUDES = $(addprefix -I,$(wildcard src/*/ src/*/*/))

CFLAGS = -std=c11 $(INCLUDES) $(shell pkg-config --cflags sdl2)
LDLIBS = $(shell pkg-config --libs sdl2)

.PHONY: zaps run clean

zaps: $(SRCS)
	$(CC) $(CFLAGS) $(EXTRA) $(SRCS) -o $@ $(LDLIBS) -lm

run: zaps
	./zaps

clean:
	rm -f zaps

glyphs:
	python3 src/graphics/tools/glyphs.py src/graphics/tools/glyphs.txt --font /usr/share/fonts/TTF/JetBrainsMono-Regular.ttf --output-dir src/graphics/assets/ --header src/graphics/glyph.h