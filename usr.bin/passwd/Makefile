#	@(#)Makefile	8.2 (Berkeley) 04/02/94

PROG=	passwd
SRCS=	passwd.c
CFLAGS+=-I${.CURDIR}
#SRCS=	passwd.c des_rw.c
#CFLAGS+=-I${.CURDIR} -DKERBEROS -DCRYPT
#DPADD=	${LIBKRB} ${LIBDES}
#LDADD=	-lkrb -ldes
BINOWN=	root
BINMODE=4555
INSTALLFLAGS=-fschg

.include <bsd.prog.mk>
