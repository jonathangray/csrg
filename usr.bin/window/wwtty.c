/*
 * Copyright (c) 1983 Regents of the University of California.
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
static char sccsid[] = "@(#)wwtty.c	3.15 (Berkeley) 06/02/90";
#endif /* not lint */

#include "ww.h"
#include <fcntl.h>

wwgettty(d, t)
register struct ww_tty *t;
{
#ifndef POSIX_TTY
	if (ioctl(d, TIOCGETP, (char *)&t->ww_sgttyb) < 0)
		goto bad;
	if (ioctl(d, TIOCGETC, (char *)&t->ww_tchars) < 0)
		goto bad;
	if (ioctl(d, TIOCGLTC, (char *)&t->ww_ltchars) < 0)
		goto bad;
	if (ioctl(d, TIOCLGET, (char *)&t->ww_lmode) < 0)
		goto bad;
	if (ioctl(d, TIOCGETD, (char *)&t->ww_ldisc) < 0)
		goto bad;
#else
	if (tcgetattr(d, &t->ww_termios) < 0)
		goto bad;
#endif
	if ((t->ww_fflags = fcntl(d, F_GETFL, 0)) < 0)
		goto bad;
	return 0;
bad:
	wwerrno = WWE_SYS;
	return -1;
}

/*
 * Set the modes of tty 'd' to 't'
 * 'o' is the current modes.  We set the line discipline only if
 * it changes, to avoid unnecessary flushing of typeahead.
 */
wwsettty(d, t, o)
register struct ww_tty *t, *o;
{
#ifndef POSIX_TTY
	/* for buggy tty driver that doesn't wait for output to drain */
	int i;
	while (ioctl(d, TIOCOUTQ, &i) >= 0 && i > 0)
		usleep(100000);
	if (ioctl(d, TIOCSETN, (char *)&t->ww_sgttyb) < 0)
		goto bad;
	if (ioctl(d, TIOCSETC, (char *)&t->ww_tchars) < 0)
		goto bad;
	if (ioctl(d, TIOCSLTC, (char *)&t->ww_ltchars) < 0)
		goto bad;
	if (ioctl(d, TIOCLSET, (char *)&t->ww_lmode) < 0)
		goto bad;
	if ((o == 0 || t->ww_ldisc != o->ww_ldisc) &&
	    ioctl(d, TIOCSETD, (char *)&t->ww_ldisc) < 0)
		goto bad;
#else
	if (tcsetattr(d, TCSADRAIN, &t->ww_termios) < 0)
		goto bad;
#endif
	if (fcntl(d, F_SETFL, t->ww_fflags) < 0)
		goto bad;
	return 0;
bad:
	wwerrno = WWE_SYS;
	return -1;
}
