/*-
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Cimarron D. Taylor of the University of California, Berkeley.
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
static char sccsid[] = "@(#)misc.c	5.3 (Berkeley) 10/22/90";
#endif /* not lint */

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include "find.h"
 
/*
 * find_subst --
 *	Replace occurrences of {} in s1 with s2 and return the result string.
 */
find_subst(orig, store, path, len)
	char *orig, **store, *path;
	int len;
{
	register int plen;
	register char ch, *p;
	char *realloc(), *strerror();

	plen = strlen(path);
	for (p = *store; ch = *orig; ++orig)
		if (ch == '{' && orig[1] == '}') {
			while ((p - *store) + plen > len)
				if (!(*store = realloc(*store, len *= 2))) {
					(void)fprintf(stderr,
					    "find: %s.\n", strerror(errno));
					exit(1);
				}
			bcopy(path, p, plen);
			p += plen;
			++orig;
		} else
			*p++ = ch;
	*p = '\0';
}

/*
 * find_queryuser --
 *	print a message to standard error and then read input from standard
 *	input. If the input is 'y' then 1 is returned.
 */
find_queryuser(argv)
	register char **argv;
{
	char buf[10];

	(void)fprintf(stderr, "\"%s", *argv);
	while (*++argv)
		(void)fprintf(stderr, " %s", *argv);
	(void)fprintf(stderr, "\"? ");
	(void)fflush(stderr);
	return(!fgets(buf, sizeof(buf), stdin) || buf[0] != 'y' ? 0 : 1);
}
 
/*
 * bad_arg --
 *	print out a bad argument message.
 */
void
bad_arg(option, error)
	char *option, *error;
{
	(void)fprintf(stderr, "find: %s: %s.\n", option, error);
	exit(1);
}
 
/*
 * emalloc --
 *	malloc with error checking.
 */
char *
emalloc(len)
	u_int len;
{
	char *p, *malloc(), *strerror();

	if (!(p = malloc(len))) {
		(void)fprintf(stderr, "find: %s.\n", strerror(errno));
		exit(1);
	}
	return(p);
}

usage()
{
	extern int deprecated;

	if (deprecated)
		(void)fprintf(stderr, "usage: find path-list expression\n");
	else
		(void)fprintf(stderr,
		    "usage: find [-drsx] -f path ... expression\n");
	exit(1);
}
