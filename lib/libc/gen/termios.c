/*-
 * Copyright (c) 1989 The Regents of the University of California.
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

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)termios.c	5.2 (Berkeley) 06/26/90";
#endif /* LIBC_SCCS and not lint */

#include <sys/types.h>
#include <sys/errno.h>
#include <sys/ioctl.h>
#include <sys/tty.h>
#include <sys/termios.h>
#include <stdio.h>

tcgetattr(fd, t)
	int fd;
	struct termios *t;
{
	extern errno;

	return(ioctl(fd, TIOCGETA, t));
}

tcsetattr(fd, opt, t)
	int fd, opt;
	struct termios *t;
{
	struct termios localterm;

	if (opt & TCSASOFT) {
		localterm = *t;
		localterm.c_cflag |= CIGNORE;
		t = &localterm;
		opt &= TCSASOFT;
	}
	if (opt == TCSANOW)
		return (ioctl(fd, TIOCSETA, t));
	else if (opt == TCSADRAIN)
		return (ioctl(fd, TIOCSETAW, t));
	else
		return (ioctl(fd, TIOCSETAF, t));
}

tcsetpgrp(fd, pgrp)
{
	return(ioctl(fd, TIOCSPGRP, &pgrp));
}

tcgetpgrp(fd)
{
	int pgrp;

	if (ioctl(fd, TIOCGPGRP, &pgrp) < 0)
		return(-1);
	return(pgrp);
}

cfgetospeed(t)
	struct termios *t;
{
	return(t->c_ospeed);
}

cfgetispeed(t)
	struct termios *t;
{
	return(t->c_ispeed);
}

cfsetospeed(t, speed)
	struct termios *t;
{
	t->c_ospeed = speed;
}

cfsetispeed(t, speed)
	struct termios *t;
{
	t->c_ispeed = speed;
}

cfsetspeed(t, speed)
	struct termios *t;
{
	t->c_ispeed = t->c_ospeed = speed;
}

cfmakeraw(t)
	struct termios *t;
{
	t->c_iflag &= ~(IGNBRK|BRKINT|PARMRK|INLCR|IGNCR|ICRNL|IXON);
	t->c_oflag &= ~(ONLCR|OXTABS);
	t->c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
	/* set MIN/TIME */
}
