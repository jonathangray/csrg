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
static char sccsid[] = "@(#)touchwin.c	5.6 (Berkeley) 09/14/92";
#endif /* not lint */

#include <curses.h>

/*
 * touchwin --
 *	Make it look like the whole window has been changed.
 */
int
touchwin(win)
	register WINDOW *win;
{
	register int y, maxy;

#ifdef DEBUG
	__TRACE("touchwin: (%0.2o)\n", win);
#endif
	maxy = win->maxy;
	for (y = 0; y < maxy; y++)
		touchline(win, y, 0, win->maxx - 1);
	return (OK);
}

/*
 * touchline --
 *	Touch a given line.
 */
int
touchline(win, y, sx, ex)
	register WINDOW *win;
	register int y, sx, ex;
{
#ifdef DEBUG
	__TRACE("touchline: (%0.2o, %d, %d, %d)\n", win, y, sx, ex);
	__TRACE("touchline: first = %d, last = %d\n",
	    win->lines[y]->firstch, win->lines[y]->lastch);
#endif
	sx += win->ch_off;
	ex += win->ch_off;
	if (!(win->lines[y]->flags & __ISDIRTY)) {
		win->lines[y]->flags |= __ISDIRTY;
		win->lines[y]->firstch = sx;
		win->lines[y]->lastch = ex;
	} else {
		if (win->lines[y]->firstch > sx)
			win->lines[y]->firstch = sx;
		if (win->lines[y]->lastch < ex)
			win->lines[y]->lastch = ex;
	}
#ifdef DEBUG
	__TRACE("touchline: first = %d, last = %d\n",
	    win->lines[y]->firstch, win->lines[y]->lastch);
#endif
	return (OK);
}
