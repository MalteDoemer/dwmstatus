PREFIX  := /usr/local
CC      := cc
CFLAGS  :=  -ggdb -pedantic -Wall -Wno-deprecated-declarations
LDFLAGS := -lX11

# FreeBSD (uncomment)
#LDFLAGS += -L/usr/local/lib -I/usr/local/include
# # OpenBSD (uncomment)
#LDFLAGS += -L/usr/X11R6/lib -I/usr/X11R6/include


BLOCKS=\
src/blocks/date.c\
src/blocks/disk.c\
src/blocks/ram.c\
src/blocks/cpu.c\
src/blocks/temp.c\
src/blocks/battery.c\
src/blocks/keyboard.c\
src/blocks/wifi.c\
src/blocks/volume_alsa.c\
src/blocks/powerprofile.c

SOURCES=\
src/dwmstatus.c\
src/util.c\
src/map.c

HEADERS=\
src/config.def.h\
src/config.h\
src/util.h\
src/blocks.h

all: options dwmstatus

options:
	@echo dwmstatus build options:
	@echo "CFLAGS  = ${CFLAGS}"
	@echo "LDFLAGS = ${LDFLAGS}"
	@echo "CC      = ${CC}"

dwmstatus: ${SOURCES} ${HEADERS} ${BLOCKS}
	${CC} -o $@ ${SOURCES} ${BLOCKS} ${CFLAGS} ${LDFLAGS}

src/config.h:
	cp src/config.def.h $@

clean:
	rm -f *.o *.gch dwmstatus

install: dwmstatus
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f dwmstatus ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/dwmstatus

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/dwmstatus

.PHONY: all options clean install uninstall
