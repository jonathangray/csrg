/*-
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * John B. Roll Jr.
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
"@(#) Copyright (c) 1990 The Regents of the University of California.\n\
 All rights reserved.\n";
#endif /* not lint */

#ifndef lint
static char sccsid[] = "@(#)xargs.c	5.8 (Berkeley) 04/27/91";
#endif /* not lint */

#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include "pathnames.h"

int fflag, tflag;

main(argc, argv)
	int argc;
	char **argv;
{
	extern int optind;
	extern char *optarg;
	register int ch;
	register char *p, *bp, *endbp, **bxp, **endxp, **xp;
	int cnt, indouble, insingle, nargs, nline;
	char *start, **xargs;

	nargs = 1024;				/* random value */
	nline = ARG_MAX - 2048;			/* max POSIX.2 value */

	while ((ch = getopt(argc, argv, "fn:s:t")) != EOF)
		switch(ch) {
		case 'f':
			fflag = 1;
			break;
		case 'n':
			if ((nargs = atoi(optarg)) <= 0) {
				(void)fprintf(stderr,
				    "xargs: illegal argument count.\n");
				exit(1);
			}
			break;
		case 's':
			if ((nline = atoi(optarg)) <= 0) {
				(void)fprintf(stderr,
				    "xargs: illegal command length.\n");
				exit(1);
			}
			break;
		case 't':
			tflag = 1;
			break;
		case '?':
		default:
			usage();
	}
	argc -= optind;
	argv += optind;

	/*
	 * Allocate for the utility and arguments passed to xarg, the pointers
	 * to the arguments read from stdin and the trailing NULL.  Allocate
	 * for the arguments read from stdin.
	 */
	if (!(xargs = malloc((u_int)(nargs + argc + 2) * sizeof(char **))) ||
	    !(bp = malloc((u_int)nline + 1))) {
		(void)fprintf(stderr, "xargs: %s.\n", strerror(errno));
		exit(1);
	}

	/*
	 * Use the user's name for the utility as argv[0], just like the
	 * shell.  Echo is the default.  Set up pointers for the user's
	 * arguments.
	 */
	xp = xargs + 1;
	if (!*argv)
		*xp++ = _PATH_ECHO;
	else {
		*xp++ = *argv;
		while (*++argv)
			*xp++ = *argv;
	}

	/* Set up the pointers into the buffer and the arguments */
	*(endxp = (bxp = xp) + nargs) = NULL;
	endbp = (start = p = bp) + nline;

	insingle = indouble = 0;
	for (;;)
		switch(ch = getchar()) {
		case EOF:
			/* Nothing to display. */
			if (p == bp)
				exit(0);

			/* Nothing since last arg end. */
			if (start == p) {
				*xp = NULL;
				run(xargs[0], xargs);
				exit(0);
			}
			goto addarg;
		case ' ':
		case '\t':
			if (insingle || indouble)
				goto addch;
			goto addarg;
		case '\n':
			if (start == p)			/* Empty line. */
				continue;
addarg:			if (insingle || indouble) {
				(void)fprintf(stderr,
				   "xargs: unterminated quote\n");
				exit(1);
			}
			*xp++ = start;
			*p++ = '\0';
			if (xp == endxp || p == endbp || ch == EOF) {
				*xp = NULL;
				run(xargs[0], xargs);
				if (ch == EOF)
					exit(0);
				p = bp;
				xp = bxp;
			}
			start = p;
			break;
		case '\'':
			if (indouble)
				goto addch;
			insingle = !insingle;
			break;
		case '"':
			if (insingle)
				goto addch;
			indouble = !indouble;
			break;
		case '\\':
			if ((ch = getchar()) == EOF) {
				(void)fprintf(stderr,
				    "xargs: backslash at EOF\n");
				exit(1);
			}
			/* FALLTHROUGH */
		default:
addch:			if (p != endbp) {
				*p++ = ch;
				continue;
			}
			if (bxp == xp) {
				(void)fprintf(stderr,
				    "xargs: argument too large.\n");
				exit(1);
			}
			*xp = NULL;
			run(xargs[0], xargs);
			cnt = endbp - start;
			bcopy(start, bp, cnt);
			p = (start = bp) + cnt;
			*p++ = ch;
			xp = bxp;
			break;
		}
	/* NOTREACHED */
}

run(prog, argv)
	char *prog, **argv;
{
	int noinvoke, status;
	pid_t pid;
	char **p;

	if (tflag) {
		(void)fprintf(stderr, "%s", *argv);
		for (p = argv + 1; *p; ++p)
			(void)fprintf(stderr, " %s", *p);
		(void)fprintf(stderr, "\n");
		(void)fflush(stderr);
	}
	noinvoke = 0;
	switch(pid = vfork()) {
	case -1:
		(void)fprintf(stderr,
		    "xargs: vfork: %s.\n", strerror(errno));
		exit(1);
	case 0:
		execvp(prog, argv);
		(void)fprintf(stderr,
		    "xargs: %s: %s.\n", prog, strerror(errno));
		noinvoke = 1;
		_exit(1);
	}
	pid = waitpid(pid, &status, 0);
	if (pid == -1) {
		(void)fprintf(stderr,
		    "xargs: waitpid: %s.\n", strerror(errno));
		exit(1);
	}
	/*
	 * If we couldn't invoke the utility or the utility didn't exit
	 * properly, quit with 127.
	 * Otherwise, if not specified otherwise, and the utility exits
	 * non-zero, exit with that value.
	 */
	if (noinvoke || !WIFEXITED(status) || WIFSIGNALED(status))
		exit(127);
	if (!fflag && WEXITSTATUS(status))
		exit(WEXITSTATUS(status));
}

usage()
{
	(void)fprintf(stderr,
	    "xargs: [-ft] [-n number] [-s size] [utility [argument ...]]\n");
	exit(1);
}
