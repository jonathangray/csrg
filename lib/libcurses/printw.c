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
static char sccsid[] = "@(#)printw.c	5.7 (Berkeley) 02/09/91";
#endif /* not lint */

/*
 * printw and friends
 *
 */

# include	<varargs.h>
# include	"curses.ext"

/*
 *	This routine implements a printf on the standard screen.
 */
printw(va_alist)
va_dcl {

	va_list	ap;
	int	ret;

	va_start(ap);
	ret = _sprintw(stdscr, ap);
	va_end(ap);
	return (ret);
}

/*
 *	This routine implements a printf on the given window.
 */
wprintw(va_alist)
va_dcl {

	va_list	ap;
	WINDOW	*win;
	int	ret;

	va_start(ap);
	win = va_arg(ap, WINDOW *);
	ret = _sprintw(win, ap);
	va_end(ap);
	return (ret);
}

/*
 *	Internal write-buffer-to-window function.
 */
static int
_winwrite(cookie, buf, n)
void	*cookie;
reg char *buf;
int	n; {

	reg WINDOW *win = (WINDOW *)cookie;
	reg int c = n;

	while (--c >= 0) {
		if (waddch(win, *buf++) == ERR)
			return (-1);
	}
	return n;
}

/*
 *	This routine actually executes the printf and adds it to the window.
 *	It must not be declared static as it is used in mvprintw.c.
 */
_sprintw(win, ap)
WINDOW	*win;
va_list	ap; {

	FILE	*f;
	char	*fmt;

	if ((f = fwopen((void *)win, _winwrite)) == NULL)
		return ERR;
	fmt = va_arg(ap, char *);
	(void) vfprintf(f, fmt, ap);
	return fclose(f) ? ERR : OK;
}
