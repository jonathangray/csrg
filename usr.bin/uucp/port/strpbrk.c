/*-
 * Copyright (c) 1985 The Regents of the University of California.
 * All rights reserved.
 *
 * This module is believed to contain source code proprietary to AT&T.
 * Use and redistribution is subject to the Berkeley Software License
 * Agreement and your Software Agreement with AT&T (Western Electric).
 */

#ifndef lint
static char sccsid[] = "@(#)strpbrk.c	5.2 (Berkeley) 04/24/91";
#endif /* not lint */

/*LINTLIBRARY*/

/*
 * this is like index, but takes a string as the second argument
 */
char *
strpbrk(str, chars)
register char *str, *chars;
{
	register char *cp;

	do {
		cp = chars - 1;
		while (*++cp) {
			if (*str == *cp)
				return str;
		}
	} while (*str++);
	return (char *)0;
}
