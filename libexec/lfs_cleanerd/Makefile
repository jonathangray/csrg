#	@(#)Makefile	8.1 (Berkeley) 06/04/93

PROG=	lfs_cleanerd
CFLAGS+=-I/sys/ufs/lfs -I${.CURDIR} -DDIAGNOSTIC
SRCS=	cleanerd.c lfs_cksum.c library.c misc.c print.c
.PATH:	/sys/ufs/lfs

NOMAN=	noman

.include <bsd.prog.mk>
