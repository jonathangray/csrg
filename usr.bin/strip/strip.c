/*
 * Copyright (c) 1988 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef lint
char copyright[] =
"@(#) Copyright (c) 1988 Regents of the University of California.\n\
 All rights reserved.\n";
#endif /* not lint */

#ifndef lint
static char sccsid[] = "@(#)strip.c	5.6 (Berkeley) 05/25/91";
#endif /* not lint */

#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <a.out.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void err __P((const char *fmt, ...));

typedef struct exec EXEC;

main(argc, argv)
	int argc;
	char *argv[];
{
	register off_t fsize;
	register int fd, n, pagesize;
	EXEC head;
	int ch, dstabs;

	dstabs = 0;
	while ((ch = getopt(argc, argv, "d")) != EOF)
		switch(ch) {
		case 'd':
			dstabs = 1;
			break;
		case '?':
		default:
			usage();
		}
	argc -= optind;
	argv += optind;

	pagesize = getpagesize();

	for (; *argv; ++argv) {
		if ((fd = open(*argv, O_RDWR)) < 0 ||
		    (n = read(fd, &head, sizeof(EXEC))) == -1) {
			err("%s: %s", *argv, strerror(errno));
			continue;
		}
		if (n != sizeof(EXEC) || N_BADMAG(head)) {
			err("%s: %s", *argv, strerror(EFTYPE));
			continue;
		}
		if (!head.a_syms && !head.a_trsize && !head.a_drsize) {
			(void)close(fd);
			continue;
		}
		fsize = head.a_text + head.a_data;
		if (head.a_magic == ZMAGIC)
			fsize += pagesize - sizeof(EXEC);
		head.a_syms = head.a_trsize = head.a_drsize = 0;

		if (ftruncate(fd, fsize + sizeof(EXEC)) ||
		    lseek(fd, 0L, SEEK_SET) == -1 ||
		    write(fd, &head, sizeof(EXEC)) != sizeof(EXEC))
			err("%s: %s", *argv, strerror(errno)); 
		(void)close(fd);
	}
	exit(0);
}

usage()
{
	(void)fprintf(stderr, "usage: strip [-d] file ...\n");
	exit(1);
}

#if __STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#endif

void
#if __STDC__
err(const char *fmt, ...)
#else
err(fmt, va_alist)
	char *fmt;
        va_dcl
#endif
{
	va_list ap;
#if __STDC__
	va_start(ap, fmt);
#else
	va_start(ap);
#endif
	(void)fprintf(stderr, "strip: ");
	(void)vfprintf(stderr, fmt, ap);
	va_end(ap);
	(void)fprintf(stderr, "\n");
}
