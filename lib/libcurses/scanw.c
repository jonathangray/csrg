/*
 * Copyright (c) 1981 Regents of the University of California.
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
static char sccsid[] = "@(#)scanw.c	5.5 (Berkeley) 01/21/91";
#endif /* not lint */

/*
 * scanw and friends
 *
 */

# include	"curses.ext"

/*
 *	This routine implements a scanf on the standard screen.
 */
scanw(fmt, args)
char	*fmt;
int	args; {

	return _sscans(stdscr, fmt, &args);
}

/*
 *	This routine implements a scanf on the given window.
 */
wscanw(win, fmt, args)
WINDOW	*win;
char	*fmt;
int	args; {

	return _sscans(win, fmt, &args);
}

/*
 *	Internal routine to read from a string, and its data structure.
 */
struct strinfo {
	char	*addr;		/* address */
	int	len;		/* remaining bytes */
};

static int
_winread(cookie, buf, n)
char	*cookie, *buf;
reg int	n; {

	reg struct strinfo *s = (struct strinfo *)cookie;

	if (n > s->len)
		n = s->len;
	bcopy(s->addr, buf, n);
	s->len -= n;
	s->addr += n;
	return n;
}

/*
 *	This routine actually executes the scanf from the window.
 *	SHOULD IMPLEMENT VSSCANF
 */
_sscans(win, fmt, args)
WINDOW	*win;
char	*fmt;
int	*args; {

	int	ret;
	FILE	*f;
	struct	strinfo s;
	char	buf[100];

	if ((f = fropen((char *)&s, _winread)) == NULL)
		return ERR;
	if (wgetstr(win, buf) == ERR) {
		(void) fclose(f);
		return ERR;
	}
	s.addr = buf;
	s.len = strlen(buf);
	ret = __svfscanf(f, fmt, args);
	(void) fclose(f);
	return ret;
}
