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
static char sccsid[] = "@(#)header.c	5.1 (Berkeley) 01/17/91";
#endif /* not lint */

#include <sys/param.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <ar.h>
#include "archive.h"

typedef struct ar_hdr HDR;
extern CHDR chdr;			/* converted header */
static char hb[sizeof(HDR) + 1];	/* real header */
extern char *archive;			/* archive name */

/* Convert ar header field to an integer. */
#define	AR_ATOI(from, to, len, base) { \
	bcopy(from, buf, len); \
	buf[len] = '\0'; \
	to = strtol(buf, (char **)NULL, base); \
}

/*
 * get_header --
 *	read the archive header for this member
 */
get_header(fd)
	int fd;
{
	struct ar_hdr *hdr;
	register int len, nr;
	register char *p, buf[20];

	nr = read(fd, hb, sizeof(HDR));
	if (nr != sizeof(HDR)) {
		if (!nr)
			return(0);
		if (nr < 0)
			error(archive);
		badfmt();
	}

	hdr = (struct ar_hdr *)hb;
	if (strncmp(hdr->ar_fmag, ARFMAG, sizeof(ARFMAG) - 1))
		badfmt();

	/* Convert the header into the internal format. */
#define	DECIMAL	10
#define	OCTAL	 8

	AR_ATOI(hdr->ar_date, chdr.date, sizeof(hdr->ar_date), DECIMAL);
	AR_ATOI(hdr->ar_uid, chdr.uid, sizeof(hdr->ar_uid), DECIMAL);
	AR_ATOI(hdr->ar_gid, chdr.gid, sizeof(hdr->ar_gid), DECIMAL);
	AR_ATOI(hdr->ar_mode, chdr.mode, sizeof(hdr->ar_mode), OCTAL);
	AR_ATOI(hdr->ar_size, chdr.size, sizeof(hdr->ar_size), DECIMAL);

	/* Leading spaces should never happen. */
	if (hdr->ar_name[0] == ' ')
		badfmt();

	/*
	 * Long name support.  Set the "real" size of the file, and the
	 * long name flag/size.
	 */
	if (!bcmp(hdr->ar_name, AR_EFMT1, sizeof(AR_EFMT1) - 1)) {
		len = atoi(hdr->ar_name + sizeof(AR_EFMT1) - 1);
		if (len <= 0 || len > MAXNAMLEN)
			badfmt();
		nr = read(fd, chdr.name, len);
		if (nr != len) {
			if (nr < 0)
				error(archive);
			badfmt();
		}
		chdr.size -= (chdr.lname = len);
	} else {
		bcopy(hdr->ar_name, chdr.name, sizeof(hdr->ar_name));

		/* Only strip off trailing spaces. */
		for (p = chdr.name + sizeof(hdr->ar_name) - 1; *p == ' '; --p);
		*++p = '\0';
		chdr.lname = 0;
	}
	return(1);
}

/*
 * put_header --
 *	Write the archive member header to a file.
 */
put_header(cfp, sb)
	CF *cfp;
	struct stat *sb;
{
	register int lname;
	register char *name;
	struct ar_hdr *hdr;
	char *rname();

	/*
	 * If passed an sb structure, reading a file from disk.  Get stat(2)
	 * information, build a name and construct a header.  (Files are named
	 * by their last component in the archive.)  If not, then just write
	 * the last header read.
	 */
	if (sb) {
		name = rname(cfp->rname);
		(void)fstat(cfp->rfd, sb);

		if ((lname = strlen(name)) > sizeof(hdr->ar_name) ||
		    index(name, ' ')) {
			(void)sprintf(hb, HDR1, AR_EFMT1, lname, sb->st_mtime,
			    sb->st_uid, sb->st_gid, sb->st_mode,
			    sb->st_size + lname, ARFMAG);
		} else {
			lname = 0;
			(void)sprintf(hb, HDR2, name, sb->st_mtime, sb->st_uid,
			    sb->st_gid, sb->st_mode, sb->st_size, ARFMAG);
		}
	} else {
		lname = chdr.lname;
		name = chdr.name;
	}

	if (write(cfp->wfd, hb, sizeof(HDR)) != sizeof(HDR) ||
	    lname && write(cfp->wfd, name, lname) != lname)
		error(cfp->wname);
}
