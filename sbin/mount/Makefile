#	@(#)Makefile	5.6 (Berkeley) 01/15/91

PROG=	mount
CFLAGS+=-DNFS
MAN8=	mount.0
DPADD=	${LIBRPC}
LDADD=	-lrpc
MLINKS=	mount.8 umount.8

.include <bsd.prog.mk>
