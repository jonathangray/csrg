/*-
 * Copyright (c) 1980, 1991 The Regents of the University of California.
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
static char sccsid[] = "@(#)err.c	5.5 (Berkeley) 04/04/91";
#endif /* not lint */

#include "sh.h"
#include <sys/ioctl.h>

/*
 * C Shell
 */

bool	errspl;			/* Argument to error was spliced by seterr2 */
char	one[2] = { '1', 0 };
char	*onev[2] = { one, NOSTR };
/*
 * Print error string s with optional argument arg.
 * This routine always resets or exits.  The flag haderr
 * is set so the routine who catches the unwind can propogate
 * it if they want.
 *
 * Note that any open files at the point of error will eventually
 * be closed in the routine process in sh.c which is the only
 * place error unwinds are ever caught.
 */
/*VARARGS1*/
error(s, arg)
	char *s;
{
	register char **v;
	register char *ep;

	/*
	 * Must flush before we print as we wish output before the error
	 * to go on (some form of) standard output, while output after
	 * goes on (some form of) diagnostic output.
	 * If didfds then output will go to 1/2 else to FSHOUT/FSHDIAG.
	 * See flush in sh.print.c.
	 */
	flush();
	haderr = 1;		/* Now to diagnostic output */
	timflg = 0;		/* This isn't otherwise reset */
	if (v = pargv)
		pargv = 0, blkfree(v);
	if (v = gargv)
		gargv = 0, blkfree(v);

	/*
	 * A zero arguments causes no printing, else print
	 * an error diagnostic here.
	 */
	if (s)
		printf(s, arg), printf(".\n");

	didfds = 0;		/* Forget about 0,1,2 */
	if ((ep = err) && errspl) {
		errspl = 0;
		xfree(ep);
	}
	errspl = 0;

	/*
	 * Go away if -e or we are a child shell
	 */
	if (exiterr || child)
		exit(1);

	/*
	 * Reset the state of the input.
	 * This buffered seek to end of file will also
	 * clear the while/foreach stack.
	 */
	btoeof();

	setq("status", onev, &shvhed);
	if (tpgrp > 0)
		(void) ioctl(FSHTTY, TIOCSPGRP, (char *)&tpgrp);
	longjmp(reslab, 0);		/* Unwind */
}

/*
 * Perror is the shells version of perror which should otherwise
 * never be called.
 */
Perror(s)
	char *s;
{

	/*
	 * Perror uses unit 2, thus if we didn't set up the fd's
	 * we must set up unit 2 now else the diagnostic will disappear
	 */
	if (!didfds) {
		register int oerrno = errno;

		(void) dcopy(SHDIAG, 2);
		errno = oerrno;
	}
	perror(s);
	error(NOSTR);		/* To exit or unwind */
}

bferr(cp)
	char *cp;
{

	flush();
	haderr = 1;
	printf("%s: ", bname);
	error(cp);
}

/*
 * The parser and scanner set up errors for later by calling seterr,
 * which sets the variable err as a side effect; later to be tested,
 * e.g. in process.
 */
seterr(s)
	char *s;
{

	if (err == 0)
		err = s, errspl = 0;
}

/* Set err to a splice of cp and dp, to be freed later in error() */
seterr2(cp, dp)
	char *cp, *dp;
{

	if (err)
		return;
	err = strspl(cp, dp);
	errspl++;
}

/* Set err to a splice of cp with a string form of character d */
seterrc(cp, d)
	char *cp, d;
{
	char chbuf[2];

	chbuf[0] = d;
	chbuf[1] = 0;
	seterr2(cp, chbuf);
}
