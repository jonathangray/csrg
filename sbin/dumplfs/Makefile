#	@(#)Makefile	8.1 (Berkeley) 06/05/93

PROG=	dumplfs
CFLAGS+=-I/sys/ufs/lfs
SRCS=	dumplfs.c lfs_cksum.c misc.c
.PATH:	/sys/ufs/lfs
NOMAN=	noman

.include <bsd.prog.mk>
