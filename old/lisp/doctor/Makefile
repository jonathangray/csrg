#	@(#)Makefile	4.6	(Berkeley)	87/06/03
#
DESTDIR=

doctor: /usr/ucb/lisp /usr/ucb/liszt
	liszt -mrq -o doctor doctor.l

install: doctor
	install doctor $(DESTDIR)/usr/games

clean:
	rm -f doctor
