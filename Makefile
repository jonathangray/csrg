#	@(#)Makefile	8.1 (Berkeley) 06/09/93

SUBDIR=	bin contrib games include kerberosIV lib libexec sbin share \
	usr.bin usr.sbin

afterinstall:
	(cd share/man && make makedb)

build:
	(cd include && make install)
	make cleandir && make depend
	(cd lib && make && make install)
	(cd kerberosIV && make && make install)
	make && make install
	
.include <bsd.subdir.mk>
