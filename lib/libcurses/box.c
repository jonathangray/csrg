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
static char sccsid[] = "@(#)box.c	5.7 (Berkeley) 10/20/92";
#endif	/* not lint */

#include <curses.h>

/*
 * box --
 *	Draw a box around the given window with "vert" as the vertical
 *	delimiting char, and "hor", as the horizontal one.
 */
int
box(win, vert, hor)
	register WINDOW *win;
	int vert, hor;
{
	register int endy, endx, i;
	register char *fp, *lp, *sfp, *slp;

	endx = win->maxx;
	endy = win->maxy - 1;
	fp = win->lines[0]->line;
	lp = win->lines[endy]->line;
	sfp = win->lines[0]->standout;
	slp = win->lines[endy]->standout;
	for (i = 0; i < endx; i++) {
		fp[i] = lp[i] = hor;
		sfp[i] &= ~__STANDOUT;
		slp[i] &= ~__STANDOUT;
	}
	endx--;
	for (i = 0; i <= endy; i++) {
		win->lines[i]->line[0] = (win->lines[i]->line[endx] = vert);
		win->lines[i]->standout[0] &= ~__STANDOUT;
		win->lines[i]->standout[endx] &= ~__STANDOUT;
	}
	if (!(win->flags & __SCROLLOK) && (win->flags & __SCROLLWIN)) {
		fp[0] = fp[endx] = lp[0] = lp[endx] = ' ';
		sfp[0] &= ~__STANDOUT;
		sfp[endx] &= ~__STANDOUT;
		slp[0] &= ~__STANDOUT;
		slp[endx] &= ~__STANDOUT;
	}
	touchwin(win);
	return (OK);
}
