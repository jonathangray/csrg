#	@(#)Makefile	5.2 (Berkeley) 07/09/93

NOMAN=noman
NOOBJ=noobj

all nologin clean cleandir depend lint tags:

beforeinstall:
	install -c -o ${BINOWN} -g ${BINGRP} -m ${BINMODE} \
	    ${.CURDIR}/cpp.sh ${DESTDIR}/usr/bin/cpp

.include <bsd.prog.mk>
