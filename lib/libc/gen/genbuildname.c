/*
 * Copyright (c) 1990 The Regents of the University of California.
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
static char sccsid[] = "@(#)genbuildname.c	5.3 (Berkeley) 06/01/90";
#endif LIBC_SCCS and not lint

#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

char *objdir = "obj";

#define	UNKNOWN	0
#define	NODIR	1
#define	USEDIR	2

char *
genbuildname(name)
	char *name;
{
	static int dirlen, chkobjdir = UNKNOWN;
	struct stat stbuf;
	char *newname;

	if (chkobjdir == NODIR || index(name, '/') != (char *)0)
		return (name);
	if (chkobjdir == UNKNOWN &&
	    (stat(objdir, &stbuf) < 0 ||
	    (stbuf.st_mode & S_IFMT) != S_IFDIR)) {
		chkobjdir = NODIR;
		return (name);
	} else {
		chkobjdir = USEDIR;
		dirlen = strlen(objdir) + 2;
	}
	newname = (char *)malloc(dirlen + strlen(name));
	if (newname == (char *)0)
		return (name);
	strcpy(newname, objdir);
	strcat(newname, "/");
	strcat(newname, name);
	return (newname);
}
