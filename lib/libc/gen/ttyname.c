/*
 * Copyright (c) 1988 The Regents of the University of California.
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
static char sccsid[] = "@(#)ttyname.c	5.6 (Berkeley) 06/01/90";
#endif /* LIBC_SCCS and not lint */

#include <sys/types.h>
#include <sys/dir.h>
#include <sys/stat.h>
#include <sgtty.h>
#include <paths.h>

char *
ttyname(fd)
	int fd;
{
	register struct direct *dirp;
	register DIR *dp;
	struct stat sb1, sb2;
	struct sgttyb ttyb;
	static char buf[sizeof(_PATH_DEV) + MAXNAMLEN] = _PATH_DEV;
	char *rval, *strcpy();

	if (ioctl(fd, TIOCGETP, &ttyb) < 0)
		return(NULL);
	if (fstat(fd, &sb1) < 0 || (sb1.st_mode&S_IFMT) != S_IFCHR)
		return(NULL);
	if ((dp = opendir(_PATH_DEV)) == NULL)
		return(NULL);
	for (rval = NULL; dirp = readdir(dp);) {
		if (dirp->d_ino != sb1.st_ino)
			continue;
		(void)strcpy(buf + sizeof(_PATH_DEV) - 1, dirp->d_name);
		if (stat(buf, &sb2) < 0 || sb1.st_dev != sb2.st_dev ||
		    sb1.st_ino != sb2.st_ino)
			continue;
		closedir(dp);
		rval = buf;
		break;
	}
	closedir(dp);
	return(rval);
}
