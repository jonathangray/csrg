#	@(#)Makefile	5.4 (Berkeley) 01/15/91

PROG=	ps
SRCS=	ps.c devname.c attime.c proc_compare.c
DPADD=	${LIBMATH} ${LIBUTIL}
LDADD=	-lm -lutil
BINGRP=	kmem
BINMODE=2555

.include <bsd.prog.mk>
