#	@(#)Makefile	5.5 (Berkeley) 01/15/91

PROG=	fstat
SRCS=	fstat.c devname.c
.PATH:	${.CURDIR}/../../bin/ps
DPADD=	${LIBUTIL}
LDADD=	-lutil
BINGRP=	kmem
BINMODE=2555

.include <bsd.prog.mk>
