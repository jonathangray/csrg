/*-
 * Copyright (c) 1990 The Regents of the University of California.
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
static char sccsid[] = "@(#)main.c	5.4 (Berkeley) 11/15/90";
#endif /* not lint */

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fts.h>
#include "find.h"

int relative;

void
newsyntax(argc, argvp)
	int argc;
	char ***argvp;
{
	extern char *optarg;
	extern int depth, optind;
	int ch;
	char **argv, **cur;

	cur = argv = *argvp;
	while ((ch = getopt(argc, argv, "df:rsx")) != EOF)
		switch(ch) {
		case 'd':
			depth = 1;
			break;
		case 'f':
			*cur++ = optarg;
			break;
		case 'r':
			relative = 1;
			break;
		case 's':
			ftsoptions &= ~FTS_PHYSICAL;
			ftsoptions |= FTS_LOGICAL;
			break;
		case 'x':
			ftsoptions &= ~FTS_NOSTAT;
			ftsoptions |= FTS_XDEV;
			break;
		case '?':
		default:
			usage();
		}

	*argvp += optind;
	if (cur == argv) {
		if (!**argvp)
			usage();
		*cur++ = **argvp;
		++*argvp;
	}
	*cur = NULL;
}
