/*-
 * Copyright (c) 1992 Regents of the University of California.
 * All rights reserved.
 *
 * This software was developed by the Computer Systems Engineering group
 * at Lawrence Berkeley Laboratory under DARPA contract BG 91-66 and
 * contributed to Berkeley.
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

#include <stdio.h>
#ifdef __STDC__
#include <stdlib.h>
#endif
#include <sys/types.h>
#include <sys/time.h>
#include <ctype.h>
#include <varargs.h>
#include <sys/file.h>
#include <sys/stat.h>

static char *program_name;

static char *
stripdir(s)
	register char *s;
{
	register char *cp;
	char *rindex();

	cp = rindex(s, '/');
	return (cp != 0) ? cp + 1 : s;
}

setprog(cp)
	char *cp;
{
	program_name = stripdir(cp);
}

void
usage()
{
	fprintf(stderr, "usage: %s [ -c core ] executable pid\n",
		program_name);
	exit(1);
}

/* VARARGS */
void
error(va_alist)
	va_dcl
{
	register char *cp;
	va_list ap;

	(void)fprintf(stderr, "%s: ", stripdir(program_name));

	va_start(ap);
	cp = va_arg(ap, char *);
	(void)vfprintf(stderr, cp, ap);
	va_end(ap);
	if (*cp) {
		cp += strlen(cp);
		if (cp[-1] != '\n')
			(void)fputc('\n', stderr);
	}
	exit(1);
	/* NOTREACHED */
}

/* VARARGS */
void
warning(va_alist)
	va_dcl
{
	register char *cp;
	va_list ap;

	(void)fprintf(stderr, "%s: warning: ", stripdir(program_name));

	va_start(ap);
	cp = va_arg(ap, char *);
	(void)vfprintf(stderr, cp, ap);
	va_end(ap);
	if (*cp) {
		cp += strlen(cp);
		if (cp[-1] != '\n')
			(void)fputc('\n', stderr);
	}
}
