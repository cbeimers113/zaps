SRCS = main.c $(wildcard src/*/*.c)
INCLUDES = $(addprefix -I,$(wildcard src/*/))

CFLAGS = -std=c11 -Wall -Wextra $(INCLUDES) $(shell pkg-config --cflags sdl2)
LDLIBS = $(shell pkg-config --libs sdl2)

.PHONY: zaps run clean

zaps: $(SRCS)
	$(CC) $(CFLAGS) $(EXTRA) $(SRCS) -o $@ $(LDLIBS)

run: zaps
	./zaps

clean:
	rm -f zaps

glyphs:
	python3 src/gui/tools/glyphs.py src/gui/tools/glyphs.txt --font /usr/share/fonts/TTF/JetBrainsMono-Regular.ttf --output-dir src/gui/assets/ --header src/gui/glyph.h