#	@(#)Makefile	5.4 (Berkeley) 07/18/92

SUBDIR=	bin contrib games include kerberosIV lib libexec sbin share \
	usr.bin usr.sbin

afterinstall:
	(cd share/man && make makedb)

.include <bsd.subdir.mk>
