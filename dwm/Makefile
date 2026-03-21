# dwm - dynamic window manager
# See LICENSE file for copyright and license details.

include config.mk

SRC = drw.c dwm.c util.c
OBJ = ${SRC:.c=.o}

all: dwm

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: config.h config.mk

config.h:
	cp config.def.h $@

dwm: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f dwm ${OBJ} dwm-${VERSION}.tar.gz

dist: clean
	mkdir -p dwm-${VERSION}
	cp -R LICENSE Makefile README config.def.h config.mk\
		dwm.1 drw.h util.h ${SRC} dwm.png transient.c dwm-${VERSION}
	tar -cf dwm-${VERSION}.tar dwm-${VERSION}
	gzip dwm-${VERSION}.tar
	rm -rf dwm-${VERSION}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f dwm ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/dwm
	mkdir -p ${DESTDIR}${MANPREFIX}/man1
	sed "s/VERSION/${VERSION}/g" < dwm.1 > ${DESTDIR}${MANPREFIX}/man1/dwm.1
	chmod 644 ${DESTDIR}${MANPREFIX}/man1/dwm.1

	# Install our custom scripts
	cp -f ../scripts/dwm-start ${DESTDIR}${PREFIX}/bin/dwm-start
	chmod 755 ${DESTDIR}${PREFIX}/bin/dwm-start

	mkdir -p /usr/share/xsessions
	cp -f ../scripts/dwm.desktop /usr/share/xsessions/dwm.desktop
	
	cp -f ../scripts/dwm-menu ${DESTDIR}${PREFIX}/bin/dwm-menu
	chmod 755 ${DESTDIR}${PREFIX}/bin/dwm-menu

	cp -f ../scripts/app_manager ${DESTDIR}${PREFIX}/bin/app_manager
	chmod 755 ${DESTDIR}${PREFIX}/bin/app_manager
	
	cp -f ../scripts/bt_status ${DESTDIR}${PREFIX}/bin/bt_status
	chmod 755 ${DESTDIR}${PREFIX}/bin/bt_status

	cp -f ../scripts/layout_toggle ${DESTDIR}${PREFIX}/bin/layout_toggle
	chmod 755 ${DESTDIR}${PREFIX}/bin/layout_toggle

	cp -f ../scripts/hw_status ${DESTDIR}${PREFIX}/bin/hw_status
	chmod 755 ${DESTDIR}${PREFIX}/bin/hw_status
	
	cp -f ../scripts/lock ${DESTDIR}${PREFIX}/bin/lock
	chmod 755 ${DESTDIR}${PREFIX}/bin/lock

	cp -f ../scripts/screenshot ${DESTDIR}${PREFIX}/bin/screenshot
	chmod 755 ${DESTDIR}${PREFIX}/bin/screenshot

	cp -f ../scripts/mycal ${DESTDIR}${PREFIX}/bin/mycal
	chmod 755 ${DESTDIR}${PREFIX}/bin/mycal

	cp -f ../scripts/apply-theme ${DESTDIR}${PREFIX}/bin/apply-theme
	chmod 755 ${DESTDIR}${PREFIX}/bin/apply-theme
	
	cp -f ../scripts/display-setup ${DESTDIR}${PREFIX}/bin/display-setup
	chmod 755 ${DESTDIR}${PREFIX}/bin/display-setup
uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/dwm\
		${DESTDIR}${MANPREFIX}/man1/dwm.1
	# Remove custom scripts
	rm -f ${DESTDIR}${PREFIX}/bin/dwm-start \
		${DESTDIR}${PREFIX}/bin/dwm-menu \
		${DESTDIR}${PREFIX}/bin/app_manager \
		${DESTDIR}${PREFIX}/bin/display-setup
	# Remove desktop session entry
	rm -f /usr/share/xsessions/dwm.desktop

.PHONY: all clean dist install uninstall
