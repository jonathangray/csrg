/*-
 * Copyright (c) 1991 The Regents of the University of California.
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
static char sccsid[] = "@(#)cchar.c	5.1 (Berkeley) 05/02/91";
#endif /* not lint */

#include <sys/types.h>
#include <termios.h>
#include <stddef.h>
#include "stty.h"

/*
 * Special control characters.
 *
 * Cchars1 are the standard names, cchars2 are the old aliases.
 * The first are displayed, but both are recognized on the
 * command line.
 */
struct cchar cchars1[] = {
	"discard",	VDISCARD, 	CDISCARD,
	"dsusp", 	VDSUSP,		CDSUSP,
	"eof",		VEOF,		CEOF,
	"eol",		VEOL,		CEOL,
	"eol2",		VEOL2,		CEOL,
	"erase",	VERASE,		CERASE,
	"intr",		VINTR,		CINTR,
	"kill",		VKILL,		CKILL,
	"lnext",	VLNEXT,		CLNEXT,
	"quit",		VQUIT,		CQUIT,
	"reprint",	VREPRINT, 	CREPRINT,
	"start",	VSTART,		CSTART,
	"status",	VSTATUS, 	CSTATUS,
	"stop",		VSTOP,		CSTOP,
	"susp",		VSUSP,		CSUSP,
	"werase",	VWERASE,	CWERASE,
	NULL
};

struct cchar cchars2[] = {
	"brk",		VEOL,		CEOL,
	"flush",	VDISCARD, 	CDISCARD,
	"rprnt",	VREPRINT, 	CREPRINT,
	"xoff",		VSTOP,		CSTOP,
	"xon",		VSTART,		CSTART,
	NULL
};
