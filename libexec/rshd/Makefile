#	@(#)Makefile	8.1 (Berkeley) 06/04/93

PROG=	rshd
SRCS=	rshd.c
#SRCS=	rshd.c des_rw.c
#CFLAGS+=-DKERBEROS -DCRYPT
MAN8=	rshd.0
#DPADD=	${LIBUTIL} ${LIBKRB} ${LIBDES}
#LDADD=	-lutil -lkrb -ldes
DPADD=	${LIBUTIL}
LDADD=	-lutil
.PATH: ${.CURDIR}/../../usr.bin/rlogin

.include <bsd.prog.mk>
