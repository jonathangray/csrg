/*-
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Hugh Smith at The University of Guelph.
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
static char sccsid[] = "@(#)archive.c	5.1 (Berkeley) 01/17/91";
#endif /* not lint */

#include <sys/param.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ar.h>
#include "archive.h"

extern char *archive;			/* archive name */

open_archive(mode)
	int mode;
{
	int created, fd, nr;
	char buf[SARMAG];
	
	created = 0;
	if (mode & O_CREAT) {
		mode |= O_EXCL;
		if ((fd = open(archive, mode, DEFFILEMODE)) >= 0) {
			/* POSIX.2 puts create message on stderr. */
			if (!(options & AR_C))
				(void)fprintf(stderr,
				    "ar: creating archive %s.\n", archive);
			created = 1;
			goto opened;
		}
		if (errno != EEXIST)
			error(archive);
		mode &= ~O_EXCL;
	}
	if ((fd = open(archive, mode, DEFFILEMODE)) < 0)
		error(archive);

	/* 
	 * Attempt to place a lock on the opened file - if we get an 
	 * error then someone is already working on this library.
	 */
opened:	if (flock(fd, LOCK_EX|LOCK_NB))
		error(archive);
	
	/*
	 * If not created, O_RDONLY|O_RDWR indicates that it has to be
	 * in archive format.
	 */
	if (!created &&
	    ((mode & O_ACCMODE) == O_RDONLY || (mode & O_ACCMODE) == O_RDWR)) {
		if ((nr = read(fd, buf, SARMAG) != SARMAG)) {
			if (nr >= 0)
				badfmt();
			error(archive);
		} else if (bcmp(buf, ARMAG, SARMAG))
			badfmt();
	} else if (write(fd, ARMAG, SARMAG) != SARMAG)
		error(archive);
	return(fd);
}

close_archive(fd)
	int fd;
{
	(void)flock(fd, LOCK_UN);
	(void)close(fd);
}

/*
 * copyfile --
 *	Copy size bytes from one file to another - taking care to handle the
 *	extra byte (for odd size files) when reading archives and tmpfiles
 *	and writing an extra byte if necessary when adding files to archive.
 *
 *	The padding is really unnecessary, and is almost certainly a remnant
 *	of early archive formats where the header included binary data which
 *	a PDP-11 required to start on an even byte boundary.  It should have
 *	been ripped out when the format changed.
 */
copyfile(cfp, size)
	CF *cfp;
	register off_t size;
{
	register int from, nr, nw, off, to;
	char pad, buf[8*1024];
	
	if (!size)
		return;

	pad = size & 1;
	from = cfp->rfd;
	to = cfp->wfd;
	while (size &&
	    (nr = read(from, buf, MIN(size, sizeof(buf)))) > 0) {
		size -= nr;
		for (off = 0; off < nr; nr -= off, off += nw)
			if ((nw = write(to, buf + off, nr)) < 0)
				error(cfp->wname);
	}
	if (size) {
		if (nr == 0)
			badfmt();
		error(cfp->rname);
	}

	if (pad) {
		if (cfp->flags & RPAD && (nr = read(from, buf, 1)) != 1) {
			if (nr == 0)
				badfmt();
			error(cfp->rname);
		}
		if (cfp->flags & WPAD) {
			pad = '\n';
			if (write(to, &pad, 1) != 1)
				error(cfp->wname);
		}
	}
}
