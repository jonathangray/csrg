#	@(#)Makefile	5.4 (Berkeley) 01/14/91

PROG=	pstat
SRCS=	pstat.c devname.c
.PATH:	${.CURDIR}/../../bin/ps
MAN8=	pstat.0
DPADD= ${LIBUTIL}
LDADD=	-lutil
BINGRP=	kmem
BINMODE=2555

.include <bsd.prog.mk>
