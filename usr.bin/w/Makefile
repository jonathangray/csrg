#	@(#)Makefile	5.5 (Berkeley) 01/14/91

PROG=	w
SRCS=	w.c attime.c proc_compare.c
MAN1=	w.0 uptime.0
DPADD=	${LIBUTIL}
LDADD=	-lutil
BINGRP=	kmem
BINMODE=2555
LINKS=	${BINDIR}/w ${BINDIR}/uptime
.PATH:	${.CURDIR}/../../bin/ps

.include <bsd.prog.mk>
